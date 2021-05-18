INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/Samples/Lighting/include -I$(top_srcdir)/OgreMain/include -I$(top_srcdir)/Samples/Common/include

noinst_PROGRAMS = Lighting

Lighting_SOURCES = Lighting.cpp
Lighting_LDFLAGS = -L$(top_builddir)/OgreMain/src
Lighting_LDADD = -lOgreMain

install-exec-local:
	$(INSTALL_PROGRAM) .libs/Lighting $(top_builddir)/Samples/Common/bin/Lighting
