BASE=../..
LIBS=${BASE}/libs

CPPFLAGS=-g -I${BASE} -I${GZSTREAM} -Wall
LDFLAGS=-L${BASE} -L${GZSTREAM} -lgzstream -lz
AR=ar

default: test

%.o: %.cpp
	g++ ${CPPFLAGS} -c $^ -o $@
	  
test: test.o refs.o
	g++ ${CPPFLAGS} $^ -o $@ ${LDFLAGS}

clean:
	@rm -f *.o
