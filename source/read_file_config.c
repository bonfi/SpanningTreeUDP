#include "lib.h"
#include "extern.h"
#define config_txt "Config_v3.txt"
 
void *read_file_config(void* arg){
	
	int BUFF_CHAR=20;
	/* interi d'appoggio per cicli */
	int 	q,z, z1, x, ris,n, n_lan, n_br ;

	char *ptr;
	char ch,ch1, tmp[BUFF_CHAR], tmp_ip[BUFF_CHAR], tmp_ip21[BUFF_CHAR];
	char Br[]="BRIDGE";
	char Ln[]="LAN";
	char Lk[]="LINK";
	char string_local_ip_address[100];			/* ip delle lan*/
	FILE *fp;
	
	BRIDGE 	*b[MAX_NUM_BRIDGE];
	LAN 	*l[MAX_NUM_LAN];
	DATI	*arg_return0 = (DATI *)arg;
	
	fp = fopen(config_txt,"r");				/* apre il file in read mode */
	
	if( fp == NULL ){
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}
	printf("The contents of %s file are:\n", config_txt);
	
	while( ( ch = fgetc(fp) ) != EOF ){							/* scorre tutti i caratteri del file fino alla fine, salvo in ch il carattere uno alla volta */
		
		if(ch == '>'){
			fgets(tmp,BUFF_CHAR,fp);							/*prende una stringa per capire a che blocco siamo, Bridge, Lan o Link */
			if( (ptr = strchr(tmp, '\n')) != NULL){				/* rimuove \n dalla stringa tmp */
				*ptr = '\0';}
			printf("%s \n",tmp);
			if ( (strchr(tmp, ':'))!=NULL){						/* se tmp contiene ':' */
				strcpy(tmp_ip21, (strchr(tmp, ':')));			/* copia quello che c'è dai : in poi in tmp_ip21, i : compresi*/
				if( (ptr = strchr(tmp_ip21, ':')) != NULL){		/* sostituisco i : con 0 nella stringa tmp_ip21 */
					*ptr = '0';}
				n=atoi(tmp_ip21);								/* converto la stringa tmp_ip21 in intero */
				}
		}
		
		/* Blocco BRIDGE */
		if ( (ch!='e') && (ch!='>') && (ris=strncmp(tmp,Br,6))==0){
			n_br=n;
			for (x=1; x<=n_br; x++){							/* alloco n bridge */
				b[x]=(BRIDGE *) malloc(sizeof(BRIDGE));
				if (b[x]==NULL) printf(_KRED "ERRORE MALLOC BRIDGE \n" _KNRM);
				b[x]->id=x;										/* inizializzo gli id dei bridge*/ 
				b[x]->n_port=0;
				b[x]->state_l=1;
				b[x]->state_r=1;
				b[x]->num_lan=MAX_NUM_LAN-1;
				for (q=0; q<MAX_NUM_LAN; q++){
					b[x]->port_lan[q]=0;
					b[x]->port_br_local[q]=0;
					b[x]->sock_fd_local[q]=-1;}
				printf("Bridge[%d]: %d \n",x,b[x]->id);
			}
			arg_return0->n_br=n_br;
			
		}
		
		/* Blocco LAN */
		else if( (ch!='\n') && (ch!='e') && (strncmp(tmp,Ln,3)==0) ){
			n_lan=n;
			for (x=1; x<=n_lan; x++){
				/* alloco n lan*/
				l[x]=(LAN *) malloc(sizeof(LAN));
				if (b[x]==NULL) printf(_KRED "ERRORE MALLOC LAN \n" _KNRM);
				l[x]->id=x;										/*inizializzo gli id delle lan */
				l[x]->n_port=0;
				l[x]->num_br=MAX_NUM_BRIDGE-1;
				printf("Lan[%d]: %d ",x,l[x]->id);
				fgets(tmp_ip,BUFF_CHAR,fp);						/* prendo la stringa */ 
				if( (ptr = strchr(tmp_ip, '\n')) != NULL){		/* questo sostituisce \n con \0 nella stringa tmp_ip2 */
					*ptr = '\0';}
				strncpy(string_local_ip_address,tmp_ip, 99);	/* copio la stringa in string_ip_local_address */
				
				l[x]->IP=string_local_ip_address;				/* salvo dentro la struct l[] */
				for (q=0; q<MAX_NUM_BRIDGE; q++){
					l[x]->l_port_in[q]=0;
					l[x]->l_port_br[q]=0;
					l[x]->sock_fd_local[q]=-1;}
				printf("IP: %s\n",l[x]->IP);
			}
			arg_return0->n_lan=n_lan;
		}
		
		/* Blocco LINKS */
		else if( (ch!='\n') && (ch!='>') && (strncmp(tmp,Lk,4)==0) ){
			if( (ch != 'e') && (ch != '-') && (ch != ' ') ){
				/* LAN */
				if (ch=='L'){										/* se LAN */
					fscanf(fp,"%d", &z);							/* prendo il numero della LAN */
					ch1=fgetc(fp);									/* prendo il carattere successivo */
					if (ch1==':'){
						fscanf(fp,"%d", &z1);}						/* prendo il numero della porta, lo metto in z1 int */
					l[z]->l_port_in[l[z]->n_port]=z1;		/* salvo in port[n_port] (inizialmente n_port è 0) */
					l[z]->n_port = (l[z]->n_port + 1);		/* incremento il numero delle connessioni */
					printf("(Log lan:%d port:%d) -" , z, z1);
					ch1=fgetc(fp);							
					ch1=fgetc(fp);									/* vado avanti con i caratteri*/
					/* BRIDGE */
					if (ch1=='B'){									/* se Bridge */
						fscanf(fp,"%d", &x);						/* prendo numero bridge */
						/* mi occupo di salavare l'id del br a cui mi voglio connettere, non la porta.
						poi gestisco tutte le connessioni con la select */
						l[z]->br_id[(l[z]->n_port)]=x;
						b[x]->n_port = (b[x]->n_port + 1);
						b[x]->port_lan[z]=z1;
						printf(" (Log bridge:%d) \n" , l[z]->br_id[(l[z]->n_port)]);
					}
				}
			}
		}
	}
	fclose(fp);
	
	
	memcpy(arg_return0->b, b, (n_br+1)*(sizeof(BRIDGE*)));
	memcpy(arg_return0->l, l, (n_lan+1)*(sizeof(LAN*)));
	
	
}
