##
## Copyright 2013 Fabon Dzogang
##
## This file is part of ellkm.

## ellkm is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## any later version.

## ellkm is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.

## You should have received a copy of the GNU General Public License
## along with ellkm.  If not, see <http://www.gnu.org/licenses/>.
##

include ../Makefile.rules

SRC=    main.cc				\
	kmeans.cc			\
	data_types.cc

HEADER= data_types.hh			\
	kmeans.hh			\
	similarity_function.hh

DEBUG_FLAG=""

OBJ=${SRC:.cc=.o}

all: $(OUT)
	chmod 700 $(OUT) && cp $(OUT) ../

$(OBJ): $(HEADER)

.cc.o : $< $(SRC) $(HEADER)
	$(CC) $(CFLAGS) -std=c++0x -c $< -o $@

$(OUT): $(OBJ)
	$(CC) $(OBJ) -o $(OUT) 

clean:
	$(CLEAN)

distclean: clean
	rm -f ../$(OUT) $(OUT)
