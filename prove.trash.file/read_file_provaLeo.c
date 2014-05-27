#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "BRLN.h"

#define config_txt "Config.txt"
 
int main()
{
	int BUFF_CHAR=20;
	//interi d'appoggio per cicli
	int i=0;
	int z,ris,n,x;
	char *ptr,*ptr1;
	char ch, ch1, tmp1[BUFF_CHAR], tmp[BUFF_CHAR], tmp_ip[BUFF_CHAR], tmp_ip2[BUFF_CHAR], tmp_str[BUFF_CHAR], ch2[2] ;
	char Br[]="BRIDGE";
	char Ln[]="LAN";
	char Lk[]="LINK";
	char ip[]="P->";
	char string_local_ip_address[100];					//ip delle lan
	FILE *fp, *fl, *ftmp;
	
	LAN *l1, *l2, *l3, *l4;
	BRIDGE *b;
	b=(BRIDGE *) malloc(sizeof(BRIDGE));
	l1=(LAN *) malloc(sizeof(LAN));
	
	fp = fopen(config_txt,"r"); // read mode
	
	if( fp == NULL ){
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}
	fl =fp;
	ftmp=fp;
	
	printf("The contents of %s file are:\n", config_txt);
	
	fscanf(fl,"%d", &z);									//prendo ogni carattere come fosse intero e lo salvo in z
	while( ( ch = fgetc(fp) ) != EOF ){						//scorre tutti i caratteri del file fino alla fine, salvo in ch il carattere uno alla volta
		
		if(ch == '>'){
			fgets(tmp,BUFF_CHAR,fp);						//prende una stringa per capire a che blocco siamo, Bridge, Lan o Link
			if( (ptr = strchr(tmp, '\n')) != NULL){			//questo rimuove \n dalla stringa tmp
				*ptr = '\0';}
			printf("%s \n",tmp);
			if ( (strchr(tmp, ':'))!=NULL){
				strcpy(tmp_ip2, (strchr(tmp, ':')));
				if( (ptr = strchr(tmp_ip2, ':')) != NULL){		//questo rimuove \n dalla stringa tmp
					*ptr = '0';}
				n=atoi(tmp_ip2);
				printf("%d \n",n);}
		}
		
		if ( (ch!='>') && (ris=strncmp(tmp,Br,6))==0){		//blocco BRIDGE: VA BENE!!! bisogna allocare dinamicamente le struct br (array di br?) stessa cosa per le Lan
				if( ch != 'e'){
				/* fgets(ptr,3,fl);								// this method don't work and i don't know
				printf("tmp_ip2 è %s \n",ptr);
				if( (ptr1 = strchr(ptr, '\n')) != NULL){			//questo rimuove \n dalla stringa e mette \0 alla fine
					*ptr1 = '\0';}
				b->id=atoi(ptr); */
				b->id=z;					//salvo z nel campo id i numero del blocco BRIDGE
				printf("Bridge n: %d \n",b->id);
			}
		}
		else if( (ch!='>') && strncmp(tmp,Ln,3)==0 ){		// Blocco LAN: OK
			if( ch != 'e' && ch != ' ')
			{
			//printf("(Log %c)" , ch);
		 	l1->id=z; 										//salvo z nel campo id i numero del blocco LAN
		 	printf("Lan n: %d ",l1->id);
		 	
		 	fgets(tmp_ip,BUFF_CHAR,fp);						//prendo la stringa da fp in poi, prende l'ind ip come stringa ma prende anche \n (non deve!!)
		 	if( (ptr = strchr(tmp_ip, '\n')) != NULL){		//questo sostituisce \n con \0 nella stringa tmp_ip
				*ptr = '\0';}
		 	/* printf("(IP: |%s|) - ch: |%c|",tmp_ip, ch);*/	//non capisco perchè punta a un carattere dopo, infatti stampa P->192.....(sistemato mettendo la stringa ip=P-> (soluzione merdosa)
			}
			if( ch == 'I' &&  strncmp(tmp_ip,ip,3)==0 ){		//la stringa tmp_ip è uguale a P-> ??
				
				/* sposto il puntatore alla primma occorrenza di 1 nella stringa tmp_ip e copio il resto della stringa da lì in poi in tmp_ip2 */
				strcpy(tmp_ip2, (strchr(tmp_ip, '1')));
				if( (ptr = strchr(tmp_ip2, '\n')) != NULL){		//questo sostituisce \n con \0 nella stringa tmp_ip2
					*ptr = '\0';}
				strncpy(string_local_ip_address,tmp_ip2, 99);	/* copio la stringa in string_ip_local_address */
				
				l1->IP=string_local_ip_address;					/* salvo dentro la struct l1 */
				printf("IP: %s\n",l1->IP);
				}
		}
		else if( (ch!='>') && strncmp(tmp,Lk,4)==0 ){		//blocco LINK: Da FARE
			if( ch != 'e' && ch != '-' && ch != ' '){
				
				printf("(Log link %d) \n" , z);
				}
				
			}
		fscanf(fl,"%d", &z);								//prendo ogni carattere come fosse intero e lo salvo in z
    }
	printf("\n");
	fclose(fp);
	return 0;
}
