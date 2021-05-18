INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/Samples/Fresnel/include -I$(top_srcdir)/OgreMain/include -I$(top_srcdir)/Samples/Common/include

noinst_PROGRAMS = Fresnel

Fresnel_SOURCES = Fresnel.cpp
Fresnel_LDFLAGS = -L$(top_builddir)/OgreMain/src
Fresnel_LDADD = -lOgreMain

install-exec-local:
	$(INSTALL_PROGRAM) .libs/Fresnel $(top_builddir)/Samples/Common/bin/Fresnel
