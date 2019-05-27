BASE=../..
LIBS=${BASE}/libs
GZSTREAM=${LIBS}/gzstream
SHA1=${LIBS}/sha1
SRC=..
OBJECTS=${SRC}/

CPPFLAGS=-g -I${BASE} -I${GZSTREAM} -Wall
LDFLAGS=-L${BASE} -L${GZSTREAM} -lgzstream -lz
AR=ar cr

default: tree

tree: ${GZSTREAM}/libgzstream.a tree.o objects.o ${SHA1}/sha1.o 	
	g++ ${CPPFLAGS} $^ -o $@  ${LDFLAGS} 

objects.o: 
	make -C ${OBJECTS}

${GZSTREAM}/libgzstream.a:
	make -C ${GZSTREAM}

%.o: %.cpp
	g++ ${CPPFLAGS} -c $^ -o $@

clean:
	@rm -f tree
	@rm -f *.o
