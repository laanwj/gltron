noinst_LIBRARIES=libscripting.a

libscripting_a_SOURCES= \
	scripting.c

libscripting_a_LIBADD= \
	$(top_builddir)/lua5/liblua.a \
	$(top_builddir)/lua5/lib/liblualib.a

AM_CFLAGS = \
	-I$(top_srcdir)/nebu/include \
	-I$(top_srcdir)/lua5/include
