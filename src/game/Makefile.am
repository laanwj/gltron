noinst_LIBRARIES=libgame.a

libgame_a_SOURCES = \
	camera.c computer.c computer_utilities.c credits.c \
	engine.c event.c game.c globals.c gui.c \
	init.c level.c pause.c \
	timedemo.c switchCallbacks.c scripting_interface.c \
	resource.c 32bit_warning.c

AM_CFLAGS = \
	-I$(top_srcdir)/nebu/include \
	-I$(top_srcdir)/src/include \
	-I$(top_srcdir)/glew/include \
	-I$(top_srcdir)/lua5/include
