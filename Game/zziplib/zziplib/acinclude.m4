dnl acinclude.m4 generated automatically by ac-archive's acinclude 0.5.39

dnl Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY, to the extent permitted by law; without
dnl even the implied warranty of MERCHANTABILITY or FITNESS FOR A
dnl PARTICULAR PURPOSE.

dnl ______ /usr/share/aclocal/guidod/ac_set_default_paths_system.m4 ______
dnl @synopsis AC_SET_DEFAULT_PATHS_SYSTEM
dnl
dnl the most interesting changes go about windows-targets - where the
dnl default_prefix is set to /programs, and quite some directories
dnl are aliased: sbindir := libdir := bindir  and the docprefix-defaults
dnl are also a bit different, even on FHS2-compliant systems where the
dnl mandir is going to $prefix/man only if prefix=/usr, otherwise they
dnl shall go to $datadir/man. We use an extra docprefix to express it
dnl which is either defined as being prefix or datadir. not SUBSTed here.
dnl
dnl @version %Id: ac_set_default_paths_system.m4,v 1.4 2001/08/25 08:31:23 guidod Exp %
dnl @author Guido Draheim <guidod@gmx.de>       STATUS: one time use
AC_DEFUN([AC_SET_DEFAULT_PATHS_SYSTEM],
[AC_REQUIRE([AC_CANONICAL_HOST]) # --------------------------------------------
case "$prefix:$ac_default_prefix" in
  NONE:/usr/local)
    result=""
    AC_MSG_CHECKING(default prefix path)
    case "${target_os}" in
      *cygwin* | *mingw* | *uwin* | *djgpp | *emx* )
	if test "${host_os}" = "${target_os}" ; then
           ac_default_prefix="/programs"
           result="(win/dos target)"
        else
           case "$PATH" in
              *:/usr/local/cross-tools/$target_alias/bin:*)
	          ac_default_prefix="/usr/local/cross-tools/$target_alias" ;;
              *:/usr/local/$target_alias/bin:*)
	          ac_default_prefix="/usr/local/$target_alias" ;;
              *:/usr/local/$target_cpu-$target_os/bin:*)
	          ac_default_prefix="/usr/local/$target_cpu-$target_os" ;;
              *)  
                  ac_default_prefix="/programs" ;;
           esac
           result="(win/dos cross-compiler)"
        fi
    ;;
    esac
    AC_MSG_RESULT($ac_default_prefix $result)
  ;;
esac
AC_MSG_CHECKING(default prefix system)
result="$prefix" ; test "$result" = "NONE" && result="$ac_default_prefix"
case ${result} in
  /programs | /programs/*) result="is win-/programs"
     # on win/dos, .exe .dll and .cfg live in the same directory
     libdir=`echo $libdir |sed -e 's:^..exec_prefix./lib$:${bindir}:'`
     sbindir=`echo $sbindir |sed -e 's:^..exec_prefix./sbin$:${libdir}:'`
     sysconfdir=`echo $sysconfdir |sed -e 's:^..prefix./etc$:${sbindir}:'`
     libexecdir=`echo $libexecdir |sed -e 's:/libexec$:/system:'`
     # help-files shall be set with --infodir, docprefix is datadir
     docprefix="${datadir}"
     mandir=`echo $mandir \
	                     |sed -e 's:^..prefix./man$:${datadir}/info:'`
     includedir=`echo $includedir \
                |sed -e 's:^..prefix./include$:${datadir}/include:'`
     # other state files (but /etc) are moved to datadir
     sharedstatedir=`echo $sharedstatedir \
                     |sed -e 's:^..prefix./com$:${datadir}/default:'`
     localstatedir=`echo $localstatedir \
                     |sed -e 's:^..prefix./var$:${datadir}/current:'`
  ;;
  /usr) result="is /usr-shipped"
     # doc files are left at prefix
     docprefix="${prefix}"
     # state files go under /top
     sysconfdir=`echo $sysconfdir |sed -e 's:^..prefix./etc$:/etc:'`
     sharedstatedir=`echo $sharedstatedir \
                     |sed -e 's:^..prefix./com$:/etc/default:'`
     # $prefix/var is going to end up in /var/lib
     localstatedir=`echo $localstatedir \
                     |sed -e 's:^..prefix./var$:/var/lib:'`
  ;;
  /opt | /opt/*) result="is /opt-package"
     # state files go under /top/prefix
     sysconfdir=`echo $sysconfdir \
                     |sed -e 's:^..prefix./etc$:/etc${prefix}:'`
     sharedstatedir=`echo $sharedstatedir \
                     |sed -e 's:^..prefix./com$:/etc/default${prefix}:'`
     # $prefix/var is going to to be /var$prefix... once again
     localstatedir=`echo $localstatedir \
                     |sed -e 's:^..prefix./var$:/var${prefix}:'`
     # doc files are left at prefix
     docprefix="${prefix}"
  ;;
  *) result="is /local-package"
     # doc files are moved from prefix down to datadir
     docprefix="${datadir}"
     mandir=`echo $mandir \
                     |sed -e 's:^..prefix./man$:${datadir}/man:'`
     infodir=`echo $infodir \
                     |sed -e 's:^..prefix./infodir$:${datadir}/info:'`
     # never use $prefix/com - that is no good idea
     sharedstatedir=`echo $sharedstatedir \
                     |sed -e 's:^..prefix./com$:${sysconfdir}/default:'`
  ;;
esac
AC_MSG_RESULT($result)
# -------------------------------------------------------- 
])     

dnl ______ /usr/share/aclocal/guidod/ac_spec_package_version.m4 ______
dnl @SYNPOPSIS AC_SPEC_PACKAGE_VERSION(rpmspecfile)
dnl
dnl set PACKAGE and VERSION from the defines in the given specfile
dnl default to basename and currentde if rpmspecfile is not found
dnl
dnl @version %Id: ac_spec_package_version.m4,v 1.1.1.1 2001/07/26 00:46:36 guidod Exp %
dnl @author Guido Draheim <guidod@gmx.de>

AC_DEFUN([AC_SPEC_PACKAGE_VERSION],[dnl
  pushdef([specfile], ac_spec_package_version_file)
  specfile=`basename $1`
  AC_MSG_CHECKING( $specfile package version)
  if test -z "$1"; then
    AC_MSG_ERROR( no rpm spec file given )
  else
    # find specfile
    for i in $1 $srcdir/$1 $srcdir/../$1 ; do
      if test -f "$i" ; then
        specfile="$i"
        break
      fi
    done
    if test ! -f $specfile ; then
      k="w/o spec"
    else
      if test -z "$PACKAGE" ; then
        i=`grep -i '^name:' $specfile | head -1 | sed -e 's/.*://'`
	PACKAGE=`echo $i | sed -e 's/ /-/'`
      fi
      if test -z "$VERSION" ; then
        i=`grep -i '^version:' $specfile | head -1 | sed -e 's/.*://'`
	VERSION=`echo $i | sed -e 's/ /-/'`
      fi
    fi
    if test -z "$PACKAGE" ; then
      PACKAGE=`basename $specfile .spec`
    fi
    if test -z "$VERSION" ; then
      VERSION=`date +%Y.%m.%d`
    fi
    AC_MSG_RESULT( $PACKAGE $VERSION $k )
  fi
])

  


dnl ______ /usr/share/aclocal/guidod/ac_set_releaseinfo_versioninfo.m4 ______
dnl AC_SET_RELEASEINFO_VERSIONINFO [(VERSION)]                    -*- sh -*-
dnl   default $1 = $VERSION                                     
dnl
dnl check the $VERSION number and cut the two last digit-sequences off
dnl which will form a -version-info in a @VERSIONINFO@ ac_subst while
dnl the rest is going to the -release name in a @RELEASEINFO@ ac_subst.
dnl
dnl you should keep these two seperate - the release-name may contain
dnl alpha-characters and can be modified later with extra release-hints
dnl e.g. RELEASEINFO="$RELEASINFO-debug" for a debug version of your lib.
dnl
dnl example: a VERSION="2.4.18" will be transformed into
dnl           -release 2 -version-info 4:18
dnl and for a linux-target this will tell libtool to install the lib as
dnl           libmy.so libmy.la libmy.a libmy-2.so.4 libmy-2.so.4.0.18
dnl and executables will get link-resolve-infos for libmy-2.so.4 - therefore
dnl the patch-level is ignored during ldso linking, and ldso will use the
dnl one with the highest patchlevel. Using just "-release $(VERSION)"
dnl during libtool-linking would not do that - omitting the -version-info
dnl will libtool install libmy.so libmy.la libmy.a libmy-2.4.18.so and
dnl executables would get hardlinked with the 2.4.18 version of your lib.
dnl
dnl This background does also explain the default dll name for a win32
dnl target : libtool will choose to make up libmy-2-4.dll for this 
dnl version spec.
dnl
dnl this macro does set the three parts VERSION_REL.VERSION_REQ.VERSION_REL
dnl from the VERSION-spec but does not ac_subst them like the two INFOs.
dnl If you prefer a two-part VERSION-spec, the VERSION_REL will still be set,
dnl either to the host_cpu or just a simple "00".
dnl You may add sublevel parts like "1.4.2-ac5" where the sublevel is 
dnl just killed from these versioninfo/releasinfo substvars.
dnl
dnl @version %Id: ac_set_releaseinfo_versioninfo.m4,v 1.2 2001/08/25 15:59:53 guidod Exp %
dnl @author Guido Draheim <guidod@gmx.de>

AC_DEFUN([AC_SET_RELEASEINFO_VERSIONINFO],
[# ------ AC SET RELEASEINFO VERSIONINFO --------------------------------
AC_MSG_CHECKING(version info)
  VERSION_REQ=`echo ifelse( $1, , $VERSION, $1 )` # VERSION_TMP really...
  VERSION_REL=`echo $VERSION_REQ | sed -e 's/[[.]][[^.]]*$//'`  # delete micro
  VERSION_REV=`echo $VERSION_REQ | sed -e "s/^$VERSION_REL.//"` # the rest
  VERSION_REQ=`echo $VERSION_REL | sed -e 's/.*[[.]]//'`  # delete prefix now
  VERSION_REV=`echo $VERSION_REV | sed -e 's/[[^0-9]].*//'` # 5-p4 -> 5
  if test "$VERSION_REQ" != "$VERSION_REL" ; then # three-part version...
  VERSION_REL=`echo $VERSION_REL | sed -e "s/.$VERSION_REQ\$//"`
  else # or has been two-part version - try using host_cpu if available
  VERSION_REL="00" ; test "_$host_cpu" != "_" && VERSION_REL="$host_cpu"
  fi
  RELEASEINFO="-release $VERSION_REL"
  VERSIONINFO="-version-info $VERSION_REQ:$VERSION_REV"
AC_MSG_RESULT([$RELEASEINFO $VERSIONINFO])
AC_SUBST([RELEASEINFO])
AC_SUBST([VERSIONINFO])
])





dnl ______ /usr/share/aclocal/guidod/ax_prefix_config_h.m4 ______
dnl @synopsis AX_PREFIX_CONFIG_H [(OUTPUT-HEADER [,PREFIX [,ORIG-HEADER]])]
dnl
dnl guidod@gmx.de

AC_DEFUN([AX_PREFIX_CONFIG_H],[AC_REQUIRE([AC_CONFIG_HEADER])
AC_CONFIG_COMMANDS([ifelse($1,,$PACKAGE-config.h,$1)],[dnl

ac_prefix_conf_OUT=`echo ifelse($1, , $PACKAGE-config.h, $1)`
ac_prefix_conf_DEF=`echo _$ac_prefix_conf_OUT | sed -e 'y:abcdefghijklmnopqrstuvwxyz./,-:ABCDEFGHIJKLMNOPQRSTUVWXYZ____:'`
ac_prefix_conf_PKG=`echo ifelse($2, , $PACKAGE, $2)`
ac_prefix_conf_LOW=`echo _$ac_prefix_conf_PKG | sed -e 'y:ABCDEFGHIJKLMNOPQRSTUVWXYZ-:abcdefghijklmnopqrstuvwxyz_:'`
ac_prefix_conf_UPP=`echo $ac_prefix_conf_PKG | sed -e 'y:abcdefghijklmnopqrstuvwxyz-:ABCDEFGHIJKLMNOPQRSTUVWXYZ_:'  -e '/^[0-9]/s/^/_/'`
ac_prefix_conf_INP=`echo ifelse($3, , _, $3)`
if test "$ac_prefix_conf_INP" = "_"; then
   for ac_file in : $CONFIG_HEADERS; do test "_$ac_file" = _: && continue
     test -f "$ac_prefix_conf_INP" && continue
     case $ac_file in
        *.h) test -f $ac_file && ac_prefix_conf_INP=$ac_file ;;
        *)
     esac
   done
fi
if test "$ac_prefix_conf_INP" = "_"; then
   case $ac_prefix_conf_OUT in
      */*) ac_prefix_conf_INP=`basename $ac_prefix_conf_OUT` 
      ;;
      *-*) ac_prefix_conf_INP=`echo $ac_prefix_conf_OUT | sed -e 's/[a-zA-Z0-9_]*-//'`
      ;;
      *) ac_prefix_conf_INP=config.h
      ;;
   esac
fi
if test -z "$ac_prefix_conf_PKG" ; then
   AC_MSG_ERROR([no prefix for _PREFIX_PKG_CONFIG_H])
else
  AC_MSG_NOTICE(creating $ac_prefix_conf_OUT - prefix $ac_prefix_conf_UPP for $ac_prefix_conf_INP defines)
  if test -f $ac_prefix_conf_INP ; then
    echo '#ifndef '$ac_prefix_conf_DEF      >$tmp/pconfig.h
    echo '#define '$ac_prefix_conf_DEF' 1' >>$tmp/pconfig.h
    echo ' ' >>$tmp/pconfig.h
    echo /'*' $ac_prefix_conf_OUT. Generated automatically at end of configure. '*'/ >>$tmp/pconfig.h

    echo 's/#undef  *\([A-Z_]\)/#undef '$ac_prefix_conf_UPP'_\1/' >conftest.prefix
    echo 's/#undef  *\([a-z]\)/#undef '$ac_prefix_conf_LOW'_\1/' >>conftest.prefix
    echo 's/#define  *\([A-Z_][A-Za-z0-9_]*\)\(.*\)/#ifndef '$ac_prefix_conf_UPP"_\\1 \\" >>conftest.prefix
    echo '#define '$ac_prefix_conf_UPP"_\\1 \\2 \\" >>conftest.prefix
    echo '#endif/' >>conftest.prefix
    echo 's/#define  *\([a-z][A-Za-z0-9_]*\)\(.*\)/#ifndef '$ac_prefix_conf_LOW"_\\1 \\" >>conftest.prefix
    echo '#define '$ac_prefix_conf_LOW"_\\1 \\2 \\" >>conftest.prefix
    echo '#endif/' >>conftest.prefix
    sed -f conftest.prefix $ac_prefix_conf_INP >>$tmp/pconfig.h
    echo ' ' >>$tmp/pconfig.h
    echo '/* once:' $ac_prefix_conf_DEF '*/' >>$tmp/pconfig.h
    echo '#endif' >>$tmp/pconfig.h
    if cmp -s $ac_prefix_conf_OUT $tmp/pconfig.h 2>/dev/null; then
      AC_MSG_NOTICE([$ac_prefix_conf_OUT is unchanged])
    else
      ac_dir=`AS_DIRNAME(["$ac_prefix_conf_OUT"])`
      AS_MKDIR_P(["$ac_dir"])
      rm -f $ac_prefix_conf_OUT
      mv $tmp/pconfig.h $ac_prefix_conf_OUT
    fi
  else
    AC_MSG_ERROR([input file $ac_prefix_conf_IN does not exist - dnl
    skip generating $ac_prefix_conf_OUT])
  fi
  rm -f conftest.* 
fi
])])

dnl ______ /usr/share/aclocal/Miscellaneous/ac_compile_check_sizeof.m4 ______
dnl @synopsis AC_COMPILE_CHECK_SIZEOF(TYPE [, HEADERS [, EXTRA_SIZES...]])
dnl
dnl This macro checks for the size of TYPE using compile checks, not
dnl run checks. You can supply extra HEADERS to look into. the check
dnl will cycle through 1 2 4 8 16 and any EXTRA_SIZES the user
dnl supplies. If a match is found, it will #define SIZEOF_`TYPE' to
dnl that value. Otherwise it will emit a configure time error
dnl indicating the size of the type could not be determined.
dnl
dnl The trick is that C will not allow duplicate case labels. While
dnl this is valid C code:
dnl
dnl      switch (0) case 0: case 1:;
dnl
dnl The following is not:
dnl
dnl      switch (0) case 0: case 0:;
dnl
dnl Thus, the AC_TRY_COMPILE will fail if the currently tried size
dnl does not match.
dnl
dnl Here is an example skeleton configure.in script, demonstrating the
dnl macro's usage:
dnl
dnl      AC_PROG_CC
dnl      AC_CHECK_HEADERS(stddef.h unistd.h)
dnl      AC_TYPE_SIZE_T
dnl      AC_CHECK_TYPE(ssize_t, int)
dnl
dnl      headers='#ifdef HAVE_STDDEF_H
dnl      #include <stddef.h>
dnl      #endif
dnl      #ifdef HAVE_UNISTD_H
dnl      #include <unistd.h>
dnl      #endif
dnl      '
dnl
dnl      AC_COMPILE_CHECK_SIZEOF(char)
dnl      AC_COMPILE_CHECK_SIZEOF(short)
dnl      AC_COMPILE_CHECK_SIZEOF(int)
dnl      AC_COMPILE_CHECK_SIZEOF(long)
dnl      AC_COMPILE_CHECK_SIZEOF(unsigned char *)
dnl      AC_COMPILE_CHECK_SIZEOF(void *)
dnl      AC_COMPILE_CHECK_SIZEOF(size_t, $headers)
dnl      AC_COMPILE_CHECK_SIZEOF(ssize_t, $headers)
dnl      AC_COMPILE_CHECK_SIZEOF(ptrdiff_t, $headers)
dnl      AC_COMPILE_CHECK_SIZEOF(off_t, $headers)
dnl
dnl @author Kaveh Ghazi <ghazi@caip.rutgers.edu>
dnl @version %Id: ac_compile_check_sizeof.m4,v 1.1.1.1 2001/07/26 00:46:24 guidod Exp %
dnl
AC_DEFUN([AC_COMPILE_CHECK_SIZEOF],
[changequote(<<, >>)dnl
dnl The name to #define.
define(<<AC_TYPE_NAME>>, translit(sizeof_$1, [a-z *], [A-Z_P]))dnl
dnl The cache variable name.
define(<<AC_CV_NAME>>, translit(ac_cv_sizeof_$1, [ *], [_p]))dnl
changequote([, ])dnl
AC_MSG_CHECKING(size of $1)
AC_CACHE_VAL(AC_CV_NAME,
[for ac_size in 4 8 1 2 16 $2 ; do # List sizes in rough order of prevalence.
  AC_TRY_COMPILE([#include "confdefs.h"
#include <sys/types.h>
$2
], [switch (0) case 0: case (sizeof ($1) == $ac_size):;], AC_CV_NAME=$ac_size)
  if test x$AC_CV_NAME != x ; then break; fi
done
])
if test x$AC_CV_NAME = x ; then
  AC_MSG_ERROR([cannot determine a size for $1])
fi
AC_MSG_RESULT($AC_CV_NAME)
AC_DEFINE_UNQUOTED(AC_TYPE_NAME, $AC_CV_NAME, [The number of bytes in type $1])
undefine([AC_TYPE_NAME])dnl
undefine([AC_CV_NAME])dnl
])

dnl ______ /usr/share/aclocal/guidod/ax_cflags_warn_all.m4 ______
dnl @synopsis AX_CFLAGS_WARN_ALL [(shellvar)]
dnl
dnl Try to find a compiler option that enables most reasonable warnings.
dnl This macro is directly derived from VL_PROG_CC_WARNINGS which is
dnl split up into two AX_CFLAGS_WARN_ALL and AX_CFLAGS_WARN_ALL_ANSI
dnl
dnl For the GNU CC compiler it will be -Wall (and -ansi -pedantic)
dnl The result is added to the shellvar being CFLAGS by default.
dnl
dnl Currently this macro knows about GCC, Solaris C compiler,
dnl Digital Unix C compiler, C for AIX Compiler, HP-UX C compiler,
dnl IRIX C compiler, NEC SX-5 (Super-UX 10) C compiler, and Cray J90
dnl (Unicos 10.0.0.8) C compiler.
dnl
dnl @version %Id: guidod Exp %
dnl @author Guido Draheim <guidod@gmx.de>
dnl
AC_DEFUN([AX_CFLAGS_WARN_ALL], 
[AC_MSG_CHECKING(m4_ifval($1,$1,CFLAGS) for maximum warnings)
AC_CACHE_VAL(ac_cv_cflags_warn_all,
[ac_cv_cflags_warn_all="no, unknown"
 AC_LANG_SAVE
 AC_LANG_C
 ac_save_CFLAGS="$CFLAGS"
# IRIX C compiler:
#      -use_readonly_const is the default for IRIX C,
#       puts them into .rodata, but they are copied later.
#       need to be "-G0 -rdatashared" for strictmode but
#       I am not sure what effect that has really.         - guidod
for ac_arg dnl
in "-pedantic  % -Wall"       dnl   GCC
   "-xstrconst % -v"          dnl Solaris C 
   "-std1      % -verbose -w0 -warnprotos" dnl Digital Unix 
   "-qlanglvl=ansi % -qsrcmsg -qinfo=all:noppt:noppc:noobs:nocnd" dnl AIX
   "-ansi -ansiE % -fullwarn" dnl IRIX
   "+ESlit     % +w1"         dnl HP-UX C 
   "-Xc        % -pvctl[,]fullmsg" dnl NEC SX-5 (Super-UX 10)
   "-h conform % -h msglevel 2" dnl Cray C (Unicos)
   # 
do CFLAGS="$ac_save_CFLAGS "`echo $ac_arg | sed -e 's,%%.*,,' -e 's,%,,'`
   AC_TRY_COMPILE([],[return 0;],
   [ac_cv_cflags_warn_all=`echo $ac_arg | sed -e 's,.*% *,,'`
   break])
done
 CFLAGS="$ac_save_CFLAGS"
 AC_LANG_RESTORE
])
case ".$ac_cv_cflags_warn_all" in
   .|.no,*) AC_MSG_RESULT($ac_cv_cflags_warn_all) ;;
   *) m4_ifval($1,$1,CFLAGS)="$m4_ifval($1,$1,CFLAGS) dnl
$ac_cv_cflags_warn_all"
      AC_MSG_RESULT($ac_cv_cflags_warn_all) ;;
esac])

dnl the only difference - the LANG selection... and the default FLAGS

AC_DEFUN([AX_CXXFLAGS_WARN_ALL], 
[AC_MSG_CHECKING(m4_ifval($1,$1,CXXFLAGS) for maximum warnings)
AC_CACHE_VAL(ac_cv_cxxflags_warn_all,
[ac_cv_cxxflags_warn_all="no, unknown"
 AC_LANG_SAVE
 AC_LANG_CXX
 ac_save_CXXFLAGS="$CXXFLAGS"
# IRIX C compiler:
#      -use_readonly_const is the default for IRIX C,
#       puts them into .rodata, but they are copied later.
#       need to be "-G0 -rdatashared" for strictmode but
#       I am not sure what effect that has really.         - guidod
for ac_arg dnl
in "-pedantic  % -Wall"       dnl   GCC
   "-xstrconst % -v"          dnl Solaris C 
   "-std1      % -verbose -w0 -warnprotos" dnl Digital Unix 
   "-qlanglvl=ansi % -qsrcmsg -qinfo=all:noppt:noppc:noobs:nocnd" dnl AIX
   "-ansi -ansiE % -fullwarn" dnl IRIX
   "+ESlit     % +w1"         dnl HP-UX C 
   "-Xc        % -pvctl[,]fullmsg" dnl NEC SX-5 (Super-UX 10)
   "-h conform % -h msglevel 2" dnl Cray C (Unicos)
   # 
do CXXFLAGS="$ac_save_CXXFLAGS "`echo $ac_arg | sed -e 's,%%.*,,' -e 's,%,,'`
   AC_TRY_COMPILE([],[return 0;],
   [ac_cv_cxxflags_warn_all=`echo $ac_arg | sed -e 's,.*% *,,'`
   break])
done
 CXXFLAGS="$ac_save_CXXFLAGS"
 AC_LANG_RESTORE
])
case ".$ac_cv_cxxflags_warn_all" in
   .|.no,*) AC_MSG_RESULT($ac_cv_cxxflags_warn_all) ;;
   *) m4_ifval($1,$1,CXXFLAGS)="$m4_ifval($1,$1,CXXFLAGS) dnl
$ac_cv_cxxflags_warn_all"
      AC_MSG_RESULT($ac_cv_cxxflags_warn_all) ;;
esac])


dnl ______ /usr/share/aclocal/guidod/ax_cflags_no_writable_strings.m4 ______
dnl @synopsis AX_CFLAGS_NO_WRITABLE_STRINGS [(shellvar)]
dnl
dnl Try to find a compiler option that makes all stringliteral readonly.
dnl
dnl The sanity check is done by looking at string.h which has a set
dnl of strcpy definitions that should be defined with const-modifiers
dnl to not emit a warning in all so many places.
dnl
dnl For the GNU CC compiler it will be -fno-writable-strings -Wwrite-strings
dnl The result is added to the shellvar being CFLAGS by default.
dnl
dnl @version %Id: guidod Exp %
dnl @author Guido Draheim <guidod@gmx.de>
dnl
AC_DEFUN([AX_CFLAGS_NO_WRITABLE_STRINGS], 
[AC_MSG_CHECKING(m4_ifval($1,$1,CFLAGS) making strings readonly)
AC_CACHE_VAL(ac_cv_cflags_no_writable_strings,
[ac_cv_cflags_no_writable_strings="no, unknown"
 AC_LANG_SAVE
 AC_LANG_C
 ac_save_CFLAGS="$CFLAGS"
# IRIX C compiler:
#      -use_readonly_const is the default for IRIX C,
#       puts them into .rodata, but they are copied later.
#       need to be "-G0 -rdatashared" for strictmode but
#       I am not sure what effect that has really.         - guidod
for ac_arg dnl
in "-Wall     % -fno-writable-strings -Wwrite-strings" dnl   GCC
   "-v -Xc    % -xstrconst" dnl Solaris C - strings go into readonly segment
   "+w1 -Aa   % +ESlit"      dnl HP-UX C - strings go into readonly segment
   "-w0 -std1 % -readonly_strings" dnl Digital Unix - again readonly segment
   "-fullwarn -use_readonly_const %% no, not needed" dnl IRIX C its the default
   # 
do CFLAGS="$ac_save_CFLAGS "`echo $ac_arg | sed -e 's,%%.*,,' -e 's,%,,'`
   AC_TRY_COMPILE([],[return 0;],
   [ac_cv_cflags_no_writable_strings=`echo $ac_arg | sed -e 's,.*% *,,'`
   break])
done
case "$ac_cv_cflags_no_writable_strings" in
   no,*) ;;
   *)
AC_TRY_COMPILE(
[#include <string.h>],[
    char test[16];
    if (strcpy (test, "test")) return 1;],[],
[ac_cv_cflags_no_writable_strings="no, suppressed, string.h"
 cp conftest.c testtest.c
]) ;;
esac
 CFLAGS="$ac_save_CFLAGS"
 AC_LANG_RESTORE
])
case ".$ac_cv_cflags_no_writable_strings" in
   .|.no,*) AC_MSG_RESULT($ac_cv_cflags_no_writable_strings) ;;
   *) m4_ifval($1,$1,CFLAGS)="$m4_ifval($1,$1,CFLAGS) dnl
$ac_cv_cflags_no_writable_strings"
      AC_MSG_RESULT($ac_cv_cflags_no_writable_strings) ;;
esac])

dnl the only difference - the LANG selection... and the default FLAGS

AC_DEFUN([AX_CXXFLAGS_NO_WRITABLE_STRINGS], 
[AC_MSG_CHECKING(m4_ifval($1,$1,CXXFLAGS) making strings readonly)
AC_CACHE_VAL(ac_cv_cxxflags_no_writable_strings,
[ac_cv_cxxflags_no_writable_strings="no, unknown"
 AC_LANG_SAVE
 AC_LANG_CXX
 ac_save_CXXFLAGS="$CXXFLAGS"
# IRIX C compiler:
#      -use_readonly_const is the default for IRIX C,
#       puts them into .rodata, but they are copied later.
#       need to be "-G0 -rdatashared" for strictmode but
#       I am not sure what effect that has really.         - guidod
for ac_arg dnl
in "-Wall     % -fno-writable-strings -Wwrite-strings" dnl   GCC
   "-v -Xc    % -xstrconst" dnl Solaris C - strings go into readonly segment
   "+w1 -Aa   % +ESlit"      dnl HP-UX C - strings go into readonly segment
   "-w0 -std1 % -readonly_strings" dnl Digital Unix - again readonly segment
   "-fullwarn -use_readonly_const %% no, not needed" dnl IRIX C its the default
   # 
do CXXFLAGS="$ac_save_CXXFLAGS "`echo $ac_arg | sed -e 's,%%.*,,' -e 's,%,,'`
   AC_TRY_COMPILE([],[return 0;],
   [ac_cv_cxxflags_no_writable_strings=`echo $ac_arg | sed -e 's,.*% *,,'`
   break])
done
case "$ac_cv_cxxflags_no_writable_strings" in
   no,*) ;;
   *)
AC_TRY_COMPILE(
[#include <string.h>],[
    char test[16];
    if (strcpy (test, "test")) return 1;],[],
[ac_cv_cxxflags_no_writable_strings="no, suppressed, string.h"
 cp conftest.c testtest.c
]) ;;
esac
 CXXFLAGS="$ac_save_CXXFLAGS"
 AC_LANG_RESTORE
])
case ".$ac_cv_cxxflags_no_writable_strings" in
   .|.no,*) AC_MSG_RESULT($ac_cv_cxxflags_no_writable_strings) ;;
   *) m4_ifval($1,$1,CXXFLAGS)="$m4_ifval($1,$1,CXXFLAGS) dnl
$ac_cv_cxxflags_no_writable_strings"
      AC_MSG_RESULT($ac_cv_cxxflags_no_writable_strings) ;;
esac])

dnl ______ /usr/share/aclocal/guidod/ax_cflags_strict_prototypes.m4 ______
dnl @synopsis AX_CFLAGS_STRICT_PROTOTYPES [(shellvar)]
dnl
dnl Try to find a compiler option that requires strict prototypes.
dnl
dnl The sanity check is done by looking at sys/signal.h which has a set
dnl of macro-definitions SIG_DFL and SIG_IGN that are cast to the local
dnl signal-handler type. If that signal-handler type is not fully
dnl qualified then the system headers are not seen as strictly prototype clean.
dnl
dnl For the GNU CC compiler it will be -fstrict-prototypes -Wstrict-prototypes
dnl The result is added to the shellvar being CFLAGS by default.
dnl
dnl @version %Id: guidod Exp %
dnl @author Guido Draheim <guidod@gmx.de>
dnl
AC_DEFUN([AX_CFLAGS_STRICT_PROTOTYPES], 
[AC_MSG_CHECKING(m4_ifval($1,$1,CFLAGS) for strict prototypes)
AC_CACHE_VAL(ac_cv_cflags_strict_prototypes,
[ac_cv_cflags_strict_prototypes="no, unknown"
 AC_LANG_SAVE
 AC_LANG_C
 ac_save_CFLAGS="$CFLAGS"
for ac_arg dnl
in "-Wall     % -fstrict-prototypes -Wstrict-prototypes" dnl   GCC
   "-Wall     % -Wstrict-prototypes" dnl try to warn atleast
   # 
do CFLAGS="$ac_save_CFLAGS "`echo $ac_arg | sed -e 's,%%.*,,' -e 's,%,,'`
   AC_TRY_COMPILE([],[return 0;],
   [ac_cv_cflags_strict_prototypes=`echo $ac_arg | sed -e 's,.*% *,,'`
   break])
done
case "$ac_cv_cflags_strict_prototypes" in
   no,*) ;;
   *)
AC_TRY_COMPILE(
[#include <sys/signal.h>],[
    if (signal (SIGINT, SIG_IGN) == SIG_DFL) return 1;],[],
[ac_cv_cflags_strict_prototypes="no, suppressed, sys/signal.h"
 cp conftest.c testtest.c
]) ;;
esac
 CFLAGS="$ac_save_CFLAGS"
 AC_LANG_RESTORE
])
case ".$ac_cv_cflags_strict_prototypes" in
   .|.no,*) AC_MSG_RESULT($ac_cv_cflags_strict_prototypes) ;;
   *) m4_ifval($1,$1,CFLAGS)="$m4_ifval($1,$1,CFLAGS) dnl
$ac_cv_cflags_strict_prototypes"
      AC_MSG_RESULT($ac_cv_cflags_strict_prototypes) ;;
esac])

dnl the only difference - the LANG selection... and the default FLAGS

AC_DEFUN([AX_CXXFLAGS_STRICT_PROTOTYPES], 
[AC_MSG_CHECKING(m4_ifval($1,$1,CXXFLAGS) for strict prototypes)
AC_CACHE_VAL(ac_cv_cxxflags_strict_prototypes,
[ac_cv_cxxflags_strict_prototypes="no, unknown"
 AC_LANG_SAVE
 AC_LANG_CXX
 ac_save_CXXFLAGS="$CXXFLAGS"
for ac_arg dnl
in "-Wall     % -fstrict-prototypes -Wstrict-prototypes" dnl   GCC
   "-Wall     % -Wstrict-prototypes" dnl try to warn atleast
   # 
do CXXFLAGS="$ac_save_CXXFLAGS "`echo $ac_arg | sed -e 's,%%.*,,' -e 's,%,,'`
   AC_TRY_COMPILE([],[return 0;],
   [ac_cv_cxxflags_strict_prototypes=`echo $ac_arg | sed -e 's,.*% *,,'`
   break])
done
case "$ac_cv_cxxflags_strict_prototypes" in
   no,*) ;;
   *)
AC_TRY_COMPILE(
[#include <sys/signal.h>],[
    if (signal (SIGINT, SIG_IGN) == SIG_DFL) return 1;],[],
[ac_cv_cxxflags_strict_prototypes="no, suppressed, sys/signal.h"
 cp conftest.c testtest.c
]) ;;
esac
 CXXFLAGS="$ac_save_CXXFLAGS"
 AC_LANG_RESTORE
])
case ".$ac_cv_cxxflags_strict_prototypes" in
   .|.no,*) AC_MSG_RESULT($ac_cv_cxxflags_strict_prototypes) ;;
   *) m4_ifval($1,$1,CXXFLAGS)="$m4_ifval($1,$1,CXXFLAGS) dnl
$ac_cv_cxxflags_strict_prototypes"
      AC_MSG_RESULT($ac_cv_cxxflags_strict_prototypes) ;;
esac])

dnl ______ /usr/share/aclocal/guidod/patch_libtool_on_darwin_zsh_overquoting.m4 ______
AC_DEFUN([PATCH_LIBTOOL_ON_DARWIN_ZSH_OVERQUOTING],
[# libtool-1.4 specific, on zsh target the final requoting does one too much
case "$host_os" in
  darwin*)
    if grep "1.92" libtool ; then
AC_MSG_RESULT(patching libtool on .so-sharedlib creation (zsh overquoting))
      test -f libtool.old || (mv libtool libtool.old && cp libtool.old libtool)
      sed -e '/archive_cmds=/s:[[\\]][[\\]][[\\]]*":\\":g' libtool >libtool.new
      mv libtool.new libtool
      test -f libtool || (test -f libtool.old && mv libtool.old libtool)
    fi
  ;;
esac
])
dnl ______ /usr/share/aclocal/guidod/patch_libtool_sys_lib_search_path_spec.m4 ______
AC_DEFUN([PATCH_LIBTOOL_SYS_LIB_SEARCH_PATH_SPEC],
[# patch libtool to fix sys_lib_search_path (e.g. crosscompiling a win32 dll)
if test "_$PATH_SEPARATOR" = "_:" ; then
  if grep "^sys_lib_search_path_spec.*:" libtool >/dev/null ; then
AC_MSG_RESULT(patching libtool to fix sys_lib_search_path_spec)
    test -f libtool.old || (mv libtool libtool.old && cp libtool.old libtool)
    sed -e "/^sys_lib_search_path_spec/s/:/ /g" libtool >libtool.new
    mv libtool.new libtool
    test -f libtool || (test -f libtool.old && mv libtool.old libtool)
  fi
fi
])
dnl ______ /usr/share/aclocal/guidod/patch_libtool_to_add_host_cc.m4 ______
AC_DEFUN([PATCH_LIBTOOL_TO_ADD_HOST_CC],
[# patch libtool to add HOST_CC sometimes needed in crosscompiling a win32 dll
if grep "HOST_CC" libtool >/dev/null; then
  if test "$build" != "$host" ; then
    if test "_$HOST_CC" = "_" ; then
      HOST_CC="false"
      for i in `echo $PATH | sed 's,:, ,g'` ; do
      test -x $i/cc && HOST_CC=$i/cc
      done
    fi
AC_MSG_RESULT(patching libtool to add HOST_CC=$HOST_CC)
    test -f libtool.old || (mv libtool libtool.old && cp libtool.old libtool)
    sed -e "/BEGIN.*LIBTOOL.*CONFIG/a\\
HOST_CC=$HOST_CC" libtool >libtool.new
    mv libtool.new libtool
    test -f libtool || (test -f libtool.old && mv libtool.old libtool)
  fi
fi
])
