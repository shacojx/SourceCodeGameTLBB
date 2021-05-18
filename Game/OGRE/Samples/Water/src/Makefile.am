INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/Samples/Water/include -I$(top_srcdir)/OgreMain/include -I$(top_srcdir)/Samples/Common/include

noinst_PROGRAMS = Water

Water_SOURCES = Water.cpp WaterMesh.cpp
Water_LDFLAGS = -L$(top_builddir)/OgreMain/src
Water_LDADD = -lOgreMain

install-exec-local:
	$(INSTALL_PROGRAM) .libs/Water $(top_builddir)/Samples/Common/bin/Water
