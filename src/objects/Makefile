BASE=../..
LIBS=${BASE}/libs
GZSTREAM=${LIBS}/gzstream
SHA1=${LIBS}/sha1

CPPFLAGS=-g -I${BASE} -I${GZSTREAM} -Wall
LDFLAGS=-L${BASE} -L${GZSTREAM} -lgzstream -lz
AR=ar

default: objects.o

objects: ${GZSTREAM}/libgzstream.a objects.o ${SHA1}/sha1.o 	
	g++ ${CPPFLAGS} $^ -o $@  ${LDFLAGS} 

${GZSTREAM}/libgzstream.a:
	make -C ${GZSTREAM}

%.o: %.cpp
	g++ ${CPPFLAGS} -c $^ -o $@

objects.a : ${GZSTREAM}/libgzstream.a objects.o ${SHA1}/sha1.o
	${AR} -rcT $@ $^
	  
clean:
	@rm -f objects
	@rm -f *.o
	@rm -f objects.a
