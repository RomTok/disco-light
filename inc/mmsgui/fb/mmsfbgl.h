/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      BerLinux Solutions GmbH                       *
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

#ifndef MMSFBGL_H_
#define MMSFBGL_H_

#ifdef __HAVE_GL2__
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#ifdef __HAVE_GLX__
#include <GL/glx.h>
#include <GL/glu.h>
#endif

#ifdef __HAVE_GLES2__
#include <GLES2/gl2.h>
#endif

#ifdef __HAVE_EGL__
#include <EGL/egl.h>
#endif

typedef GLfloat MMSFBGLMatrix[4][4];

//! Wrapper class for all supported Open GL versions.
/*!
\author Jens Schneider
*/
class MMSFBGL {
    private:
    	// bind the "VSVertex" attribute to shader index 0
    	#define MMSFBGL_VSV_LOC	0

		#define MMSFBGL_CALC_2X(v1, v2) (((v1)<(v2)) ? (float)(v1) : (float)(v1) + 0.99)
		#define MMSFBGL_CALC_2Y(v1, v2) (((v1)<(v2)) ? (float)(v1) : (float)(v1) + 0.99)

    	typedef enum {
    		MMSFBGL_SHADER_TYPE_FRAGMENT_SHADER = 0,
    		MMSFBGL_SHADER_TYPE_VERTEX_SHADER
    	} MMSFBGL_SHADER_TYPE;

#ifdef __HAVE_EGL__

    	EGLDisplay eglDisplay;
    	EGLConfig  eglConfig[10];
    	EGLSurface eglSurface;
    	EGLContext eglContext;

#endif

    	bool initialized;

    	int screen_width;
    	int screen_height;

    	//! program handle to the fragment and vertex shader used for drawing primitives
    	GLuint po_draw;

    	//! program handle to the fragment and vertex shader used for blitting
    	GLuint po_blit;

    	//! program handle to the fragment and vertex shader used for blitting with source modulation beforehand
    	GLuint po_modulateblit;

    	//! currently active program
    	GLuint po_current;


    	GLint VSMatrixLoc;
    	bool VSMatrixLoc_initialized;
    	GLint FSColorLoc;
    	bool FSColorLoc_initialized;


    	bool getError(const char* where);

    	bool buildShader(MMSFBGL_SHADER_TYPE shader_type, const char *shader_code, GLuint *shader);
    	bool linkProgram(GLuint fragment_shader, GLuint vertex_shader, GLuint *program);
    	bool buildShaderProgram(const char *fragment_shader_code, const char *vertex_shader_code, GLuint *program);
    	bool buildShaderProgram4Drawing(GLuint *program);
    	bool buildShaderProgram4Blitting(GLuint *program);
    	bool buildShaderProgram4ModulateBlitting(GLuint *program);

    	void deleteShaders();
        bool initShaders();

    public:
        MMSFBGL();
        ~MMSFBGL();

        bool init();
    	bool terminate();
    	bool getResolution(int *w, int *h);
    	bool swap();
    	bool alloc(int width, int height, GLuint *ogl_fbo, GLuint *ogl_tex, GLuint *ogl_rb);
    	bool free(GLuint ogl_fbo, GLuint ogl_rb, GLuint ogl_tex);
    	bool bindFrameBuffer(GLuint ogl_fbo);

        void matrixMultiply(MMSFBGLMatrix result, MMSFBGLMatrix srcA, MMSFBGLMatrix srcB);
        void matrixLoadIdentity(MMSFBGLMatrix result);
        void scale(MMSFBGLMatrix result, GLfloat sx, GLfloat sy, GLfloat sz);
        void translate(MMSFBGLMatrix result, GLfloat tx, GLfloat ty, GLfloat tz);
        void rotate(MMSFBGLMatrix result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
        void frustum(MMSFBGLMatrix result, float left, float right, float bottom, float top, float nearZ, float farZ);
        void perspective(MMSFBGLMatrix result, float fovy, float aspect, float nearZ, float farZ);
        void ortho(MMSFBGLMatrix result, float left, float right, float bottom, float top, float nearZ, float farZ);

        bool useShaderProgram4Drawing();
        bool useShaderProgram4Blitting();
        bool useShaderProgram4ModulateBlitting();

        bool setModelViewMatrix(int w, int h);
        bool clear(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
        bool setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
        bool enableBlend();
        bool disableBlend();
        bool fillTriangle(float x1, float y1, float z1,
							 float x2, float y2, float z2,
							 float x3, float y3, float z3);
        bool fillTriangle2D(float x1, float y1, float x2, float y2, float x3, float y3);
        bool fillRectangle3D(float x1, float y1, float z1, float x2, float y2, float z2);
        bool fillRectangle2D(float x1, float y1, float x2, float y2);
        bool fillRectangle2Di(int x1, int y1, int x2, int y2);

        bool stretchBlit(GLuint src_tex, int sw, int sh, int dx, int dy, int dw, int dh);
        bool stretchBlitBuffer(void *buffer, int sw, int sh, int dx, int dy, int dw, int dh);
};


#endif /* MMSFBGL_H_ */
