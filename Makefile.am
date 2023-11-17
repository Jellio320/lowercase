ACLOCAL_AMFLAGS = -I m4 --install

EXTRA_DIST = src/changecase.in \
			 m4/NOTES

bin_PROGRAMS = lowercase
lowercase_SOURCES = src/main.c

bin_SCRIPTS = uppercase flipcase
CLEANFILES = $(bin_SCRIPTS)
uppercase flipcase: src/changecase.in Makefile
	case $@ in uppercase) option=-U;; flipcase) option=-F;; esac && \
	sed -e 's|[@]shell[@]|$(SHELL)|g' \
		-e 's|[@]case[@]|lowercase|g' \
		-e "s|[@]option[@]|$$option|g" < $< > $@
	chmod +x $@
