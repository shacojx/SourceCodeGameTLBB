INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/Samples/CelShading/include -I$(top_srcdir)/OgreMain/include -I$(top_srcdir)/Samples/Common/include

noinst_PROGRAMS = CelShading

CelShading_SOURCES = CelShading.cpp
CelShading_LDFLAGS = -L$(top_builddir)/OgreMain/src
CelShading_LDADD = -lOgreMain

install-exec-local:
	$(INSTALL_PROGRAM) .libs/CelShading $(top_builddir)/Samples/Common/bin/CelShading
