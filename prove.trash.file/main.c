#include "lib.h"

pthread_mutex_t mutex	=	PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond		=	PTHREAD_COND_INITIALIZER;

int main(int argc, char *argv[]){
	
	pthread_t threads_br[MAX_NUM_BRIDGE];
	pthread_attr_t attr;
	
	pthread_cond_init (&cond, NULL);
	pthread_attr_init (&attr);
	
	LAN *l;
	BRIDGE *b;
	
	int rc, t;
	
	DATI *dati_setup = malloc(sizeof(DATI));
	
	printf(_KBLU"####################################\n");
	printf(_KRED" EMULAZIONE DI SPANNING TREE MINIMO\n");
	printf(_KBLU"####################################\n");
	
	printf(_KGRN"inizio a configurare la rete lan, leggendo il file di configurazione\n" _KNRM);
	
	dati_setup = read_file_config();
	
	
	
	for(t=0; t < dati_setup->n_br;t++){
		b=(BRIDGE *) malloc(sizeof(BRIDGE));
		b=&(dati_setup->b[t]);
		printf("Creating thread bridge %d\n", (t+1));
		rc = pthread_create(&threads_br[t], NULL, create_br, &b );		/* creo i thread */
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n",rc);
			exit(-1);
			}
		sleep(2);
	}
	
	for(t=0; t < dati_setup->n_lan;t++){
		l=(LAN *) malloc(sizeof(LAN));
		l=&(dati_setup->l[t]);
		printf("Creating thread lan %d\n", (t+1));
		rc = pthread_create(&threads_br[t], NULL, create_lan, &l );		/* creo i thread */
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
