BASE=../..
LIBS=${BASE}/libs
GZSTREAM=${LIBS}/gzstream
SHA1=${LIBS}/sha1
SRC=..
OBJECTS=${SRC}/objects


CPPFLAGS=-g -I${BASE} -I${GZSTREAM} -Wall -std=c++11
LDFLAGS=-L${BASE} -L${GZSTREAM} -lgzstream -lz
AR=ar cr

default: tree

tree: ${GZSTREAM}/libgzstream.a tree.o objects.o ${SHA1}/sha1.o 	
	g++ ${CPPFLAGS} $^ -o $@  ${LDFLAGS} 

tree.a: ${OBJECTS}/objects.a tree.o
	${AR} $@ $^

${OBJECTS}/objects.a: 
	make -C ${OBJECTS} objects.a

${GZSTREAM}/libgzstream.a:
	make -C ${GZSTREAM}

%.o: %.cpp
	g++ ${CPPFLAGS} -c $^ -o $@

#test: tree.a test.o
test: test.o tree.o ${OBJECTS}/objects.o ${SHA1}/sha1.o 
	g++ ${CPPFLAGS} $^ -o $@ ${LDFLAGS}


clean:
	@rm -f tree
	@rm -f *.o
