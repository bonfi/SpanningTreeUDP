#ifndef _LIB_H
#define _LIB_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <pthread.h>
#include <fcntl.h>



#define MAX_NUM_BRIDGE	6
#define MAX_NUM_LAN		13

#include "BRLN.h"
#include "dati.h"
#include "funzioni.h"
#include "read_file_config.h"
#include "setup_thread_lan.h"
#include "setup_thread_br.h"
#include "main.h"


 
#define DEBUG			0			/* 1 per debug, 0 no debug */
#define MAXBUF			3000
#define SIZEBUF			50
#define SOCKET_ERROR	((int)-1)

/* color text in console for debug better */

#define _KNRM  "\x1B[0m"
#define _KRED  "\x1B[31m"
#define _KGRN  "\x1B[32m"
#define _KYEL  "\x1B[33m"
#define _KBLU  "\x1B[34m"
#define _KMAG  "\x1B[35m"
#define _KCYN  "\x1B[36m"
#define _KWHT  "\x1B[37m"



#endif

