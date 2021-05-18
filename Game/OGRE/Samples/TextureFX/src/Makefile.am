INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/Samples/TextureFX/include -I$(top_srcdir)/OgreMain/include -I$(top_srcdir)/Samples/Common/include

noinst_PROGRAMS = TextureFX
TextureFX_SOURCES = TextureFX.cpp
TextureFX_LDFLAGS = -L$(top_builddir)/OgreMain/src
TextureFX_LDADD = -lOgreMain

install-exec-local:
	$(INSTALL_PROGRAM) .libs/TextureFX $(top_builddir)/Samples/Common/bin/TextureFX
