INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/Samples/CubeMapping/include -I$(top_srcdir)/OgreMain/include -I$(top_srcdir)/Samples/Common/include

noinst_PROGRAMS = CubeMapping
CubeMapping_SOURCES = CubeMapping.cpp
CubeMapping_LDFLAGS = -L$(top_builddir)/OgreMain/src
CubeMapping_LDADD = -lOgreMain

install-exec-local:
	$(INSTALL_PROGRAM) .libs/CubeMapping $(top_builddir)/Samples/Common/bin/CubeMapping
