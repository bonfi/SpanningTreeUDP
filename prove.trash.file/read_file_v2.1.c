#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "BRLN.h"

#define config_txt "Config_v2.txt"
 
int main(){
	
	int BUFF_CHAR=20, NUM_MAX=20;
	/* interi d'appoggio per cicli */
	int z, x, z1, ris,n, n_lan, n_br ;

	char *ptr;
	char ch,ch1, tmp[BUFF_CHAR], tmp_ip[BUFF_CHAR], tmp_ip21[BUFF_CHAR];
	char Br[]="BRIDGE";
	char Ln[]="LAN";
	char Lk[]="LINK";
	char string_local_ip_address[100];					/* ip delle lan*/
	FILE *fp;
	
	LAN *l[NUM_MAX];
	BRIDGE *b[NUM_MAX];
	
	fp = fopen(config_txt,"r");			/* apre il file in read mode */
	
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
				printf("%d \n",n);}
		}
		/* Blocco BRIDGE */
		if ( ch!='e' && (ch!='>') && (ris=strncmp(tmp,Br,6))==0){
			n_br=n;
			for (x=0; x!=n_br; x++){							/* alloco n bridge */
				b[x]=(BRIDGE *) malloc(sizeof(BRIDGE));
				b[x]->id=x;										/* inizializzo gli id dei bridge*/ 
				b[x]->n_port=0;
				printf("Bridge[%d]: %d \n",x,b[x]->id);
			}
		}
		/* Blocco LAN */
		else if( ch!='\n' && ch!='e' && strncmp(tmp,Ln,3)==0 ){
			n_lan=n;
			for (x=0; x!=n_lan; x++){
				/* alloco n lan*/
				l[x]=(LAN *) malloc(sizeof(LAN)); 
				l[x]->id=x;										/*inizializzo gli id delle lan */
				l[x]->n_port=0;
				printf("Lan[%d]: %d ",x,l[x]->id);
				fgets(tmp_ip,BUFF_CHAR,fp);						/* prendo la stringa, perche salta il primo 1 degli ip??? */ 
				if( (ptr = strchr(tmp_ip, '\n')) != NULL){		/* questo sostituisce \n con \0 nella stringa tmp_ip2 */
					*ptr = '\0';}
				strncpy(string_local_ip_address,tmp_ip, 99);	/* copio la stringa in string_ip_local_address */
				l[x]->IP=string_local_ip_address;				/* salvo dentro la struct l[] */
				printf("IP: %s\n",l[x]->IP);
			}
		}
		/* Blocco LINKS */
		else if( ch!='\n' && (ch!='>') && strncmp(tmp,Lk,4)==0 ){
			if( ch != 'e' && ch != '-' && ch != ' '){
				/* LAN */
				if (ch=='L'){										/* se LAN */
					fscanf(fp,"%d", &z);							/* prendo il numero della LAN */
					ch1=fgetc(fp);									/* prendo il carattere successivo */
					if (ch1==':'){
						fscanf(fp,"%d", &z1);}						/* prendo il numero della porta, lo metto in z1 int */
					l[(z-1)]->port_in[(l[z-1]->n_port)]=z1;			/* salvo in port[n_port] (inizialmente n_port è 0) */
					l[(z-1)]->n_port = (l[(z-1)]->n_port + 1);
					printf("(Log lan:%d port:%d) -" , (z-1), z1);
					ch1=fgetc(fp);							
					ch1=fgetc(fp);									/* vado avanti con i caratteri*/
					/* BRIDGE */
					if (ch1=='B'){									/* se Bridge */
						fscanf(fp,"%d", &z);						/* prendo numero bridge */
						ch1=fgetc(fp);								/* prendo il carattere successivo */
						if (ch1==':'){	
							fscanf(fp,"%d", &z1);}					/* prendo il numero della porta */
						b[(z-1)]->port_in[(b[(z-1)]->n_port)]=z1;	/* salvo in port[y] (inizialmente 0) */
						b[(z-1)]->n_port = (b[(z-1)]->n_port + 1);
						printf(" (Log bridge:%d port:%d) \n" , (z-1), z1);
					}
				}
			}
		}
	}
	printf("\n");
	fclose(fp);
	return 0;
}
