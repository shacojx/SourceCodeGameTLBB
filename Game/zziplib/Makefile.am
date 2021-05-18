AUTOMAKE_OPTIONS = 1.4 foreign dist-bzip2
AUTOTOOL_VERSION=autoconf-2.52 automake-1.5 libtool-1.4.2

DIST_SUBDIRS = zzip zzipwrap bins test docs  SDL  zziplib
SUBDIRS =      zzip zzipwrap bins test docs @SDL@

# see Makefile.mk for the "make rpm" target
rpm2: dist-bzip2 $(PACKAGE).spec
	rpm -ta $(PACKAGE)-$(VERSION).tar.bz2

zzip.html zzip.xml zzip.pdf \
doc docs clean-doc clean-docs man manpages htmpages unpack clean-unpack \
omf install-omf install-doc install-sfnet install-man3 x :
	(cd docs && $(MAKE) `basename $@`)

sdl testsdl test-sdl install-sdl :
	(cd SDL && $(MAKE) `basename $@`)

check-test0 check-test1 check-zzdir check-zzcat \
check-zzxor check-zzxordir check-zzxorcat \
check-sfx     check-readme : \
	(cd test && $(MAKE) `basename $@`)

MSVC7 =   msvc7/pkzip.exe msvc7/test1.zip msvc7/test.zip \
msvc7/zzdir.vcproj    msvc7/zzipself.vcproj     msvc7/zzip.vcproj \
msvc7/zziplib.sln     msvc7/zzipsetstub.sln     msvc7/zzobfuscated.sln \
msvc7/zziplib.vcproj  msvc7/zzipsetstub.vcproj  msvc7/zzobfuscated.vcproj \
msvc7/zzcat.sln       msvc7/zzipself.bat        msvc7/zzip.sln \
msvc7/zzcat.vcproj    msvc7/zzipself.sln        msvc7/zziptest.sln \
msvc7/zzdir.sln       msvc7/zzipself.txt        msvc7/zziptest.vcproj

MSVC6 = \
msvc6/zzcat.dsp        msvc6/zziplib.dsp      msvc6/zzipwrap.dsp \
msvc6/zzdir.dsp        msvc6/zziplib.dsw      msvc6/zzobfuscated.dsp \
msvc6/zziptest.dsp     msvc6/zzip.dsp  

EXTRA_DIST =   zziplib.spec zzipback.sed Makefile.mk $(MSVC7) $(MSVC6)

.PHONY : auto boottrap rpm doc docs man manpages htmpages sdl testsdl \
         comp compats msvc6 msvc7
# ------------------------------------------------------------------------

compatsrcdir=$(srcdir)
C = $(compatsrcdir)/zziplib
S = $(srcdir)
ZZIPBACK = sed -f $(srcdir)/zzipback.sed
comp compats : zzipback.sed
	cp $S//ChangeLog $C
	cp $S//config.h.in $C
	head $S//zziplib.spec > $C/zziplib.spec
	$(ZZIPBACK) $S/zzip/__debug.h        > $C/__debug.h
	$(ZZIPBACK) $S/zzip/__dirent.h       > $C/__dirent.h
	$(ZZIPBACK) $S/zzip/__hints.h        > $C/__hints.h
	$(ZZIPBACK) $S/zzip/__mmap.h         > $C/__mmap.h
#	$(ZZIPBACK) $S/zzip/_config.h        > $C/zzip-config.h
#	$(ZZIPBACK) $S/zzip/_msvc.h          > $C/zzip-msvc.h
	$(ZZIPBACK) $S/zzip/conf.h           > $C/zzip-conf.h
	$(ZZIPBACK) $S/zzip/dir.c            > $C/zzip-dir.c
	$(ZZIPBACK) $S/zzip/err.c            > $C/zzip-err.c
	$(ZZIPBACK) $S/zzip/file.c           > $C/zzip-file.c
	$(ZZIPBACK) $S/zzip/file.h           > $C/zzip-file.h
	$(ZZIPBACK) $S/zzip/format.h         > $C/zzipformat.h
	$(ZZIPBACK) $S/zzip/info.c           > $C/zzip-info.c
	$(ZZIPBACK) $S/zzip/lib.h            > $C/zzip.h
	$(ZZIPBACK) $S/zzip/plugin.c         > $C/zzip-io.c
	$(ZZIPBACK) $S/zzip/plugin.h         > $C/zzip-plugin.h
	$(ZZIPBACK) $S/zzip/stat.c           > $C/zzip-stat.c
	$(ZZIPBACK) $S/zzip/stdint.h         > $C/zzip-stdint.h
	$(ZZIPBACK) $S/zzip/zip.c            > $C/zzip-zip.c
	$(ZZIPBACK) $S/zzip/zzip.h           > $C/zziplib.h
	$(ZZIPBACK) $S/zzipwrap/wrap.c       > $C/zzipwrap-mem.c
	$(ZZIPBACK) $S/zzipwrap/wrap.h       > $C/zzipwrap.h
	$(ZZIPBACK) $S/zzipwrap/zzipwrap.c   > $C/zzipwrap.c
	$(ZZIPBACK) $S/bins/zzcat.c          > $C/zzcat.c
	$(ZZIPBACK) $S/bins/zzdir.c          > $C/zzdir.c
	$(ZZIPBACK) $S/bins/zzip-config.in   > $C/zzip-config.in
	$(ZZIPBACK) $S/bins/zziptest.c       > $C/zziptest.c
	$(ZZIPBACK) $S/bins/zzobfuscated.c   > $C/zzobfuscated.c
	$(ZZIPBACK) $S/bins/zzxorcat.c       > $C/zzxorcat.c
	$(ZZIPBACK) $S/bins/zzxorcopy.c      > $C/zzxorcopy.c
	$(ZZIPBACK) $S/bins/zzxordir.c       > $C/zzxordir.c
	$(ZZIPBACK) $S/SDL/SDL_rwops_zzcat.c > $C/SDL_rwops_zzcat.c
	$(ZZIPBACK) $S/SDL/SDL_rwops_zzip.c  > $C/SDL_rwops_zzip.c
	$(ZZIPBACK) $S/SDL/SDL_rwops_zzip.h  > $C/SDL_rwops_zzip.h

dist-hook :
	$(MAKE) compats "C=$(distdir)/zziplib"
	sed -e 's/top_builddir *=.*/top_builddir = ./' $S/zziplib/Makefile.in \
		                             > $(distdir)/zziplib/Makefile.in

test-comp:
	test ! -d testing || rm -r testing
	mkdir testing
	(cd testing && tar xzvf ../$(PACKAGE)-$(VERSION).tar.gz)
	(cd "testing/$(PACKAGE)-$(VERSION)/zziplib" && sh configure)
	$(MAKE) -C  "testing/$(PACKAGE)-$(VERSION)/zziplib"
	$(MAKE) -C  "testing/$(PACKAGE)-$(VERSION)/zziplib"  check

clean-comp:
	test ! -d testing || rm -r testing

msvc :
	mkdir bin
	mkdir lib
	mkdir include
	mkdir include/zzip
	cp $(srcdir)/msvc6/Release/*.exe bin/
	cp $(srcdir)/msvc6/Release/*.dll bin/
	cp $(srcdir)/msvc6/Release/*.lib lib/
	cp $(srcdir)/zzip/*.h include/zzip/
	zip -9r $(srcdir)/$(PACKAGE)-$(VERSION)-msvc6-bin.zip bin/
	zip -9r $(srcdir)/$(PACKAGE)-$(VERSION)-msvc6-lib.zip lib/ include/
	rm -r bin lib include
