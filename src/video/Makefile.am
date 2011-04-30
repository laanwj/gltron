noinst_LIBRARIES=libvideo.a

libvideo_a_SOURCES = \
	artpack.c explosion.c fonts.c gamegraphics.c \
	graphics_fx.c graphics_hud.c graphics_lights.c \
	graphics_utility.c graphics_world.c level.c \
	material.c model.c recognizer.c \
	screenshot.c skybox.c texture.c trail.c trail_geometry.c \
	trail_render.c video.c visuals_2d.c

AM_CFLAGS = \
	-I$(top_srcdir)/lua5/include \
	-I$(top_srcdir)/nebu/include \
	-I$(top_srcdir)/glew/include \
	-I$(top_srcdir)/src/include
