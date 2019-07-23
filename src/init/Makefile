BASE=../..
LIBS=${BASE}/libs

CPPFLAGS=-g -I${BASE} -I${GZSTREAM} -Wall
LDFLAGS=-L${BASE} -L${GZSTREAM} -lgzstream -lz
AR=ar

default: init.o

%.o: %.cpp
	g++ ${CPPFLAGS} -c $^ -o $@
	  
clean:
	@rm -f *.o
