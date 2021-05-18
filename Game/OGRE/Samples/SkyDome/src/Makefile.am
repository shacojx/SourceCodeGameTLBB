INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/Samples/SkyDome/include -I$(top_srcdir)/OgreMain/include -I$(top_srcdir)/Samples/Common/include

noinst_PROGRAMS = SkyDome
SkyDome_SOURCES = SkyDome.cpp
SkyDome_LDFLAGS = -L$(top_builddir)/OgreMain/src
SkyDome_LDADD = -lOgreMain

install-exec-local:
	$(INSTALL_PROGRAM) .libs/SkyDome $(top_builddir)/Samples/Common/bin/SkyDome
