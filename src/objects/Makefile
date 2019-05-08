BASE=../..
CPPFLAGS=-g -I${BASE}
LDFLAGS=-L${BASE}
AR=ar cr

default: objects

objects: objects.o ${BASE}/libs/sha1/sha1.o
	g++ ${CPPFLAGS} ${LDFLAGS} $^ -o $@

%.o: %.cpp
	g++ ${CPPFLAGS} -c $^ -o $@
