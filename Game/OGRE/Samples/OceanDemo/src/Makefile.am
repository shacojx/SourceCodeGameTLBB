INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/Samples/OceanDemo/include -I$(top_srcdir)/OgreMain/include -I$(top_srcdir)/Samples/Common/CEGUIRenderer/include -I$(top_srcdir)/Samples/Common/include @CEGUI_CFLAGS@

noinst_PROGRAMS = OceanDemo

OceanDemo_SOURCES = MaterialControls.cpp OceanDemo.cpp
OceanDemo_LDFLAGS = -L$(top_builddir)/OgreMain/src -L$(top_builddir)/Samples/Common/CEGUIRenderer/src
OceanDemo_LDADD = -lOgreMain @CEGUI_LIBS@ -lCEGUIOgreRenderer

install-exec-local:
	$(INSTALL_PROGRAM) .libs/OceanDemo $(top_builddir)/Samples/Common/bin/OceanDemo
