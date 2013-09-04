include ./Makefile.rules

all:
	cd src/ && make

clean:
	$(CLEAN) && cd src/ && make clean

distclean: clean
	cd src/ && make distclean && cd ../ && rm -f Makefile.rules

dist: distclean
	cd ../ && tar cvjf dzogan_f-libdata.tar.bz2 --exclude=*.svn dzogan_f-libdata/ &&	\
	chmod 705 dzogan_f-libdata.tar.bz2

debug: clean
	cd src/ && make debug