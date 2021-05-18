INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/PlatformManagers/gtk/include \
-I$(top_srcdir)/OgreMain/include -DSHAREDIR=\"$(pkgdatadir)\" \
-I$(top_srcdir)/RenderSystems/GL/include $(PLATFORM_CFLAGS) $(GLSUPPORT_CFLAGS)

platformdir = $(libdir)
platform_PROGRAMS = libOgrePlatform.so

libOgrePlatform_so_SOURCES = GTKConfig.cpp\
                             GTKInput.cpp \
                             GTKTimer.cpp \
                             Platform.cpp

libOgrePlatform_so_LDFLAGS = $(SHARED_FLAGS) -L$(top_builddir)/OgreMain/src
libOgrePlatform_so_LDADD = $(PLATFORM_LIBS) $(GL_LIBS) -lOgreMain

pkgdata_DATA = ogre-config.glade ogre-gtk-side.png
