noinst_HEADERS = OgreSDLConfig_cli.h \
				OgreSDLConfig_gtk.h \
				OgreSDLError.h \
				OgreSDLInput.h \
				OgreSDLTimer.h

all:
	-rm -f OgreSDLConfig.h
	$(LN_S) $(srcdir)/OgreSDLConfig_$(OGRE_CFGTK).h OgreSDLConfig.h
