INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/RenderSystems/GL/include \
           -I$(top_srcdir)/OgreMain/include $(GLSUPPORT_CFLAGS) \
           -I$(top_srcdir)/PlatformManagers/$(OGRE_PLATFORM)/include \
           -I$(top_srcdir)/RenderSystems/GL/src/$(OGRE_GLSUPPORT) \
           -I$(top_srcdir)/RenderSystems/GL/src/GLSL/include

noinst_LTLIBRARIES=libGLSL.la
libGLSL_la_SOURCES = OgreGLSLExtSupport.cpp \
                    OgreGLSLLinkProgramManager.cpp \
                    OgreGLSLGpuProgram.cpp \
                    OgreGLSLProgram.cpp \
                    OgreGLSLLinkProgram.cpp \
                    OgreGLSLProgramFactory.cpp
libGLSL_la_LDFLAGS =
