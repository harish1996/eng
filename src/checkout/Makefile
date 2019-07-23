BASE=../..
LIBS=${BASE}/libs
GZSTREAM=${LIBS}/gzstream
SHA1=${LIBS}/sha1
SRC=..
OBJECTS=${SRC}/objects
TREE=${SRC}/tree
ADD_CMD=${SRC}/add_command
COM_CMD=${SRC}/commit_command

CPPFLAGS=-g -I${BASE} -I${GZSTREAM} -Wall -std=c++11
LDFLAGS=-L${BASE} -L${GZSTREAM} -lgzstream -lz
AR=ar

default: test

checkout.a: ${COM_CMD}/commit_cmd.a checkout.o
	@rm -f $@
	${AR} -rcT $@ $^

${COM_CMD}/commit_cmd.a:
	make -C ${COM_CMD} commit_cmd.a

# ${ADD_CMD}/add.a:
# 	make -C ${ADD_CMD} add.a

# ${TREE}/tree.a:
# 	make -C ${TREE} tree.a

# ${OBJECTS}/objects.a:
# 	make -C ${OBJECTS} objects.a 

%.o: %.cpp
	g++ ${CPPFLAGS} -c $^ -o $@

test: test.o checkout.a 
	g++ ${CPPFLAGS} $^ -o $@ ${LDFLAGS}

clean:
	@rm -f test
	@rm -f *.o
	@rm -f checkout.a
	@make -C ${OBJECTS} clean
	@make -C ${TREE} clean
	@make -C ${ADD_CMD} clean
	@make -C ${COM_CMD} clean