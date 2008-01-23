/***************************************************************************
 *   Copyright (C) 2005-2007 by                                            *
 *                                                                         *
 *      Stefan Schwarzer <sxs@morphine.tv>                                 *
 *      Guido Madaus     <bere@morphine.tv>                                *
 *      Jens Schneider   <pupeider@morphine.tv>                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.        *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "mmsgifloader.h"
#include "fb/mmsfbsurface.h"

//#define GIFTRACE

MMSGIFLoader::MMSGIFLoader(MMSIM_DESC *desc) {
    this->desc = desc;
    this->myfile = NULL;
}

bool MMSGIFLoader::loadHeader() {

    unsigned char   buffer[1024];
    size_t          count = 0;

    this->myfile = new MMSFile(this->desc->imagefile);
    if (!this->myfile) return false;

    /* read header */
    if (!this->myfile->readBuffer((void*)buffer, &count, 1, 6)) {
        /* cannot read file */
        return false;
    }

    /* check header */
    memset(&this->gif_header, 0, sizeof(this->gif_header));
    if (count < 6) {
        /* it is not an GIF file */
        return false;
    }
    memcpy(gif_header.signature, &(buffer[0]), 3);
    memcpy(gif_header.version, &(buffer[3]), 3);
    if (memcmp(gif_header.signature, "GIF", 3)!=0) {
        /* it is not an GIF file */
        return false;
    }

#ifdef GIFTRACE
    printf("FILE=%s\n", this->desc->imagefile.c_str());
    printf("HEADER\nSIGNATURE='%s'\nVERSION='%s'\n\n", this->gif_header.signature, this->gif_header.version);
#endif

    /* read Logical Screen Descriptor */
    if (!this->myfile->readBuffer((void*)buffer, &count, 1, 7)) {
        /* cannot read file */
        return false;
    }

    /* put it in structure */
    memset(&this->gif_lsd, 0, sizeof(this->gif_lsd));
    this->gif_lsd.width = *((unsigned short*)&buffer[0]);
    this->gif_lsd.height = *((unsigned short*)&buffer[2]);
    this->gif_lsd.flags = buffer[4];
    this->gif_lsd.bgcolor = buffer[5];
    this->gif_lsd.ratio = buffer[6];
    this->gif_lsd.global_color_table = ((this->gif_lsd.flags & 0x80) == 0x80);

#ifdef GIFTRACE
    printf("LOGICAL SCREEN DESCRIPTOR\nWIDTH='%u'\nHEIGHT='%u'\nFLAGS='%x'\nBGCOLOR='0x%02x'\nRATIO='%u'\nGCT=%s\n\n", gif_lsd.width, gif_lsd.height,
                    this->gif_lsd.flags, this->gif_lsd.bgcolor, this->gif_lsd.ratio,
                    (this->gif_lsd.global_color_table)?"yes":"no");
#endif

    if (gif_lsd.global_color_table) {
        /* read the global color table */
        memset(&this->gif_gct, 0, sizeof(this->gif_gct));
        int y = (this->gif_lsd.flags & 0x07) + 1;
        this->gif_gct.size = 2;
        while (y-->1) this->gif_gct.size*=2;
        if (!this->myfile->readBuffer((void*)this->gif_gct.table, &count, 1, 3 * this->gif_gct.size)) {
            /* cannot read file */
            return false;
        }

        if (count < 3 * this->gif_gct.size) {
            /* bad format */
            return false;
        }

#ifdef GIFTRACE
        printf("GLOBAL COLOR TABLE\nSIZE='%d'\n\n", this->gif_gct.size);
#endif
    }

    return true;
}

bool MMSGIFLoader::loadBlocks() {

    unsigned char   buffer[1024];
    size_t          count = 0;
    MMS_GIF_GCE     gif_gce;
    MMS_GIF_GCE     gif_gce_old;
    bool            loop_forever = false;
    
    memset(&gif_gce, 0, sizeof(gif_gce));
    memset(&gif_gce_old, 0, sizeof(gif_gce_old));

//int hh=0;

    /* through each block */
    while (1) {
        /* read block identifier */
        if (!this->myfile->readBuffer((void*)buffer, &count, 1, 1)) {
            /* cannot read file */
            return false;
        }
        

//hh++;
//printf("nc=%02x\n", buffer[0]);
//if (i>20) break;
//if (hh>2) break;

        switch (buffer[0]) {
            case 0x21:
                /* extension block */
                /* read extension block identifier */
                if (!this->myfile->readBuffer((void*)buffer, &count, 1, 1)) {
                    /* cannot read file */
                    return false;
                }

//printf("nc=nc=%02x\n", buffer[0]);

                switch (buffer[0]) {
                    case 0xff:
                        /* application extension block */
                        /* we do not need it - jump over */
                        if (!this->myfile->readBuffer((void*)buffer, &count, 1, 12)) {
                            /* cannot read file */
                            return false;
                        }

                        /* jump over data buffer */
                        while (1) {
                            if (!this->myfile->readBuffer((void*)buffer, &count, 1, 1)) {
                                /* cannot read file */
                                return false;
                            }
                            if (buffer[0]) {
                                if (!this->myfile->readBuffer((void*)buffer, &count, 1, buffer[0])) {
                                    /* cannot read file */
                                    return false;
                                }
                                continue;
                            }
                            break;
                        }

#ifdef GIFTRACE
                        printf("APPLICATION EXTENSION, skipped\n\n");
#endif

                        /* if application extension is specified, we think that we should never stop the animation */
                        loop_forever = true;
                        
                        break;

                    case 0xf9:
                        /* graphic control extension block */
                        if (!this->myfile->readBuffer((void*)buffer, &count, 1, 6)) {
                            /* cannot read file */
                            return false;
                        }
                        memcpy(&gif_gce_old, &gif_gce, sizeof(gif_gce));
                        memset(&gif_gce, 0, sizeof(gif_gce));
                        gif_gce.flags = buffer[1];
                        gif_gce.delaytime = *((unsigned short*)&buffer[2]);
                        gif_gce.transcolor = buffer[4];
                        gif_gce.transparent_color = ((gif_gce.flags & 0x01) == 0x01);
                        gif_gce.disposal = (gif_gce.flags >> 2) & 0x07;

#ifdef GIFTRACE
                        printf("GRAPHIC CONTROL EXTENSION\nFLAGS='%x',DELAYTIME='%d',TRANSCOLOR='0x%02x'\n\n",
                                        gif_gce.flags, gif_gce.delaytime, (gif_gce.transparent_color)?gif_gce.transcolor:-1);
#endif

                        break;

                    case 0x01:
                        /* plain text extension block */
                        /* we do not need it - jump over */
                        if (!this->myfile->readBuffer((void*)buffer, &count, 1, 13)) {
                            /* cannot read file */
                            return false;
                        }

                        /* jump over data buffer */
                        while (1) {
                            if (!this->myfile->readBuffer((void*)buffer, &count, 1, 1)) {
                                /* cannot read file */
                                return false;
                            }
                            if (buffer[0]) {
                                if (!this->myfile->readBuffer((void*)buffer, &count, 1, buffer[0])) {
                                    /* cannot read file */
                                    return false;
                                }
                                continue;
                            }
                            break;
                        }

//TODO
#ifdef GIFTRACE
                        printf("PLAIN TEXT EXTENSION, skipped\n\n");
#endif

                        break;

                    case 0xfe:
                        /* comment extension block */

                        /* jump over data buffer */
                        while (1) {
                            if (!this->myfile->readBuffer((void*)buffer, &count, 1, 1)) {
                                /* cannot read file */
                                return false;
                            }
                            if (buffer[0]) {
                                if (!this->myfile->readBuffer((void*)buffer, &count, 1, buffer[0])) {
                                    /* cannot read file */
                                    return false;
                                }
                                continue;
                            }
                            break;
                        }

#ifdef GIFTRACE
                        printf("COMMENT EXTENSION, skipped\n\n");
#endif

                        break;
                }
                break;

            case 0x2c: {
                /* image descriptor */
                if (!this->myfile->readBuffer((void*)buffer, &count, 1, 9)) {
                    /* cannot read file */
                    return false;
                }
                MMS_GIF_ID gif_id;
                MMS_GIF_CT gif_lct;     
                MMS_GIF_CT *my_color_table;     
                memset(&gif_id, 0, sizeof(gif_id));
                gif_id.x = *((unsigned short*)&buffer[0]);
                gif_id.y = *((unsigned short*)&buffer[2]);
                gif_id.w = *((unsigned short*)&buffer[4]);
                gif_id.h = *((unsigned short*)&buffer[6]);
                gif_id.flags = buffer[8];
                gif_id.local_color_table = ((gif_id.flags & 0x80) == 0x80);
                gif_id.interlaced = ((gif_id.flags & 0x40) == 0x40);
                unsigned int line_size = gif_id.w * 4;
                unsigned int image_size = line_size * gif_id.h;
                char interlace_pass = 0;

#ifdef GIFTRACE
                printf("IMAGE DESCRIPTOR\nX='%d',Y='%d',W='%d',H='%d',FLAGS='%x',LCT=%s,INTERLACED=%s\n\n",
                                gif_id.x, gif_id.y, gif_id.w, gif_id.h, gif_id.flags,
                                (gif_id.local_color_table)?"yes":"no", (gif_id.interlaced)?"yes":"no");
#endif

                if (gif_id.local_color_table) {
                    /* read the local color table */
                    memset(&gif_lct, 0, sizeof(gif_lct));
                    int y = (gif_id.flags & 0x07) + 1;
                    gif_lct.size = 2;
                    while (y-->1) gif_lct.size*=2;
                    if (!this->myfile->readBuffer((void*)gif_lct.table, &count, 1, 3 * gif_lct.size)) {
                        /* cannot read file */
                        return false;
                    }

                    if (count < 3 * gif_lct.size) {
                        /* bad format */
                        return false;
                    }

#ifdef GIFTRACE
                    printf("LOCAL COLOR TABLE\nSIZE='%d'\n\n", gif_lct.size);
#endif

                    my_color_table = &gif_lct;
                }
                else {
                    /* use local color table */
                    my_color_table = &(this->gif_gct);
                }

                /* here some values needed for decoding... */
                if (!this->myfile->readBuffer((void*)buffer, &count, 1, 1)) {
                    /* cannot read file */
                    return false;
                }
                int initial_code_size = buffer[0];
                int code_size = buffer[0];
                int clr_code = 1 << code_size;
                int end_code = clr_code + 1;
                int max_code = clr_code << 1;
                int next_code = clr_code + 2;
                code_size++;

                unsigned char data_table[128*1024];
                unsigned int index_table[4*1024+1];
                memset(data_table, 0, sizeof(data_table));
                memset(index_table, 0, sizeof(index_table));
                for (int t = 0; t < clr_code + 3; t++)
                    data_table[t]=index_table[t]=t;
                bool end_of_stream = false;

                /* create the surface */
                MMSFBSurface *newsuf;
                if (!mmsfb->createSurface(&newsuf, gif_lsd.width, gif_lsd.height, "ARGB", 0)) {
                    /* cannot create surface */
                    return false; 
                }
                IDirectFBSurface *dfbsuf = newsuf->getDFBSurface();

                int ci;
                if (desc->sufcount <= 0) {
                    /* first surface */
                    if (this->gif_lsd.global_color_table) {
                        if ((gif_gce.transparent_color) && (this->gif_lsd.bgcolor == gif_gce.transcolor)) {
                            /* clear surface */
                            newsuf->clear();
                        }
                        else {
                            /* fill with background color */
                            ci = this->gif_lsd.bgcolor*3;
                            newsuf->clear(gif_gct.table[ci], gif_gct.table[ci+1], gif_gct.table[ci+2], 0xff);
                        }
                    }
                    else
                        /* clear surface */
                        newsuf->clear();
                }
                else {
                    /* second or following surfaces */
                    switch (gif_gce_old.disposal) {
                        case 0: //No disposal specified
                        case 1: //Do not dispose
                            newsuf->blit(desc->suf[desc->sufcount-1].surface, NULL, 0, 0);
                            break;
                        case 2: //Restore to background
                            newsuf->blit(desc->suf[desc->sufcount-1].surface, NULL, 0, 0);
                            ci = this->gif_lsd.bgcolor*3;
                            if ((gif_gce_old.transparent_color) && (this->gif_lsd.bgcolor == gif_gce_old.transcolor))
                                newsuf->setColor(0xff, 0xff, 0xff, 0xff);
                            else
                                newsuf->setColor(gif_gct.table[ci], gif_gct.table[ci+1], gif_gct.table[ci+2], 0xff);
                            newsuf->fillRectangle(gif_id.x, gif_id.y, gif_id.w, gif_id.h);
                            break;
                        case 3: //Restore to previous
                            if (desc->sufcount >= 2)
                                newsuf->blit(desc->suf[desc->sufcount-2].surface, NULL, 0, 0);
                            break;
                    }
                }

                /* get direct access to the surface */
                unsigned char *sufbuf, *sufbuf_start, *sufbuf_end;
                int pitch;
                dfbsuf->Lock(dfbsuf, DSLF_WRITE, (void**)&sufbuf, &pitch);
                sufbuf+= gif_id.x*4 + gif_id.y * pitch;
                sufbuf_start = sufbuf;
                sufbuf_end = sufbuf_start + gif_lsd.height * pitch;

                unsigned int outlen = 0;
//                unsigned int wbytes = gif_id.w * 4; 
                int oddbits = 0;
                bool first = true;

                while (1) {

                    /* get the block length */
                    unsigned char len;
                    if (!this->myfile->readBuffer((void*)&len, &count, 1, 1)) {
                        /* cannot read file */
                        dfbsuf->Unlock(dfbsuf);
                        delete newsuf;                        
                        return false;
                    }

                    /* the first two bytes are reserved for the bit from the buffer before */ 
                    int bits = 2*8;
                    int bitlen = bits + (((int)len) << 3);
                    if (oddbits > 0)
                        bits-=oddbits;

                    if (!len) {
                        /* the block terminator was found */
                        break;
                    }
    
                    /* read the block data */
                    /* the first two bytes are reserved for the bit from the buffer before */ 
                    if (!this->myfile->readBuffer((void*)&buffer[2], &count, 1, len)) {
                        /* cannot read file */
                        dfbsuf->Unlock(dfbsuf);
                        delete newsuf;                        
                        return false;
                    }
                    
                    if (count < len) {
                        /* bad format */
                        dfbsuf->Unlock(dfbsuf);
                        delete newsuf;                        
                        return false;
                    }
    
#ifdef GIFTRACE
                    printf("pixel indexes=\n");
#endif
    
                    /* read all pixel indexes from buffer */
                    do {
                        /* read the next bit code from buffer */
                        int code = 0;
                        for (int i = 0; i < code_size; i++, bits++)
                            code = code | ((buffer[bits >> 3] & (1 << (bits & 0x07))) != 0) << i; 
                        if (code == clr_code) {
                            /* clear all values and tables *********************************/
                            /* code_size:   the code size in bits which will used at start */
                            /* clr_code:    the code which indicates this reset            */
                            /* end_code:    the code which indicates the end of the stream */
                            /* max_code:    the maximum code                               */
                            /* next_code:   the first free code                            */
                            /* data_table:  this table contains data for decompression     */
                            /* index_table: this table contains index values to data_table */
                            /***************************************************************/  
                            code_size  = initial_code_size;
                            clr_code = 1 << code_size;
                            end_code = clr_code + 1;
                            max_code = clr_code << 1;
                            next_code = clr_code + 2;
                            code_size++;
                            memset(data_table, 0, sizeof(data_table));
                            memset(index_table, 0, sizeof(index_table));
                            for (int t = 0; t < clr_code + 3; t++)
                                data_table[t]=index_table[t]=t;
                            first = true;
                            continue;
                        }
    
                        if (code == end_code) {
                            /* end of stream reached */
#ifdef GIFTRACE
                            printf("\nend of stream\n");
#endif
                            end_of_stream = true;
                            break;
                        }
    
                        /* saves old code which is written to the output stream */
                        unsigned char oc[8192];
                        int oclen;
    
                        if (first) {
                            /* first loop */
                            first = false;
    
                            *oc = data_table[index_table[code]];
                            oclen = 1;
                        }
                        else {
                            /* second or following loops */
                            /* is code in table? */
                            if (code < next_code) {
                                /* yes, add to table (oc + first pixel of new code) */
                                memcpy(&data_table[index_table[next_code]], oc, oclen);
                                data_table[index_table[next_code]+oclen] = data_table[index_table[code]];
                                index_table[next_code+1] = index_table[next_code]+oclen+1;
                                next_code++;
    
                                /* take table data for code */
                                oclen = index_table[code+1] - index_table[code];
                                memcpy(oc, &data_table[index_table[code]], oclen);
                            }
                            else {
                                /* no, add to table (oc + first pixel of oc) */
                                memcpy(&data_table[index_table[next_code]], oc, oclen);
                                data_table[index_table[next_code]+oclen] = *oc;
                                index_table[next_code+1] = index_table[next_code]+oclen+1;
                                next_code++;
    
                                /* take table data for new entry */
                                oclen = index_table[next_code] - index_table[next_code-1];
                                memcpy(oc, &data_table[index_table[next_code-1]], oclen);
                            }

                        }

                        /* output... */
                        if (!gif_id.interlaced) {
                            /* gif is not interlaced */
                            for (int x = 0; x < oclen; x++) {
                                if ((gif_gce.transparent_color) && (oc[x] == gif_gce.transcolor)) {
                                    /* use full transparent pixel */
                                    outlen+=4;
                                    sufbuf+=4;
    
#ifdef GIFTRACE
                                    printf (",--");
#endif
                                }
                                else {
                                    /* create pixel from palette */ 
                                    int ci = oc[x]*3;
    
                                    sufbuf[0] = my_color_table->table[ci+2];
                                    sufbuf[1] = my_color_table->table[ci+1];
                                    sufbuf[2] = my_color_table->table[ci];
                                    sufbuf[3] = 0xff;
    
                                    outlen+=4;
                                    sufbuf+=4;
    
#ifdef GIFTRACE
                                    printf (",%02x", oc[x]);
#endif
                                }
    
                                if (outlen % line_size == 0)
                                    sufbuf+=pitch - line_size; 
                            }
                        }
                        else {
                            /* interlaced gif */
                            for (int x = 0; x < oclen; x++) {
                                if ((gif_gce.transparent_color) && (oc[x] == gif_gce.transcolor)) {
                                    /* use full transparent pixel */
                                    outlen+=4;
                                    sufbuf+=4;
    
#ifdef GIFTRACE
                                    printf (",--");
#endif
                                }
                                else {
                                    /* create pixel from palette */ 
                                    int ci = oc[x]*3;
    
                                    sufbuf[0] = my_color_table->table[ci+2];
                                    sufbuf[1] = my_color_table->table[ci+1];
                                    sufbuf[2] = my_color_table->table[ci];
                                    sufbuf[3] = 0xff;
    
                                    outlen+=4;
                                    sufbuf+=4;
    
#ifdef GIFTRACE
                                    printf (",%02x", oc[x]);
#endif
                                }

                            
                                if (outlen % line_size == 0) {
                                    sufbuf+=pitch - line_size;

                                    switch (interlace_pass) {
                                        case 0:
                                            sufbuf+=pitch * 7;
                                            if (sufbuf >= sufbuf_end) {
                                                sufbuf=sufbuf_start + (pitch << 2);
                                                interlace_pass++;
                                            }
                                            break;
                                        case 1:
                                            sufbuf+=pitch * 7;
                                            if (sufbuf >= sufbuf_end) {
                                                sufbuf=sufbuf_start + (pitch << 1);
                                                interlace_pass++;
                                            }
                                            break;
                                        case 2:
                                            sufbuf+=pitch * 3;
                                            if (sufbuf >= sufbuf_end) {
                                                sufbuf=sufbuf_start + pitch;
                                                interlace_pass++;
                                            }
                                            break;
                                        case 3:
                                            sufbuf+=pitch;
                                            break;
                                    }
                                }
                            
                            }
                        }

                        /* is next_code outside of 0..max_code range? */
                        if (next_code >= max_code) {
                            if (code_size < 12) {
                                /* yes, increase code_size by 1 and double max code */
                                max_code = max_code << 1;
                                code_size++;
                            }
                        }
    
                    } while (bits <= bitlen - code_size);

                    if (end_of_stream)
                        break;
                    
                    oddbits = bitlen - bits; 
                    if (oddbits > 0) {
                        /* save the last bits because the amount is less than the code_size and a next GIF block will come */
                        /* two bytes because code size for GIF is not higher than 12 */
                        if (oddbits > 8) {
                            buffer[0] = buffer[bits >> 3];
                            buffer[1] = buffer[(bits >> 3) + 1];
                        }
                        else
                            buffer[1] = buffer[bits >> 3];
                    }

#ifdef GIFTRACE
                    printf("\n\n");
#endif
                }

                /* stream okay?, check the available data with the given image size */
                if ((!end_of_stream) || (outlen != image_size)) { 
                    /* bad format */
#ifdef GIFTRACE
                    printf("bad format, outlen=%d, needed len=%d\n", outlen, image_size);
#endif
                    dfbsuf->Unlock(dfbsuf);
                    delete newsuf;                        
                    return false;
                }

                dfbsuf->Unlock(dfbsuf);
         
                /* fill the communication structure */
                if (desc->sufcount < MMSIM_MAX_DESC_SUF) {
                    /* mark the next end of the array */
                    desc->suf[desc->sufcount+1].delaytime = MMSIM_DESC_SUF_LOADING;

                    /* fill the surface pointer */
                    desc->suf[desc->sufcount].surface = newsuf;

                    /* the lowest delaytime is 100 milliseconds */
                    if (gif_gce.delaytime > 10)
                        desc->suf[desc->sufcount].delaytime = 10 * (unsigned int)gif_gce.delaytime;
                    else
                        desc->suf[desc->sufcount].delaytime = 100;

                    desc->sufcount++;
                }
                else {
                    /* no free index */
#ifdef GIFTRACE
                    printf("no free index\n");
#endif
                    delete newsuf;
                }
                }
                break;

            case 0x3b:
                /* normal end of GIF stream */

#ifdef GIFTRACE
                printf("TRAILER, end of the GIF stream\n\n");
#endif

                if ((!loop_forever)&&(desc->sufcount>0))
                    /* set delaytime to 0 if the animation should stop at the last image */
                    desc->suf[desc->sufcount-1].delaytime = 0;

                return true;

                break;
        }

    }
    
    return true;

}

void MMSGIFLoader::threadMain() {

    /* start loading */
    this->desc->loading = true;

    /* load some header informations */
    if (loadHeader()) {
        /* load the separated blocks */
        loadBlocks();
    }

    if (this->myfile)
        delete this->myfile;

    /* mark end of list */
    this->desc->suf[this->desc->sufcount].delaytime = MMSIM_DESC_SUF_END;

    /* stop loading */
    this->desc->loading = false;

    delete this;
}

bool isGIF(string file) {
    MMSFile         *myfile;
    unsigned char   *gif_header;
    size_t          count = 0;

    myfile = new MMSFile(file);
    if (!myfile) return false;

    if (!myfile->readBufferEx((void**)&gif_header, &count, 1, 3)) {
        /* cannot read file */
        delete myfile;
        return false;
    }
    delete myfile;

    /* check header */
    if (count < 3) {
        /* it is not an GIF file */
        free(gif_header);
        return false;
    }
    if (memcmp(gif_header, "GIF", 3)!=0) {
        /* it is not an GIF file */
        free(gif_header);
        return false;
    }

    return true;
}

