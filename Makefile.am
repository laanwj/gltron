SUBDIRS = glew lib3ds lua5 nebu src art data music scripts levels

bin_PROGRAMS = gltron

gltron_SOURCES = @top_srcdir@/src/gltron.c

gltron_LINK = $(CXX) -o $@
gltron_LDADD = \
	src/game/libgame.a \
	src/input/libinput.a \
	src/audio/libaudio.a \
	src/video/libvideo.a \
	src/configuration/libconfiguration.a \
	src/base/libbase.a \
	src/filesystem/libfilesystem.a \
	nebu/input/libinput.a \
	nebu/audio/libaudio.a \
	nebu/video/libvideo.a \
	nebu/scripting/libscripting.a \
	nebu/filesystem/libfilesystem.a \
	nebu/base/libbase.a \
	lua5/liblua.a \
	lua5/lib/liblualib.a \
	lib3ds/lib3ds.a \
	glew//libglew.a

#gltron_LDFLAGS = -static

AM_CFLAGS = \
	-I@top_srcdir@/src/include \
	-I@top_srcdir@/lua5/include \
	-I@top_srcdir@/nebu/include

AM_CXXFLAGS = \
	-I@top_srcdir@/src/include \
	-I@top_srcdir@/lua5/include \
	-I@top_srcdir@/nebu/include

tags:
	etags `find . -name '*.c' -or -name '*.cpp' -or -name '*.h' -or -name '*.lua'`
