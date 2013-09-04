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
