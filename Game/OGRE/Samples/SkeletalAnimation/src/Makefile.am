INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/Samples/SkeletalAnimation/include -I$(top_srcdir)/OgreMain/include -I$(top_srcdir)/Samples/Common/include

noinst_PROGRAMS = SkeletalAnimation

SkeletalAnimation_SOURCES = SkeletalAnimation.cpp
SkeletalAnimation_LDFLAGS = -L$(top_builddir)/OgreMain/src
SkeletalAnimation_LDADD = -lOgreMain

install-exec-local:
	$(INSTALL_PROGRAM) .libs/SkeletalAnimation $(top_builddir)/Samples/Common/bin/SkeletalAnimation
