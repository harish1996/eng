BASE=../..
LIBS=${BASE}/libs
GZSTREAM=${LIBS}/gzstream
SHA1=${LIBS}/sha1
SRC=..
OBJECTS=${SRC}/objects


CPPFLAGS=-g -I${BASE} -I${GZSTREAM} -Wall -std=c++11
LDFLAGS=-L${BASE} -L${GZSTREAM} -lgzstream -lz
AR=ar

default: test

commit.a: ${OBJECTS}/objects.a commit.o
	${AR} -rcT $@ $^

${OBJECTS}/objects.a: 
	make -C ${OBJECTS} objects.a

${GZSTREAM}/libgzstream.a:
	make -C ${GZSTREAM}

%.o: %.cpp
	g++ ${CPPFLAGS} -c $^ -o $@

#test: tree.a test.o
test: test.o  commit.o ${OBJECTS}/objects.o ${SHA1}/sha1.o 
	g++ ${CPPFLAGS} $^ -o $@ ${LDFLAGS}


clean:
	@rm -f test
	@rm -f *.o