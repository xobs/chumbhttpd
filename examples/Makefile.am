noinst_PROGRAMS = example

INCLUDES = -I../src -I../include

example_SOURCES = example.cpp
example_LDFLAGS = -L../src -L../src/.libs
example_LDADD = -lchumbhttpd -lpthread
example_DEPENDENCIES = ../src/.libs/libchumbhttpd.la
