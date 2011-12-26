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

#include "mmsgui/ft/mmsfttesselator.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifdef __HAVE_GLU__
#include <GL/glu.h>

// GLU_TESS_BEGIN_DATA callback
void gluTBeginData(GLenum type, MMSFTGlyph *glyph) {
	glyph->tessBegin(type);
}

// GLU_TESS_VERTEX_DATA callback
void gluTVertexData(void *data, MMSFTGlyph *glyph) {
    double* vertex = static_cast<double*>(data);
    glyph->tessVertex(vertex[0], vertex[1], vertex[2]);
}

// GLU_TESS_COMBINE_DATA callback
void gluTCombineData(double coords[3], void *vertex_data[4], GLfloat weight[4], void **outData, MMSFTGlyph *glyph) {
    const double *vertex = static_cast<const double*>(coords);
    *outData = const_cast<double*>(glyph->tessCombine(vertex[0], vertex[1], vertex[2]));
}

// GLU_TESS_END_DATA callback
void gluTEndData(MMSFTGlyph *glyph) {
	glyph->tessEnd();
}

// GLU_TESS_ERROR_DATA callback
void gluTErrorData(GLenum errCode, MMSFTGlyph *glyph) {
    glyph->tessError(errCode);
}
#endif


MMSFTTesselator::MMSFTTesselator(const FT_GlyphSlot glyph) {
    this->glyph = NULL;
    this->contourList = NULL;
    this->ftContourCount = 0;

	if (!glyph) return;

	outline = glyph->outline;

	ftContourCount = outline.n_contours;
	contourList = NULL;
	contourFlag = outline.flags;

	processContours();
}


MMSFTTesselator::~MMSFTTesselator() {
    for (size_t c = 0; c < this->ftContourCount; c++) {
        delete contourList[c];
    }
    delete [] contourList;
    delete this->glyph;
}


void MMSFTTesselator::processContours() {
    int contourLength = 0;
    int startIndex = 0;
    int endIndex = 0;

    contourList = new MMSFTContour*[ftContourCount];

    for (int i = 0; i < ftContourCount; i++) {
        FT_Vector* pointList = &outline.points[startIndex];
        char* tagList = &outline.tags[startIndex];

        endIndex = outline.contours[i];
        contourLength =  (endIndex - startIndex) + 1;

        MMSFTContour* contour = new MMSFTContour(pointList, tagList, contourLength);

        contourList[i] = contour;

        startIndex = endIndex + 1;
    }

    // Compute each contour's parity.
    // FIXME: see if FT_Outline_Get_Orientation can do it for us.
    for (int i = 0; i < ftContourCount; i++) {
        MMSFTContour *c1 = contourList[i];

        // 1. Find the leftmost point.
        MMSFTVertex leftmost(65536.0, 0.0);

        for (size_t n = 0; n < c1->getVertexCount(); n++) {
            MMSFTVertex p = c1->Vertex(n);
            if(p.X() < leftmost.X())
            {
                leftmost = p;
            }
        }

        // 2. Count how many other contours we cross when going further to
        // the left.
        int parity = 0;

        for (int j = 0; j < ftContourCount; j++) {
            if (j == i) continue;

            MMSFTContour *c2 = contourList[j];

            for (size_t n = 0; n < c2->getVertexCount(); n++) {
                MMSFTVertex p1 = c2->Vertex(n);
                MMSFTVertex p2 = c2->Vertex((n + 1) % c2->getVertexCount());

                /* FIXME: combinations of >= > <= and < do not seem stable */
                if((p1.Y() < leftmost.Y() && p2.Y() < leftmost.Y())
                    || (p1.Y() >= leftmost.Y() && p2.Y() >= leftmost.Y())
                    || (p1.X() > leftmost.X() && p2.X() > leftmost.X()))
                {
                    continue;
                }
                else if(p1.X() < leftmost.X() && p2.X() < leftmost.X())
                {
                    parity++;
                }
                else
                {
                    MMSFTVertex a = p1 - leftmost;
                    MMSFTVertex b = p2 - leftmost;
                    if(b.X() * a.Y() > b.Y() * a.X())
                    {
                        parity++;
                    }
                }
            }
        }

        // 3. Make sure the glyph has the proper parity.
        c1->setParity(parity);
    }
}


bool MMSFTTesselator::generateGlyph(double zNormal, int outsetType, float outsetSize) {
#ifdef __HAVE_GLU__
	// new glyph
	if (this->glyph) delete this->glyph;
	this->glyph = new MMSFTGlyph();

    GLUtesselator* tobj = gluNewTess();

    gluTessCallback(tobj, GLU_TESS_BEGIN_DATA,     (_GLUfuncptr)gluTBeginData);
    gluTessCallback(tobj, GLU_TESS_VERTEX_DATA,    (_GLUfuncptr)gluTVertexData);
    gluTessCallback(tobj, GLU_TESS_COMBINE_DATA,   (_GLUfuncptr)gluTCombineData);
    gluTessCallback(tobj, GLU_TESS_END_DATA,       (_GLUfuncptr)gluTEndData);
    gluTessCallback(tobj, GLU_TESS_ERROR_DATA,     (_GLUfuncptr)gluTErrorData);

    if(contourFlag & ft_outline_even_odd_fill) // ft_outline_reverse_fill
    {
        gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
    }
    else
    {
        gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
    }

    gluTessProperty(tobj, GLU_TESS_TOLERANCE, 0);
    gluTessNormal(tobj, 0.0f, 0.0f, zNormal);
    gluTessBeginPolygon(tobj, this->glyph);

	for (size_t c = 0; c < this->ftContourCount; c++) {
		switch(outsetType) {
			case 1:
				contourList[c]->buildFrontOutset(outsetSize);
				break;
			case 2:contourList[c]->buildBackOutset(outsetSize);
				break;
		}
		const MMSFTContour* contour = contourList[c];

		gluTessBeginContour(tobj);

		for (size_t p = 0; p < contour->getVertexCount(); ++p) {
			const double* d;
			switch(outsetType) {
				case 1:
					d = contour->FrontVertex(p);
					break;
				case 2:
					d = contour->BackVertex(p);
					break;
				case 0:
				default:
					d = contour->Vertex(p);
					break;
			}

			gluTessVertex(tobj, (GLdouble *)d, (GLvoid *)d);
		}

		gluTessEndContour(tobj);
	}

	gluTessEndPolygon(tobj);
	gluDeleteTess(tobj);

	if (this->glyph->getErrorCode()) {
		// failed
		delete this->glyph;
		this->glyph = NULL;
		return false;
	}

	return true;
#else
	return false;
#endif
}

const MMSFTGlyph* const MMSFTTesselator::getGlyph() const {
	return this->glyph;
}

