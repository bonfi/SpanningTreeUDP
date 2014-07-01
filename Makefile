GCCFLAGS= -ansi -Wall -pedantic
LINKERFLAGS= -pthread
DIR_SRC = source
DIR_HDR = header
DIR_OBJ = object

all: simulation.bin

# Compilazione dei moduli (creazione file oggetto)

main: ${DIR_SRC}/main.c
	gcc -o ${DIR_OBJ}/main.o -I ${DIR_HDR} -c ${GCCFLAGS} ${DIR_SRC}/main.c

funzioni: ${DIR_SRC}/funzioni.c
	gcc -o ${DIR_OBJ}/funzioni.o -I ${DIR_HDR} -c ${GCCFLAGS} ${DIR_SRC}/funzioni.c

funz_setup_link: ${DIR_SRC}/funz_setup_link.c
	gcc -o ${DIR_OBJ}/funz_setup_link.o -I ${DIR_HDR} -c ${GCCFLAGS} ${DIR_SRC}/funz_setup_link.c

funz_root_br: ${DIR_SRC}/funz_root_br.c
	gcc -o ${DIR_OBJ}/funz_root_br.o -I ${DIR_HDR} -c ${GCCFLAGS} ${DIR_SRC}/funz_root_br.c

funz_spanning_tree: ${DIR_SRC}/funz_spanning_tree.c
	gcc -o ${DIR_OBJ}/funz_spanning_tree.o -I ${DIR_HDR} -c ${GCCFLAGS} ${DIR_SRC}/funz_spanning_tree.c

read_file_config: ${DIR_SRC}/read_file_config.c
	gcc -o ${DIR_OBJ}/read_file_config.o -I ${DIR_HDR} -c ${GCCFLAGS} ${DIR_SRC}/read_file_config.c
	
setup_thread_br: ${DIR_SRC}/setup_thread_br.c
	gcc -o ${DIR_OBJ}/setup_thread_br.o -I ${DIR_HDR} -c ${GCCFLAGS} ${DIR_SRC}/setup_thread_br.c

setup_thread_lan: ${DIR_SRC}/setup_thread_lan.c
	gcc -o ${DIR_OBJ}/setup_thread_lan.o -I ${DIR_HDR} -c ${GCCFLAGS} ${DIR_SRC}/setup_thread_lan.c

# read_file: ${DIR_SRC}/read_file_v3.c
#	gcc -o ${DIR_OBJ}/read_file_v3.o -I ${DIR_HDR} -c ${GCCFLAGS} ${DIR_SRC}/read_file_v3.c


# Linker

simulation.bin: main funzioni funz_setup_link funz_root_br funz_spanning_tree setup_thread_br setup_thread_lan read_file_config
	gcc -o simulation.bin ${LINKERFLAGS} ${DIR_OBJ}/*.o

# Cancellazione dei file di compilazione

clean: 
	rm -f ${DIR_OBJ}/*
	rm -f simulation.bin
