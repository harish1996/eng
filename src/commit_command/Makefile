BASE=../..
LIBS=${BASE}/libs
GZSTREAM=${LIBS}/gzstream
SHA1=${LIBS}/sha1
SRC=..
OBJECTS=${SRC}/objects
TREE=${SRC}/tree
ADD_CMD=${SRC}/add_command

CPPFLAGS=-g -I${BASE} -I${GZSTREAM} -Wall -std=c++11
LDFLAGS=-L${BASE} -L${GZSTREAM} -lgzstream -lz
AR=ar

default: test

commit_cmd.a: ${ADD_CMD}/add.a commit_cmd.o
	@rm -f $@
	${AR} -rcT $@ $^

${ADD_CMD}/add.a: ${TREE}/tree.a
	make -C ${ADD_CMD} add.a

${TREE}/tree.a: ${OBJECTS}/objects.a
	make -C ${TREE} tree.a

${OBJECTS}/objects.a:
	make -C ${OBJECTS} objects.a 

%.o: %.cpp
	g++ ${CPPFLAGS} -c $^ -o $@

test: test.o commit_cmd.a 
	g++ ${CPPFLAGS} $^ -o $@ ${LDFLAGS}

clean:
	make -C ${ADD_CMD} clean
	make -C ${TREE} clean
	make -C ${OBJECTS} clean
	@rm -f test
	@rm -f *.o
	@rm -f commit_cmd.a