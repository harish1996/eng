BASE=../..
LIBS=${BASE}/libs
GZSTREAM=${LIBS}/gzstream
SHA1=${LIBS}/sha1
SRC=..
OBJECTS=${SRC}/objects
TREE=${SRC}/tree

CPPFLAGS=-g -I${BASE} -I${GZSTREAM} -Wall -std=c++11
LDFLAGS=-L${BASE} -L${GZSTREAM} -lgzstream -lz
AR=ar

default: test

add.a: ${TREE}/tree.a add.o
	${AR} -rcT $@ $^

${TREE}/tree.a:
	make -C ${TREE} tree.a

${OBJECTS}/objects.a: 
	make -C ${OBJECTS} objects.a

${GZSTREAM}/libgzstream.a:
	make -C ${GZSTREAM}


%.o: %.cpp
	g++ ${CPPFLAGS} -c $^ -o $@

#test: tree.a test.o
# test: test.o add.o ${TREE}/tree.o ${OBJECTS}/objects.o ${SHA1}/sha1.o 
# 	g++ ${CPPFLAGS} $^ -o $@ ${LDFLAGS}

test: test.o add.a 
	g++ ${CPPFLAGS} $^ -o $@ ${LDFLAGS}

clean:
	@rm -f tree
	@rm -f *.o
