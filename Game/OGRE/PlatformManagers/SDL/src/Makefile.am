INCLUDES = $(STLPORT_CFLAGS) -I$(top_builddir)/PlatformManagers/SDL/include -I$(top_srcdir)/PlatformManagers/SDL/include -I$(top_srcdir)/OgreMain/include $(SDL_CFLAGS) $(CFGTK_DEPS_CFLAGS) -DSHAREDIR=\"$(pkgdatadir)\"

platformdir = $(libdir)
platform_LTLIBRARIES = libOgrePlatform.la

libOgrePlatform_la_SOURCES = OgreSDLConfig.cpp\
                          OgreSDLInput.cpp \
						  OgreSDLTimer.cpp \
                          OgreSDLPlatform.cpp

BUILT_SOURCES = OgreSDLConfig.cpp
EXTRA_DIST = OgreSDLConfig_gtk.cpp OgreSDLConfig_cli.cpp


libOgrePlatform_la_LDFLAGS = -module $(PLUGIN_FLAGS) -L$(top_builddir)/OgreMain/src -Wl,-z,defs
libOgrePlatform_la_LIBADD = $(SDL_LIBS) $(CFGTK_DEPS_LIBS) $(GL_LIBS) -lOgreMain

pkgdata_DATA = ogre-config.glade

OgreSDLConfig.cpp:
	-rm -f OgreSDLConfig.cpp
	$(LN_S) $(srcdir)/OgreSDLConfig_$(OGRE_CFGTK).cpp OgreSDLConfig.cpp

clean-local:
	-rm -f $(BUILT_SOURCES)
