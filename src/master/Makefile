BASE=../..
LIBS=${BASE}/libs
GZSTREAM=${LIBS}/gzstream
SHA1=${LIBS}/sha1
SRC=..
OBJECTS=${SRC}/objects
TREE=${SRC}/tree
ADD_CMD=${SRC}/add_command
INIT=${SRC}/init
CHECKOUT=${SRC}/checkout 
COMMIT=${SRC}/commit 
COMMIT_CMD=${SRC}/commit_command
PUSH_PULL=${SRC}/push_pull 
REDIRECT=${SRC}/redirect 
REFS=${SRC}/refs
# TREE=${SRC}/tree

CPPFLAGS=-g -I${BASE} -I${GZSTREAM} -Wall -std=c++11
LDFLAGS=-L${BASE} -L${GZSTREAM} -lgzstream -lz
AR=ar

default: master

# commit_cmd.a: ${ADD_CMD}/add.a commit_cmd.o
# 	@rm -f $@
# 	${AR} -rcT $@ $^

# ${ADD_CMD}/add.a:
# 	make -C ${ADD_CMD} add.a

# ${TREE}/tree.a: ${OBJECTS}/objects.a
# 	make -C ${TREE} tree.a

# ${OBJECTS}/objects.a:
# 	make -C ${OBJECTS} objects.a 

%.o: %.cpp
	g++ ${CPPFLAGS} -c $^ -o $@

master: master.o ${INIT}/init.o  
	g++ ${CPPFLAGS} $^ -o $@ ${LDFLAGS}

${INIT}/init.o:
	make -C ${INIT} init.o

clean:
	@rm -f *.o 
	@rm -f master
	@make -C ${INIT} clean