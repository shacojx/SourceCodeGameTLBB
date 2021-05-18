#ifndef _GLPROCS_H_
#define _GLPROCS_H_

/*
** GLprocs utility for getting function addresses for OpenGL(R) 1.2, 
** OpenGL 1.3 and OpenGL extension functions.
**
** Version:  1.0
**
** License Applicability. Except to the extent portions of this file are
** made subject to an alternative license as permitted in the SGI Free
** Software License B, Version 1.1 (the "License"), the contents of this
** file are subject only to the provisions of the License. You may not use
** this file except in compliance with the License. You may obtain a copy
** of the License at Silicon Graphics, Inc., attn: Legal Services, 1600
** Amphitheatre Parkway, Mountain View, CA 94043-1351, or at:
** 
** http://oss.sgi.com/projects/FreeB
** 
** Note that, as provided in the License, the Software is distributed on an
** "AS IS" basis, with ALL EXPRESS AND IMPLIED WARRANTIES AND CONDITIONS
** DISCLAIMED, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTIES AND
** CONDITIONS OF MERCHANTABILITY, SATISFACTORY QUALITY, FITNESS FOR A
** PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
** 
** Original Code. The Original Code is: OpenGL Sample Implementation,
** Version 1.2.1, released January 26, 2000, developed by Silicon Graphics,
** Inc. The Original Code is Copyright (c) 1991-2000 Silicon Graphics, Inc.
** Copyright in any portions created by third parties is as indicated
** elsewhere herein. All Rights Reserved.
** 
** Additional Notice Provisions: This software was created using the
** OpenGL(R) version 1.2.1 Sample Implementation published by SGI, but has
** not been independently verified as being compliant with the OpenGL(R)
** version 1.2.1 Specification.
**
** Initial version of glprocs.{c,h} contributed by Intel(R) Corporation.
*/

#ifdef _WIN32
  #include "glext.h"
  #include "wglext.h"
#else
  #include <GL/glext.h>
#endif

#ifndef _WIN32 /* non-Windows environment */
  #define APIENTRY
  #ifdef __GNUC__
    #define _inline __inline__
  #else
    #define _inline
  #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Structure of all OpenGL {1.2, 1.3}, GL extension procs.*/

typedef struct {
  void (APIENTRY *BlendColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
  void (APIENTRY *BlendEquation) (GLenum mode);
  void (APIENTRY *DrawRangeElements) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
  void (APIENTRY *ColorTable) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
  void (APIENTRY *ColorTableParameterfv) (GLenum target, GLenum pname, const GLfloat *params);
  void (APIENTRY *ColorTableParameteriv) (GLenum target, GLenum pname, const GLint *params);
  void (APIENTRY *CopyColorTable) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
  void (APIENTRY *GetColorTable) (GLenum target, GLenum format, GLenum type, GLvoid *table);
  void (APIENTRY *GetColorTableParameterfv) (GLenum target, GLenum pname, GLfloat *params);
  void (APIENTRY *GetColorTableParameteriv) (GLenum target, GLenum pname, GLint *params);
  void (APIENTRY *ColorSubTable) (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data);
  void (APIENTRY *CopyColorSubTable) (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
  void (APIENTRY *ConvolutionFilter1D) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image);
  void (APIENTRY *ConvolutionFilter2D) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image);
  void (APIENTRY *ConvolutionParameterf) (GLenum target, GLenum pname, GLfloat params);
  void (APIENTRY *ConvolutionParameterfv) (GLenum target, GLenum pname, const GLfloat *params);
  void (APIENTRY *ConvolutionParameteri) (GLenum target, GLenum pname, GLint params);
  void (APIENTRY *ConvolutionParameteriv) (GLenum target, GLenum pname, const GLint *params);
  void (APIENTRY *CopyConvolutionFilter1D) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
  void (APIENTRY *CopyConvolutionFilter2D) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
  void (APIENTRY *GetConvolutionFilter) (GLenum target, GLenum format, GLenum type, GLvoid *image);
  void (APIENTRY *GetConvolutionParameterfv) (GLenum target, GLenum pname, GLfloat *params);
  void (APIENTRY *GetConvolutionParameteriv) (GLenum target, GLenum pname, GLint *params);
  void (APIENTRY *GetSeparableFilter) (GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span);
  void (APIENTRY *SeparableFilter2D) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column);
  void (APIENTRY *GetHistogram) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
  void (APIENTRY *GetHistogramParameterfv) (GLenum target, GLenum pname, GLfloat *params);
  void (APIENTRY *GetHistogramParameteriv) (GLenum target, GLenum pname, GLint *params);
  void (APIENTRY *GetMinmax) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
  void (APIENTRY *GetMinmaxParameterfv) (GLenum target, GLenum pname, GLfloat *params);
  void (APIENTRY *GetMinmaxParameteriv) (GLenum target, GLenum pname, GLint *params);
  void (APIENTRY *Histogram) (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
  void (APIENTRY *Minmax) (GLenum target, GLenum internalformat, GLboolean sink);
  void (APIENTRY *ResetHistogram) (GLenum target);
  void (APIENTRY *ResetMinmax) (GLenum target);
  void (APIENTRY *TexImage3D) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
  void (APIENTRY *TexSubImage3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
  void (APIENTRY *CopyTexSubImage3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
  void (APIENTRY *ActiveTextureARB) (GLenum texture);
  void (APIENTRY *ClientActiveTextureARB) (GLenum texture);
  void (APIENTRY *MultiTexCoord1dARB) (GLenum target, GLdouble s);
  void (APIENTRY *MultiTexCoord1dvARB) (GLenum target, const GLdouble *v);
  void (APIENTRY *MultiTexCoord1fARB) (GLenum target, GLfloat s);
  void (APIENTRY *MultiTexCoord1fvARB) (GLenum target, const GLfloat *v);
  void (APIENTRY *MultiTexCoord1iARB) (GLenum target, GLint s);
  void (APIENTRY *MultiTexCoord1ivARB) (GLenum target, const GLint *v);
  void (APIENTRY *MultiTexCoord1sARB) (GLenum target, GLshort s);
  void (APIENTRY *MultiTexCoord1svARB) (GLenum target, const GLshort *v);
  void (APIENTRY *MultiTexCoord2dARB) (GLenum target, GLdouble s, GLdouble t);
  void (APIENTRY *MultiTexCoord2dvARB) (GLenum target, const GLdouble *v);
  void (APIENTRY *MultiTexCoord2fARB) (GLenum target, GLfloat s, GLfloat t);
  void (APIENTRY *MultiTexCoord2fvARB) (GLenum target, const GLfloat *v);
  void (APIENTRY *MultiTexCoord2iARB) (GLenum target, GLint s, GLint t);
  void (APIENTRY *MultiTexCoord2ivARB) (GLenum target, const GLint *v);
  void (APIENTRY *MultiTexCoord2sARB) (GLenum target, GLshort s, GLshort t);
  void (APIENTRY *MultiTexCoord2svARB) (GLenum target, const GLshort *v);
  void (APIENTRY *MultiTexCoord3dARB) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
  void (APIENTRY *MultiTexCoord3dvARB) (GLenum target, const GLdouble *v);
  void (APIENTRY *MultiTexCoord3fARB) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
  void (APIENTRY *MultiTexCoord3fvARB) (GLenum target, const GLfloat *v);
  void (APIENTRY *MultiTexCoord3iARB) (GLenum target, GLint s, GLint t, GLint r);
  void (APIENTRY *MultiTexCoord3ivARB) (GLenum target, const GLint *v);
  void (APIENTRY *MultiTexCoord3sARB) (GLenum target, GLshort s, GLshort t, GLshort r);
  void (APIENTRY *MultiTexCoord3svARB) (GLenum target, const GLshort *v);
  void (APIENTRY *MultiTexCoord4dARB) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
  void (APIENTRY *MultiTexCoord4dvARB) (GLenum target, const GLdouble *v);
  void (APIENTRY *MultiTexCoord4fARB) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
  void (APIENTRY *MultiTexCoord4fvARB) (GLenum target, const GLfloat *v);
  void (APIENTRY *MultiTexCoord4iARB) (GLenum target, GLint s, GLint t, GLint r, GLint q);
  void (APIENTRY *MultiTexCoord4ivARB) (GLenum target, const GLint *v);
  void (APIENTRY *MultiTexCoord4sARB) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
  void (APIENTRY *MultiTexCoord4svARB) (GLenum target, const GLshort *v);
  void (APIENTRY *LoadTransposeMatrixfARB) (const GLfloat *m);
  void (APIENTRY *LoadTransposeMatrixdARB) (const GLdouble *m);
  void (APIENTRY *MultTransposeMatrixfARB) (const GLfloat *m);
  void (APIENTRY *MultTransposeMatrixdARB) (const GLdouble *m);
  void (APIENTRY *SampleCoverageARB) (GLclampf value, GLboolean invert);
  void (APIENTRY *CompressedTexImage3DARB) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
  void (APIENTRY *CompressedTexImage2DARB) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
  void (APIENTRY *CompressedTexImage1DARB) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
  void (APIENTRY *CompressedTexSubImage3DARB) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
  void (APIENTRY *CompressedTexSubImage2DARB) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
  void (APIENTRY *CompressedTexSubImage1DARB) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
  void (APIENTRY *GetCompressedTexImageARB) (GLenum target, GLint level);
  void (APIENTRY *WeightbvARB) (GLint size, const GLbyte *weights);
  void (APIENTRY *WeightsvARB) (GLint size, const GLshort *weights);
  void (APIENTRY *WeightivARB) (GLint size, const GLint *weights);
  void (APIENTRY *WeightfvARB) (GLint size, const GLfloat *weights);
  void (APIENTRY *WeightdvARB) (GLint size, const GLdouble *weights);
  void (APIENTRY *WeightubvARB) (GLint size, const GLubyte *weights);
  void (APIENTRY *WeightusvARB) (GLint size, const GLushort *weights);
  void (APIENTRY *WeightuivARB) (GLint size, const GLuint *weights);
  void (APIENTRY *WeightPointerARB) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
  void (APIENTRY *VertexBlendARB) (GLint count);
  void (APIENTRY *CurrentPaletteMatrixARB) (GLint index);
  void (APIENTRY *MatrixIndexubvARB) (GLint size, const GLubyte *indices);
  void (APIENTRY *MatrixIndexusvARB) (GLint size, const GLushort *indices);
  void (APIENTRY *MatrixIndexuivARB) (GLint size, const GLuint *indices);
  void (APIENTRY *MatrixIndexPointerARB) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
  void (APIENTRY *BlendColorEXT) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
  void (APIENTRY *PolygonOffsetEXT) (GLfloat factor, GLfloat bias);
  void (APIENTRY *TexImage3DEXT) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
  void (APIENTRY *TexSubImage3DEXT) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
  void (APIENTRY *GetTexFilterFuncSGIS) (GLenum target, GLenum filter, GLfloat *weights);
  void (APIENTRY *TexFilterFuncSGIS) (GLenum target, GLenum filter, GLsizei n, const GLfloat *weights);
  void (APIENTRY *TexSubImage1DEXT) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
  void (APIENTRY *TexSubImage2DEXT) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
  void (APIENTRY *CopyTexImage1DEXT) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
  void (APIENTRY *CopyTexImage2DEXT) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
  void (APIENTRY *CopyTexSubImage1DEXT) (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
  void (APIENTRY *CopyTexSubImage2DEXT) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
  void (APIENTRY *CopyTexSubImage3DEXT) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
  void (APIENTRY *GetHistogramEXT) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
  void (APIENTRY *GetHistogramParameterfvEXT) (GLenum target, GLenum pname, GLfloat *params);
  void (APIENTRY *GetHistogramParameterivEXT) (GLenum target, GLenum pname, GLint *params);
  void (APIENTRY *GetMinmaxEXT) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
  void (APIENTRY *GetMinmaxParameterfvEXT) (GLenum target, GLenum pname, GLfloat *params);
  void (APIENTRY *GetMinmaxParameterivEXT) (GLenum target, GLenum pname, GLint *params);
  void (APIENTRY *HistogramEXT) (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
  void (APIENTRY *MinmaxEXT) (GLenum target, GLenum internalformat, GLboolean sink);
  void (APIENTRY *ResetHistogramEXT) (GLenum target);
  void (APIENTRY *ResetMinmaxEXT) (GLenum target);
  void (APIENTRY *ConvolutionFilter1DEXT) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image);
  void (APIENTRY *ConvolutionFilter2DEXT) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image);
  void (APIENTRY *ConvolutionParameterfEXT) (GLenum target, GLenum pname, GLfloat params);
  void (APIENTRY *ConvolutionParameterfvEXT) (GLenum target, GLenum pname, const GLfloat *params);
  void (APIENTRY *ConvolutionParameteriEXT) (GLenum target, GLenum pname, GLint params);
  void (APIENTRY *ConvolutionParameterivEXT) (GLenum target, GLenum pname, const GLint *params);
  void (APIENTRY *CopyConvolutionFilter1DEXT) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
  void (APIENTRY *CopyConvolutionFilter2DEXT) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
  void (APIENTRY *GetConvolutionFilterEXT) (GLenum target, GLenum format, GLenum type, GLvoid *image);
  void (APIENTRY *GetConvolutionParameterfvEXT) (GLenum target, GLenum pname, GLfloat *params);
  void (APIENTRY *GetConvolutionParameterivEXT) (GLenum target, GLenum pname, GLint *params);
  void (APIENTRY *GetSeparableFilterEXT) (GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span);
  void (APIENTRY *SeparableFilter2DEXT) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column);
  void (APIENTRY *ColorTableSGI) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
  void (APIENTRY *ColorTableParameterfvSGI) (GLenum target, GLenum pname, const GLfloat *params);
  void (APIENTRY *ColorTableParameterivSGI) (GLenum target, GLenum pname, const GLint *params);
  void (APIENTRY *CopyColorTableSGI) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
  void (APIENTRY *GetColorTableSGI) (GLenum target, GLenum format, GLenum type, GLvoid *table);
  void (APIENTRY *GetColorTableParameterfvSGI) (GLenum target, GLenum pname, GLfloat *params);
  void (APIENTRY *GetColorTableParameterivSGI) (GLenum target, GLenum pname, GLint *params);
  void (APIENTRY *PixelTexGenSGIX) (GLenum mode);
  void (APIENTRY *PixelTexGenParameteriSGIS) (GLenum pname, GLint param);
  void (APIENTRY *PixelTexGenParameterivSGIS) (GLenum pname, const GLint *params);
  void (APIENTRY *PixelTexGenParameterfSGIS) (GLenum pname, GLfloat param);
  void (APIENTRY *PixelTexGenParameterfvSGIS) (GLenum pname, const GLfloat *params);
  void (APIENTRY *GetPixelTexGenParameterivSGIS) (GLenum pname, GLint *params);
  void (APIENTRY *GetPixelTexGenParameterfvSGIS) (GLenum pname, GLfloat *params);
  void (APIENTRY *TexImage4DSGIS) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
  void (APIENTRY *TexSubImage4DSGIS) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint woffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLenum format, GLenum type, const GLvoid *pixels);
  GLboolean (APIENTRY *AreTexturesResidentEXT) (GLsizei n, const GLuint *textures, GLboolean *residences);
  void (APIENTRY *BindTextureEXT) (GLenum target, GLuint texture);
  void (APIENTRY *DeleteTexturesEXT) (GLsizei n, const GLuint *textures);
  void (APIENTRY *GenTexturesEXT) (GLsizei n, GLuint *textures);
  GLboolean (APIENTRY *IsTextureEXT) (GLuint texture);
  void (APIENTRY *PrioritizeTexturesEXT) (GLsizei n, const GLuint *textures, const GLclampf *priorities);
  void (APIENTRY *DetailTexFuncSGIS) (GLenum target, GLsizei n, const GLfloat *points);
  void (APIENTRY *GetDetailTexFuncSGIS) (GLenum target, GLfloat *points);
  void (APIENTRY *SharpenTexFuncSGIS) (GLenum target, GLsizei n, const GLfloat *points);
  void (APIENTRY *GetSharpenTexFuncSGIS) (GLenum target, GLfloat *points);
  void (APIENTRY *SampleMaskSGIS) (GLclampf value, GLboolean invert);
  void (APIENTRY *SamplePatternSGIS) (GLenum pattern);
  void (APIENTRY *ArrayElementEXT) (GLint i);
  void (APIENTRY *ColorPointerEXT) (GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
  void (APIENTRY *DrawArraysEXT) (GLenum mode, GLint first, GLsizei count);
  void (APIENTRY *EdgeFlagPointerEXT) (GLsizei stride, GLsizei count, const GLboolean *pointer);
  void (APIENTRY *GetPointervEXT) (GLenum pname, GLvoid* *params);
  void (APIENTRY *IndexPointerEXT) (GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
  void (APIENTRY *NormalPointerEXT) (GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
  void (APIENTRY *TexCoordPointerEXT) (GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
  void (APIENTRY *VertexPointerEXT) (GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
  void (APIENTRY *BlendEquationEXT) (GLenum mode);
  void (APIENTRY *SpriteParameterfSGIX) (GLenum pname, GLfloat param);
  void (APIENTRY *SpriteParameterfvSGIX) (GLenum pname, const GLfloat *params);
  void (APIENTRY *SpriteParameteriSGIX) (GLenum pname, GLint param);
  void (APIENTRY *SpriteParameterivSGIX) (GLenum pname, const GLint *params);
  void (APIENTRY *PointParameterfARB) (GLenum pname, GLfloat param);
  void (APIENTRY *PointParameterfvARB) (GLenum pname, const GLfloat *params);
  void (APIENTRY *PointParameterfEXT) (GLenum pname, GLfloat param);
  void (APIENTRY *PointParameterfvEXT) (GLenum pname, const GLfloat *params);
  void (APIENTRY *PointParameterfSGIS) (GLenum pname, GLfloat param);
  void (APIENTRY *PointParameterfvSGIS) (GLenum pname, const GLfloat *params);
  GLint (APIENTRY *GetInstrumentsSGIX) (void);
  void (APIENTRY *InstrumentsBufferSGIX) (GLsizei size, GLint *buffer);
  GLint (APIENTRY *PollInstrumentsSGIX) (GLint *marker_p);
  void (APIENTRY *ReadInstrumentsSGIX) (GLint marker);
  void (APIENTRY *StartInstrumentsSGIX) (void);
  void (APIENTRY *StopInstrumentsSGIX) (GLint marker);
  void (APIENTRY *FrameZoomSGIX) (GLint factor);
  void (APIENTRY *TagSampleBufferSGIX) (void);
  void (APIENTRY *DeformationMap3dSGIX) (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, GLdouble w1, GLdouble w2, GLint wstride, GLint worder, const GLdouble *points);
  void (APIENTRY *DeformationMap3fSGIX) (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, GLfloat w1, GLfloat w2, GLint wstride, GLint worder, const GLfloat *points);
  void (APIENTRY *DeformSGIX) (GLbitfield mask);
  void (APIENTRY *LoadIdentityDeformationMapSGIX) (GLbitfield mask);
  void (APIENTRY *ReferencePlaneSGIX) (const GLdouble *equation);
  void (APIENTRY *FlushRasterSGIX) (void);
  void (APIENTRY *FogFuncSGIS) (GLsizei n, const GLfloat *points);
  void (APIENTRY *GetFogFuncSGIS) (GLfloat *points);
  void (APIENTRY *ImageTransformParameteriHP) (GLenum target, GLenum pname, GLint param);
  void (APIENTRY *ImageTransformParameterfHP) (GLenum target, GLenum pname, GLfloat param);
  void (APIENTRY *ImageTransformParameterivHP) (GLenum target, GLenum pname, const GLint *params);
  void (APIENTRY *ImageTransformParameterfvHP) (GLenum target, GLenum pname, const GLfloat *params);
  void (APIENTRY *GetImageTransformParameterivHP) (GLenum target, GLenum pname, GLint *params);
  void (APIENTRY *GetImageTransformParameterfvHP) (GLenum target, GLenum pname, GLfloat *params);
  void (APIENTRY *ColorSubTableEXT) (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data);
  void (APIENTRY *CopyColorSubTableEXT) (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
  void (APIENTRY *HintPGI) (GLenum target, GLint mode);
  void (APIENTRY *ColorTableEXT) (GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
  void (APIENTRY *GetColorTableEXT) (GLenum target, GLenum format, GLenum type, GLvoid *data);
  void (APIENTRY *GetColorTableParameterivEXT) (GLenum target, GLenum pname, GLint *params);
  void (APIENTRY *GetColorTableParameterfvEXT) (GLenum target, GLenum pname, GLfloat *params);
  void (APIENTRY *GetListParameterfvSGIX) (GLuint list, GLenum pname, GLfloat *params);
  void (APIENTRY *GetListParameterivSGIX) (GLuint list, GLenum pname, GLint *params);
  void (APIENTRY *ListParameterfSGIX) (GLuint list, GLenum pname, GLfloat param);
  void (APIENTRY *ListParameterfvSGIX) (GLuint list, GLenum pname, const GLfloat *params);
  void (APIENTRY *ListParameteriSGIX) (GLuint list, GLenum pname, GLint param);
  void (APIENTRY *ListParameterivSGIX) (GLuint list, GLenum pname, const GLint *params);
  void (APIENTRY *IndexMaterialEXT) (GLenum face, GLenum mode);
  void (APIENTRY *IndexFuncEXT) (GLenum func, GLclampf ref);
  void (APIENTRY *LockArraysEXT) (GLint first, GLsizei count);
  void (APIENTRY *UnlockArraysEXT) (void);
  void (APIENTRY *CullParameterdvEXT) (GLenum pname, GLdouble *params);
  void (APIENTRY *CullParameterfvEXT) (GLenum pname, GLfloat *params);
  void (APIENTRY *FragmentColorMaterialSGIX) (GLenum face, GLenum mode);
  void (APIENTRY *FragmentLightfSGIX) (GLenum light, GLenum pname, GLfloat param);
  void (APIENTRY *FragmentLightfvSGIX) (GLenum light, GLenum pname, const GLfloat *params);
  void (APIENTRY *FragmentLightiSGIX) (GLenum light, GLenum pname, GLint param);
  void (APIENTRY *FragmentLightivSGIX) (GLenum light, GLenum pname, const GLint *params);
  void (APIENTRY *FragmentLightModelfSGIX) (GLenum pname, GLfloat param);
  void (APIENTRY *FragmentLightModelfvSGIX) (GLenum pname, const GLfloat *params);
  void (APIENTRY *FragmentLightModeliSGIX) (GLenum pname, GLint param);
  void (APIENTRY *FragmentLightModelivSGIX) (GLenum pname, const GLint *params);
  void (APIENTRY *FragmentMaterialfSGIX) (GLenum face, GLenum pname, GLfloat param);
  void (APIENTRY *FragmentMaterialfvSGIX) (GLenum face, GLenum pname, const GLfloat *params);
  void (APIENTRY *FragmentMaterialiSGIX) (GLenum face, GLenum pname, GLint param);
  void (APIENTRY *FragmentMaterialivSGIX) (GLenum face, GLenum pname, const GLint *params);
  void (APIENTRY *GetFragmentLightfvSGIX) (GLenum light, GLenum pname, GLfloat *params);
  void (APIENTRY *GetFragmentLightivSGIX) (GLenum light, GLenum pname, GLint *params);
  void (APIENTRY *GetFragmentMaterialfvSGIX) (GLenum face, GLenum pname, GLfloat *params);
  void (APIENTRY *GetFragmentMaterialivSGIX) (GLenum face, GLenum pname, GLint *params);
  void (APIENTRY *LightEnviSGIX) (GLenum pname, GLint param);
  void (APIENTRY *DrawRangeElementsEXT) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
  void (APIENTRY *ApplyTextureEXT) (GLenum mode);
  void (APIENTRY *TextureLightEXT) (GLenum pname);
  void (APIENTRY *TextureMaterialEXT) (GLenum face, GLenum mode);
  void (APIENTRY *AsyncMarkerSGIX) (GLuint marker);
  GLint (APIENTRY *FinishAsyncSGIX) (GLuint *markerp);
  GLint (APIENTRY *PollAsyncSGIX) (GLuint *markerp);
  GLuint (APIENTRY *GenAsyncMarkersSGIX) (GLsizei range);
  void (APIENTRY *DeleteAsyncMarkersSGIX) (GLuint marker, GLsizei range);
  GLboolean (APIENTRY *IsAsyncMarkerSGIX) (GLuint marker);
  void (APIENTRY *VertexPointervINTEL) (GLint size, GLenum type, const GLvoid* *pointer);
  void (APIENTRY *NormalPointervINTEL) (GLenum type, const GLvoid* *pointer);
  void (APIENTRY *ColorPointervINTEL) (GLint size, GLenum type, const GLvoid* *pointer);
  void (APIENTRY *TexCoordPointervINTEL) (GLint size, GLenum type, const GLvoid* *pointer);
  void (APIENTRY *PixelTransformParameteriEXT) (GLenum target, GLenum pname, GLint param);
  void (APIENTRY *PixelTransformParameterfEXT) (GLenum target, GLenum pname, GLfloat param);
  void (APIENTRY *PixelTransformParameterivEXT) (GLenum target, GLenum pname, const GLint *params);
  void (APIENTRY *PixelTransformParameterfvEXT) (GLenum target, GLenum pname, const GLfloat *params);
  void (APIENTRY *SecondaryColor3bEXT) (GLbyte red, GLbyte green, GLbyte blue);
  void (APIENTRY *SecondaryColor3bvEXT) (const GLbyte *v);
  void (APIENTRY *SecondaryColor3dEXT) (GLdouble red, GLdouble green, GLdouble blue);
  void (APIENTRY *SecondaryColor3dvEXT) (const GLdouble *v);
  void (APIENTRY *SecondaryColor3fEXT) (GLfloat red, GLfloat green, GLfloat blue);
  void (APIENTRY *SecondaryColor3fvEXT) (const GLfloat *v);
  void (APIENTRY *SecondaryColor3iEXT) (GLint red, GLint green, GLint blue);
  void (APIENTRY *SecondaryColor3ivEXT) (const GLint *v);
  void (APIENTRY *SecondaryColor3sEXT) (GLshort red, GLshort green, GLshort blue);
  void (APIENTRY *SecondaryColor3svEXT) (const GLshort *v);
  void (APIENTRY *SecondaryColor3ubEXT) (GLubyte red, GLubyte green, GLubyte blue);
  void (APIENTRY *SecondaryColor3ubvEXT) (const GLubyte *v);
  void (APIENTRY *SecondaryColor3uiEXT) (GLuint red, GLuint green, GLuint blue);
  void (APIENTRY *SecondaryColor3uivEXT) (const GLuint *v);
  void (APIENTRY *SecondaryColor3usEXT) (GLushort red, GLushort green, GLushort blue);
  void (APIENTRY *SecondaryColor3usvEXT) (const GLushort *v);
  void (APIENTRY *SecondaryColorPointerEXT) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
  void (APIENTRY *TextureNormalEXT) (GLenum mode);
  void (APIENTRY *MultiDrawArraysEXT) (GLenum mode, GLint *first, GLsizei *count, GLsizei primcount);
  void (APIENTRY *MultiDrawElementsEXT) (GLenum mode, const GLsizei *count, GLenum type, const GLvoid* *indices, GLsizei primcount);
  void (APIENTRY *FogCoordfEXT) (GLfloat coord);
  void (APIENTRY *FogCoordfvEXT) (const GLfloat *coord);
  void (APIENTRY *FogCoorddEXT) (GLdouble coord);
  void (APIENTRY *FogCoorddvEXT) (const GLdouble *coord);
  void (APIENTRY *FogCoordPointerEXT) (GLenum type, GLsizei stride, const GLvoid *pointer);
  void (APIENTRY *Tangent3bEXT) (GLbyte tx, GLbyte ty, GLbyte tz);
  void (APIENTRY *Tangent3bvEXT) (const GLbyte *v);
  void (APIENTRY *Tangent3dEXT) (GLdouble tx, GLdouble ty, GLdouble tz);
  void (APIENTRY *Tangent3dvEXT) (const GLdouble *v);
  void (APIENTRY *Tangent3fEXT) (GLfloat tx, GLfloat ty, GLfloat tz);
  void (APIENTRY *Tangent3fvEXT) (const GLfloat *v);
  void (APIENTRY *Tangent3iEXT) (GLint tx, GLint ty, GLint tz);
  void (APIENTRY *Tangent3ivEXT) (const GLint *v);
  void (APIENTRY *Tangent3sEXT) (GLshort tx, GLshort ty, GLshort tz);
  void (APIENTRY *Tangent3svEXT) (const GLshort *v);
  void (APIENTRY *Binormal3bEXT) (GLbyte bx, GLbyte by, GLbyte bz);
  void (APIENTRY *Binormal3bvEXT) (const GLbyte *v);
  void (APIENTRY *Binormal3dEXT) (GLdouble bx, GLdouble by, GLdouble bz);
  void (APIENTRY *Binormal3dvEXT) (const GLdouble *v);
  void (APIENTRY *Binormal3fEXT) (GLfloat bx, GLfloat by, GLfloat bz);
  void (APIENTRY *Binormal3fvEXT) (const GLfloat *v);
  void (APIENTRY *Binormal3iEXT) (GLint bx, GLint by, GLint bz);
  void (APIENTRY *Binormal3ivEXT) (const GLint *v);
  void (APIENTRY *Binormal3sEXT) (GLshort bx, GLshort by, GLshort bz);
  void (APIENTRY *Binormal3svEXT) (const GLshort *v);
  void (APIENTRY *TangentPointerEXT) (GLenum type, GLsizei stride, const GLvoid *pointer);
  void (APIENTRY *BinormalPointerEXT) (GLenum type, GLsizei stride, const GLvoid *pointer);
  void (APIENTRY *FinishTextureSUNX) (void);
  void (APIENTRY *GlobalAlphaFactorbSUN) (GLbyte factor);
  void (APIENTRY *GlobalAlphaFactorsSUN) (GLshort factor);
  void (APIENTRY *GlobalAlphaFactoriSUN) (GLint factor);
  void (APIENTRY *GlobalAlphaFactorfSUN) (GLfloat factor);
  void (APIENTRY *GlobalAlphaFactordSUN) (GLdouble factor);
  void (APIENTRY *GlobalAlphaFactorubSUN) (GLubyte factor);
  void (APIENTRY *GlobalAlphaFactorusSUN) (GLushort factor);
  void (APIENTRY *GlobalAlphaFactoruiSUN) (GLuint factor);
  void (APIENTRY *ReplacementCodeuiSUN) (GLuint code);
  void (APIENTRY *ReplacementCodeusSUN) (GLushort code);
  void (APIENTRY *ReplacementCodeubSUN) (GLubyte code);
  void (APIENTRY *ReplacementCodeuivSUN) (const GLuint *code);
  void (APIENTRY *ReplacementCodeusvSUN) (const GLushort *code);
  void (APIENTRY *ReplacementCodeubvSUN) (const GLubyte *code);
  void (APIENTRY *ReplacementCodePointerSUN) (GLenum type, GLsizei stride, const GLvoid* *pointer);
  void (APIENTRY *Color4ubVertex2fSUN) (GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y);
  void (APIENTRY *Color4ubVertex2fvSUN) (const GLubyte *c, const GLfloat *v);
  void (APIENTRY *Color4ubVertex3fSUN) (GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY *Color4ubVertex3fvSUN) (const GLubyte *c, const GLfloat *v);
  void (APIENTRY *Color3fVertex3fSUN) (GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY *Color3fVertex3fvSUN) (const GLfloat *c, const GLfloat *v);
  void (APIENTRY *Normal3fVertex3fSUN) (GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY *Normal3fVertex3fvSUN) (const GLfloat *n, const GLfloat *v);
  void (APIENTRY *Color4fNormal3fVertex3fSUN) (GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY *Color4fNormal3fVertex3fvSUN) (const GLfloat *c, const GLfloat *n, const GLfloat *v);
  void (APIENTRY *TexCoord2fVertex3fSUN) (GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY *TexCoord2fVertex3fvSUN) (const GLfloat *tc, const GLfloat *v);
  void (APIENTRY *TexCoord4fVertex4fSUN) (GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
  void (APIENTRY *TexCoord4fVertex4fvSUN) (const GLfloat *tc, const GLfloat *v);
  void (APIENTRY *TexCoord2fColor4ubVertex3fSUN) (GLfloat s, GLfloat t, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY *TexCoord2fColor4ubVertex3fvSUN) (const GLfloat *tc, const GLubyte *c, const GLfloat *v);
  void (APIENTRY *TexCoord2fColor3fVertex3fSUN) (GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY *TexCoord2fColor3fVertex3fvSUN) (const GLfloat *tc, const GLfloat *c, const GLfloat *v);
  void (APIENTRY *TexCoord2fNormal3fVertex3fSUN) (GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY *TexCoord2fNormal3fVertex3fvSUN) (const GLfloat *tc, const GLfloat *n, const GLfloat *v);
  void (APIENTRY *TexCoord2fColor4fNormal3fVertex3fSUN) (GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY *TexCoord2fColor4fNormal3fVertex3fvSUN) (const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
  void (APIENTRY *TexCoord4fColor4fNormal3fVertex4fSUN) (GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
  void (APIENTRY *TexCoord4fColor4fNormal3fVertex4fvSUN) (const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
  void (APIENTRY *ReplacementCodeuiVertex3fSUN) (GLenum rc, GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY *ReplacementCodeuiVertex3fvSUN) (const GLenum *rc, const GLfloat *v);
  void (APIENTRY *ReplacementCodeuiColor4ubVertex3fSUN) (GLenum rc, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY *ReplacementCodeuiColor4ubVertex3fvSUN) (const GLenum *rc, const GLubyte *c, const GLfloat *v);
  void (APIENTRY *ReplacementCodeuiColor3fVertex3fSUN) (GLenum rc, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY *ReplacementCodeuiColor3fVertex3fvSUN) (const GLenum *rc, const GLfloat *c, const GLfloat *v);
  void (APIENTRY *ReplacementCodeuiNormal3fVertex3fSUN) (GLenum rc, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY *ReplacementCodeuiNormal3fVertex3fvSUN) (const GLenum *rc, const GLfloat *n, const GLfloat *v);
  void (APIENTRY *ReplacementCodeuiColor4fNormal3fVertex3fSUN) (GLenum rc, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY *ReplacementCodeuiColor4fNormal3fVertex3fvSUN) (const GLenum *rc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
  void (APIENTRY *ReplacementCodeuiTexCoord2fVertex3fSUN) (GLenum rc, GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY *ReplacementCodeuiTexCoord2fVertex3fvSUN) (const GLenum *rc, const GLfloat *tc, const GLfloat *v);
  void (APIENTRY *ReplacementCodeuiTexCoord2fNormal3fVertex3fSUN) (GLenum rc, GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY *ReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN) (const GLenum *rc, const GLfloat *tc, const GLfloat *n, const GLfloat *v);
  void (APIENTRY *ReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN) (GLenum rc, GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY *ReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN) (const GLenum *rc, const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
  void (APIENTRY *BlendFuncSeparateEXT) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
  void (APIENTRY *BlendFuncSeparateINGR) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
  void (APIENTRY *VertexWeightfEXT) (GLfloat weight);
  void (APIENTRY *VertexWeightfvEXT) (const GLfloat *weight);
  void (APIENTRY *VertexWeightPointerEXT) (GLsizei size, GLenum type, GLsizei stride, const GLvoid *pointer);
  void (APIENTRY *FlushVertexArrayRangeNV) (void);
  void (APIENTRY *VertexArrayRangeNV) (GLsizei length, const GLvoid *pointer);
  void (APIENTRY *CombinerParameterfvNV) (GLenum pname, const GLfloat *params);
  void (APIENTRY *CombinerParameterfNV) (GLenum pname, GLfloat param);
  void (APIENTRY *CombinerParameterivNV) (GLenum pname, const GLint *params);
  void (APIENTRY *CombinerParameteriNV) (GLenum pname, GLint param);
  void (APIENTRY *CombinerInputNV) (GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
  void (APIENTRY *CombinerOutputNV) (GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum);
  void (APIENTRY *FinalCombinerInputNV) (GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
  void (APIENTRY *GetCombinerInputParameterfvNV) (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat *params);
  void (APIENTRY *GetCombinerInputParameterivNV) (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint *params);
  void (APIENTRY *GetCombinerOutputParameterfvNV) (GLenum stage, GLenum portion, GLenum pname, GLfloat *params);
  void (APIENTRY *GetCombinerOutputParameterivNV) (GLenum stage, GLenum portion, GLenum pname, GLint *params);
  void (APIENTRY *GetFinalCombinerInputParameterfvNV) (GLenum variable, GLenum pname, GLfloat *params);
  void (APIENTRY *GetFinalCombinerInputParameterivNV) (GLenum variable, GLenum pname, GLint *params);
  void (APIENTRY *ResizeBuffersMESA) (void);
  void (APIENTRY *WindowPos2dMESA) (GLdouble x, GLdouble y);
  void (APIENTRY *WindowPos2dvMESA) (const GLdouble *v);
  void (APIENTRY *WindowPos2fMESA) (GLfloat x, GLfloat y);
  void (APIENTRY *WindowPos2fvMESA) (const GLfloat *v);
  void (APIENTRY *WindowPos2iMESA) (GLint x, GLint y);
  void (APIENTRY *WindowPos2ivMESA) (const GLint *v);
  void (APIENTRY *WindowPos2sMESA) (GLshort x, GLshort y);
  void (APIENTRY *WindowPos2svMESA) (const GLshort *v);
  void (APIENTRY *WindowPos3dMESA) (GLdouble x, GLdouble y, GLdouble z);
  void (APIENTRY *WindowPos3dvMESA) (const GLdouble *v);
  void (APIENTRY *WindowPos3fMESA) (GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY *WindowPos3fvMESA) (const GLfloat *v);
  void (APIENTRY *WindowPos3iMESA) (GLint x, GLint y, GLint z);
  void (APIENTRY *WindowPos3ivMESA) (const GLint *v);
  void (APIENTRY *WindowPos3sMESA) (GLshort x, GLshort y, GLshort z);
  void (APIENTRY *WindowPos3svMESA) (const GLshort *v);
  void (APIENTRY *WindowPos4dMESA) (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
  void (APIENTRY *WindowPos4dvMESA) (const GLdouble *v);
  void (APIENTRY *WindowPos4fMESA) (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
  void (APIENTRY *WindowPos4fvMESA) (const GLfloat *v);
  void (APIENTRY *WindowPos4iMESA) (GLint x, GLint y, GLint z, GLint w);
  void (APIENTRY *WindowPos4ivMESA) (const GLint *v);
  void (APIENTRY *WindowPos4sMESA) (GLshort x, GLshort y, GLshort z, GLshort w);
  void (APIENTRY *WindowPos4svMESA) (const GLshort *v);
  void (APIENTRY *MultiModeDrawArraysIBM) (GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount, GLint modestride);
  void (APIENTRY *MultiModeDrawElementsIBM) (const GLenum *mode, const GLsizei *count, GLenum type, const GLvoid* *indices, GLsizei primcount, GLint modestride);
  void (APIENTRY *ColorPointerListIBM) (GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
  void (APIENTRY *SecondaryColorPointerListIBM) (GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
  void (APIENTRY *EdgeFlagPointerListIBM) (GLint stride, const GLboolean* *pointer, GLint ptrstride);
  void (APIENTRY *FogCoordPointerListIBM) (GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
  void (APIENTRY *IndexPointerListIBM) (GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
  void (APIENTRY *NormalPointerListIBM) (GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
  void (APIENTRY *TexCoordPointerListIBM) (GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
  void (APIENTRY *VertexPointerListIBM) (GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
  void (APIENTRY *TbufferMask3DFX) (GLuint mask);
  void (APIENTRY *SampleMaskEXT) (GLclampf value, GLboolean invert);
  void (APIENTRY *SamplePatternEXT) (GLenum pattern);
  void (APIENTRY *TextureColorMaskSGIS) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
  void (APIENTRY *IglooInterfaceSGIX) (GLenum pname, const GLvoid *params);
  void (APIENTRY *GenFencesNV) (GLsizei n, GLuint *fences);
  void (APIENTRY *DeleteFencesNV) (GLsizei n, const GLuint *fences);
  void (APIENTRY *SetFenceNV) (GLuint fence, GLenum condition);
  GLboolean (APIENTRY *TestFenceNV) (GLuint fence);
  void (APIENTRY *FinishFenceNV) (GLuint fence);
  GLboolean (APIENTRY *IsFenceNV) (GLuint fence);
  void (APIENTRY *GetFenceivNV) (GLuint fence, GLenum pname, GLint *params);
  void (APIENTRY *MapControlPointsNV) (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const GLvoid *points);
  void (APIENTRY *MapParameterivNV) (GLenum target, GLenum pname, const GLint *params);
  void (APIENTRY *MapParameterfvNV) (GLenum target, GLenum pname, const GLfloat *params);
  void (APIENTRY *GetMapControlPointsNV) (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLboolean packed, GLvoid *points);
  void (APIENTRY *GetMapParameterivNV) (GLenum target, GLenum pname, GLint *params);
  void (APIENTRY *GetMapParameterfvNV) (GLenum target, GLenum pname, GLfloat *params);
  void (APIENTRY *GetMapAttribParameterivNV) (GLenum target, GLuint index, GLenum pname, GLint *params);
  void (APIENTRY *GetMapAttribParameterfvNV) (GLenum target, GLuint index, GLenum pname, GLfloat *params);
  void (APIENTRY *EvalMapsNV) (GLenum target, GLenum mode);
  void (APIENTRY *CombinerStageParameterfvNV) (GLenum stage, GLenum pname, const GLfloat *params);
  void (APIENTRY *GetCombinerStageParameterfvNV) (GLenum stage, GLenum pname, GLfloat *params);
  void (APIENTRY *BindProgramNV) (GLenum target, GLuint id);
  void (APIENTRY *DeleteProgramsNV) (GLsizei n, const GLuint *ids);
  void (APIENTRY *ExecuteProgramNV) (GLenum target, GLuint id, const GLfloat *params);
  void (APIENTRY *GenProgramsNV) (GLsizei n, GLuint *ids);
  GLboolean (APIENTRY *AreProgramsResidentNV) (GLsizei n, const GLuint *ids, GLboolean *residences);
  void (APIENTRY *RequestResidentProgramsNV) (GLsizei n, const GLuint *ids);
  void (APIENTRY *GetProgramParameterfvNV) (GLenum target, GLuint index, GLenum pname, GLfloat *params);
  void (APIENTRY *GetProgramParameterdvNV) (GLenum target, GLuint index, GLenum pname, GLdouble *params);
  void (APIENTRY *GetProgramivNV) (GLuint id, GLenum pname, GLint *params);
  void (APIENTRY *GetProgramStringNV) (GLuint id, GLenum pname, GLubyte *program);
  void (APIENTRY *GetTrackMatrixivNV) (GLenum target, GLuint address, GLenum pname, GLint *params);
  void (APIENTRY *GetVertexAttribdvNV) (GLuint index, GLenum pname, GLdouble *params);
  void (APIENTRY *GetVertexAttribfvNV) (GLuint index, GLenum pname, GLfloat *params);
  void (APIENTRY *GetVertexAttribivNV) (GLuint index, GLenum pname, GLint *params);
  void (APIENTRY *GetVertexAttribPointervNV) (GLuint index, GLenum pname, GLvoid* *pointer);
  GLboolean (APIENTRY *IsProgramNV) (GLuint id);
  void (APIENTRY *LoadProgramNV) (GLenum target, GLuint id, GLsizei len, const GLubyte *program);
  void (APIENTRY *ProgramParameter4fNV) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
  void (APIENTRY *ProgramParameter4dNV) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
  void (APIENTRY *ProgramParameter4dvNV) (GLenum target, GLuint index, const GLdouble *params);
  void (APIENTRY *ProgramParameter4fvNV) (GLenum target, GLuint index, const GLfloat *params);
  void (APIENTRY *ProgramParameters4dvNV) (GLenum target, GLuint index, GLuint num, const GLdouble *params);
  void (APIENTRY *ProgramParameters4fvNV) (GLenum target, GLuint index, GLuint num, const GLfloat *params);
  void (APIENTRY *TrackMatrixNV) (GLenum target, GLuint address, GLenum matrix, GLenum transform);
  void (APIENTRY *VertexAttribPointerNV) (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
  void (APIENTRY *VertexAttrib1sNV) (GLuint index, GLshort x);
  void (APIENTRY *VertexAttrib1fNV) (GLuint index, GLfloat x);
  void (APIENTRY *VertexAttrib1dNV) (GLuint index, GLdouble x);
  void (APIENTRY *VertexAttrib2sNV) (GLuint index, GLshort x, GLshort y);
  void (APIENTRY *VertexAttrib2fNV) (GLuint index, GLfloat x, GLfloat y);
  void (APIENTRY *VertexAttrib2dNV) (GLuint index, GLdouble x, GLdouble y);
  void (APIENTRY *VertexAttrib3sNV) (GLuint index, GLshort x, GLshort y, GLshort z);
  void (APIENTRY *VertexAttrib3fNV) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY *VertexAttrib3dNV) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
  void (APIENTRY *VertexAttrib4sNV) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
  void (APIENTRY *VertexAttrib4fNV) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
  void (APIENTRY *VertexAttrib4dNV) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
  void (APIENTRY *VertexAttrib4ubNV) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
  void (APIENTRY *VertexAttrib1svNV) (GLuint index, const GLshort *v);
  void (APIENTRY *VertexAttrib1fvNV) (GLuint index, const GLfloat *v);
  void (APIENTRY *VertexAttrib1dvNV) (GLuint index, const GLdouble *v);
  void (APIENTRY *VertexAttrib2svNV) (GLuint index, const GLshort *v);
  void (APIENTRY *VertexAttrib2fvNV) (GLuint index, const GLfloat *v);
  void (APIENTRY *VertexAttrib2dvNV) (GLuint index, const GLdouble *v);
  void (APIENTRY *VertexAttrib3svNV) (GLuint index, const GLshort *v);
  void (APIENTRY *VertexAttrib3fvNV) (GLuint index, const GLfloat *v);
  void (APIENTRY *VertexAttrib3dvNV) (GLuint index, const GLdouble *v);
  void (APIENTRY *VertexAttrib4svNV) (GLuint index, const GLshort *v);
  void (APIENTRY *VertexAttrib4fvNV) (GLuint index, const GLfloat *v);
  void (APIENTRY *VertexAttrib4dvNV) (GLuint index, const GLdouble *v);
  void (APIENTRY *VertexAttrib4ubvNV) (GLuint index, const GLubyte *v);
  void (APIENTRY *VertexAttribs1svNV) (GLuint index, GLsizei n, const GLshort *v);
  void (APIENTRY *VertexAttribs1fvNV) (GLuint index, GLsizei n, const GLfloat *v);
  void (APIENTRY *VertexAttribs1dvNV) (GLuint index, GLsizei n, const GLdouble *v);
  void (APIENTRY *VertexAttribs2svNV) (GLuint index, GLsizei n, const GLshort *v);
  void (APIENTRY *VertexAttribs2fvNV) (GLuint index, GLsizei n, const GLfloat *v);
  void (APIENTRY *VertexAttribs2dvNV) (GLuint index, GLsizei n, const GLdouble *v);
  void (APIENTRY *VertexAttribs3svNV) (GLuint index, GLsizei n, const GLshort *v);
  void (APIENTRY *VertexAttribs3fvNV) (GLuint index, GLsizei n, const GLfloat *v);
  void (APIENTRY *VertexAttribs3dvNV) (GLuint index, GLsizei n, const GLdouble *v);
  void (APIENTRY *VertexAttribs4svNV) (GLuint index, GLsizei n, const GLshort *v);
  void (APIENTRY *VertexAttribs4fvNV) (GLuint index, GLsizei n, const GLfloat *v);
  void (APIENTRY *VertexAttribs4dvNV) (GLuint index, GLsizei n, const GLdouble *v);
  void (APIENTRY *VertexAttribs4ubvNV) (GLuint index, GLsizei n, const GLubyte *v);
  void (APIENTRY *AddSwapHintRectWIN) (GLint x, GLint y, GLsizei width, GLsizei height);
#ifdef _WIN32
  HANDLE (WINAPI *CreateBufferRegionARB) (HDC hDC, int iLayerPlane, UINT uType);
  VOID (WINAPI *DeleteBufferRegionARB) (HANDLE hRegion);
  BOOL (WINAPI *SaveBufferRegionARB) (HANDLE hRegion, int x, int y, int width, int height);
  BOOL (WINAPI *RestoreBufferRegionARB) (HANDLE hRegion, int x, int y, int width, int height, int xSrc, int ySrc);
  const (WINAPI *GetExtensionsStringARB) (HDC hdc);
  BOOL (WINAPI *GetPixelFormatAttribivARB) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues);
  BOOL (WINAPI *GetPixelFormatAttribfvARB) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues);
  BOOL (WINAPI *ChoosePixelFormatARB) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
  BOOL (WINAPI *MakeContextCurrentARB) (HDC hDrawDC, HDC hReadDC, HGLRC hglrc);
  HDC (WINAPI *GetCurrentReadDCARB) (void);
  HPBUFFERARB (WINAPI *CreatePbufferARB) (HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
  HDC (WINAPI *GetPbufferDCARB) (HPBUFFERARB hPbuffer);
  int (WINAPI *ReleasePbufferDCARB) (HPBUFFERARB hPbuffer, HDC hDC);
  BOOL (WINAPI *DestroyPbufferARB) (HPBUFFERARB hPbuffer);
  BOOL (WINAPI *QueryPbufferARB) (HPBUFFERARB hPbuffer, int iAttribute, int *piValue);
  GLboolean (WINAPI *CreateDisplayColorTableEXT) (GLushort id);
  GLboolean (WINAPI *LoadDisplayColorTableEXT) (const GLushort *table, GLuint length);
  GLboolean (WINAPI *BindDisplayColorTableEXT) (GLushort id);
  VOID (WINAPI *DestroyDisplayColorTableEXT) (GLushort id);
  const (WINAPI *GetExtensionsStringEXT) (void);
  BOOL (WINAPI *MakeContextCurrentEXT) (HDC hDrawDC, HDC hReadDC, HGLRC hglrc);
  HDC (WINAPI *GetCurrentReadDCEXT) (void);
  HPBUFFEREXT (WINAPI *CreatePbufferEXT) (HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
  HDC (WINAPI *GetPbufferDCEXT) (HPBUFFEREXT hPbuffer);
  int (WINAPI *ReleasePbufferDCEXT) (HPBUFFEREXT hPbuffer, HDC hDC);
  BOOL (WINAPI *DestroyPbufferEXT) (HPBUFFEREXT hPbuffer);
  BOOL (WINAPI *QueryPbufferEXT) (HPBUFFEREXT hPbuffer, int iAttribute, int *piValue);
  BOOL (WINAPI *GetPixelFormatAttribivEXT) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int *piAttributes, int *piValues);
  BOOL (WINAPI *GetPixelFormatAttribfvEXT) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int *piAttributes, FLOAT *pfValues);
  BOOL (WINAPI *ChoosePixelFormatEXT) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
  BOOL (WINAPI *SwapIntervalEXT) (int interval);
  int (WINAPI *GetSwapIntervalEXT) (void);
#endif /* _WIN32 */
} _GLextensionProcs;

#define glBlendColor                     (_GET_TLS_PROCTABLE()->BlendColor)
#define glBlendEquation                  (_GET_TLS_PROCTABLE()->BlendEquation)
#define glDrawRangeElements              (_GET_TLS_PROCTABLE()->DrawRangeElements)
#define glColorTable                     (_GET_TLS_PROCTABLE()->ColorTable)
#define glColorTableParameterfv          (_GET_TLS_PROCTABLE()->ColorTableParameterfv)
#define glColorTableParameteriv          (_GET_TLS_PROCTABLE()->ColorTableParameteriv)
#define glCopyColorTable                 (_GET_TLS_PROCTABLE()->CopyColorTable)
#define glGetColorTable                  (_GET_TLS_PROCTABLE()->GetColorTable)
#define glGetColorTableParameterfv       (_GET_TLS_PROCTABLE()->GetColorTableParameterfv)
#define glGetColorTableParameteriv       (_GET_TLS_PROCTABLE()->GetColorTableParameteriv)
#define glColorSubTable                  (_GET_TLS_PROCTABLE()->ColorSubTable)
#define glCopyColorSubTable              (_GET_TLS_PROCTABLE()->CopyColorSubTable)
#define glConvolutionFilter1D            (_GET_TLS_PROCTABLE()->ConvolutionFilter1D)
#define glConvolutionFilter2D            (_GET_TLS_PROCTABLE()->ConvolutionFilter2D)
#define glConvolutionParameterf          (_GET_TLS_PROCTABLE()->ConvolutionParameterf)
#define glConvolutionParameterfv         (_GET_TLS_PROCTABLE()->ConvolutionParameterfv)
#define glConvolutionParameteri          (_GET_TLS_PROCTABLE()->ConvolutionParameteri)
#define glConvolutionParameteriv         (_GET_TLS_PROCTABLE()->ConvolutionParameteriv)
#define glCopyConvolutionFilter1D        (_GET_TLS_PROCTABLE()->CopyConvolutionFilter1D)
#define glCopyConvolutionFilter2D        (_GET_TLS_PROCTABLE()->CopyConvolutionFilter2D)
#define glGetConvolutionFilter           (_GET_TLS_PROCTABLE()->GetConvolutionFilter)
#define glGetConvolutionParameterfv      (_GET_TLS_PROCTABLE()->GetConvolutionParameterfv)
#define glGetConvolutionParameteriv      (_GET_TLS_PROCTABLE()->GetConvolutionParameteriv)
#define glGetSeparableFilter             (_GET_TLS_PROCTABLE()->GetSeparableFilter)
#define glSeparableFilter2D              (_GET_TLS_PROCTABLE()->SeparableFilter2D)
#define glGetHistogram                   (_GET_TLS_PROCTABLE()->GetHistogram)
#define glGetHistogramParameterfv        (_GET_TLS_PROCTABLE()->GetHistogramParameterfv)
#define glGetHistogramParameteriv        (_GET_TLS_PROCTABLE()->GetHistogramParameteriv)
#define glGetMinmax                      (_GET_TLS_PROCTABLE()->GetMinmax)
#define glGetMinmaxParameterfv           (_GET_TLS_PROCTABLE()->GetMinmaxParameterfv)
#define glGetMinmaxParameteriv           (_GET_TLS_PROCTABLE()->GetMinmaxParameteriv)
#define glHistogram                      (_GET_TLS_PROCTABLE()->Histogram)
#define glMinmax                         (_GET_TLS_PROCTABLE()->Minmax)
#define glResetHistogram                 (_GET_TLS_PROCTABLE()->ResetHistogram)
#define glResetMinmax                    (_GET_TLS_PROCTABLE()->ResetMinmax)
#define glTexImage3D                     (_GET_TLS_PROCTABLE()->TexImage3D)
#define glTexSubImage3D                  (_GET_TLS_PROCTABLE()->TexSubImage3D)
#define glCopyTexSubImage3D              (_GET_TLS_PROCTABLE()->CopyTexSubImage3D)
#define glActiveTextureARB               (_GET_TLS_PROCTABLE()->ActiveTextureARB)
#define glClientActiveTextureARB         (_GET_TLS_PROCTABLE()->ClientActiveTextureARB)
#define glMultiTexCoord1dARB             (_GET_TLS_PROCTABLE()->MultiTexCoord1dARB)
#define glMultiTexCoord1dvARB            (_GET_TLS_PROCTABLE()->MultiTexCoord1dvARB)
#define glMultiTexCoord1fARB             (_GET_TLS_PROCTABLE()->MultiTexCoord1fARB)
#define glMultiTexCoord1fvARB            (_GET_TLS_PROCTABLE()->MultiTexCoord1fvARB)
#define glMultiTexCoord1iARB             (_GET_TLS_PROCTABLE()->MultiTexCoord1iARB)
#define glMultiTexCoord1ivARB            (_GET_TLS_PROCTABLE()->MultiTexCoord1ivARB)
#define glMultiTexCoord1sARB             (_GET_TLS_PROCTABLE()->MultiTexCoord1sARB)
#define glMultiTexCoord1svARB            (_GET_TLS_PROCTABLE()->MultiTexCoord1svARB)
#define glMultiTexCoord2dARB             (_GET_TLS_PROCTABLE()->MultiTexCoord2dARB)
#define glMultiTexCoord2dvARB            (_GET_TLS_PROCTABLE()->MultiTexCoord2dvARB)
#define glMultiTexCoord2fARB             (_GET_TLS_PROCTABLE()->MultiTexCoord2fARB)
#define glMultiTexCoord2fvARB            (_GET_TLS_PROCTABLE()->MultiTexCoord2fvARB)
#define glMultiTexCoord2iARB             (_GET_TLS_PROCTABLE()->MultiTexCoord2iARB)
#define glMultiTexCoord2ivARB            (_GET_TLS_PROCTABLE()->MultiTexCoord2ivARB)
#define glMultiTexCoord2sARB             (_GET_TLS_PROCTABLE()->MultiTexCoord2sARB)
#define glMultiTexCoord2svARB            (_GET_TLS_PROCTABLE()->MultiTexCoord2svARB)
#define glMultiTexCoord3dARB             (_GET_TLS_PROCTABLE()->MultiTexCoord3dARB)
#define glMultiTexCoord3dvARB            (_GET_TLS_PROCTABLE()->MultiTexCoord3dvARB)
#define glMultiTexCoord3fARB             (_GET_TLS_PROCTABLE()->MultiTexCoord3fARB)
#define glMultiTexCoord3fvARB            (_GET_TLS_PROCTABLE()->MultiTexCoord3fvARB)
#define glMultiTexCoord3iARB             (_GET_TLS_PROCTABLE()->MultiTexCoord3iARB)
#define glMultiTexCoord3ivARB            (_GET_TLS_PROCTABLE()->MultiTexCoord3ivARB)
#define glMultiTexCoord3sARB             (_GET_TLS_PROCTABLE()->MultiTexCoord3sARB)
#define glMultiTexCoord3svARB            (_GET_TLS_PROCTABLE()->MultiTexCoord3svARB)
#define glMultiTexCoord4dARB             (_GET_TLS_PROCTABLE()->MultiTexCoord4dARB)
#define glMultiTexCoord4dvARB            (_GET_TLS_PROCTABLE()->MultiTexCoord4dvARB)
#define glMultiTexCoord4fARB             (_GET_TLS_PROCTABLE()->MultiTexCoord4fARB)
#define glMultiTexCoord4fvARB            (_GET_TLS_PROCTABLE()->MultiTexCoord4fvARB)
#define glMultiTexCoord4iARB             (_GET_TLS_PROCTABLE()->MultiTexCoord4iARB)
#define glMultiTexCoord4ivARB            (_GET_TLS_PROCTABLE()->MultiTexCoord4ivARB)
#define glMultiTexCoord4sARB             (_GET_TLS_PROCTABLE()->MultiTexCoord4sARB)
#define glMultiTexCoord4svARB            (_GET_TLS_PROCTABLE()->MultiTexCoord4svARB)
#define glLoadTransposeMatrixfARB        (_GET_TLS_PROCTABLE()->LoadTransposeMatrixfARB)
#define glLoadTransposeMatrixdARB        (_GET_TLS_PROCTABLE()->LoadTransposeMatrixdARB)
#define glMultTransposeMatrixfARB        (_GET_TLS_PROCTABLE()->MultTransposeMatrixfARB)
#define glMultTransposeMatrixdARB        (_GET_TLS_PROCTABLE()->MultTransposeMatrixdARB)
#define glSampleCoverageARB              (_GET_TLS_PROCTABLE()->SampleCoverageARB)
#define glCompressedTexImage3DARB        (_GET_TLS_PROCTABLE()->CompressedTexImage3DARB)
#define glCompressedTexImage2DARB        (_GET_TLS_PROCTABLE()->CompressedTexImage2DARB)
#define glCompressedTexImage1DARB        (_GET_TLS_PROCTABLE()->CompressedTexImage1DARB)
#define glCompressedTexSubImage3DARB     (_GET_TLS_PROCTABLE()->CompressedTexSubImage3DARB)
#define glCompressedTexSubImage2DARB     (_GET_TLS_PROCTABLE()->CompressedTexSubImage2DARB)
#define glCompressedTexSubImage1DARB     (_GET_TLS_PROCTABLE()->CompressedTexSubImage1DARB)
#define glGetCompressedTexImageARB       (_GET_TLS_PROCTABLE()->GetCompressedTexImageARB)
#define glWeightbvARB                    (_GET_TLS_PROCTABLE()->WeightbvARB)
#define glWeightsvARB                    (_GET_TLS_PROCTABLE()->WeightsvARB)
#define glWeightivARB                    (_GET_TLS_PROCTABLE()->WeightivARB)
#define glWeightfvARB                    (_GET_TLS_PROCTABLE()->WeightfvARB)
#define glWeightdvARB                    (_GET_TLS_PROCTABLE()->WeightdvARB)
#define glWeightubvARB                   (_GET_TLS_PROCTABLE()->WeightubvARB)
#define glWeightusvARB                   (_GET_TLS_PROCTABLE()->WeightusvARB)
#define glWeightuivARB                   (_GET_TLS_PROCTABLE()->WeightuivARB)
#define glWeightPointerARB               (_GET_TLS_PROCTABLE()->WeightPointerARB)
#define glVertexBlendARB                 (_GET_TLS_PROCTABLE()->VertexBlendARB)
#define glCurrentPaletteMatrixARB        (_GET_TLS_PROCTABLE()->CurrentPaletteMatrixARB)
#define glMatrixIndexubvARB              (_GET_TLS_PROCTABLE()->MatrixIndexubvARB)
#define glMatrixIndexusvARB              (_GET_TLS_PROCTABLE()->MatrixIndexusvARB)
#define glMatrixIndexuivARB              (_GET_TLS_PROCTABLE()->MatrixIndexuivARB)
#define glMatrixIndexPointerARB          (_GET_TLS_PROCTABLE()->MatrixIndexPointerARB)
#define glBlendColorEXT                  (_GET_TLS_PROCTABLE()->BlendColorEXT)
#define glPolygonOffsetEXT               (_GET_TLS_PROCTABLE()->PolygonOffsetEXT)
#define glTexImage3DEXT                  (_GET_TLS_PROCTABLE()->TexImage3DEXT)
#define glTexSubImage3DEXT               (_GET_TLS_PROCTABLE()->TexSubImage3DEXT)
#define glGetTexFilterFuncSGIS           (_GET_TLS_PROCTABLE()->GetTexFilterFuncSGIS)
#define glTexFilterFuncSGIS              (_GET_TLS_PROCTABLE()->TexFilterFuncSGIS)
#define glTexSubImage1DEXT               (_GET_TLS_PROCTABLE()->TexSubImage1DEXT)
#define glTexSubImage2DEXT               (_GET_TLS_PROCTABLE()->TexSubImage2DEXT)
#define glCopyTexImage1DEXT              (_GET_TLS_PROCTABLE()->CopyTexImage1DEXT)
#define glCopyTexImage2DEXT              (_GET_TLS_PROCTABLE()->CopyTexImage2DEXT)
#define glCopyTexSubImage1DEXT           (_GET_TLS_PROCTABLE()->CopyTexSubImage1DEXT)
#define glCopyTexSubImage2DEXT           (_GET_TLS_PROCTABLE()->CopyTexSubImage2DEXT)
#define glCopyTexSubImage3DEXT           (_GET_TLS_PROCTABLE()->CopyTexSubImage3DEXT)
#define glGetHistogramEXT                (_GET_TLS_PROCTABLE()->GetHistogramEXT)
#define glGetHistogramParameterfvEXT     (_GET_TLS_PROCTABLE()->GetHistogramParameterfvEXT)
#define glGetHistogramParameterivEXT     (_GET_TLS_PROCTABLE()->GetHistogramParameterivEXT)
#define glGetMinmaxEXT                   (_GET_TLS_PROCTABLE()->GetMinmaxEXT)
#define glGetMinmaxParameterfvEXT        (_GET_TLS_PROCTABLE()->GetMinmaxParameterfvEXT)
#define glGetMinmaxParameterivEXT        (_GET_TLS_PROCTABLE()->GetMinmaxParameterivEXT)
#define glHistogramEXT                   (_GET_TLS_PROCTABLE()->HistogramEXT)
#define glMinmaxEXT                      (_GET_TLS_PROCTABLE()->MinmaxEXT)
#define glResetHistogramEXT              (_GET_TLS_PROCTABLE()->ResetHistogramEXT)
#define glResetMinmaxEXT                 (_GET_TLS_PROCTABLE()->ResetMinmaxEXT)
#define glConvolutionFilter1DEXT         (_GET_TLS_PROCTABLE()->ConvolutionFilter1DEXT)
#define glConvolutionFilter2DEXT         (_GET_TLS_PROCTABLE()->ConvolutionFilter2DEXT)
#define glConvolutionParameterfEXT       (_GET_TLS_PROCTABLE()->ConvolutionParameterfEXT)
#define glConvolutionParameterfvEXT      (_GET_TLS_PROCTABLE()->ConvolutionParameterfvEXT)
#define glConvolutionParameteriEXT       (_GET_TLS_PROCTABLE()->ConvolutionParameteriEXT)
#define glConvolutionParameterivEXT      (_GET_TLS_PROCTABLE()->ConvolutionParameterivEXT)
#define glCopyConvolutionFilter1DEXT     (_GET_TLS_PROCTABLE()->CopyConvolutionFilter1DEXT)
#define glCopyConvolutionFilter2DEXT     (_GET_TLS_PROCTABLE()->CopyConvolutionFilter2DEXT)
#define glGetConvolutionFilterEXT        (_GET_TLS_PROCTABLE()->GetConvolutionFilterEXT)
#define glGetConvolutionParameterfvEXT   (_GET_TLS_PROCTABLE()->GetConvolutionParameterfvEXT)
#define glGetConvolutionParameterivEXT   (_GET_TLS_PROCTABLE()->GetConvolutionParameterivEXT)
#define glGetSeparableFilterEXT          (_GET_TLS_PROCTABLE()->GetSeparableFilterEXT)
#define glSeparableFilter2DEXT           (_GET_TLS_PROCTABLE()->SeparableFilter2DEXT)
#define glColorTableSGI                  (_GET_TLS_PROCTABLE()->ColorTableSGI)
#define glColorTableParameterfvSGI       (_GET_TLS_PROCTABLE()->ColorTableParameterfvSGI)
#define glColorTableParameterivSGI       (_GET_TLS_PROCTABLE()->ColorTableParameterivSGI)
#define glCopyColorTableSGI              (_GET_TLS_PROCTABLE()->CopyColorTableSGI)
#define glGetColorTableSGI               (_GET_TLS_PROCTABLE()->GetColorTableSGI)
#define glGetColorTableParameterfvSGI    (_GET_TLS_PROCTABLE()->GetColorTableParameterfvSGI)
#define glGetColorTableParameterivSGI    (_GET_TLS_PROCTABLE()->GetColorTableParameterivSGI)
#define glPixelTexGenSGIX                (_GET_TLS_PROCTABLE()->PixelTexGenSGIX)
#define glPixelTexGenParameteriSGIS      (_GET_TLS_PROCTABLE()->PixelTexGenParameteriSGIS)
#define glPixelTexGenParameterivSGIS     (_GET_TLS_PROCTABLE()->PixelTexGenParameterivSGIS)
#define glPixelTexGenParameterfSGIS      (_GET_TLS_PROCTABLE()->PixelTexGenParameterfSGIS)
#define glPixelTexGenParameterfvSGIS     (_GET_TLS_PROCTABLE()->PixelTexGenParameterfvSGIS)
#define glGetPixelTexGenParameterivSGIS  (_GET_TLS_PROCTABLE()->GetPixelTexGenParameterivSGIS)
#define glGetPixelTexGenParameterfvSGIS  (_GET_TLS_PROCTABLE()->GetPixelTexGenParameterfvSGIS)
#define glTexImage4DSGIS                 (_GET_TLS_PROCTABLE()->TexImage4DSGIS)
#define glTexSubImage4DSGIS              (_GET_TLS_PROCTABLE()->TexSubImage4DSGIS)
#define glAreTexturesResidentEXT         (_GET_TLS_PROCTABLE()->AreTexturesResidentEXT)
#define glBindTextureEXT                 (_GET_TLS_PROCTABLE()->BindTextureEXT)
#define glDeleteTexturesEXT              (_GET_TLS_PROCTABLE()->DeleteTexturesEXT)
#define glGenTexturesEXT                 (_GET_TLS_PROCTABLE()->GenTexturesEXT)
#define glIsTextureEXT                   (_GET_TLS_PROCTABLE()->IsTextureEXT)
#define glPrioritizeTexturesEXT          (_GET_TLS_PROCTABLE()->PrioritizeTexturesEXT)
#define glDetailTexFuncSGIS              (_GET_TLS_PROCTABLE()->DetailTexFuncSGIS)
#define glGetDetailTexFuncSGIS           (_GET_TLS_PROCTABLE()->GetDetailTexFuncSGIS)
#define glSharpenTexFuncSGIS             (_GET_TLS_PROCTABLE()->SharpenTexFuncSGIS)
#define glGetSharpenTexFuncSGIS          (_GET_TLS_PROCTABLE()->GetSharpenTexFuncSGIS)
#define glSampleMaskSGIS                 (_GET_TLS_PROCTABLE()->SampleMaskSGIS)
#define glSamplePatternSGIS              (_GET_TLS_PROCTABLE()->SamplePatternSGIS)
#define glArrayElementEXT                (_GET_TLS_PROCTABLE()->ArrayElementEXT)
#define glColorPointerEXT                (_GET_TLS_PROCTABLE()->ColorPointerEXT)
#define glDrawArraysEXT                  (_GET_TLS_PROCTABLE()->DrawArraysEXT)
#define glEdgeFlagPointerEXT             (_GET_TLS_PROCTABLE()->EdgeFlagPointerEXT)
#define glGetPointervEXT                 (_GET_TLS_PROCTABLE()->GetPointervEXT)
#define glIndexPointerEXT                (_GET_TLS_PROCTABLE()->IndexPointerEXT)
#define glNormalPointerEXT               (_GET_TLS_PROCTABLE()->NormalPointerEXT)
#define glTexCoordPointerEXT             (_GET_TLS_PROCTABLE()->TexCoordPointerEXT)
#define glVertexPointerEXT               (_GET_TLS_PROCTABLE()->VertexPointerEXT)
#define glBlendEquationEXT               (_GET_TLS_PROCTABLE()->BlendEquationEXT)
#define glSpriteParameterfSGIX           (_GET_TLS_PROCTABLE()->SpriteParameterfSGIX)
#define glSpriteParameterfvSGIX          (_GET_TLS_PROCTABLE()->SpriteParameterfvSGIX)
#define glSpriteParameteriSGIX           (_GET_TLS_PROCTABLE()->SpriteParameteriSGIX)
#define glSpriteParameterivSGIX          (_GET_TLS_PROCTABLE()->SpriteParameterivSGIX)
#define glPointParameterfARB             (_GET_TLS_PROCTABLE()->PointParameterfARB)
#define glPointParameterfvARB            (_GET_TLS_PROCTABLE()->PointParameterfvARB)
#define glPointParameterfEXT             (_GET_TLS_PROCTABLE()->PointParameterfEXT)
#define glPointParameterfvEXT            (_GET_TLS_PROCTABLE()->PointParameterfvEXT)
#define glPointParameterfSGIS            (_GET_TLS_PROCTABLE()->PointParameterfSGIS)
#define glPointParameterfvSGIS           (_GET_TLS_PROCTABLE()->PointParameterfvSGIS)
#define glGetInstrumentsSGIX             (_GET_TLS_PROCTABLE()->GetInstrumentsSGIX)
#define glInstrumentsBufferSGIX          (_GET_TLS_PROCTABLE()->InstrumentsBufferSGIX)
#define glPollInstrumentsSGIX            (_GET_TLS_PROCTABLE()->PollInstrumentsSGIX)
#define glReadInstrumentsSGIX            (_GET_TLS_PROCTABLE()->ReadInstrumentsSGIX)
#define glStartInstrumentsSGIX           (_GET_TLS_PROCTABLE()->StartInstrumentsSGIX)
#define glStopInstrumentsSGIX            (_GET_TLS_PROCTABLE()->StopInstrumentsSGIX)
#define glFrameZoomSGIX                  (_GET_TLS_PROCTABLE()->FrameZoomSGIX)
#define glTagSampleBufferSGIX            (_GET_TLS_PROCTABLE()->TagSampleBufferSGIX)
#define glDeformationMap3dSGIX           (_GET_TLS_PROCTABLE()->DeformationMap3dSGIX)
#define glDeformationMap3fSGIX           (_GET_TLS_PROCTABLE()->DeformationMap3fSGIX)
#define glDeformSGIX                     (_GET_TLS_PROCTABLE()->DeformSGIX)
#define glLoadIdentityDeformationMapSGIX (_GET_TLS_PROCTABLE()->LoadIdentityDeformationMapSGIX)
#define glReferencePlaneSGIX             (_GET_TLS_PROCTABLE()->ReferencePlaneSGIX)
#define glFlushRasterSGIX                (_GET_TLS_PROCTABLE()->FlushRasterSGIX)
#define glFogFuncSGIS                    (_GET_TLS_PROCTABLE()->FogFuncSGIS)
#define glGetFogFuncSGIS                 (_GET_TLS_PROCTABLE()->GetFogFuncSGIS)
#define glImageTransformParameteriHP     (_GET_TLS_PROCTABLE()->ImageTransformParameteriHP)
#define glImageTransformParameterfHP     (_GET_TLS_PROCTABLE()->ImageTransformParameterfHP)
#define glImageTransformParameterivHP    (_GET_TLS_PROCTABLE()->ImageTransformParameterivHP)
#define glImageTransformParameterfvHP    (_GET_TLS_PROCTABLE()->ImageTransformParameterfvHP)
#define glGetImageTransformParameterivHP (_GET_TLS_PROCTABLE()->GetImageTransformParameterivHP)
#define glGetImageTransformParameterfvHP (_GET_TLS_PROCTABLE()->GetImageTransformParameterfvHP)
#define glColorSubTableEXT               (_GET_TLS_PROCTABLE()->ColorSubTableEXT)
#define glCopyColorSubTableEXT           (_GET_TLS_PROCTABLE()->CopyColorSubTableEXT)
#define glHintPGI                        (_GET_TLS_PROCTABLE()->HintPGI)
#define glColorTableEXT                  (_GET_TLS_PROCTABLE()->ColorTableEXT)
#define glGetColorTableEXT               (_GET_TLS_PROCTABLE()->GetColorTableEXT)
#define glGetColorTableParameterivEXT    (_GET_TLS_PROCTABLE()->GetColorTableParameterivEXT)
#define glGetColorTableParameterfvEXT    (_GET_TLS_PROCTABLE()->GetColorTableParameterfvEXT)
#define glGetListParameterfvSGIX         (_GET_TLS_PROCTABLE()->GetListParameterfvSGIX)
#define glGetListParameterivSGIX         (_GET_TLS_PROCTABLE()->GetListParameterivSGIX)
#define glListParameterfSGIX             (_GET_TLS_PROCTABLE()->ListParameterfSGIX)
#define glListParameterfvSGIX            (_GET_TLS_PROCTABLE()->ListParameterfvSGIX)
#define glListParameteriSGIX             (_GET_TLS_PROCTABLE()->ListParameteriSGIX)
#define glListParameterivSGIX            (_GET_TLS_PROCTABLE()->ListParameterivSGIX)
#define glIndexMaterialEXT               (_GET_TLS_PROCTABLE()->IndexMaterialEXT)
#define glIndexFuncEXT                   (_GET_TLS_PROCTABLE()->IndexFuncEXT)
#define glLockArraysEXT                  (_GET_TLS_PROCTABLE()->LockArraysEXT)
#define glUnlockArraysEXT                (_GET_TLS_PROCTABLE()->UnlockArraysEXT)
#define glCullParameterdvEXT             (_GET_TLS_PROCTABLE()->CullParameterdvEXT)
#define glCullParameterfvEXT             (_GET_TLS_PROCTABLE()->CullParameterfvEXT)
#define glFragmentColorMaterialSGIX      (_GET_TLS_PROCTABLE()->FragmentColorMaterialSGIX)
#define glFragmentLightfSGIX             (_GET_TLS_PROCTABLE()->FragmentLightfSGIX)
#define glFragmentLightfvSGIX            (_GET_TLS_PROCTABLE()->FragmentLightfvSGIX)
#define glFragmentLightiSGIX             (_GET_TLS_PROCTABLE()->FragmentLightiSGIX)
#define glFragmentLightivSGIX            (_GET_TLS_PROCTABLE()->FragmentLightivSGIX)
#define glFragmentLightModelfSGIX        (_GET_TLS_PROCTABLE()->FragmentLightModelfSGIX)
#define glFragmentLightModelfvSGIX       (_GET_TLS_PROCTABLE()->FragmentLightModelfvSGIX)
#define glFragmentLightModeliSGIX        (_GET_TLS_PROCTABLE()->FragmentLightModeliSGIX)
#define glFragmentLightModelivSGIX       (_GET_TLS_PROCTABLE()->FragmentLightModelivSGIX)
#define glFragmentMaterialfSGIX          (_GET_TLS_PROCTABLE()->FragmentMaterialfSGIX)
#define glFragmentMaterialfvSGIX         (_GET_TLS_PROCTABLE()->FragmentMaterialfvSGIX)
#define glFragmentMaterialiSGIX          (_GET_TLS_PROCTABLE()->FragmentMaterialiSGIX)
#define glFragmentMaterialivSGIX         (_GET_TLS_PROCTABLE()->FragmentMaterialivSGIX)
#define glGetFragmentLightfvSGIX         (_GET_TLS_PROCTABLE()->GetFragmentLightfvSGIX)
#define glGetFragmentLightivSGIX         (_GET_TLS_PROCTABLE()->GetFragmentLightivSGIX)
#define glGetFragmentMaterialfvSGIX      (_GET_TLS_PROCTABLE()->GetFragmentMaterialfvSGIX)
#define glGetFragmentMaterialivSGIX      (_GET_TLS_PROCTABLE()->GetFragmentMaterialivSGIX)
#define glLightEnviSGIX                  (_GET_TLS_PROCTABLE()->LightEnviSGIX)
#define glDrawRangeElementsEXT           (_GET_TLS_PROCTABLE()->DrawRangeElementsEXT)
#define glApplyTextureEXT                (_GET_TLS_PROCTABLE()->ApplyTextureEXT)
#define glTextureLightEXT                (_GET_TLS_PROCTABLE()->TextureLightEXT)
#define glTextureMaterialEXT             (_GET_TLS_PROCTABLE()->TextureMaterialEXT)
#define glAsyncMarkerSGIX                (_GET_TLS_PROCTABLE()->AsyncMarkerSGIX)
#define glFinishAsyncSGIX                (_GET_TLS_PROCTABLE()->FinishAsyncSGIX)
#define glPollAsyncSGIX                  (_GET_TLS_PROCTABLE()->PollAsyncSGIX)
#define glGenAsyncMarkersSGIX            (_GET_TLS_PROCTABLE()->GenAsyncMarkersSGIX)
#define glDeleteAsyncMarkersSGIX         (_GET_TLS_PROCTABLE()->DeleteAsyncMarkersSGIX)
#define glIsAsyncMarkerSGIX              (_GET_TLS_PROCTABLE()->IsAsyncMarkerSGIX)
#define glVertexPointervINTEL            (_GET_TLS_PROCTABLE()->VertexPointervINTEL)
#define glNormalPointervINTEL            (_GET_TLS_PROCTABLE()->NormalPointervINTEL)
#define glColorPointervINTEL             (_GET_TLS_PROCTABLE()->ColorPointervINTEL)
#define glTexCoordPointervINTEL          (_GET_TLS_PROCTABLE()->TexCoordPointervINTEL)
#define glPixelTransformParameteriEXT    (_GET_TLS_PROCTABLE()->PixelTransformParameteriEXT)
#define glPixelTransformParameterfEXT    (_GET_TLS_PROCTABLE()->PixelTransformParameterfEXT)
#define glPixelTransformParameterivEXT   (_GET_TLS_PROCTABLE()->PixelTransformParameterivEXT)
#define glPixelTransformParameterfvEXT   (_GET_TLS_PROCTABLE()->PixelTransformParameterfvEXT)
#define glSecondaryColor3bEXT            (_GET_TLS_PROCTABLE()->SecondaryColor3bEXT)
#define glSecondaryColor3bvEXT           (_GET_TLS_PROCTABLE()->SecondaryColor3bvEXT)
#define glSecondaryColor3dEXT            (_GET_TLS_PROCTABLE()->SecondaryColor3dEXT)
#define glSecondaryColor3dvEXT           (_GET_TLS_PROCTABLE()->SecondaryColor3dvEXT)
#define glSecondaryColor3fEXT            (_GET_TLS_PROCTABLE()->SecondaryColor3fEXT)
#define glSecondaryColor3fvEXT           (_GET_TLS_PROCTABLE()->SecondaryColor3fvEXT)
#define glSecondaryColor3iEXT            (_GET_TLS_PROCTABLE()->SecondaryColor3iEXT)
#define glSecondaryColor3ivEXT           (_GET_TLS_PROCTABLE()->SecondaryColor3ivEXT)
#define glSecondaryColor3sEXT            (_GET_TLS_PROCTABLE()->SecondaryColor3sEXT)
#define glSecondaryColor3svEXT           (_GET_TLS_PROCTABLE()->SecondaryColor3svEXT)
#define glSecondaryColor3ubEXT           (_GET_TLS_PROCTABLE()->SecondaryColor3ubEXT)
#define glSecondaryColor3ubvEXT          (_GET_TLS_PROCTABLE()->SecondaryColor3ubvEXT)
#define glSecondaryColor3uiEXT           (_GET_TLS_PROCTABLE()->SecondaryColor3uiEXT)
#define glSecondaryColor3uivEXT          (_GET_TLS_PROCTABLE()->SecondaryColor3uivEXT)
#define glSecondaryColor3usEXT           (_GET_TLS_PROCTABLE()->SecondaryColor3usEXT)
#define glSecondaryColor3usvEXT          (_GET_TLS_PROCTABLE()->SecondaryColor3usvEXT)
#define glSecondaryColorPointerEXT       (_GET_TLS_PROCTABLE()->SecondaryColorPointerEXT)
#define glTextureNormalEXT               (_GET_TLS_PROCTABLE()->TextureNormalEXT)
#define glMultiDrawArraysEXT             (_GET_TLS_PROCTABLE()->MultiDrawArraysEXT)
#define glMultiDrawElementsEXT           (_GET_TLS_PROCTABLE()->MultiDrawElementsEXT)
#define glFogCoordfEXT                   (_GET_TLS_PROCTABLE()->FogCoordfEXT)
#define glFogCoordfvEXT                  (_GET_TLS_PROCTABLE()->FogCoordfvEXT)
#define glFogCoorddEXT                   (_GET_TLS_PROCTABLE()->FogCoorddEXT)
#define glFogCoorddvEXT                  (_GET_TLS_PROCTABLE()->FogCoorddvEXT)
#define glFogCoordPointerEXT             (_GET_TLS_PROCTABLE()->FogCoordPointerEXT)
#define glTangent3bEXT                   (_GET_TLS_PROCTABLE()->Tangent3bEXT)
#define glTangent3bvEXT                  (_GET_TLS_PROCTABLE()->Tangent3bvEXT)
#define glTangent3dEXT                   (_GET_TLS_PROCTABLE()->Tangent3dEXT)
#define glTangent3dvEXT                  (_GET_TLS_PROCTABLE()->Tangent3dvEXT)
#define glTangent3fEXT                   (_GET_TLS_PROCTABLE()->Tangent3fEXT)
#define glTangent3fvEXT                  (_GET_TLS_PROCTABLE()->Tangent3fvEXT)
#define glTangent3iEXT                   (_GET_TLS_PROCTABLE()->Tangent3iEXT)
#define glTangent3ivEXT                  (_GET_TLS_PROCTABLE()->Tangent3ivEXT)
#define glTangent3sEXT                   (_GET_TLS_PROCTABLE()->Tangent3sEXT)
#define glTangent3svEXT                  (_GET_TLS_PROCTABLE()->Tangent3svEXT)
#define glBinormal3bEXT                  (_GET_TLS_PROCTABLE()->Binormal3bEXT)
#define glBinormal3bvEXT                 (_GET_TLS_PROCTABLE()->Binormal3bvEXT)
#define glBinormal3dEXT                  (_GET_TLS_PROCTABLE()->Binormal3dEXT)
#define glBinormal3dvEXT                 (_GET_TLS_PROCTABLE()->Binormal3dvEXT)
#define glBinormal3fEXT                  (_GET_TLS_PROCTABLE()->Binormal3fEXT)
#define glBinormal3fvEXT                 (_GET_TLS_PROCTABLE()->Binormal3fvEXT)
#define glBinormal3iEXT                  (_GET_TLS_PROCTABLE()->Binormal3iEXT)
#define glBinormal3ivEXT                 (_GET_TLS_PROCTABLE()->Binormal3ivEXT)
#define glBinormal3sEXT                  (_GET_TLS_PROCTABLE()->Binormal3sEXT)
#define glBinormal3svEXT                 (_GET_TLS_PROCTABLE()->Binormal3svEXT)
#define glTangentPointerEXT              (_GET_TLS_PROCTABLE()->TangentPointerEXT)
#define glBinormalPointerEXT             (_GET_TLS_PROCTABLE()->BinormalPointerEXT)
#define glFinishTextureSUNX              (_GET_TLS_PROCTABLE()->FinishTextureSUNX)
#define glGlobalAlphaFactorbSUN          (_GET_TLS_PROCTABLE()->GlobalAlphaFactorbSUN)
#define glGlobalAlphaFactorsSUN          (_GET_TLS_PROCTABLE()->GlobalAlphaFactorsSUN)
#define glGlobalAlphaFactoriSUN          (_GET_TLS_PROCTABLE()->GlobalAlphaFactoriSUN)
#define glGlobalAlphaFactorfSUN          (_GET_TLS_PROCTABLE()->GlobalAlphaFactorfSUN)
#define glGlobalAlphaFactordSUN          (_GET_TLS_PROCTABLE()->GlobalAlphaFactordSUN)
#define glGlobalAlphaFactorubSUN         (_GET_TLS_PROCTABLE()->GlobalAlphaFactorubSUN)
#define glGlobalAlphaFactorusSUN         (_GET_TLS_PROCTABLE()->GlobalAlphaFactorusSUN)
#define glGlobalAlphaFactoruiSUN         (_GET_TLS_PROCTABLE()->GlobalAlphaFactoruiSUN)
#define glReplacementCodeuiSUN           (_GET_TLS_PROCTABLE()->ReplacementCodeuiSUN)
#define glReplacementCodeusSUN           (_GET_TLS_PROCTABLE()->ReplacementCodeusSUN)
#define glReplacementCodeubSUN           (_GET_TLS_PROCTABLE()->ReplacementCodeubSUN)
#define glReplacementCodeuivSUN          (_GET_TLS_PROCTABLE()->ReplacementCodeuivSUN)
#define glReplacementCodeusvSUN          (_GET_TLS_PROCTABLE()->ReplacementCodeusvSUN)
#define glReplacementCodeubvSUN          (_GET_TLS_PROCTABLE()->ReplacementCodeubvSUN)
#define glReplacementCodePointerSUN      (_GET_TLS_PROCTABLE()->ReplacementCodePointerSUN)
#define glColor4ubVertex2fSUN            (_GET_TLS_PROCTABLE()->Color4ubVertex2fSUN)
#define glColor4ubVertex2fvSUN           (_GET_TLS_PROCTABLE()->Color4ubVertex2fvSUN)
#define glColor4ubVertex3fSUN            (_GET_TLS_PROCTABLE()->Color4ubVertex3fSUN)
#define glColor4ubVertex3fvSUN           (_GET_TLS_PROCTABLE()->Color4ubVertex3fvSUN)
#define glColor3fVertex3fSUN             (_GET_TLS_PROCTABLE()->Color3fVertex3fSUN)
#define glColor3fVertex3fvSUN            (_GET_TLS_PROCTABLE()->Color3fVertex3fvSUN)
#define glNormal3fVertex3fSUN            (_GET_TLS_PROCTABLE()->Normal3fVertex3fSUN)
#define glNormal3fVertex3fvSUN           (_GET_TLS_PROCTABLE()->Normal3fVertex3fvSUN)
#define glColor4fNormal3fVertex3fSUN     (_GET_TLS_PROCTABLE()->Color4fNormal3fVertex3fSUN)
#define glColor4fNormal3fVertex3fvSUN    (_GET_TLS_PROCTABLE()->Color4fNormal3fVertex3fvSUN)
#define glTexCoord2fVertex3fSUN          (_GET_TLS_PROCTABLE()->TexCoord2fVertex3fSUN)
#define glTexCoord2fVertex3fvSUN         (_GET_TLS_PROCTABLE()->TexCoord2fVertex3fvSUN)
#define glTexCoord4fVertex4fSUN          (_GET_TLS_PROCTABLE()->TexCoord4fVertex4fSUN)
#define glTexCoord4fVertex4fvSUN         (_GET_TLS_PROCTABLE()->TexCoord4fVertex4fvSUN)
#define glTexCoord2fColor4ubVertex3fSUN  (_GET_TLS_PROCTABLE()->TexCoord2fColor4ubVertex3fSUN)
#define glTexCoord2fColor4ubVertex3fvSUN (_GET_TLS_PROCTABLE()->TexCoord2fColor4ubVertex3fvSUN)
#define glTexCoord2fColor3fVertex3fSUN   (_GET_TLS_PROCTABLE()->TexCoord2fColor3fVertex3fSUN)
#define glTexCoord2fColor3fVertex3fvSUN  (_GET_TLS_PROCTABLE()->TexCoord2fColor3fVertex3fvSUN)
#define glTexCoord2fNormal3fVertex3fSUN  (_GET_TLS_PROCTABLE()->TexCoord2fNormal3fVertex3fSUN)
#define glTexCoord2fNormal3fVertex3fvSUN (_GET_TLS_PROCTABLE()->TexCoord2fNormal3fVertex3fvSUN)
#define glTexCoord2fColor4fNormal3fVertex3fSUN (_GET_TLS_PROCTABLE()->TexCoord2fColor4fNormal3fVertex3fSUN)
#define glTexCoord2fColor4fNormal3fVertex3fvSUN (_GET_TLS_PROCTABLE()->TexCoord2fColor4fNormal3fVertex3fvSUN)
#define glTexCoord4fColor4fNormal3fVertex4fSUN (_GET_TLS_PROCTABLE()->TexCoord4fColor4fNormal3fVertex4fSUN)
#define glTexCoord4fColor4fNormal3fVertex4fvSUN (_GET_TLS_PROCTABLE()->TexCoord4fColor4fNormal3fVertex4fvSUN)
#define glReplacementCodeuiVertex3fSUN   (_GET_TLS_PROCTABLE()->ReplacementCodeuiVertex3fSUN)
#define glReplacementCodeuiVertex3fvSUN  (_GET_TLS_PROCTABLE()->ReplacementCodeuiVertex3fvSUN)
#define glReplacementCodeuiColor4ubVertex3fSUN (_GET_TLS_PROCTABLE()->ReplacementCodeuiColor4ubVertex3fSUN)
#define glReplacementCodeuiColor4ubVertex3fvSUN (_GET_TLS_PROCTABLE()->ReplacementCodeuiColor4ubVertex3fvSUN)
#define glReplacementCodeuiColor3fVertex3fSUN (_GET_TLS_PROCTABLE()->ReplacementCodeuiColor3fVertex3fSUN)
#define glReplacementCodeuiColor3fVertex3fvSUN (_GET_TLS_PROCTABLE()->ReplacementCodeuiColor3fVertex3fvSUN)
#define glReplacementCodeuiNormal3fVertex3fSUN (_GET_TLS_PROCTABLE()->ReplacementCodeuiNormal3fVertex3fSUN)
#define glReplacementCodeuiNormal3fVertex3fvSUN (_GET_TLS_PROCTABLE()->ReplacementCodeuiNormal3fVertex3fvSUN)
#define glReplacementCodeuiColor4fNormal3fVertex3fSUN (_GET_TLS_PROCTABLE()->ReplacementCodeuiColor4fNormal3fVertex3fSUN)
#define glReplacementCodeuiColor4fNormal3fVertex3fvSUN (_GET_TLS_PROCTABLE()->ReplacementCodeuiColor4fNormal3fVertex3fvSUN)
#define glReplacementCodeuiTexCoord2fVertex3fSUN (_GET_TLS_PROCTABLE()->ReplacementCodeuiTexCoord2fVertex3fSUN)
#define glReplacementCodeuiTexCoord2fVertex3fvSUN (_GET_TLS_PROCTABLE()->ReplacementCodeuiTexCoord2fVertex3fvSUN)
#define glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN (_GET_TLS_PROCTABLE()->ReplacementCodeuiTexCoord2fNormal3fVertex3fSUN)
#define glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN (_GET_TLS_PROCTABLE()->ReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN)
#define glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN (_GET_TLS_PROCTABLE()->ReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN)
#define glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN (_GET_TLS_PROCTABLE()->ReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN)
#define glBlendFuncSeparateEXT           (_GET_TLS_PROCTABLE()->BlendFuncSeparateEXT)
#define glBlendFuncSeparateINGR          (_GET_TLS_PROCTABLE()->BlendFuncSeparateINGR)
#define glVertexWeightfEXT               (_GET_TLS_PROCTABLE()->VertexWeightfEXT)
#define glVertexWeightfvEXT              (_GET_TLS_PROCTABLE()->VertexWeightfvEXT)
#define glVertexWeightPointerEXT         (_GET_TLS_PROCTABLE()->VertexWeightPointerEXT)
#define glFlushVertexArrayRangeNV        (_GET_TLS_PROCTABLE()->FlushVertexArrayRangeNV)
#define glVertexArrayRangeNV             (_GET_TLS_PROCTABLE()->VertexArrayRangeNV)
#define glCombinerParameterfvNV          (_GET_TLS_PROCTABLE()->CombinerParameterfvNV)
#define glCombinerParameterfNV           (_GET_TLS_PROCTABLE()->CombinerParameterfNV)
#define glCombinerParameterivNV          (_GET_TLS_PROCTABLE()->CombinerParameterivNV)
#define glCombinerParameteriNV           (_GET_TLS_PROCTABLE()->CombinerParameteriNV)
#define glCombinerInputNV                (_GET_TLS_PROCTABLE()->CombinerInputNV)
#define glCombinerOutputNV               (_GET_TLS_PROCTABLE()->CombinerOutputNV)
#define glFinalCombinerInputNV           (_GET_TLS_PROCTABLE()->FinalCombinerInputNV)
#define glGetCombinerInputParameterfvNV  (_GET_TLS_PROCTABLE()->GetCombinerInputParameterfvNV)
#define glGetCombinerInputParameterivNV  (_GET_TLS_PROCTABLE()->GetCombinerInputParameterivNV)
#define glGetCombinerOutputParameterfvNV (_GET_TLS_PROCTABLE()->GetCombinerOutputParameterfvNV)
#define glGetCombinerOutputParameterivNV (_GET_TLS_PROCTABLE()->GetCombinerOutputParameterivNV)
#define glGetFinalCombinerInputParameterfvNV (_GET_TLS_PROCTABLE()->GetFinalCombinerInputParameterfvNV)
#define glGetFinalCombinerInputParameterivNV (_GET_TLS_PROCTABLE()->GetFinalCombinerInputParameterivNV)
#define glResizeBuffersMESA              (_GET_TLS_PROCTABLE()->ResizeBuffersMESA)
#define glWindowPos2dMESA                (_GET_TLS_PROCTABLE()->WindowPos2dMESA)
#define glWindowPos2dvMESA               (_GET_TLS_PROCTABLE()->WindowPos2dvMESA)
#define glWindowPos2fMESA                (_GET_TLS_PROCTABLE()->WindowPos2fMESA)
#define glWindowPos2fvMESA               (_GET_TLS_PROCTABLE()->WindowPos2fvMESA)
#define glWindowPos2iMESA                (_GET_TLS_PROCTABLE()->WindowPos2iMESA)
#define glWindowPos2ivMESA               (_GET_TLS_PROCTABLE()->WindowPos2ivMESA)
#define glWindowPos2sMESA                (_GET_TLS_PROCTABLE()->WindowPos2sMESA)
#define glWindowPos2svMESA               (_GET_TLS_PROCTABLE()->WindowPos2svMESA)
#define glWindowPos3dMESA                (_GET_TLS_PROCTABLE()->WindowPos3dMESA)
#define glWindowPos3dvMESA               (_GET_TLS_PROCTABLE()->WindowPos3dvMESA)
#define glWindowPos3fMESA                (_GET_TLS_PROCTABLE()->WindowPos3fMESA)
#define glWindowPos3fvMESA               (_GET_TLS_PROCTABLE()->WindowPos3fvMESA)
#define glWindowPos3iMESA                (_GET_TLS_PROCTABLE()->WindowPos3iMESA)
#define glWindowPos3ivMESA               (_GET_TLS_PROCTABLE()->WindowPos3ivMESA)
#define glWindowPos3sMESA                (_GET_TLS_PROCTABLE()->WindowPos3sMESA)
#define glWindowPos3svMESA               (_GET_TLS_PROCTABLE()->WindowPos3svMESA)
#define glWindowPos4dMESA                (_GET_TLS_PROCTABLE()->WindowPos4dMESA)
#define glWindowPos4dvMESA               (_GET_TLS_PROCTABLE()->WindowPos4dvMESA)
#define glWindowPos4fMESA                (_GET_TLS_PROCTABLE()->WindowPos4fMESA)
#define glWindowPos4fvMESA               (_GET_TLS_PROCTABLE()->WindowPos4fvMESA)
#define glWindowPos4iMESA                (_GET_TLS_PROCTABLE()->WindowPos4iMESA)
#define glWindowPos4ivMESA               (_GET_TLS_PROCTABLE()->WindowPos4ivMESA)
#define glWindowPos4sMESA                (_GET_TLS_PROCTABLE()->WindowPos4sMESA)
#define glWindowPos4svMESA               (_GET_TLS_PROCTABLE()->WindowPos4svMESA)
#define glMultiModeDrawArraysIBM         (_GET_TLS_PROCTABLE()->MultiModeDrawArraysIBM)
#define glMultiModeDrawElementsIBM       (_GET_TLS_PROCTABLE()->MultiModeDrawElementsIBM)
#define glColorPointerListIBM            (_GET_TLS_PROCTABLE()->ColorPointerListIBM)
#define glSecondaryColorPointerListIBM   (_GET_TLS_PROCTABLE()->SecondaryColorPointerListIBM)
#define glEdgeFlagPointerListIBM         (_GET_TLS_PROCTABLE()->EdgeFlagPointerListIBM)
#define glFogCoordPointerListIBM         (_GET_TLS_PROCTABLE()->FogCoordPointerListIBM)
#define glIndexPointerListIBM            (_GET_TLS_PROCTABLE()->IndexPointerListIBM)
#define glNormalPointerListIBM           (_GET_TLS_PROCTABLE()->NormalPointerListIBM)
#define glTexCoordPointerListIBM         (_GET_TLS_PROCTABLE()->TexCoordPointerListIBM)
#define glVertexPointerListIBM           (_GET_TLS_PROCTABLE()->VertexPointerListIBM)
#define glTbufferMask3DFX                (_GET_TLS_PROCTABLE()->TbufferMask3DFX)
#define glSampleMaskEXT                  (_GET_TLS_PROCTABLE()->SampleMaskEXT)
#define glSamplePatternEXT               (_GET_TLS_PROCTABLE()->SamplePatternEXT)
#define glTextureColorMaskSGIS           (_GET_TLS_PROCTABLE()->TextureColorMaskSGIS)
#define glIglooInterfaceSGIX             (_GET_TLS_PROCTABLE()->IglooInterfaceSGIX)
#define glGenFencesNV                    (_GET_TLS_PROCTABLE()->GenFencesNV)
#define glDeleteFencesNV                 (_GET_TLS_PROCTABLE()->DeleteFencesNV)
#define glSetFenceNV                     (_GET_TLS_PROCTABLE()->SetFenceNV)
#define glTestFenceNV                    (_GET_TLS_PROCTABLE()->TestFenceNV)
#define glFinishFenceNV                  (_GET_TLS_PROCTABLE()->FinishFenceNV)
#define glIsFenceNV                      (_GET_TLS_PROCTABLE()->IsFenceNV)
#define glGetFenceivNV                   (_GET_TLS_PROCTABLE()->GetFenceivNV)
#define glMapControlPointsNV             (_GET_TLS_PROCTABLE()->MapControlPointsNV)
#define glMapParameterivNV               (_GET_TLS_PROCTABLE()->MapParameterivNV)
#define glMapParameterfvNV               (_GET_TLS_PROCTABLE()->MapParameterfvNV)
#define glGetMapControlPointsNV          (_GET_TLS_PROCTABLE()->GetMapControlPointsNV)
#define glGetMapParameterivNV            (_GET_TLS_PROCTABLE()->GetMapParameterivNV)
#define glGetMapParameterfvNV            (_GET_TLS_PROCTABLE()->GetMapParameterfvNV)
#define glGetMapAttribParameterivNV      (_GET_TLS_PROCTABLE()->GetMapAttribParameterivNV)
#define glGetMapAttribParameterfvNV      (_GET_TLS_PROCTABLE()->GetMapAttribParameterfvNV)
#define glEvalMapsNV                     (_GET_TLS_PROCTABLE()->EvalMapsNV)
#define glCombinerStageParameterfvNV     (_GET_TLS_PROCTABLE()->CombinerStageParameterfvNV)
#define glGetCombinerStageParameterfvNV  (_GET_TLS_PROCTABLE()->GetCombinerStageParameterfvNV)
#define glBindProgramNV                  (_GET_TLS_PROCTABLE()->BindProgramNV)
#define glDeleteProgramsNV               (_GET_TLS_PROCTABLE()->DeleteProgramsNV)
#define glExecuteProgramNV               (_GET_TLS_PROCTABLE()->ExecuteProgramNV)
#define glGenProgramsNV                  (_GET_TLS_PROCTABLE()->GenProgramsNV)
#define glAreProgramsResidentNV          (_GET_TLS_PROCTABLE()->AreProgramsResidentNV)
#define glRequestResidentProgramsNV      (_GET_TLS_PROCTABLE()->RequestResidentProgramsNV)
#define glGetProgramParameterfvNV        (_GET_TLS_PROCTABLE()->GetProgramParameterfvNV)
#define glGetProgramParameterdvNV        (_GET_TLS_PROCTABLE()->GetProgramParameterdvNV)
#define glGetProgramivNV                 (_GET_TLS_PROCTABLE()->GetProgramivNV)
#define glGetProgramStringNV             (_GET_TLS_PROCTABLE()->GetProgramStringNV)
#define glGetTrackMatrixivNV             (_GET_TLS_PROCTABLE()->GetTrackMatrixivNV)
#define glGetVertexAttribdvNV            (_GET_TLS_PROCTABLE()->GetVertexAttribdvNV)
#define glGetVertexAttribfvNV            (_GET_TLS_PROCTABLE()->GetVertexAttribfvNV)
#define glGetVertexAttribivNV            (_GET_TLS_PROCTABLE()->GetVertexAttribivNV)
#define glGetVertexAttribPointervNV      (_GET_TLS_PROCTABLE()->GetVertexAttribPointervNV)
#define glIsProgramNV                    (_GET_TLS_PROCTABLE()->IsProgramNV)
#define glLoadProgramNV                  (_GET_TLS_PROCTABLE()->LoadProgramNV)
#define glProgramParameter4fNV           (_GET_TLS_PROCTABLE()->ProgramParameter4fNV)
#define glProgramParameter4dNV           (_GET_TLS_PROCTABLE()->ProgramParameter4dNV)
#define glProgramParameter4dvNV          (_GET_TLS_PROCTABLE()->ProgramParameter4dvNV)
#define glProgramParameter4fvNV          (_GET_TLS_PROCTABLE()->ProgramParameter4fvNV)
#define glProgramParameters4dvNV         (_GET_TLS_PROCTABLE()->ProgramParameters4dvNV)
#define glProgramParameters4fvNV         (_GET_TLS_PROCTABLE()->ProgramParameters4fvNV)
#define glTrackMatrixNV                  (_GET_TLS_PROCTABLE()->TrackMatrixNV)
#define glVertexAttribPointerNV          (_GET_TLS_PROCTABLE()->VertexAttribPointerNV)
#define glVertexAttrib1sNV               (_GET_TLS_PROCTABLE()->VertexAttrib1sNV)
#define glVertexAttrib1fNV               (_GET_TLS_PROCTABLE()->VertexAttrib1fNV)
#define glVertexAttrib1dNV               (_GET_TLS_PROCTABLE()->VertexAttrib1dNV)
#define glVertexAttrib2sNV               (_GET_TLS_PROCTABLE()->VertexAttrib2sNV)
#define glVertexAttrib2fNV               (_GET_TLS_PROCTABLE()->VertexAttrib2fNV)
#define glVertexAttrib2dNV               (_GET_TLS_PROCTABLE()->VertexAttrib2dNV)
#define glVertexAttrib3sNV               (_GET_TLS_PROCTABLE()->VertexAttrib3sNV)
#define glVertexAttrib3fNV               (_GET_TLS_PROCTABLE()->VertexAttrib3fNV)
#define glVertexAttrib3dNV               (_GET_TLS_PROCTABLE()->VertexAttrib3dNV)
#define glVertexAttrib4sNV               (_GET_TLS_PROCTABLE()->VertexAttrib4sNV)
#define glVertexAttrib4fNV               (_GET_TLS_PROCTABLE()->VertexAttrib4fNV)
#define glVertexAttrib4dNV               (_GET_TLS_PROCTABLE()->VertexAttrib4dNV)
#define glVertexAttrib4ubNV              (_GET_TLS_PROCTABLE()->VertexAttrib4ubNV)
#define glVertexAttrib1svNV              (_GET_TLS_PROCTABLE()->VertexAttrib1svNV)
#define glVertexAttrib1fvNV              (_GET_TLS_PROCTABLE()->VertexAttrib1fvNV)
#define glVertexAttrib1dvNV              (_GET_TLS_PROCTABLE()->VertexAttrib1dvNV)
#define glVertexAttrib2svNV              (_GET_TLS_PROCTABLE()->VertexAttrib2svNV)
#define glVertexAttrib2fvNV              (_GET_TLS_PROCTABLE()->VertexAttrib2fvNV)
#define glVertexAttrib2dvNV              (_GET_TLS_PROCTABLE()->VertexAttrib2dvNV)
#define glVertexAttrib3svNV              (_GET_TLS_PROCTABLE()->VertexAttrib3svNV)
#define glVertexAttrib3fvNV              (_GET_TLS_PROCTABLE()->VertexAttrib3fvNV)
#define glVertexAttrib3dvNV              (_GET_TLS_PROCTABLE()->VertexAttrib3dvNV)
#define glVertexAttrib4svNV              (_GET_TLS_PROCTABLE()->VertexAttrib4svNV)
#define glVertexAttrib4fvNV              (_GET_TLS_PROCTABLE()->VertexAttrib4fvNV)
#define glVertexAttrib4dvNV              (_GET_TLS_PROCTABLE()->VertexAttrib4dvNV)
#define glVertexAttrib4ubvNV             (_GET_TLS_PROCTABLE()->VertexAttrib4ubvNV)
#define glVertexAttribs1svNV             (_GET_TLS_PROCTABLE()->VertexAttribs1svNV)
#define glVertexAttribs1fvNV             (_GET_TLS_PROCTABLE()->VertexAttribs1fvNV)
#define glVertexAttribs1dvNV             (_GET_TLS_PROCTABLE()->VertexAttribs1dvNV)
#define glVertexAttribs2svNV             (_GET_TLS_PROCTABLE()->VertexAttribs2svNV)
#define glVertexAttribs2fvNV             (_GET_TLS_PROCTABLE()->VertexAttribs2fvNV)
#define glVertexAttribs2dvNV             (_GET_TLS_PROCTABLE()->VertexAttribs2dvNV)
#define glVertexAttribs3svNV             (_GET_TLS_PROCTABLE()->VertexAttribs3svNV)
#define glVertexAttribs3fvNV             (_GET_TLS_PROCTABLE()->VertexAttribs3fvNV)
#define glVertexAttribs3dvNV             (_GET_TLS_PROCTABLE()->VertexAttribs3dvNV)
#define glVertexAttribs4svNV             (_GET_TLS_PROCTABLE()->VertexAttribs4svNV)
#define glVertexAttribs4fvNV             (_GET_TLS_PROCTABLE()->VertexAttribs4fvNV)
#define glVertexAttribs4dvNV             (_GET_TLS_PROCTABLE()->VertexAttribs4dvNV)
#define glVertexAttribs4ubvNV            (_GET_TLS_PROCTABLE()->VertexAttribs4ubvNV)
#define glAddSwapHintRectWIN             (_GET_TLS_PROCTABLE()->AddSwapHintRectWIN)
#ifdef _WIN32
#define wglCreateBufferRegionARB          (_GET_TLS_PROCTABLE()->CreateBufferRegionARB)
#define wglDeleteBufferRegionARB          (_GET_TLS_PROCTABLE()->DeleteBufferRegionARB)
#define wglSaveBufferRegionARB            (_GET_TLS_PROCTABLE()->SaveBufferRegionARB)
#define wglRestoreBufferRegionARB         (_GET_TLS_PROCTABLE()->RestoreBufferRegionARB)
#define wglGetExtensionsStringARB         (_GET_TLS_PROCTABLE()->GetExtensionsStringARB)
#define wglGetPixelFormatAttribivARB      (_GET_TLS_PROCTABLE()->GetPixelFormatAttribivARB)
#define wglGetPixelFormatAttribfvARB      (_GET_TLS_PROCTABLE()->GetPixelFormatAttribfvARB)
#define wglChoosePixelFormatARB           (_GET_TLS_PROCTABLE()->ChoosePixelFormatARB)
#define wglMakeContextCurrentARB          (_GET_TLS_PROCTABLE()->MakeContextCurrentARB)
#define wglGetCurrentReadDCARB            (_GET_TLS_PROCTABLE()->GetCurrentReadDCARB)
#define wglCreatePbufferARB               (_GET_TLS_PROCTABLE()->CreatePbufferARB)
#define wglGetPbufferDCARB                (_GET_TLS_PROCTABLE()->GetPbufferDCARB)
#define wglReleasePbufferDCARB            (_GET_TLS_PROCTABLE()->ReleasePbufferDCARB)
#define wglDestroyPbufferARB              (_GET_TLS_PROCTABLE()->DestroyPbufferARB)
#define wglQueryPbufferARB                (_GET_TLS_PROCTABLE()->QueryPbufferARB)
#define wglCreateDisplayColorTableEXT     (_GET_TLS_PROCTABLE()->CreateDisplayColorTableEXT)
#define wglLoadDisplayColorTableEXT       (_GET_TLS_PROCTABLE()->LoadDisplayColorTableEXT)
#define wglBindDisplayColorTableEXT       (_GET_TLS_PROCTABLE()->BindDisplayColorTableEXT)
#define wglDestroyDisplayColorTableEXT    (_GET_TLS_PROCTABLE()->DestroyDisplayColorTableEXT)
#define wglGetExtensionsStringEXT         (_GET_TLS_PROCTABLE()->GetExtensionsStringEXT)
#define wglMakeContextCurrentEXT          (_GET_TLS_PROCTABLE()->MakeContextCurrentEXT)
#define wglGetCurrentReadDCEXT            (_GET_TLS_PROCTABLE()->GetCurrentReadDCEXT)
#define wglCreatePbufferEXT               (_GET_TLS_PROCTABLE()->CreatePbufferEXT)
#define wglGetPbufferDCEXT                (_GET_TLS_PROCTABLE()->GetPbufferDCEXT)
#define wglReleasePbufferDCEXT            (_GET_TLS_PROCTABLE()->ReleasePbufferDCEXT)
#define wglDestroyPbufferEXT              (_GET_TLS_PROCTABLE()->DestroyPbufferEXT)
#define wglQueryPbufferEXT                (_GET_TLS_PROCTABLE()->QueryPbufferEXT)
#define wglGetPixelFormatAttribivEXT      (_GET_TLS_PROCTABLE()->GetPixelFormatAttribivEXT)
#define wglGetPixelFormatAttribfvEXT      (_GET_TLS_PROCTABLE()->GetPixelFormatAttribfvEXT)
#define wglChoosePixelFormatEXT           (_GET_TLS_PROCTABLE()->ChoosePixelFormatEXT)
#define wglSwapIntervalEXT                (_GET_TLS_PROCTABLE()->SwapIntervalEXT)
#define wglGetSwapIntervalEXT             (_GET_TLS_PROCTABLE()->GetSwapIntervalEXT)
#endif /* _WIN32 */

#ifndef _APP_PROCTABLE

/*
 * Applications can replace the following function with its own function
 * for accessing thread local proc/context dependent proc table.
 * The following default function works for most applications which
 * are using the same device for all their contexts - even if 
 * the contexts are on different threads.
 */

static _inline _GLextensionProcs *_GET_TLS_PROCTABLE(void)

{
	extern _GLextensionProcs _extensionProcs;

	return (&_extensionProcs);
}

#else

/*
 * Application should replace this compiled function with 
 * an inlined function for maximum performance. 
 */

extern _GLextensionProcs *_GET_TLS_PROCTABLE(void);

#endif

/*
 * Provide an initialization function for the application 
 * to initialize its own proc tables in case the application 
 * needs to use multiple proc tables. 
 */

static _inline void _InitExtensionProcs(_GLextensionProcs *appProcs)
{
	extern _GLextensionProcs _extensionProcs;

	*appProcs = _extensionProcs;
}

#ifdef __cplusplus
}
#endif


#endif /* _GLPROCS_H_ */
