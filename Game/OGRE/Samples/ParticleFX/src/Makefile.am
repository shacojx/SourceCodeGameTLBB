INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/Samples/ParticleFX/include -I$(top_srcdir)/OgreMain/include -I$(top_srcdir)/Samples/Common/include

noinst_PROGRAMS = ParticleFX

ParticleFX_SOURCES = ParticleFX.cpp
ParticleFX_LDFLAGS = -L$(top_builddir)/OgreMain/src
ParticleFX_LDADD = -lOgreMain

install-exec-local:
	$(INSTALL_PROGRAM) .libs/ParticleFX $(top_builddir)/Samples/Common/bin/ParticleFX
