/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009-2011 BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <pupeider@gmx.de>,                              *
 *      Guido Madaus       <guido.madaus@diskohq.org>,                     *
 *      Patrick Helterhoff <patrick.helterhoff@diskohq.org>,               *
 *      René Bählkow       <rene.baehlkow@diskohq.org>                     *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License version 2.1 as published by the Free Software Foundation.     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 **************************************************************************/

#include "mmsgui/fb/mmsfbfont.h"
#include "mmsgui/fb/mmsfb.h"

#include "mmsgui/ft/mmsfttesselator.h"

#include <ft2build.h>
#include FT_GLYPH_H

// static variables
pthread_mutex_t	globalLock = PTHREAD_MUTEX_INITIALIZER;
void *MMSFBFont::ft_library = NULL;
unsigned int MMSFBFont::numReferences = 0;

#define INITCHECK  if(!this->isInitialized()){MMSFB_SetError(0,"MMSFBFont is not initialized");return false;}

MMSFBFont::MMSFBFont(string filename, int w, int h) :
	initialized(false),
#ifdef __HAVE_DIRECTFB__
	dfbfont(NULL),
#endif
	ft_face(NULL),
	filename(filename),
	ascender(0),
	descender(0),
	height(0) {

    if (mmsfb->backend == MMSFB_BE_DFB) {
#ifdef  __HAVE_DIRECTFB__

		// create the dfb font
		DFBResult   		dfbres;
		DFBFontDescription 	desc;
		if (w > 0) {
			desc.flags = DFDESC_WIDTH;
			desc.width = w;
		}
		if (h > 0) {
			desc.flags = DFDESC_HEIGHT;
			desc.height = h;
		}
		if ((dfbres=mmsfb->dfb->CreateFont(mmsfb->dfb, this->filename.c_str(), &desc, (IDirectFBFont**)&this->dfbfont)) != DFB_OK) {
			MMSFB_SetError(dfbres, "IDirectFB::CreateFont(" + this->filename + ") failed");
			return;
		}
		this->initialized = true;
#endif
    }
    else {
		// init freetype library
		pthread_mutex_lock(&globalLock);
    	if (!ft_library) {
    		if (FT_Init_FreeType((FT_Library*)&this->ft_library)) {
    			MMSFB_SetError(0, "FT_Init_FreeType() failed");
    			this->ft_library = NULL;
				pthread_mutex_unlock(&globalLock);
    			return;
			}
		}

    	this->numReferences++;
		pthread_mutex_unlock(&globalLock);

    	// load the face
    	if (FT_New_Face((FT_Library)this->ft_library, this->filename.c_str(), 0, (FT_Face*)&this->ft_face)) {
    		this->ft_face = NULL;
			MMSFB_SetError(0, "FT_New_Face(" + this->filename + ") failed");
			return;
    	}

    	// select the charmap
    	if (FT_Select_Charmap((FT_Face)this->ft_face, ft_encoding_unicode)) {
    		FT_Done_Face((FT_Face)this->ft_face);
    		this->ft_face = NULL;
			MMSFB_SetError(0, "FT_Select_Charmap(ft_encoding_unicode) for " + this->filename + " failed");
			return;
    	}

    	// set the font size
    	if (FT_Set_Char_Size((FT_Face)this->ft_face, w << 6, h << 6, 0, 0)) {
    		FT_Done_Face((FT_Face)this->ft_face);
    		this->ft_face = NULL;
			MMSFB_SetError(0, "FT_Set_Char_Size(" + iToStr(w << 6) + "," + iToStr(h << 6) + ") for " + this->filename + " failed");
			return;
    	}

    	// try to load a first glyph
    	if (FT_Load_Glyph((FT_Face)this->ft_face, FT_Get_Char_Index((FT_Face)this->ft_face, '0'), FT_LOAD_RENDER)) {
    		FT_Done_Face((FT_Face)this->ft_face);
    		this->ft_face = NULL;
			MMSFB_SetError(0, "FT_Load_Glyph('0') for " + this->filename + " failed");
			return;
    	}

    	if (((FT_Face)this->ft_face)->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
    		FT_Done_Face((FT_Face)this->ft_face);
    		this->ft_face = NULL;
			MMSFB_SetError(0, "Glyph format is not FT_GLYPH_FORMAT_BITMAP for " + this->filename);
			return;
    	}

    	this->ascender = ((FT_Face)this->ft_face)->size->metrics.ascender / 64;
    	this->descender = abs(((FT_Face)this->ft_face)->size->metrics.descender / 64);
    	this->height = this->ascender + this->descender + 1;

    	this->initialized = true;
    }
}

MMSFBFont::~MMSFBFont() {
	lock();

	for (std::map<unsigned int, MMSFBFont_Glyph>::iterator it = this->charmap.begin();
			it != this->charmap.end(); ++it) {

		MMSFBFont_Glyph *glyph = &it->second;

		if (glyph->buffer)
			free(glyph->buffer);

#ifdef  __HAVE_OPENGL__
#ifndef __HAVE_GLU__
		// release texture
		if (mmsfb->bei)
			if (glyph->texture)
				mmsfb->bei->deleteTexture(glyph->texture);
#else
		// release mesh memory
		if (glyph->indices) {
			for (unsigned int m = 0; m < glyph->meshes; m++) {
				MMS3D_INDEX_ARRAY *indices = &glyph->indices[m];
				if (indices->buf)
					free(indices->buf);
			}
			free(glyph->indices);
		}
		if (glyph->vertices) {
			for (unsigned int m = 0; m < glyph->meshes; m++) {
				MMS3D_VERTEX_ARRAY *vertices = &glyph->vertices[m];
				if (vertices->buf)
					free(vertices->buf);
			}
			free(glyph->vertices);
		}

		// release outline memory
		if (glyph->outline_indices) {
			for (unsigned int l = 0; l < glyph->outline_lines; l++) {
				MMS3D_INDEX_ARRAY *indices = &glyph->outline_indices[l];
				if (indices->buf)
					free(indices->buf);
			}
			free(glyph->outline_indices);
		}
		if (glyph->outline_vertices) {
			for (unsigned int m = 0; m < glyph->outline_lines; m++) {
				MMS3D_VERTEX_ARRAY *vertices = &glyph->outline_vertices[m];
				if (vertices->buf)
					free(vertices->buf);
			}
			free(glyph->outline_vertices);
		}

#endif
#endif

	}
	this->charmap.clear();

	if(mmsfb->backend != MMSFB_BE_DFB) {
		if(this->ft_face) {
			FT_Done_Face((FT_Face)this->ft_face);
		}
	}

	unlock();

	pthread_mutex_lock(&globalLock);
	this->numReferences--;

	if(mmsfb->backend != MMSFB_BE_DFB) {
		if(this->ft_library && this->numReferences == 0) {
			FT_Done_FreeType((FT_Library)this->ft_library);
			this->ft_library = NULL;
		}
	}
	pthread_mutex_unlock(&globalLock);
}

bool MMSFBFont::isInitialized() {
    return this->initialized;
}

void MMSFBFont::lock() {
	this->Lock.lock();
}

void MMSFBFont::unlock() {
	this->Lock.unlock();
}
/*
void showGlyphAttributes(FT_GlyphSlot glyph) {
	FT_Glyph_Metrics *metrics = &glyph->metrics;
    FT_Bitmap  *bitmap = &glyph->bitmap;
	FT_Outline *outline = &glyph->outline;

	printf("glyph***\n");

	printf("format = %d\n", glyph->format);

	printf("metrics***\n");
	printf("  width        = %d, %.02f\n", metrics->width, (float)metrics->width / 64);
	printf("  height       = %d, %.02f\n", metrics->height, (float)metrics->height / 64);
	printf("  horiBearingX = %d, %.02f\n", metrics->horiBearingX, (float)metrics->horiBearingX / 64);
	printf("  horiBearingY = %d, %.02f\n", metrics->horiBearingY, (float)metrics->horiBearingY / 64);
	printf("  horiAdvance  = %d, %.02f\n", metrics->horiAdvance, (float)metrics->horiAdvance / 64);
	printf("  vertBearingX = %d, %.02f\n", metrics->vertBearingX, (float)metrics->vertBearingX / 64);
	printf("  vertBearingY = %d, %.02f\n", metrics->vertBearingY, (float)metrics->vertBearingY / 64);
	printf("  vertAdvance  = %d, %.02f\n", metrics->vertAdvance, (float)metrics->vertAdvance / 64);
	printf("***metrics\n");

	printf("advance.x    = %d, %.02f\n", glyph->advance.x, (float)glyph->advance.x / 64);
	printf("advance.y    = %d, %.02f\n", glyph->advance.y, (float)glyph->advance.y / 64);

	printf("bitmap***\n");
	printf("  pitch  = %d\n", bitmap->pitch);
	printf("  width  = %d\n", bitmap->width);
	printf("  height = %d\n", bitmap->rows);
	printf("***bitmap\n");
	printf("bitmap_left = %d\n", glyph->bitmap_left);
	printf("bitmap_top  = %d\n", glyph->bitmap_top);

	printf("outline***\n");
	printf("  n_contours = %d\n", outline->n_contours);
	printf("  n_points   = %d\n", outline->n_points);
	for (int i = 0; i < outline->n_points; i++) {
		printf("    %d, %d\n   >%.02f, %.02f\n", outline->points[i].x, outline->points[i].y,
											  (float)outline->points[i].x / 64, (float)outline->points[i].y / 64);
	}
	printf("  flags = %d\n", outline->flags);
	printf("***outline\n");

	printf("lsb_delta    = %d\n", glyph->lsb_delta);
	printf("rsb_delta    = %d\n", glyph->rsb_delta);

	printf("***glyph\n");
}
*/

void *MMSFBFont::loadFTGlyph(unsigned int character) {
	FT_GlyphSlot g = NULL;

	// load glyph but do NOT render a bitmap
	if (!FT_Load_Glyph((FT_Face)this->ft_face,
		FT_Get_Char_Index((FT_Face)this->ft_face, (FT_ULong)character), FT_LOAD_DEFAULT | FT_LOAD_FORCE_AUTOHINT
//		| FT_LOAD_TARGET_LIGHT
//		| FT_LOAD_TARGET_MONO
//		| FT_LOAD_TARGET_LCD
//		| FT_LOAD_TARGET_LCD_V
		)) {
		g = ((FT_Face)this->ft_face)->glyph;
	} else {
		MMSFB_SetError(0, "FT_Load_Glyph(,,FT_LOAD_DEFAULT) failed for " + this->filename);
	}

/*TEST CODE
	if (!FT_Load_Glyph((FT_Face)this->ft_face,
		FT_Get_Char_Index((FT_Face)this->ft_face, (FT_ULong)character), FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT)) {
		g = ((FT_Face)this->ft_face)->glyph;
	} else {
		MMSFB_SetError(0, "FT_Load_Glyph(,,FT_LOAD_RENDER) failed for " + this->filename);
	}
	showGlyphAttributes(g);
	exit(0);
*/

	if (g) {
#ifdef __HAVE_OPENGL__
		if (!mmsfb->bei) {
#else
		if (1) {
#endif
			// OpenGL is not initialized, we need a bitmap from freetype
			if (g->format != FT_GLYPH_FORMAT_BITMAP) {
				if (FT_Render_Glyph(g, FT_RENDER_MODE_NORMAL)) {
					// failed to load glyph
					MMSFB_SetError(0, "FT_Render_Glyph(,FT_RENDER_MODE_NORMAL) failed for " + this->filename);
					return NULL;
				}
			}

			if (g->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY) {
				// failed to load glyph
				MMSFB_SetError(0, "glyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY for " + this->filename);
				return NULL;
			}
		}
		else {
#ifndef __HAVE_GLU__
			// OpenGL is initialized but GLU is missing, we need a bitmap from freetype
			if (g->format != FT_GLYPH_FORMAT_BITMAP) {
				if (FT_Render_Glyph(g, FT_RENDER_MODE_NORMAL)) {
					// failed to load glyph
					MMSFB_SetError(0, "FT_Render_Glyph(,FT_RENDER_MODE_NORMAL) failed for " + this->filename);
					return NULL;
				}
			}

			if (g->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY) {
				// failed to load glyph
				MMSFB_SetError(0, "glyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY for " + this->filename);
				return NULL;
			}
#endif
		}

		// successfully loaded
		return g;
	}
	else {
		// failed to load glyph
		return NULL;
	}
}


bool MMSFBFont::setupFTGlyph(void *ftg, MMSFBFont_Glyph *glyph) {
	if (!ftg || !glyph) return false;
	FT_GlyphSlot g = (FT_GlyphSlot)ftg;

#ifdef __HAVE_OPENGL__
	if (!mmsfb->bei) {
#else
	if (1) {
#endif
		// OpenGL is not initialized, setup glyph for software rendering
		glyph->left		= g->bitmap_left;
		glyph->top		= g->bitmap_top;
		glyph->width	= g->bitmap.width;
		glyph->height	= g->bitmap.rows;
		glyph->advanceX	= g->advance.x / 64;
		glyph->pitch	= g->bitmap.pitch;
		glyph->buffer	= (unsigned char*)calloc(1, glyph->pitch * glyph->height);
		memcpy(glyph->buffer, g->bitmap.buffer, glyph->pitch * glyph->height);

		if (MMSFBBase_rotate180) {
			// rotate glyph by 180°
			rotateUCharBuffer180(glyph->buffer, glyph->pitch, glyph->width, glyph->height);
		}

		return true;
	}

#ifndef __HAVE_GLU__
#ifdef  __HAVE_OPENGL__
	// OpenGL is initialized but GLU is missing, we need a bitmap from freetype
	glyph->left		= g->bitmap_left;
	glyph->top		= g->bitmap_top;
	glyph->width	= g->bitmap.width;
	glyph->height	= g->bitmap.rows;
	glyph->advanceX	= g->advance.x / 64;

	// add glyph to charmap, we use a pitch which is a multiple of 4 needed e.g. for OGL textures
	if(mmsfb->bei && (g->bitmap.pitch & 3)) {
		glyph->pitch = (g->bitmap.pitch & ~3) + 4;
		glyph->buffer = (unsigned char*)calloc(1, glyph->pitch * glyph->height);
		unsigned char *src = g->bitmap.buffer;
		unsigned char *dst = glyph->buffer;
		for(int i = 0; i < glyph->height; i++) {
			memcpy(dst, src, glyph->pitch);
			src += g->bitmap.pitch;
			dst += glyph->pitch;
		}
	}
	else {
		glyph->pitch  = g->bitmap.pitch;
		glyph->buffer = (unsigned char*)calloc(1, glyph->pitch * glyph->height);
		memcpy(glyph->buffer, g->bitmap.buffer, glyph->pitch * glyph->height);
	}

	if (MMSFBBase_rotate180) {
		// rotate glyph by 180°
		rotateUCharBuffer180(glyph->buffer, glyph->pitch, glyph->width, glyph->height);
	}

	// create a texture for this glyph
	glyph->texture = 0;
	mmsfb->bei->createAlphaTexture(&glyph->texture, glyph->buffer,
							glyph->pitch, glyph->height);
/*TODO: createAlphaTexture() can block the application
	glGenTextures(1, &glyph->texture);
	glBindTexture(GL_TEXTURE_2D, glyph->texture);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_ALPHA,
		glyph->pitch,
		glyph->height,
		0,
		GL_ALPHA,
		GL_UNSIGNED_BYTE,
		glyph->buffer);
*/
	return true;
#endif

#else
	// OpenGL is initialized and GLU is available, we create meshes based on freetype outlines
	MMSFTTesselator *ftv = new MMSFTTesselator(g);
    ftv->generateGlyph();
    const MMSFTGlyph *ftglyph = ftv->getGlyph();
    if (!ftglyph) {
	    // glyph not generated
		MMSFB_SetError(0, "MMSFTTesselator::generateGlyph() failed");
		delete ftv;
		return false;
    }

    // init glyph basics
    glyph->buffer	= NULL;
    glyph->pitch	= 0;
	glyph->left		= g->metrics.horiBearingX / 64;
	glyph->top		= g->metrics.horiBearingY / 64;
	glyph->width	= g->metrics.width / 64;
	glyph->height	= g->metrics.height / 64;
	glyph->advanceX	= g->advance.x / 64;

	// init glyph mesh description
	glyph->max_meshes	= 0;
    glyph->meshes		= 0;
    glyph->indices		= NULL;
    glyph->vertices		= NULL;

    // init glyph outline description
    glyph->outline_max_lines= 0;
    glyph->outline_lines	= 0;
    glyph->outline_indices	= NULL;
    glyph->outline_vertices	= NULL;

	if (!ftglyph->getMeshCount()) {
		// no meshes available, but o.k. (e.g. space char)
		delete ftv;
		return true;
	}

    // count max meshes
	for (unsigned int m = 0; m < ftglyph->getMeshCount(); m++) {
		if (!ftglyph->getMesh(m)) continue;
		glyph->max_meshes++;
		if (glyph->max_meshes >= MMSFBFONT_GLYPH_MAX_MESHES) {
			printf("MMSFBFONT_GLYPH_MAX_MESHES(%u) reached, %u needed\n", MMSFBFONT_GLYPH_MAX_MESHES, ftglyph->getMeshCount());
		}
	}

	if (!glyph->max_meshes) {
		// no meshes available
		MMSFB_SetError(0, "no meshes available");
		delete ftv;
		return false;
	}

    // allocate base buffer for vertices and indices
	// we do not need to clear because all fields will be set separately
	glyph->indices = (MMS3D_INDEX_ARRAY*)malloc(sizeof(MMS3D_INDEX_ARRAY) * glyph->max_meshes);
	glyph->vertices = (MMS3D_VERTEX_ARRAY*)malloc(sizeof(MMS3D_VERTEX_ARRAY) * glyph->max_meshes);

    // for all meshes
	for (unsigned int m = 0; m < ftglyph->getMeshCount(); m++) {
		// prepare access to vertices and indices of glyph
		if (glyph->meshes >= glyph->max_meshes) {
			printf("glyph->max_meshes(%u) reached\n", glyph->max_meshes);
			break;
		}
		MMS3D_INDEX_ARRAY  *indices  = &glyph->indices[glyph->meshes];
		MMS3D_VERTEX_ARRAY *vertices = &glyph->vertices[glyph->meshes];

		// get access to polygon data
		const MMSFTMesh *ftmesh = ftglyph->getMesh(m);
		if (!ftmesh) continue;

		// prepare indices
		// note: no need to allocate index buffer, because vertices are correctly sorted
		indices->type = MMS3D_INDEX_ARRAY_TYPE_TRIANGLES;
		switch (ftmesh->getMeshType()) {
		case GL_TRIANGLES:
			indices->type = MMS3D_INDEX_ARRAY_TYPE_TRIANGLES;
			break;
		case GL_TRIANGLE_STRIP:
			indices->type = MMS3D_INDEX_ARRAY_TYPE_TRIANGLES_STRIP;
			break;
		case GL_TRIANGLE_FAN:
			indices->type = MMS3D_INDEX_ARRAY_TYPE_TRIANGLES_FAN;
			break;
		default:
			// unsupported type
			printf("MMSFBFont: unsupported mesh type %u\n", ftmesh->getMeshType());
			delete ftv;
			return false;
		}
		indices->eNum = 0;
		indices->buf  = NULL;

		// prepare vertices
		vertices->eSize = 2;
		vertices->eNum  = ftmesh->getVertexCount();
		vertices->buf   = (float *)malloc(sizeof(float) * vertices->eSize * vertices->eNum);

		// for all vertices in the polygon
		for (unsigned int v = 0; v < ftmesh->getVertexCount(); v++) {
			const MMSFTVertex &vertex = ftmesh->getVertex(v);
			vertices->buf[v * vertices->eSize + 0] = (float)(vertex.X() - g->metrics.horiBearingX) / 64;
			vertices->buf[v * vertices->eSize + 1] = (float)(g->metrics.horiBearingY - vertex.Y()) / 64;
//			vertices->buf[v * vertices->eSize + 0] = ((float)(vertex.X() - g->metrics.horiBearingX) * 1000) / 64;
//			vertices->buf[v * vertices->eSize + 1] = ((float)(g->metrics.horiBearingY - vertex.Y()) * 1000) / 64;

//			printf("%02f, %f\n", vertices->buf[v * vertices->eSize + 0], vertices->buf[v * vertices->eSize + 1]);

//			vertices->buf[v * vertices->eSize + 0] = vertices->buf[v * vertices->eSize + 0] * 20;
//			vertices->buf[v * vertices->eSize + 1] = vertices->buf[v * vertices->eSize + 1] * 20;
		}

		// next mesh
		glyph->meshes++;
	}

/*glyph->left*=20;
glyph->width*=20;
glyph->height*=20;
glyph->advanceX*=20;*/


	if (ftv->getContourCount() > 0) {
		// add outline primitives
		glyph->outline_max_lines = ftv->getContourCount();

	    // allocate base buffer for vertices and indices
		// we do not need to clear because all fields will be set separately
		glyph->outline_indices = (MMS3D_INDEX_ARRAY*)malloc(sizeof(MMS3D_INDEX_ARRAY) * glyph->outline_max_lines);
		glyph->outline_vertices = (MMS3D_VERTEX_ARRAY*)malloc(sizeof(MMS3D_VERTEX_ARRAY) * glyph->outline_max_lines);

	    // for all contours (outlines)
		for (unsigned int c = 0; c < ftv->getContourCount(); c++) {
			// prepare access to vertices and indices of glyph
			if (glyph->outline_lines >= glyph->outline_max_lines) {
				printf("glyph->outline_max_lines(%u) reached\n", glyph->outline_max_lines);
				break;
			}
			MMS3D_INDEX_ARRAY  *indices  = &glyph->outline_indices[glyph->outline_lines];
			MMS3D_VERTEX_ARRAY *vertices = &glyph->outline_vertices[glyph->outline_lines];

			// get access to contour data
			const MMSFTContour *ftcontour = ftv->getContour(c);
			if (!ftcontour) continue;
			FT_Vector *outlineVertexList;
	        char *outlineVertexTags;
	        unsigned int outlineNumVertices;
	        outlineNumVertices = ftcontour->getOutlineVertices(&outlineVertexList, &outlineVertexTags);

			// prepare indices
			// note: no need to allocate index buffer, because vertices are correctly sorted
			indices->type = MMS3D_INDEX_ARRAY_TYPE_LINES_LOOP;
			indices->eNum = 0;
			indices->buf  = NULL;

			// prepare vertices
			vertices->eSize = 2;
			vertices->eNum  = outlineNumVertices;
			vertices->buf   = (float *)malloc(sizeof(float) * vertices->eSize * vertices->eNum);

			for (unsigned int v = 0; v < outlineNumVertices; v++) {
				FT_Vector *vertex = &outlineVertexList[v];
//				vertices->buf[v * vertices->eSize + 0] = (float)((vertex->x+32) - g->metrics.horiBearingX) / 64;
//				vertices->buf[v * vertices->eSize + 1] = (float)(g->metrics.horiBearingY - (vertex->y-32)) / 64;
				vertices->buf[v * vertices->eSize + 0] = (float)(vertex->x - g->metrics.horiBearingX) / 64;
				vertices->buf[v * vertices->eSize + 1] = (float)(g->metrics.horiBearingY - vertex->y) / 64;
			}

			// next outline
			glyph->outline_lines++;
		}
	}



    // all is successfully done
	delete ftv;
	return true;

#endif

	return false;
}

bool MMSFBFont::getGlyph(unsigned int character, MMSFBFont_Glyph *glyph) {
	if (!glyph) {
		return false;
	}

    if (mmsfb->backend == MMSFB_BE_DFB) {
#ifdef  __HAVE_DIRECTFB__
#endif
    }
    else {
    	if(!this->ft_face) {
    		return false;
    	}

    	bool ret = false;

    	lock();

    	// check if requested character is already loaded
    	std::map<unsigned int, MMSFBFont_Glyph>::iterator it;
    	it = this->charmap.find(character);
    	if (it == this->charmap.end()) {
    		// no, have to load it
			FT_GlyphSlot g;
			if (!(g = (FT_GlyphSlot)loadFTGlyph(character))) {
				// failed to load glyph
				unlock();
				return false;
			}

			// setup glyph values
			if (!setupFTGlyph(g, glyph)) {
				// failed to setup glyph
				unlock();
				return false;
			}

			// add to charmap
			this->charmap.insert(std::make_pair(character, *glyph));
			ret = true;
    	}
    	else {
    		// already loaded
    		*glyph = it->second;
			ret = true;
    	}

    	unlock();

    	return ret;
    }

    return false;
}


bool MMSFBFont::getStringWidth(string text, int len, int *width) {
    // check if initialized
    INITCHECK;

    // reset width
    if (!width) return false;
	*width = 0;

	// get the length of the string
	if (len < 0) len = text.size();
	if (!len) return true;

    // get the width of the whole string
#ifdef  __HAVE_DIRECTFB__
    if (this->dfbfont) {
		if (((IDirectFBFont*)this->dfbfont)->GetStringWidth((IDirectFBFont*)this->dfbfont, text.c_str(), len, width) != DFB_OK)
			return false;
		return true;
    } else
#endif
    {
    	MMSFBFONT_GET_UNICODE_CHAR(text, len) {
    		MMSFBFont_Glyph glyph;
    		if (!getGlyph(character, &glyph)) break;
			(*width)+=glyph.advanceX;
    	} }
    	return true;
    }
    return false;
}

bool MMSFBFont::getHeight(int *height) {
    // check if initialized
    INITCHECK;

    // get the height of the font
#ifdef  __HAVE_DIRECTFB__
	if (this->dfbfont) {
		if (((IDirectFBFont*)this->dfbfont)->GetHeight((IDirectFBFont*)this->dfbfont, height) != DFB_OK)
			return false;
		return true;
    } else
#endif
	{
    	*height = this->height;
    	return true;
    }
    return false;
}

bool MMSFBFont::getAscender(int *ascender) {
    // check if initialized
    INITCHECK;

    // get the ascender of the font
#ifdef __HAVE_DIRECTFB__
    if (this->dfbfont) {
	} else
#endif
	{
		*ascender = this->ascender;
		return true;
	}
	return false;
}

bool MMSFBFont::getDescender(int *descender) {
    // check if initialized
    INITCHECK;

    // get the ascender of the font
#ifdef __HAVE_DIRECTFB__
    if (this->dfbfont) {
	}
	else
#endif
	{
		*descender = this->descender;
		return true;
	}
	return false;
}
