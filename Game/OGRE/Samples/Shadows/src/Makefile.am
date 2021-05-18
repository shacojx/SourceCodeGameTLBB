INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/OgreMain/include -I$(top_srcdir)/Samples/Common/include

noinst_PROGRAMS = Shadows
Shadows_SOURCES = Shadows.cpp
Shadows_LDFLAGS = -L$(top_builddir)/OgreMain/src
Shadows_LDADD = -lOgreMain

install-exec-local:
	$(INSTALL_PROGRAM) .libs/Shadows $(top_builddir)/Samples/Common/bin/Shadows
