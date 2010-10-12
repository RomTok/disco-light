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

#ifdef __HAVE_OPENGL__

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

    	typedef enum {
    		MMSFBGL_SHADER_TYPE_FRAGMENT_SHADER = 0,
    		MMSFBGL_SHADER_TYPE_VERTEX_SHADER
    	} MMSFBGL_SHADER_TYPE;

#ifdef __HAVE_XLIB__
    	Display *x_display;
    	Window 	x_window;
#endif

#ifdef __HAVE_GLX__
    	XVisualInfo *xvi;
    	GLXContext	glx_context;
#endif

#ifdef __HAVE_EGL__
    	EGLDisplay eglDisplay;
    	EGLConfig  eglConfig[10];
    	EGLSurface eglSurface;
    	EGLContext eglContext;
#endif

    	bool initialized;

    	int screen_width;
    	int screen_height;

    	GLuint bound_fbo;

    	//! program handle to the fragment and vertex shader used for drawing primitives
    	GLuint po_draw;

    	//! program handle to the fragment and vertex shader used for blitting
    	GLuint po_blit;

    	//! program handle to the fragment and vertex shader used for blitting with source modulation beforehand
    	GLuint po_modulateblit;

    	//! program handle to the fragment and vertex shader used for blitting from alpha
    	GLuint po_blit_fromalpha;

    	//! program handle to the fragment and vertex shader used for blitting from alpha source and modulation beforehand
    	GLuint po_modulateblit_fromalpha;

    	//! currently active program
    	GLuint po_current;

    	//! matrix location in the vertex shader
    	GLint VSMatrixLoc;

    	//! matrix location in the vertex shader initialized
    	bool VSMatrixLoc_initialized;

    	//! color location in the fragment shader
    	GLint FSColorLoc;

    	//! color location in the fragment shader initialized
    	bool FSColorLoc_initialized;

    	//! current matrix
    	MMSFBGLMatrix	current_matrix;

    	//! current color
    	unsigned char	current_color_r;
    	unsigned char	current_color_g;
    	unsigned char	current_color_b;
    	unsigned char	current_color_a;

    	bool getError(const char* where);

    	bool buildShader(MMSFBGL_SHADER_TYPE shader_type, const char *shader_code, GLuint *shader);
    	bool linkProgram(GLuint fragment_shader, GLuint vertex_shader, GLuint *program);
    	bool buildShaderProgram(const char *fragment_shader_code, const char *vertex_shader_code, GLuint *program);
    	bool buildShaderProgram4Drawing(GLuint *program);
    	bool buildShaderProgram4Blitting(GLuint *program);
    	bool buildShaderProgram4ModulateBlitting(GLuint *program);
    	bool buildShaderProgram4BlittingFromAlpha(GLuint *program);
    	bool buildShaderProgram4ModulateBlittingFromAlpha(GLuint *program);

    	void deleteShaders();
        bool initShaders();

    public:
        MMSFBGL();
        ~MMSFBGL();

#ifdef __HAVE_XLIB__
        bool init(Display *x_display, int x_screen, Window x_window, int w, int h);
#else
        bool init();
#endif
    	bool terminate();
    	bool getResolution(int *w, int *h);
    	bool swap();

        bool genTexture(GLuint *tex);
        bool deleteTexture(GLuint tex);
        bool bindTexture2D(GLuint tex);
        bool initTexture2D(GLuint tex, GLenum texture_format, void *buffer, GLenum buffer_format, int sw, int sh);
        bool initSubTexture2D(GLuint tex, void *buffer, GLenum buffer_format, int sw, int sh, int dx, int dy);

        bool genFrameBuffer(GLuint *fbo);
        bool deleteFrameBuffer(GLuint fbo);
        bool genRenderBuffer(GLuint *rbo);
        bool deleteRenderBuffer(GLuint rbo);
        bool attachTexture2FrameBuffer(GLuint fbo, GLuint tex);
        bool attachRenderBuffer2FrameBuffer(GLuint fbo, GLuint rbo, int width, int height);

        bool allocTexture(GLuint tex, int width, int height);
        bool allocFBO(GLuint fbo, GLuint tex, int width, int height);
        bool allocFBOandRBO(GLuint fbo, GLuint tex, GLuint rbo, int width, int height);
    	bool freeFBO(GLuint fbo, GLuint tex, GLuint rbo = 0);

    	bool bindFrameBuffer(GLuint ogl);
    	bool setScissor(GLint x, GLint y, GLsizei width, GLsizei height);
    	bool disableScissor();

        void enableBlend();
        void disableBlend();
        void enableDepthTest();
        void disableDepthTest();
        void disableTexture2D();
		void setDrawingMode();
		void setTexEnvReplace(GLenum format);
		void setTexEnvModulate(GLenum format);

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
        bool useShaderProgram4BlittingFromAlpha();
        bool useShaderProgram4ModulateBlittingFromAlpha();

        bool setCurrentMatrix(MMSFBGLMatrix matrix);
        bool rotateCurrentMatrix(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

        bool getModelViewMatrix(MMSFBGLMatrix result, float left, float right, float bottom, float top, float nearZ, float farZ);
        bool setModelViewMatrix(float left, float right, float bottom, float top, float nearZ, float farZ);

        bool clear(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
        bool setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

        bool drawRectangle2D(float x1, float y1, float x2, float y2);
        bool drawRectangle2Di(int x1, int y1, int x2, int y2);


        bool fillTriangle(float x1, float y1, float z1,
							 float x2, float y2, float z2,
							 float x3, float y3, float z3);
        bool fillTriangle2D(float x1, float y1, float x2, float y2, float x3, float y3);

        bool fillRectangle2D(float x1, float y1, float x2, float y2);
        bool fillRectangle2Di(int x1, int y1, int x2, int y2);


        bool stretchBlit3D(GLuint src_tex, float sx1, float sy1, float sx2, float sy2,
										  float dx1, float dy1, float dz1,
										  float dx2, float dy2, float dz2,
										  float dx3, float dy3, float dz3,
										  float dx4, float dy4, float dz4);

        bool stretchBlit(GLuint src_tex, float sx1, float sy1, float sx2, float sy2,
										  float dx1, float dy1, float dx2, float dy2);
        bool stretchBliti(GLuint src_tex, int sx1, int sy1, int sx2, int sy2, int sw, int sh,
										   int dx1, int dy1, int dx2, int dy2);



        bool stretchBlitBuffer(void *buffer, float sx1, float sy1, float sx2, float sy2, int sw, int sh,
											  float dx1, float dy1, float dx2, float dy2);
        bool stretchBlitBufferi(void *buffer, int sx1, int sy1, int sx2, int sy2, int sw, int sh,
										   int dx1, int dy1, int dx2, int dy2);

        bool blitBuffer2Texture(GLuint dst_tex, bool realloc, void *buffer, int sw, int sh);
};

#endif

#endif /* MMSFBGL_H_ */