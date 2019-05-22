BASE=../..
LIBS=${BASE}/libs
GZSTREAM=${LIBS}/gzstream
SHA1=${LIBS}/sha1

CPPFLAGS=-g -I${BASE} -I${GZSTREAM} -O
LDFLAGS=-L${BASE} -L${GZSTREAM} -lgzstream -lz
AR=ar cr

default: objects

objects: ${GZSTREAM}/libgzstream.a objects.o ${SHA1}/sha1.o 	
	g++ ${CPPFLAGS} $^ -o $@  ${LDFLAGS} 

${GZSTREAM}/libgzstream.a:
	make -C ${GZSTREAM}

%.o: %.cpp
	g++ ${CPPFLAGS} -c $^ -o $@
