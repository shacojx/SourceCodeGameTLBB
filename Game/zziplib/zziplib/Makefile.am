AUTOMAKE_OPTIONS = 1.4 foreign
AUTOTOOL_VERSION=autoconf-2.52 automake-1.5 libtool-1.4.2

bin_PROGRAMS = zzcat zzdir zzxorcat zzxordir zzxorcopy
noinst_PROGRAMS = zziptest zzobfuscated zzipwrap
bin_SCRIPTS = zzip-config
lib_LTLIBRARIES = libzzip.la libzzipwrap.la
include_HEADERS = $(libzzip_la_headers) $(libzzipwrap_la_headers)
noinst_HEADERS = __debug.h __hints.h __mmap.h __dirent.h
readme_FILES = README TODO COPYING.ZZIP COPYING.LIB COPYING.ZLIB 
#
VERSION_INFO=@VERSION_INFO@
RELEASE_INFO=@RELEASE_INFO@
THREAD_SAFE=@THREAD_SAFE@
#

libzzip_la_SOURCES = \
	zzip-zip.c \
	zzip-file.c \
	zzip-dir.c \
	zzip-stat.c \
	zzip-info.c \
	zzip-err.c \
	zzip-io.c
libzzip_la_headers = \
	zzip-stdint.h \
	zzip-file.h \
	zzip-io.h \
	zzip.h \
	zziplib.h \
	zzipformat.h \
	zzip-conf.h \
	zzip-config.h \
	zzip-msvc.h
libzzip_la_LDFLAGS= @ZZIPLIB_LDFLAGS@ $(RELEASE_INFO) $(VERSION_INFO) \
                    $(THREAD_SAFE)
libzzip_la_LIBADD= -lz @RESOLVES@

libzzipwrap_la_SOURCES = \
	zzipwrap-mem.c
libzzipwrap_la_headers = \
	zzipwrap.h

libzzipwrap_la_LDFLAGS= @ZZIPLIB_LDFLAGS@ $(RELEASE_INFO) $(VERSION_INFO) \
		        $(THREAD_SAFE)
libzzipwrap_la_LIBADD= libzzip.la @RESOLVES@ -lz

SDL_rwops_FILES = SDL_rwops_zzip.h SDL_rwops_zzip.c SDL_rwops_zzcat.c \
	     README.SDL

MSVC6_DIST = README.MSVC6 zziplib.dsw zziplib.dsp zziplib_DLL.dsp \
           zzcat.dsp zzdir.dsp zziptest.dsp zzipwrap.dsp zzobfuscated.dsp
WCC10_DIST = zziplib.wpj
WCC10_CLEAN = *.obj *.lk1 *.mk1 *.mk *.sym *.tgt

EXTRA_DIST = test.zip test1.zip $(readme_FILES) \
        $(PACKAGE).spec $(SDL_rwops_FILES) $(MSVC6_DIST) $(WCC10_DIST) \
	zzip-msvc.sed config.h.in zzip-config.in 
CONFIG_CLEAN_FILES = zzip-config.h 
CLEANFILES = $(WCC10_CLEAN)

zziptest_LDFLAGS = @ZZIPLIB_LDFLAGS@
zziptest_LDADD = libzzip.la @RESOLVES@ -lz
zzcat_LDFLAGS =  @ZZIPLIB_LDFLAGS@
zzcat_LDADD = libzzip.la @RESOLVES@ -lz
zzdir_LDFLAGS =  @ZZIPLIB_LDFLAGS@
zzdir_LDADD = libzzip.la @RESOLVES@ -lz
zzxorcat_LDFLAGS =  @ZZIPLIB_LDFLAGS@
zzxorcat_LDADD = libzzip.la @RESOLVES@ -lz
zzxordir_LDFLAGS =  @ZZIPLIB_LDFLAGS@
zzxordir_LDADD = libzzip.la @RESOLVES@ -lz
zzobfuscated_LDFLAGS =  @ZZIPLIB_LDFLAGS@
zzobfuscated_LDADD = libzzip.la @RESOLVES@ -lz
zzipwrap_LDFLAGS =  @ZZIPLIB_LDFLAGS@
zzipwrap_LDADD = libzzipwrap.la @RESOLVES@ libzzip.la -lz

SDL_rwops_zzip.$(OBJEXT) : SDL_rwops_zzip.c
	$(COMPILE) $< -c -o $@ `sdl-config --cflags` `zzip-config --cflags`  
SDL_rwops_zzcat.$(OBJEXT) : SDL_rwops_zzcat.c
	$(COMPILE) $< -c -o $@ `sdl-config --cflags` `zzip-config --cflags`  
testsdl: SDL_rwops_zzip.$(OBJEXT) SDL_rwops_zzcat.$(OBJEXT) 
	$(LINK) SDL_rwops_zzip.$(OBJEXT) SDL_rwops_zzcat.$(OBJEXT) \
		`sdl-config --libs` `zzip-config --libs` -o zzcatsdl$(EXEEXT)
	zzcatsdl$(EXEEXT) test/README

# -------------------------------------------------------------------
NULL=/dev/null

testzip testzips : test.zip test1.zip test2.zip test3.zip 

test.zip : @MAINTAINER_MODE_TRUE@ README
	$(MKZIP) test.zip README
test1.zip :
	@ echo $(MKZIP) $@ ... \
	; mkdir test.tmp \
	; for i in 1 2 3 4 5 6 8 9 \
	; do echo "file-$i" >test.tmp/file.$$i ; done \
	; echo " a plain text file here" >test.tmp/README \
	; (cd test.tmp && $(MKZIP) ../$@ file.* >$(NULL)) \
	; rm -rf test.tmp

test2.zip : # will not be shipped in source tarball
	@ echo $(MKZIP) $@ ... \
	; mkdir test.tmp \
	; for i in 1 2 3 4 5 6 8 9 ; do for j in 1 2 3 4 5 6 7 8 9 \
	; do echo "file-$i" >test.tmp/file.$$j$$i ; done done \
	; (cd test.tmp && $(MKZIP) ../$@ file.* >$(NULL)) \
	; rm -rf test.tmp

test3.zip : # will not be shipped in source tarball
	@ echo $(MKZIP) $@ ... \
	; mkdir test.tmp ; for h in 1 2 3 4 5 6 7 8 9 ; do : \
	; for i in 1 2 3 4 5 6 8 9 ; do for j in 1 2 3 4 5 6 7 8 9 \
	; do echo "file-$i" >test.tmp/file.$$h$$j$$i ; done done done \
	; (cd test.tmp && $(MKZIP) ../$@ file.* >$(NULL)) \
	; rm -rf test.tmp

test.dat : test.zip
	./zzxorcopy$(EXEEXT) $? $@
test1.dat : test1.zip
	./zzxorcopy$(EXEEXT) $? $@
test2.dat : test2.zip
	./zzxorcopy$(EXEEXT) $? $@
test3.dat : test3.zip
	./zzxorcopy$(EXEEXT) $? $@

# -------------------------------------------------------------------

stdtests = @ZIPTESTS@ check-test0 check-test1 check-zzdir check-zzcat
xortests = @ZIPTESTS@ check-zzxor check-zzxordir check-zzxorcat

check-local: check-readme $(stdtests) $(xortests)

check-readme : zzcat$(EXEEXT) @MAINTAINER_MODE_TRUE@ test.zip
	@ echo checking ./zzcat$(EXEXT) test/README 
	@ ./zzcat$(EXEXT) test/README >test.out
	@ if diff test.out README >$(NULL) \
	; then rm test.out ; echo check OK ; true \
	; else rm test.out ; echo check FAIL ; false ; fi

check-zzxor : zzxorcat$(EXEEXT) test.dat
	@ echo checking ./zzxorcat$(EXEXT) test.dat/README 
	@ ./zzxorcat$(EXEXT) test.dat/README >test.out
	@ if diff test.out README >$(NULL) \
	; then rm test.out ; echo check OK ; true \
	; else rm test.out ; echo check FAIL ; false ; fi

test: test0 test1 test2 test3
test0: zziptest$(EXEEXT)
	./zziptest$(EXEEXT) test.zip
test1: zziptest$(EXEEXT)
	./zziptest$(EXEEXT) test1.zip
test2: zzcat$(EXEEXT)
	./zzcat$(EXEEXT) test/README
test3: zzdir$(EXTEXT)
	./zzdir test1.zip

check-test0 : zziptest$(EXEEXT)
	@ echo ___ $@ ___
	./zziptest$(EXEEXT) test.zip >test.out
	rm test.out
check-test1 : zziptest$(EXEEXT)
	@ echo ___ $@ ___ "(about ten seconds)"
	./zziptest$(EXEEXT) test1.zip >test.out
	rm test.out

check-zzdir : test1.zip test2.zip test3.zip zzdir$(EXEEXT) 
	@ echo ___ $@ ___
	   ./zzdir$(EXEEXT) test1 > test.out
	@ X="zzdir$(EXEEXT) test1 "\
	; if grep file.1 test.out >$(NULL) ; then : \
	; if grep file.9 test.out >$(NULL) ; then : \
	; echo ..$$X.. OK ; true ; else echo ..$$X.. FAIL ; false ; fi fi
	   ./zzdir$(EXEEXT) test2 > test.out
	@ X="zzdir$(EXEEXT) test2 "\
	; if grep file.11 test.out >$(NULL) ; then : \
	; if grep file.99 test.out >$(NULL) ; then : \
	; echo ..$$X.. OK ; true ; else echo ..$$X.. FAIL ; false ; fi fi
	   ./zzdir$(EXEEXT) test3 > test.out
	@ X="zzdir$(EXEEXT) test3 "\
	; if grep file.111 test.out >$(NULL) ; then : \
	; if grep file.999 test.out >$(NULL) ; then : \
	; echo ..$$X.. OK ; true ; else echo ..$$X.. FAIL ; false ; fi fi
	@ X="zzdir$(EXEEXT) test000" ; echo ./$$X "(negative test)" \
	; if ./zzdir$(EXEEXT) test000 >test.out ; then : \
	; echo ..$$X.. FAIL ; false ; else echo ..$$X.. OK ; true ; fi 
	rm test.out

check-zzcat : test1.zip test2.zip test3.zip zzcat$(EXEEXT)
	@ echo ___ $@ ___
	   ./zzcat$(EXEEXT) test1/file.1 test1/file.9 > test.out
	@ X="zzcat$(EXEEXT) test1/file.1 test1/file.9 "\
	; if grep file-1 test.out >$(NULL) ; then : \
	; if grep file-9 test.out >$(NULL) ; then : \
	; echo ..$$X.. OK ; true ; else echo ..$$X.. FAIL ; false ; fi fi
	   ./zzcat$(EXEEXT) test2/file.11 test2/file.99 > test.out
	@ X="zzcat$(EXEEXT) test2/file.10 test2/file.99 "\
	; if grep file-10 test.out >$(NULL) ; then : \
	; if grep file-99 test.out >$(NULL) ; then : \
	; echo ..$$X.. OK ; true ; else echo ..$$X.. FAIL ; false ; fi fi
	   ./zzcat$(EXEEXT) test3/file.111 test3/file.999 > test.out
	@ X="zzcat$(EXEEXT) test3/file.111 test3/file.999 "\
	; if grep file-100 test.out >$(NULL) ; then : \
	; if grep file-999 test.out >$(NULL) ; then : \
	; echo ..$$X.. OK ; true ; else echo ..$$X.. FAIL ; false ; fi fi
	@ X="zzcat$(EXEEXT) test1/file.999" ; echo ./$$X "(negative test)" \
	; if ./zzdir$(EXEEXT) test1/file.999 >test.out ; then : \
	; echo ..$$X.. FAIL ; false ; else echo ..$$X.. OK ; true ; fi 
	rm test.out

check-zzxordir : test1.dat test2.dat zzxordir$(EXEEXT) 
	@ echo ___ $@ ___
	   ./zzxordir$(EXEEXT) test1 > test.out
	@ X="zzxordir$(EXEEXT) test1 "\
	; if grep file.1 test.out >$(NULL) ; then : \
	; if grep file.9 test.out >$(NULL) ; then : \
	; echo ..$$X.. OK ; true ; else echo ..$$X.. FAIL ; false ; fi fi
	   ./zzxordir$(EXEEXT) test2.dat > test.out
	@ X="zzxordir$(EXEEXT) test2.dat "\
	; if grep file.11 test.out >$(NULL) ; then : \
	; if grep file.99 test.out >$(NULL) ; then : \
	; echo ..$$X.. OK ; true ; else echo ..$$X.. FAIL ; false ; fi fi
	rm test.out

check-zzxorcat : test1.dat test2.dat zzxorcat$(EXEEXT)
	@ echo ___ $@ ___
	   ./zzxorcat$(EXEEXT) test1/file.1 test1/file.9 > test.out
	@ X="zzxorcat$(EXEEXT) test1/file.1 test1/file.9 "\
	; if grep file-1 test.out >$(NULL) ; then : \
	; if grep file-9 test.out >$(NULL) ; then : \
	; echo ..$$X.. OK ; true ; else echo ..$$X.. FAIL ; false ; fi fi
	   ./zzxorcat$(EXEEXT) test2.dat/file.11 test2/file.99 > test.out
	@ X="zzxorcat$(EXEEXT) test2.dat/file.10 test2/file.99 "\
	; if grep file-10 test.out >$(NULL) ; then : \
	; if grep file-99 test.out >$(NULL) ; then : \
	; echo ..$$X.. OK ; true ; else echo ..$$X.. FAIL ; false ; fi fi
	rm test.out

# -------------------------------------------------------------------
ECHON = echo -n
BRUTE = 0    # default is less than a thousand tests
brute :      # which is still a brute force selftest
	@ echo $(MKZIP) $@: \
	; for u in 1 2 3 4 5 6 8 9 ; do for v in 1 2 3 4 5 6 7 8 9 ; do : \
	; for w in 1 2 3 4 5 6 8 9 ; do for x in $(BRUTE) ; do : \
	; $(ECHON) $$u$$v$$w$$x: \
	; rm -rf test.tmp ; mkdir test.tmp \
	; for i in 1 2 3 4 5 6 8 9 ; do for j in 1 2 3 4 5 6 7 8 9 ; do : \
	; for m in 1 2 3 4 5 6 8 9 ; do for n in $(BRUTE) ; do : \
	; echo "file-$$i$$j$$m$$n" >test.tmp/file$$i$$j$$m$$n.txt \
	;        test "$$i$$j$$m$$n" = "$$u$$v$$w$$x" && break \
	; done ; test "$$i$$j$$m$$n" = "$$u$$v$$w$$x" && break \
	; done ; test "$$i$$j$$m$$n" = "$$u$$v$$w$$x" && break \
	; done ; test "$$i$$j$$m$$n" = "$$u$$v$$w$$x" && break \
	; done ; test -f test.tmp/file$$i$$j$$m$$n.txt || exit 2 \
	; (cd test.tmp && $(MKZIP) ../test$$u$$v$$w$$x.zip file*.* >$(NULL)) \
	; rm -rf test.tmp \
	; for i in 1 2 3 4 5 6 8 9 ; do for j in 1 2 3 4 5 6 7 8 9 ; do : \
	; for m in 1 2 3 4 5 6 8 9 ; do for n in $(BRUTE) ; do : \
	; ./zzcat$(EXEEXT) test$$u$$v$$w$$x/file$$i$$j$$m$$n.txt >test.out \
	; if grep file-$$i$$j$$m$$n test.out >$(NULL) ; then rm test.out \
	; else echo ERROR file-$$i$$j$$m$$n diff in test$$u$$v$$w$$x \
	; exit 1 ; fi \
	; ./zzdir$(EXEEXT) test$$u$$v$$w$$x >test.out \
	; if grep file$$i$$j$$m$$n.txt test.out >$(NULL) ; then rm test.out \
	; else echo ERROR file$$i$$j$$m$$n.txt not in test$$u$$v$$w$$x \
	; exit 1 ; fi \
	;        test "$$i$$j$$m$$n" = "$$u$$v$$w$$x" && break \
	; done ; test "$$i$$j$$m$$n" = "$$u$$v$$w$$x" && break \
	; done ; test "$$i$$j$$m$$n" = "$$u$$v$$w$$x" && break \
	; done ; test "$$i$$j$$m$$n" = "$$u$$v$$w$$x" && break \
	; done ; rm test$$u$$v$$w$$x.zip \
	; done ; done ; done ; done ; true

# -------------------------------------------------------------------
auto:
	aclocal && autoconf && autoheader && automake

boottrap:
	rm -rf .deps .libs
	rm -f config.guess config.sub stamp-h.in
	rm -f install-sh ltconfig ltmain.sh depcomp mkinstalldirs
	rm -f config.h config.h.in config.log config.cache configure
	rm -f aclocal.m4 Makefile Makefile.in
	aclocal 
	autoconf 
	autoheader 
	automake -a -c 

rpm: dist $(PACKAGE).spec
	rpm -ta $(PACKAGE)-$(VERSION).tar.gz

# ---- ADDITION after change to zzip/* style --- NEEDED for `make dist`
zzip-config.h : ../zzip/_config.h
	- ln -s $< $@
zzip-msvc.h : ../zzip/_msvc.h
	- ln -s $< $@
