/* banale_quasigiusto.c  */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX_NUM_THREADS 17
#define SIZEBUF 100000L
#define SOCKET_ERROR   ((int)-1)

typedef struct{
		int thread_id;
		int n_port;
		int port[15];
		char tipe[9];
} thread_arg;
	
int Glob=10;

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
	struct sockaddr_in Local, From;
	char string_remote_ip_address[100];
	unsigned short int remote_port_number, local_port_number;
	int socketfd, OptVal, msglen, ris;
	unsigned int Fromlen;
	char msg[SIZEBUF];
	int p;
	unsigned short int required_remote_port_number;
	char string_required_remote_ip_address[100];
	struct sockaddr_in requiredFrom;
		
	/* variabili per la select */
	ssize _t n;
	fd _set rset, allset ;
		
	thread_arg *param = (thread_arg *)prm;
	printf("\nsono il thread/bridge: %d \n", (param->thread_id +1));
	
	for( p=0; p< (param->n_port); p++){
	
		if( (param->port[p]) == NULL  ) { printf ("porta non valida\n"); exit(1);  }
			else {
			local_port_number = param->port[p];
			}
		/* get a datagram socket */
		socketfd = socket(AF_INET, SOCK_DGRAM, 0);
		if (socketfd == SOCKET_ERROR) {
			printf ("socket() failed, Err: %d \"%s\"\n", errno, strerror(errno));
			exit(1);
		}
	
		memset(&Local, 0, sizeof(Local));
		Local.sin_family		=	AF_INET;
		/* indicando INADDR_ANY viene collegato il socket all'indirizzo locale IP     */
		/* dell'interaccia di rete che verra' utilizzata per inoltrare il datagram IP  */
		Local.sin_addr.s_addr	=  htonl(INADDR_ANY);       /* wildcard */
		/* Local.sin_addr.s_addr = inet_addr("130.136.4.212"); */
		Local.sin_port		=	htons(local_port_number);
		ris = bind(socketfd, (struct sockaddr*) &Local, sizeof(Local));
		if (ris == SOCKET_ERROR)  {
		printf ("bind() 1 failed, Err: %d \"%s\"\n",errno,strerror(errno));
			exit(1);
		}
		printf("in ascolto sulla porta: %d \n", local_port_number);
	}
	while(1)
  {

  	/* wait for datagram */
		memset(&From, 0, sizeof(From));
		Fromlen=sizeof(struct sockaddr_in);
		msglen = recvfrom ( socketfd, msg, (int)SIZEBUF, 0, (struct sockaddr*)&From, &Fromlen);
		if (msglen<0) {
			char msgerror[1024];
			sprintf(msgerror,"recvfrom() failed [err %d] ", errno);
			perror(msgerror); 
			/* return(1); */
		}else {
			sprintf((char*)string_remote_ip_address,"%s",inet_ntoa(From.sin_addr));
			remote_port_number = ntohs(From.sin_port);
			printf("ricevuto da socketfd %d , nel thread: %d , msg: \"%s\" len %d, from host %s, port %d\n", socketfd, (param->thread_id + 1),
						msg, msglen, string_remote_ip_address, remote_port_number);
			/* if (msg == kill) ptheard_exit() */ 
		}
  }

}

int main(int argc, char *argv[]){
	
	pthread_t threads[MAX_NUM_THREADS];
	pthread_attr_t attr;
	
	int rc, t, port[15], num_br, num_port, c;
	thread_arg *arg_br;      
	
	
	pthread_cond_init (&cond, NULL);
	pthread_attr_init (&attr);
	
	printf("quanti thread/bridge vuoi creare? \n");
	scanf("%d", &num_br);
     
	for(t=0; t < num_br;t++){
		
		printf("nel thread/bridge: %d quante porte vuoi creare? \n", (t+1));
		scanf("%d", &num_port);
		for (c=0; c!=num_port; c++){
				printf("porta num: %d Ã¨? \n", (c+1));
				scanf("%d", &port[c]);
				}
		arg_br=malloc(sizeof(thread_arg));       /* alloco memoria sullo heap in cui mettere i parametri per i pthread */
		if(arg_br==NULL) {
				perror(" malloc failed: ");
				pthread_exit (NULL);
			}else{ 
				arg_br->thread_id=t;
				arg_br->n_port=num_port;
				memcpy ( arg_br->port, port, num_port*sizeof(int));
		}
		printf("Creating thread %d\n", (t+1));
		rc = pthread_create(&threads[t], NULL, create_br, arg_br );  /* creo i thread */
		if (rc){
		printf("ERROR; return code from pthread_create() is %d\n",rc);
		exit(-1);
		}
		sleep(4);
	}

	sleep(3);
	printf("fine main\n"); fflush(stdout);
	
	pthread_exit (NULL);  /* anche qui ho tolto il commento */
	
	return 0;
}
