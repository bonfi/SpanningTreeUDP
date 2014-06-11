#include "lib.h"
#include "extern.h"

/* funzione che trova il numero che c'è dopo i : in una stringa
   restituisce l'intero */
short int quale_lan(char *msg){
	char 			*ptr;
	int 			n;
	const char 		*s = ":";
	char			*str="Ciao, sono la lan";
	ptr = strtok(msg , s);
	while (strcmp(ptr,str)){
		ptr = strtok(NULL , s);
		}
	ptr = strtok(NULL , s);
	n=atoi(ptr);
	/*printf("n = %d \n",n);*/
	return n;
}

/*stesso funzionamento della funzione quale_lan*/
short int quale_porta(char *msg){
	char 			*ptr;
	int 			n;
	const char 		*s = ":";
	char			*str="Dobbiamo comunicare sulla porta";
	ptr = strtok(msg , s);
	while (strcmp(ptr,str)){
		ptr = strtok(NULL , s);
		}
	ptr = strtok(NULL , s);
	n=atoi(ptr);
	/* printf("%d \n",n); */
	return n;
}

/* crea la stringa di risposta alle lan, comunicandogli quale porta devono usare */
char *risp_msg_port(int porta){
	char	*mess;
	char	*type_msg="tipo_msg:setup_link:";
	char	*risp="Dobbiamo comunicare sulla porta:";
	char	port_msg[5];
	
	sprintf(port_msg, "%d", porta);
	sleep(1);
	mess=(char*)malloc(sizeof(char)*(strlen(risp)+strlen(type_msg)+strlen(port_msg)+2));
	strcpy(mess, type_msg);
	strcat(mess, "msg:");
	strcat(mess,risp);
	strcat(mess, port_msg);
	strcat(mess, "\0");
	return mess;
}

/* crea la stringa di setup_link per le lan da mandare ai bridge */
char *msg_setup_link(int id){
	/* printf(_KCYN "id passato a funz : %d \n" _KNRM,id); */
	char	*mess;
	char	*type_msg="tipo_msg:setup_link:";
	char	*frase="Ciao, sono la lan:";
	char	id_lan[5];
	
	sprintf(id_lan, "%d", id);
	sleep(1);
	mess=(char*)malloc(sizeof(char)*(strlen(frase)+strlen(id_lan)+strlen(type_msg)+2));
	strcpy(mess, type_msg);
	strcat(mess, "msg:");
	strcat(mess,frase);
	strcat(mess, id_lan);
	strcat(mess, "\0");
	
	return mess;
}
