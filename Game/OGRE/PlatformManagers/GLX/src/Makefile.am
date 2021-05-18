INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/PlatformManagers/GLX/include -I$(top_srcdir)/RenderSystems/GL/include -I$(top_srcdir)/RenderSystems/GL/include/GLX -I$(top_srcdir)/OgreMain/include $(PLATFORM_CFLAGS) -DSHAREDIR=\"$(pkgdatadir)\" 

platformdir = $(libdir)
platform_LTLIBRARIES = libOgrePlatform.la

libOgrePlatform_la_SOURCES = OgreGLXConfig.cpp\
                          OgreGLXInput.cpp \
			  OgreGLXTimer.cpp \
                          OgreGLXPlatform.cpp

libOgrePlatform_la_LDFLAGS = -module $(SHARED_FLAGS) -L$(top_builddir)/OgreMain/src
libOgrePlatform_la_LIBADD = $(PLATFORM_LIBS) $(GL_LIBS) -lOgreMain -lXaw

