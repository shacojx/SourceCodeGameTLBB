INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/Samples/EnvMapping/include -I$(top_srcdir)/OgreMain/include -I$(top_srcdir)/Samples/Common/include

noinst_PROGRAMS = EnvMapping

EnvMapping_SOURCES = EnvMapping.cpp
EnvMapping_LDFLAGS = -L$(top_builddir)/OgreMain/src
EnvMapping_LDADD = -lOgreMain

install-exec-local:
	$(INSTALL_PROGRAM) .libs/EnvMapping $(top_builddir)/Samples/Common/bin/EnvMapping
