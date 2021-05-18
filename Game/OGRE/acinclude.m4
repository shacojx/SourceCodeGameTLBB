# Configure paths for FreeType2
# Marcelo Magallon 2001-10-26, based on gtk.m4 by Owen Taylor

dnl AC_CHECK_FT2([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for FreeType2, and define FT2_CFLAGS and FT2_LIBS
dnl
AC_DEFUN([AC_CHECK_FT2],
[dnl
dnl Get the cflags and libraries from the freetype-config script
dnl
AC_ARG_WITH(ft-prefix,
[  --with-ft-prefix=PREFIX
                          Prefix where FreeType is installed (optional)],
            ft_config_prefix="$withval", ft_config_prefix="")
AC_ARG_WITH(ft-exec-prefix,
[  --with-ft-exec-prefix=PREFIX
                          Exec prefix where FreeType is installed (optional)],
            ft_config_exec_prefix="$withval", ft_config_exec_prefix="")
AC_ARG_ENABLE(freetypetest,
[  --disable-freetypetest  Do not try to compile and run
                          a test FreeType program],
              [], enable_fttest=yes)

if test x$ft_config_exec_prefix != x ; then
  ft_config_args="$ft_config_args --exec-prefix=$ft_config_exec_prefix"
  if test x${FT2_CONFIG+set} != xset ; then
    FT2_CONFIG=$ft_config_exec_prefix/bin/freetype-config
  fi
fi
if test x$ft_config_prefix != x ; then
  ft_config_args="$ft_config_args --prefix=$ft_config_prefix"
  if test x${FT2_CONFIG+set} != xset ; then
    FT2_CONFIG=$ft_config_prefix/bin/freetype-config
  fi
fi
AC_PATH_PROG(FT2_CONFIG, freetype-config, no)

min_ft_version=ifelse([$1], ,9.1.0,$1)
AC_MSG_CHECKING(for FreeType - version >= $min_ft_version)
no_ft=""
if test "$FT2_CONFIG" = "no" ; then
  no_ft=yes
else
  FT2_CFLAGS=`$FT2_CONFIG $ft_config_args --cflags`
  FT2_LIBS=`$FT2_CONFIG $ft_config_args --libs`
  ft_config_major_version=`$FT2_CONFIG $ft_config_args --version | \
         sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
  ft_config_minor_version=`$FT2_CONFIG $ft_config_args --version | \
         sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
  ft_config_micro_version=`$FT2_CONFIG $ft_config_args --version | \
         sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
  ft_min_major_version=`echo $min_ft_version | \
         sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
  ft_min_minor_version=`echo $min_ft_version | \
         sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
  ft_min_micro_version=`echo $min_ft_version | \
         sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
  if test x$enable_fttest = xyes ; then
    ft_config_is_lt=""
    if test $ft_config_major_version -lt $ft_min_major_version ; then
      ft_config_is_lt=yes
    else
      if test $ft_config_major_version -eq $ft_min_major_version ; then
        if test $ft_config_minor_version -lt $ft_min_minor_version ; then
          ft_config_is_lt=yes
        else
          if test $ft_config_minor_version -eq $ft_min_minor_version ; then
            if test $ft_config_micro_version -lt $ft_min_micro_version ; then
              ft_config_is_lt=yes
            fi
          fi
        fi
      fi
    fi
    if test x$ft_config_is_lt = xyes ; then
      no_ft=yes
    else
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $FT2_CFLAGS"
      LIBS="$FT2_LIBS $LIBS"
dnl
dnl Sanity checks for the results of freetype-config to some extent
dnl
      AC_TRY_RUN([
#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdio.h>
#include <stdlib.h>

int
main()
{
  FT_Library library;
  FT_Error error;

  error = FT_Init_FreeType(&library);

  if (error)
    return 1;
  else
  {
    FT_Done_FreeType(library);
    return 0;
  }
}
],, no_ft=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
      CFLAGS="$ac_save_CFLAGS"
      LIBS="$ac_save_LIBS"
    fi             # test $ft_config_version -lt $ft_min_version
  fi               # test x$enable_fttest = xyes
fi                 # test "$FT2_CONFIG" = "no"
if test x$no_ft = x ; then
   AC_MSG_RESULT(yes)
   ifelse([$2], , :, [$2])
else
   AC_MSG_RESULT(no)
   if test "$FT2_CONFIG" = "no" ; then
     echo "*** The freetype-config script installed by FreeType 2 could not be found."
     echo "*** If FreeType 2 was installed in PREFIX, make sure PREFIX/bin is in"
     echo "*** your path, or set the FT2_CONFIG environment variable to the"
     echo "*** full path to freetype-config."
   else
     if test x$ft_config_is_lt = xyes ; then
       echo "*** Your installed version of the FreeType 2 library is too old."
       echo "*** If you have different versions of FreeType 2, make sure that"
       echo "*** correct values for --with-ft-prefix or --with-ft-exec-prefix"
       echo "*** are used, or set the FT2_CONFIG environment variable to the"
       echo "*** full path to freetype-config."
     else
       echo "*** The FreeType test program failed to run.  If your system uses"
       echo "*** shared libraries and they are installed outside the normal"
       echo "*** system library path, make sure the variable LD_LIBRARY_PATH"
       echo "*** (or whatever is appropiate for your system) is correctly set."
     fi
   fi
   FT2_CFLAGS=""
   FT2_LIBS=""
   ifelse([$3], , :, [$3])
fi
AC_SUBST(FT2_CFLAGS)
AC_SUBST(FT2_LIBS)
])

AC_DEFUN([OGRE_USE_STLPORT],
[AC_ARG_WITH(stlport, 
             AC_HELP_STRING([--with-stlport=PATH],
                           [the path to STLPort.]),
             ac_cv_use_stlport=$withval,
             ac_cv_use_stlport=no)
 AC_CACHE_CHECK([whether to use STLPort], ac_cv_use_stlport,
                ac_cv_use_stlport=no)
 if test x$ac_cv_use_stlport != xno; then
     STLPORT_CFLAGS="-I$ac_cv_use_stlport/stlport"
     STLPORT_LIBS="-L$ac_cv_use_stlport/lib -lstlport"
 fi
 AC_SUBST(STLPORT_CFLAGS)
 AC_SUBST(STLPORT_LIBS)
])

AC_DEFUN([OGRE_GET_CONFIG_TOOLKIT],
[OGRE_CFGTK=cli
 AC_ARG_WITH(cfgtk, 
             AC_HELP_STRING([--with-cfgtk=TOOLKIT],
                            [the toolkit for the config gui, currently cli or gtk]),
             OGRE_CFGTK=$withval,
             OGRE_CFGTK=cli)

 
  CFGTK_DEPS_CFLAGS=""
  CFGTK_DEPS_LIBS=""

  dnl Do the extra checks per type here
  case $OGRE_CFGTK in 
    gtk)
      PKG_CHECK_MODULES(CFGTK_DEPS, gtkmm-2.4 libglademm-2.4);;
  esac

  AC_SUBST(CFGTK_DEPS_CFLAGS)
  AC_SUBST(CFGTK_DEPS_LIBS)
  AC_SUBST(OGRE_CFGTK)
])

AC_DEFUN([OGRE_GET_PLATFORM],
[OGRE_PLATFORM=GLX
 AC_ARG_WITH(platform, 
             AC_HELP_STRING([--with-platform=PLATFORM],
                            [the platform to build, currently SDL, GLX, Win32 or gtk]),
             OGRE_PLATFORM=$withval,
             OGRE_PLATFORM=GLX)

 
  if test ! -d ${srcdir}/PlatformManagers/$OGRE_PLATFORM; then
    OGRE_PLATFORM=SDL
  fi

  PLATFORM_CFLAGS=""
  PLATFORM_LIBS=""

  dnl Do the extra checks per type here
  case $OGRE_PLATFORM in 
    SDL)
      AM_PATH_SDL(1.2.6,,[AC_MSG_ERROR("--with-platform: SDL > 1.2.6 not found")])
      PLATFORM_CFLAGS=$SDL_CFLAGS
      PLATFORM_LIBS=$SDL_LIBS
      ;;
    gtk)
      PKG_CHECK_MODULES(PLATFORM, gtkglextmm-1.0 libglademm-2.4);;
    GLX)
      AC_CHECK_HEADERS([X11/Intrinsic.h],, [AC_MSG_ERROR("libxt headers not found")])
      AC_CHECK_HEADERS([X11/Xaw/Command.h],, [AC_MSG_ERROR("libxaw headers not found")])
      AC_CHECK_HEADERS([X11/extensions/xf86vmode.h],, [AC_MSG_ERROR("libxf86vm headers not found")],[#include <X11/Xlib.h>])
      AC_CHECK_HEADERS([X11/extensions/Xrandr.h],, [AC_MSG_ERROR("libxrandr headers not found")],[#include <X11/Xlib.h>])
      PLATFORM_CFLAGS="-I/usr/X11R6/include"
      PLATFORM_LIBS="-L/usr/X11R6/lib -lX11 -lXaw"
    ;;
    Win32)
      PLATFORM_CFLAGS=""
      PLATFORM_LIBS="-lgdi32 -lwinmm -ldinput8 -ldxguid"
    ;;
  esac

  AC_SUBST(PLATFORM_CFLAGS)
  AC_SUBST(PLATFORM_LIBS)
  AC_SUBST(OGRE_PLATFORM)
])

AC_DEFUN([OGRE_GET_GLSUPPORT],
[OGRE_GLSUPPORT=none
 AC_ARG_WITH(gl-support, 
             AC_HELP_STRING([--with-gl-support=PLATFORM],
                            [ The GLsupport to build (SDL, GLX, Win32 or gtk). Defaults to the platform. Only set this if you know what you are doing. Use --with-platform otherwise.]),
             OGRE_GLSUPPORT=$withval,
             OGRE_GLSUPPORT=none)

  if test "$OGRE_GLSUPPORT" = "none" ; then
    OGRE_GLSUPPORT="$OGRE_PLATFORM"
    AC_MSG_NOTICE([setting gl-support to platform: $OGRE_GLSUPPORT])
  fi
  if test "$OGRE_GLSUPPORT" = "Win32" ; then
    # Uppercase/lowercase
    OGRE_GLSUPPORT=win32
  fi
  if test ! -d ${srcdir}/RenderSystems/GL/src/$OGRE_GLSUPPORT; then
    OGRE_GLSUPPORT=SDL
  fi

  GLSUPPORT_CFLAGS=""
  GLSUPPORT_LIBS=""

  dnl Do the extra checks per type here
  case $OGRE_GLSUPPORT in 
    SDL) AM_PATH_SDL(1.2.6,,[AC_MSG_ERROR("--with-gl-support: SDL > 1.2.6 not found")])
      GLSUPPORT_CFLAGS=$SDL_CFLAGS
      GLSUPPORT_LIBS=$SDL_LIBS;;
    gtk) 
    	PKG_CHECK_MODULES(GLSUPPORT, gtkglextmm-1.0)
    	GLSUPPORT_LIBS="$GLSUPPORT_LIBS"
    ;;
    GLX)
	GLSUPPORT_CFLAGS="-I/usr/X11R6/include"
	GLSUPPORT_LIBS="-L/usr/X11R6/lib -lX11 -lXext -lGL -lXrandr"
    ;;
    win32)
	GLSUPPORT_CFLAGS=""
	GLSUPPORT_LIBS="-lgdi32 -lwinmm"
    ;;
  esac

  AC_SUBST(GLSUPPORT_CFLAGS)
  AC_SUBST(GLSUPPORT_LIBS)
  AC_SUBST(OGRE_GLSUPPORT)
  AC_CONFIG_FILES([RenderSystems/GL/src/gtk/Makefile
                   RenderSystems/GL/src/SDL/Makefile
		   RenderSystems/GL/src/GLX/Makefile
		   RenderSystems/GL/src/win32/Makefile])
])

AC_DEFUN([OGRE_SETUP_FOR_TARGET],
[case $host in
*-*-cygwin* | *-*-mingw* | *-*-pw32*)
	AC_SUBST(SHARED_FLAGS, "-shared -no-undefined -Xlinker --export-all-symbols")
	AC_SUBST(PLUGIN_FLAGS, "-shared -no-undefined -avoid-version")
	AC_SUBST(GL_LIBS, "-lopengl32 -lglu32")	
	AC_CHECK_TOOL(RC, windres)
        nt=true
;;
*-*-darwin*)
        AC_SUBST(SHARED_FLAGS, "-shared")
        AC_SUBST(PLUGIN_FLAGS, "-shared -avoid-version")
        AC_SUBST(GL_LIBS, "-lGL -lGLU")
        osx=true
;;
 *) dnl default to standard linux
	AC_SUBST(SHARED_FLAGS, "-shared")
	AC_SUBST(PLUGIN_FLAGS, "-shared -avoid-version")
	AC_SUBST(GL_LIBS, "-lGL -lGLU")
        linux=true
;;
esac
dnl you must arrange for every AM_conditional to run every time configure runs
AM_CONDITIONAL(OGRE_NT, test x$nt = xtrue)
AM_CONDITIONAL(OGRE_LINUX, test x$linux = xtrue)
AM_CONDITIONAL(OGRE_OSX,test x$osx = xtrue )
])


AC_DEFUN([OGRE_DETECT_ENDIAN],
[AC_TRY_RUN([
		int main()
		{
			short s = 1;
			short* ptr = &s;
			unsigned char c = *((char*)ptr);
			return c;
		}
	]
	,[AC_DEFINE(CONFIG_BIG_ENDIAN,,[Big endian machine])]
	,[AC_DEFINE(CONFIG_LITTLE_ENDIAN,,[Little endian machine])])
])

AC_DEFUN([OGRE_CHECK_OPENEXR],
[AC_ARG_ENABLE(openexr,
              AC_HELP_STRING([--enable-openexr],
                             [Build the OpenEXR plugin]),
              [build_exr=$enableval],
              [build_exr=no])

if test "x$build_exr" = "xyes" ; then
	PKG_CHECK_MODULES(OPENEXR, OpenEXR, [build_exr=yes], [build_exr=no])

	if test "x$build_exr" = "xyes" ; then
	   	AC_CONFIG_FILES([ PlugIns/EXRCodec/Makefile \
    					 PlugIns/EXRCodec/src/Makefile \
    					 PlugIns/EXRCodec/include/Makefile])
		AC_SUBST(OPENEXR_CFLAGS)
		AC_SUBST(OPENEXR_LIBS)

	fi

fi

AM_CONDITIONAL(BUILD_EXRPLUGIN, test x$build_exr = xyes)

])

AC_DEFUN([OGRE_CHECK_CG],
[AC_ARG_ENABLE(cg,
              AC_HELP_STRING([--disable-cg],
                             [Do not build the Cg plugin (recommended you do so!)]),
              [build_cg=$enableval],
              [build_cg=yes])

if test "x$build_cg" = "xyes" ; then
	AC_CHECK_LIB(Cg, cgCreateProgram,,AC_MSG_ERROR([
	****************************************************************
	* You do not have the nVidia Cg libraries installed.           *
	* Go to http://developer.nvidia.com/object/cg_toolkit.html     *
	* (Click on Cg_Linux.tar.gz).                                  *
	* You can disable the building of Cg support by providing      *	
	* --disable-cg to this configure script but this is highly     *
	* discouraged as this breaks many of the examples.             *
	****************************************************************])
	)
fi

AM_CONDITIONAL(BUILD_CGPLUGIN, test x$build_cg = xyes)

])

AC_DEFUN([OGRE_CHECK_CPPUNIT],
[
AM_PATH_CPPUNIT([1.10.0], [build_unit_tests=true])
AM_CONDITIONAL([BUILD_UNIT_TESTS], [test x$build_unit_tests = xtrue])
])


AC_DEFUN([OGRE_CHECK_DX9],
[AC_ARG_ENABLE(direct3d,
              AC_HELP_STRING([--enable-direct3d],
                             [Build the DirectX 9 Render System]),
              [build_dx9=$enableval],
              [build_dx9=no])

AM_CONDITIONAL(BUILD_DX9RENDERSYSTEM, test x$build_dx9 = xyes)

])

AC_DEFUN([OGRE_CHECK_DEVIL],
[AC_ARG_ENABLE(devil,
              AC_HELP_STRING([--disable-devil],
                             [Don't use DevIL for image loading. This is not recommended unless you provide your own image loading codecs.]),
              [build_il=$enableval],
              [build_il=yes])

AM_CONDITIONAL(USE_DEVIL, test x$build_il = xyes)

if test "x$build_il" = "xyes" ; then
	AC_CHECK_LIB(IL, ilInit,,AC_MSG_ERROR([
****************************************************************
* You do not have DevIL installed.  This is required to build. *
* You may find it at http://openil.sourceforge.net/.           *
* Note: You can also provide --disable-devil to the build      *
* process to build without DevIL. This is an advanced option   *
* useful only if you provide your own image loading codecs.    *
****************************************************************]))
	AC_CHECK_LIB(ILU, iluFlipImage)
	AC_DEFINE([OGRE_NO_DEVIL], [0], [Build devil])
else
	AC_DEFINE([OGRE_NO_DEVIL], [1], [Build devil])
fi


])


AC_DEFUN([OGRE_CHECK_PIC],
[
AC_MSG_CHECKING([whether -fPIC is needed])
    case $host in
        x86_64-*)
            CXXFLAGS="$CXXFLAGS -fPIC"
            AC_MSG_RESULT(yes)
        ;;
        *)
            AC_MSG_RESULT(no)
        ;;
    esac
])

AC_DEFUN([OGRE_CHECK_CEGUI], [
    PKG_CHECK_MODULES(CEGUI, CEGUI >= 0.3.0, 
            [build_cegui_sample=true], [build_cegui_sample=false])
    if test x$build_cegui_sample = xtrue; then
        AC_CONFIG_FILES([Samples/Common/CEGUIRenderer/Makefile \
                         Samples/Common/CEGUIRenderer/CEGUI-OGRE.pc
                         Samples/Common/CEGUIRenderer/src/Makefile \
                         Samples/Common/CEGUIRenderer/include/Makefile \
                         Samples/Gui/Makefile \
                         Samples/Gui/src/Makefile])
        AC_SUBST(CEGUI_CFLAGS)
        AC_SUBST(CEGUI_LIBS)
        AC_MSG_RESULT([CEGUI available, Gui and FacialAnimation samples will be built])
    else
        AC_MSG_RESULT([CEGUI not available, Gui and FacialAnimation samples will not be built])
    fi
    AM_CONDITIONAL([HAVE_CEGUI], [test x$build_cegui_sample = xtrue])
])

AC_DEFUN([OGRE_CHECK_DOUBLE],
[
AC_ARG_ENABLE(double,
              AC_HELP_STRING([--enable-double],
                             [Build OGRE in double floating point precision mode. This is not recommended for normal use as it is slower.]),
              [build_double=$enableval],
              [build_double=no])
AC_MSG_CHECKING([whether to use double floating point precision])
	case $build_double in
        yes)
			AC_DEFINE([OGRE_DOUBLE_PRECISION], [1], [Build with double precision])
			AC_MSG_RESULT(yes)
			OGRE_DOUBLE_PRECISION_FLAGS="-DOGRE_DOUBLE_PRECISION"
        ;;
        *)
			AC_DEFINE([OGRE_DOUBLE_PRECISION], [0], [Build with double precision])
            AC_MSG_RESULT(no)
        ;;
    esac
    AC_SUBST(OGRE_DOUBLE_PRECISION_FLAGS)
])

AC_DEFUN([OGRE_CHECK_THREADING],
[
AC_ARG_ENABLE(threading,
              AC_HELP_STRING([--enable-threading],
                             [Indicate general support for multithreading. This will enable threading support in certain parts of the engine, mainly resource loading and SharedPtr handling. WARNING: highly experimental, use with caution.]),
              [build_threads=$enableval],
              [build_threads=no])
AC_MSG_CHECKING([whether to use threaded resource loading])
	case $build_threads in
        yes)
            CXXFLAGS="$CXXFLAGS -pthread"
            OGRE_THREAD_LIBS="-lboost_thread-mt"
			AC_DEFINE([OGRE_THREAD_SUPPORT], [1], [Build with thread support])
            AC_CHECK_LIB([boost_thread-mt], [main],, AC_MSG_ERROR([cannot find boost_thread-mt library]))
            AC_MSG_RESULT(yes)
        ;;
        *)
            OGRE_THREAD_LIBS=""
			AC_DEFINE([OGRE_THREAD_SUPPORT], [0], [Build with thread support])
            AC_MSG_RESULT(no)
        ;;
    esac
    AC_SUBST(OGRE_THREAD_LIBS)
])
