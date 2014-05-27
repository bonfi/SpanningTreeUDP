#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "BRLN.h"

#define config_txt "Config_v2.txt"
 
int main()
{
	int BUFF_CHAR=20, NUM_MAX=20;
	//interi d'appoggio per cicli
	int i=0;
	int z,ris,n,x;
	char *ptr,*ptr1;
	char ch, ch1, tmp1[BUFF_CHAR], tmp[BUFF_CHAR], tmp_ip[BUFF_CHAR], tmp_ip2[BUFF_CHAR], tmp_str[BUFF_CHAR], ch2[2] , tmp_ip21[BUFF_CHAR];
	char Br[]="BRIDGE";
	char Ln[]="LAN";
	char Lk[]="LINK";
	char ip[]="P->";
	char string_local_ip_address[100];					//ip delle lan
	FILE *fp, *fl, *ftmp;
	
	LAN *l[NUM_MAX];
	BRIDGE *b[NUM_MAX];
	/* b=(BRIDGE *) malloc(sizeof(BRIDGE));
	l1=(*/
	
	fp = fopen(config_txt,"r"); // read mode
	
	if( fp == NULL ){
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}
	fl =fp;
	ftmp=fp;
	
	printf("The contents of %s file are:\n", config_txt);
	
	/* fscanf(fl,"%d", &z);	*/								//prendo ogni carattere come fosse intero e lo salvo in z
	while( ( ch = fgetc(fp) ) != EOF ){						//scorre tutti i caratteri del file fino alla fine, salvo in ch il carattere uno alla volta
		
		if(ch == '>'){
			fgets(tmp,BUFF_CHAR,fp);						//prende una stringa per capire a che blocco siamo, Bridge, Lan o Link
			if( (ptr = strchr(tmp, '\n')) != NULL){			//questo rimuove \n dalla stringa tmp
				*ptr = '\0';}
			printf("%s \n",tmp);
			if ( (strchr(tmp, ':'))!=NULL){
				strcpy(tmp_ip21, (strchr(tmp, ':')));
				if( (ptr = strchr(tmp_ip21, ':')) != NULL){		//questo rimuove \n dalla stringa tmp
					*ptr = '0';}
				n=atoi(tmp_ip21);
				printf("%d \n",n);}
		}
		
		if ( ch!='e' && (ch!='>') && (ris=strncmp(tmp,Br,6))==0){		//blocco BRIDGE
		
			for (x=0; x!=n; x++){							/* alloco n bridge */
				b[x]=(BRIDGE *) malloc(sizeof(BRIDGE));
				b[x]->id=x;									/* inizializzo gli id dei bridge*/ 
				printf("Bridge[%d]: %d \n",x,b[x]->id);
			}
		}
		else if( ch!='\n' && ch!='e' && (ch!='>') && strncmp(tmp,Ln,3)==0 ){		// Blocco LAN
			for (x=0; x!=n; x++)
			{	/* alloco n lan*/
				l[x]=(LAN *) malloc(sizeof(LAN)); 
				l[x]->id=x;										//inizializzo gli id delle lan
				printf("Lan[%d]: %d ",x,l[x]->id);
				fgets(tmp_ip,BUFF_CHAR,fp);
				// strcpy(tmp_ip2, (strchr(tmp_ip, '1')));
				if( (ptr = strchr(tmp_ip, '\n')) != NULL){		//questo sostituisce \n con \0 nella stringa tmp_ip2
					*ptr = '\0';}
				strncpy(string_local_ip_address,tmp_ip, 99);	/* copio la stringa in string_ip_local_address */
				l[x]->IP=string_local_ip_address;				/* salvo dentro la struct l[] */
				printf("IP: %s\n",l[x]->IP);
			}
		}
		 else if( ch!='\n' && (ch!='>') && strncmp(tmp,Lk,4)==0 ){		//blocco LINK: Da FARE
			if( ch != 'e' && ch != '-' && ch != ' '){
				
				printf("(Log link %d) \n" , z);
				}
				
			}
			
		/* fscanf(fl,"%d", &z);*/								//prendo ogni carattere come fosse intero e lo salvo in z
    }
	printf("\n");
	fclose(fp);
	return 0;
}
