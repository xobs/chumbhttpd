EXTRA_DIST = reconf Makefile.cleanup Doxyfile docs
SUBDIRS = src include examples

libtool: @LIBTOOL_DEPS@
	cd $(srcdir) && $(SHELL) ./config.status --recheck
