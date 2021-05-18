INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/Samples/RenderToTexture/include -I$(top_srcdir)/OgreMain/include -I$(top_srcdir)/Samples/Common/include

noinst_PROGRAMS = RenderToTexture
RenderToTexture_SOURCES = RenderToTexture.cpp
RenderToTexture_LDFLAGS = -L$(top_builddir)/OgreMain/src
RenderToTexture_LDADD = -lOgreMain

install-exec-local:
	$(INSTALL_PROGRAM) .libs/RenderToTexture $(top_builddir)/Samples/Common/bin/RenderToTexture
