GCCFLAGS= -Wall -pedantic -g
LINKERFLAGS= -pthread
DIR_SRC = source
DIR_HDR = header
DIR_OBJ = object

all:	simulation.bin

# Compilazione dei moduli (creazione file oggetto)

main:	${DIR_SRC}/main.c
	gcc -o ${DIR_OBJ}/main.o -I ${DIR_HDR} -c ${GCCFLAGS} ${DIR_SRC}/main.c

funzioni: ${DIR_SRC}/funzioni.c
	gcc -o ${DIR_OBJ}/funzioni.o -I ${DIR_HDR} -c ${GCCFLAGS} ${DIR_SRC}/funzioni.c

read_file_config:	${DIR_SRC}/read_file_config.c
	gcc -o ${DIR_OBJ}/read_file_config.o -I ${DIR_HDR} -c ${GCCFLAGS} ${DIR_SRC}/read_file_config.c
	
setup_thread_br: ${DIR_SRC}/setup_thread_br.c
	gcc -o ${DIR_OBJ}/setup_thread_br.o -I ${DIR_HDR} -c ${GCCFLAGS} ${DIR_SRC}/setup_thread_br.c

setup_thread_lan: ${DIR_SRC}/setup_thread_lan.c
	gcc -o ${DIR_OBJ}/setup_thread_lan.o -I ${DIR_HDR} -c ${GCCFLAGS} ${DIR_SRC}/setup_thread_lan.c

# read_file: ${DIR_SRC}/read_file_v3.c
#	gcc -o ${DIR_OBJ}/read_file_v3.o -I ${DIR_HDR} -c ${GCCFLAGS} ${DIR_SRC}/read_file_v3.c
	

# Linker

simulation.bin:	 main funzioni setup_thread_br setup_thread_lan read_file_config
	gcc -o simulation.bin ${LINKERFLAGS} ${DIR_OBJ}/*.o

# Cancellazione dei file di compilazione

clean:	
	rm -f ${DIR_OBJ}/*
	rm -f simulation.bin
