#include "lib.h"
#include "extern.h"

pthread_mutex_t mutex	=	PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond		=	PTHREAD_COND_INITIALIZER;


unsigned short int port_br[6]={9999, 10100, 10200, 10300, 10400, 10500};
short int flag=2;

int main(int argc, char *argv[]){
	
	pthread_t 			threads_br[MAX_NUM_BRIDGE];
	pthread_t 			threads_lan[MAX_NUM_LAN];
	pthread_attr_t 		attr;
	
	int 				ris, rc, t;
	
	DATI *arg_return;
	arg_return=malloc(sizeof(DATI));
	if (arg_return==NULL) printf(_KRED "ERRORE MALLOC DATI NEL MAIN \n" _KNRM); 
	
	pthread_cond_init (&cond, NULL);
	pthread_attr_init (&attr);
	
	printf(_KBLU"####################################\n");
	printf(_KRED" EMULAZIONE DI SPANNING TREE MINIMO\n");
	printf(_KBLU"####################################\n");
	
	printf(_KGRN"inizio a configurare la rete lan, leggendo il file di configurazione\n" _KNRM);
	
	read_file_config(arg_return);
	
	printf(_KGRN "finito lettura file config. Sono ritornato nel main\n" _KNRM);
	
	/* creo i threads dei Bridge */
	for(t=1; t <= arg_return->n_br;t++){
		
		printf(_KGRN"\nmain: Creating thread bridge %d\n" _KNRM, (t));
		rc = pthread_create(&threads_br[t], NULL, create_br, (void *)arg_return->b[t] );		/* creo i thread */
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n",rc);
			exit(-1);
			}
		sleep(1);
	}
	
	/* creo i threads delle LAN */
	for(t=1; t <= (arg_return->n_lan);t++){
		
		printf(_KGRN"\nmain: Creating thread lan %d\n" _KNRM, arg_return->l[t]->id);
		printf(_KGRN"main: che ha %d link \n" _KNRM, arg_return->l[t]->n_port);
		sleep(1);
		rc = pthread_create(&threads_lan[t], NULL, create_lan, (void *)arg_return->l[t] );		/* creo i thread */
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n",rc);
			exit(-1);
		}
		sleep(2);
	}
	
	sleep(8);
	
	pthread_mutex_lock(&mutex);
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);
	
	
	printf("fine main\n"); /*fflush(stdout);*/
	
	pthread_exit (NULL);
	
	return 0;
}
