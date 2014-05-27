/* banale_quasigiusto.c  */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "BRLN.h"					/* struct con tipi di dati per i thread(lan & bridge) */
#include "lib.h"


pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;

/* void *PrintHello(void *p_index)
{
	//Glob+=3;
	sleep(1);
	printf("\n index %d: Hello World! Glob=%d\n", *((int*)p_index), Glob);
	free(p_index);
	pthread_exit (NULL);
} */

void *create_br(void *prm){
	struct sockaddr_in	Local, From;
	char				string_remote_ip_address[100];
	unsigned short int	remote_port_number, local_port_number;
	int					socketfd, OptVal,p, msglen, sock_fd[10];
	unsigned int		Fromlen;
	char				msg[SIZEBUF];
	unsigned short int	required_remote_port_number;
	char				string_required_remote_ip_address[100];
	struct sockaddr_in	requiredFrom;
	fd_set				service_fd_set, read_fd_set;
	
	BRIDGE *param = (BRIDGE *)prm;
	printf("\nsono il thread/bridge: %d \n", (param->id +1));
	
	FD_ZERO(&service_fd_set);
	FD_ZERO(&read_fd_set);
	
	for( p=0; p< (param->n_port); p++){
	
		if( ((int)param->port_in[p]) == NULL  ) { printf ("porta non valida\n"); exit(1);  }
			else {
			local_port_number = param->port_in[p];
			}
		/* get a datagram socket */
		socketfd = socket(AF_INET, SOCK_DGRAM, 0);
		if (socketfd == SOCKET_ERROR) {
			printf ("socket() failed, Err: %d \"%s\"\n", errno, strerror(errno));
			exit(1);
		}
	
		memset(&Local, 0, sizeof(Local));
		Local.sin_family		=	AF_INET;
		Local.sin_addr.s_addr	=  htonl(INADDR_ANY);
		Local.sin_port		=	htons(local_port_number);
		
		/* BIND */
		sock_fd[p] = bind(socketfd, (struct sockaddr*) &Local, sizeof(Local));
		if (sock_fd[p] == SOCKET_ERROR)  {
		printf ("bind() su porta: failed, Err: %d \"%s\"\n",param->port_in[p],errno,strerror(errno));
			exit(1);
		}
		printf("in ascolto sulla porta: %d \n", local_port_number);
	}
	while(1){
	/* wait for datagram */
		memset(&From, 0, sizeof(From));
		Fromlen=sizeof(struct sockaddr_in);
		/* RECVFROM */
		msglen = recvfrom ( socketfd, msg, (int)SIZEBUF, 0, (struct sockaddr*)&From, &Fromlen);
		if (msglen<0) {
			char msgerror[1024];
			sprintf(msgerror,"recvfrom() failed [err %d] ", errno);
			perror(msgerror); 
			/* return(1); */
		}else {
			sprintf((char*)string_remote_ip_address,"%s",inet_ntoa(From.sin_addr));
			remote_port_number = ntohs(From.sin_port);
			printf("ricevuto da socketfd %d , nel thread: %d , msg: \"%s\" len %d, from host %s, port %d\n", socketfd,
						(param->id + 1), msg, msglen, string_remote_ip_address, remote_port_number);
		}
	}
}

int main(int argc, char *argv[]){
	
	pthread_t threads_br[MAX_NUM_BRIDGE];
	pthread_attr_t attr;
	
	pthread_cond_init (&cond, NULL);
	pthread_attr_init (&attr);

	int rc, t, port[15], num_br, num_port, c;     
 
	for(t=0; t < num_br;t++){
		
		printf("nel thread/bridge: %d quante porte vuoi creare? \n", (t+1));
		scanf("%d", &num_port);
		for (c=0; c!=num_port; c++){
				printf("porta num: %d è? \n", (c+1));
				scanf("%d", &port[c]);
				}
		arg_br=malloc(sizeof(BRIDGE));				/* alloco memoria sullo heap in cui mettere i parametri per i pthread */
		if(arg_br==NULL) {
				perror(" malloc failed: ");
				pthread_exit (NULL);
			}else{ 
				arg_br->thread_id=t;
				arg_br->n_port=num_port;
				memcpy ( arg_br->port, port, num_port*sizeof(int));
		}
		printf("Creating thread %d\n", (t+1));
		rc = pthread_create(&threads_br[t], NULL, create_br, arg_br );		/* creo i thread */
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n",rc);
			exit(-1);
			}
		sleep(4);
	}
	
	sleep(3);
	printf("fine main\n"); fflush(stdout);
	
	pthread_exit (NULL);
	
	return 0;
}
