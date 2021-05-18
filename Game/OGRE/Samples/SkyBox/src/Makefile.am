INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/Samples/SkyBox/include -I$(top_srcdir)/OgreMain/include -I$(top_srcdir)/Samples/Common/include

noinst_PROGRAMS = SkyBox
SkyBox_SOURCES = SkyBox.cpp
SkyBox_LDFLAGS = -L$(top_builddir)/OgreMain/src
SkyBox_LDADD = -lOgreMain

install-exec-local:
	$(INSTALL_PROGRAM) .libs/SkyBox $(top_builddir)/Samples/Common/bin/SkyBox
