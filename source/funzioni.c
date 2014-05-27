#include "lib.h"
#include "extern.h"

int quale_lan(char *msg){
	//char frase[]="Ciao, sono la lan:";
	char 			temp[10];
	char 			*ptr;
	int 			n;
	if (strchr(msg, ':')!=NULL){					/* il msg ricevuto contiene la stringa frase[]? */
		strcpy(temp, (strchr(msg, ':')));
		if( (ptr = strchr(temp, ':')) != NULL){		/* sostituisco i : con 0 nella stringa temp */
			*ptr = '0';}
		n=atoi(temp);								/* converto la stringa a cui punta temp in poi, in intero */
		/* printf("%d \n",n); */
	}
	return n;
}

char *risp_msg_port(int porta){
	char	*mess;
	char	*risp="Dobbiamo comunicare sulla porta:";
	char	port_msg[5];
	
	sprintf(port_msg, "%d", porta);
	mess=malloc(sizeof(risp)+sizeof(port_msg));
	
	strcpy(mess,risp);
	strcat(mess, port_msg);
	strcat(mess, "\0");
	return mess;
}

char *msg_setup_link(int id){
	/* printf(_KCYN "id passato a funz : %d \n" _KNRM,id); */
	
	char		*mess;
	char		*frase="Ciao, sono la lan:";
	char		id_lan[5];
	
	//strcpy(frase, "Ciao, sono la lan:");
	
	sprintf(id_lan, "%d", id);
	
	mess=malloc(sizeof(frase)+sizeof(id_lan));
	
	strcpy(mess,frase);

	strcat(mess, id_lan);
	strcat(mess, "\0");
	
	return mess;
}


int crea_socket(unsigned short int porta){
	short int				socketfd, ris;
	struct sockaddr_in		Local;
	
	/* get a datagram socket =//= creo il socket sulla porta passata */
	socketfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (socketfd == SOCKET_ERROR) {
		printf (_KRED "socket() failed, Err: %d \"%s\"\n" _KNRM, errno, strerror(errno));
		exit(1);
	}
	
	/* inizializzo il socket con i parametri */
	memset(&Local, 0, sizeof(Local));
	Local.sin_family		=	AF_INET;
	Local.sin_addr.s_addr	=	inet_addr("127.0.0.1");
	Local.sin_port			=	htons(porta);
	
	/* BIND */
	ris = bind(socketfd, (struct sockaddr*) &Local, sizeof(Local));
	if (ris == SOCKET_ERROR){
		printf (_KRED "bind() su porta:%d failed, Err: %d \"%s\"\n" _KNRM ,porta,errno,strerror(errno) );
		exit(1);
	}else {
		printf(_KCYN "Nuovo SOCKET! fd= %d e porta= %d \n" _KNRM, socketfd, porta);
	}
	
	return socketfd;
}

void stampa_pacchetto_ricevuto(char *msg, int id_dispositivo, int port, char tipo, short int socket){
	char			*tipo_dispositivo;
	if (tipo=='B'){
		tipo_dispositivo = "BRIDGE";
	}else if(tipo='L'){
		tipo_dispositivo = "LAN";
	}else{
		printf(_KRED "Errore tipo dispositivo");
		exit(0);
	}
	printf(_KBLU "\nMessaggio ricevuto dal socket %d \n", socket);
	printf(_KBLU "da %s %d dalla porta: %d Messaggio: '%s' ", tipo_dispositivo, id_dispositivo , port, msg);
	printf(_KNRM "\n");

}

void stampa_pacchetto_trasmesso(char *msg, int id_dispositivo, int port, char tipo, short int socket){
	char *tipo_dispositivo;
	if (tipo=='B'){
		tipo_dispositivo = "BRIDGE";
	}else if(tipo='L'){
		tipo_dispositivo = "LAN";
	}else{
		printf(_KRED "Errore tipo dispositivo" _KNRM);
	exit(0);
	}
	printf(_KBLU "Messaggio inviato dal socket %d \n", socket);
	printf(_KBLU "da %s: %d sulla porta: %d Messaggio: '%s' ", tipo_dispositivo, id_dispositivo, port, msg);
	printf(_KNRM "\n");

}

void invia_msg(short int socket_fd, unsigned short int porta, char *msg, int id_disp, char tipo_disp){
	struct sockaddr_in					To;
	char string_remote_ip_address[99]	="127.0.0.1";
	int 								ris, addr_size;
	
	To.sin_family		=	AF_INET;
	To.sin_addr.s_addr	=	inet_addr(string_remote_ip_address);
	To.sin_port		 	=	htons(porta);
	addr_size = sizeof(struct sockaddr_in);
	printf("lunghezza del messaggio spedito è: %d byte\n", strlen(msg)+1);
	/* send to the address */
	ris = sendto(socket_fd, msg, strlen(msg)+1 , 0, (struct sockaddr*)&To, addr_size);
	if (ris < 0) {
		printf ("sendto() failed, Error: %d \"%s\"\n", errno,strerror(errno));
		exit(1);
	}else
		/*printf("datagram UDP \"%s\" sent to (%s:%d)\n",msg, string_remote_ip_address, porta);*/
		stampa_pacchetto_trasmesso(msg, id_disp, porta, tipo_disp, socket_fd);
}

/* da implementare per i bridge

stampa_tabella 

*/
