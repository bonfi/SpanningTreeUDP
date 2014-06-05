#include "lib.h"
#include "extern.h"

void *create_lan(void *parametri){
	
	char					tipo='L';
	int 					br_id;
	struct sockaddr_in		From;
	char					string_remote_ip_address[100];
	unsigned short int		remote_port_number, local_port_number;
	int						sock_fd_tmp, p, x, msglen,ris, sock_fd_br[10];
	short int 				socketfd;
	unsigned int			Fromlen;
	char					*msg,*msg2;
	unsigned short int		required_remote_port_number;
	char					string_required_remote_ip_address[100];
	
	int						fdmax;
	fd_set					write_fd_set, read_fd_set, service_fd_set;
	
	/*setto la grandezza del messaggio da ricevere con la recvfrom */
	msg=malloc(sizeof(char)*(SIZEBUF));
	
	LAN *param = (LAN *)parametri;
	if (DEBUG) printf("sono il thread/LAN: %d \n", (param->id));
	
	FD_ZERO(&write_fd_set);
	FD_ZERO(&read_fd_set);
	FD_ZERO(&service_fd_set);
	
	fdmax=0;
	
	/* creo un socket per ogni link con i bridge */
	if (param->n_port != 0){
		
		for (x=0; x<(param->n_port); x++){
			pthread_mutex_lock (&mutex);																
			if (DEBUG) printf("inizio ciclo for per i link LAN//BRIDGE \n");
			
			/*acquisisco la porta in input che deve usare la lan */
			local_port_number = param->l_port_in[x];
			/* creo il socket */
			socketfd=create_socket(local_port_number);
			
			/* salvo il fd sel socket creato */
			param->sock_fd_local[x]=socketfd;
			
			/* aggiungo il socket default nel "set di ascolto" / "insieme di fd di ascolto" della select */
			FD_SET(socketfd,&service_fd_set);
			
			if (socketfd > (fdmax-1)){
				fdmax=socketfd + 1;
			}
			/* mi collego al rispettivo bridge, creando prima il messaggio di setup_connection */
			msg=msg_setup_link(param->id);
			if (DEBUG){
				printf("id LAN è : %d \n",param->id); 
				printf("id BR destinazione: %d \n", param->br_id[x+1]);}
			/*  e poi spedendo il messaggio */
			send_msg(socketfd, port_br[param->br_id[x+1]], msg, param->id, tipo);
			//sleep(1);
			pthread_mutex_unlock (&mutex);														
			
			if (DEBUG) printf("LAN: %d inizio ricezione pacchetti setup link #: %d\n",param->id, x);
			
			/* setup datagram da ricevere,salvare */
			memset(&From, 0, sizeof(From));
			Fromlen=sizeof(struct sockaddr_in);
			
			/*pthread_mutex_lock (&mutex);															
			pthread_cond_wait (&cond,&mutex);*/
			/* RECVFROM */
			msglen = recvfrom ( socketfd, msg, (int)SIZEBUF, 0, (struct sockaddr*)&From, &Fromlen);
			if (msglen<0){
				char msgerror[1024];
				sprintf(msgerror,"recvfrom() failed [err %d] ", errno);
				perror(msgerror); 
				/* return(1); */
			}else{
				sprintf((char*)string_remote_ip_address,"%s",inet_ntoa(From.sin_addr));
				remote_port_number = ntohs(From.sin_port);
				stampa_pacchetto_ricevuto(msg, param->id, remote_port_number, tipo, param->sock_fd_local[x]);
			}
			ris = quale_lan(msg);
			if (ris==-1){ printf("Errore nel messaggio: porta non valida \n");}
			else{ param->l_port_br[ris]=ris;}
			
			/*pthread_mutex_unlock (&mutex);*/														
			
		}
	}
	
	for(;;){
		/* resto in ascolto sui socket creati */
		read_fd_set=service_fd_set;
		
		ris=select(fdmax,&read_fd_set,NULL,NULL,NULL);
		
		pthread_mutex_lock (&mutex);														
		if(ris<0){
			if (errno!=EINTR){
				printf(_KRED "Error in select: errno diverso da EINTR \n");
			}
		}
		/* se ris>0 ho ricevuto qualcosa in read_fd_set */
		if (ris>0){
			
			/* in che socket ho ricevuto qualcosa? */
			for(p=0; p<fdmax; p++){
				
				if((FD_ISSET(p,&read_fd_set))!=0){
					/* il socket_fd p e' gia' stato creato */
					for( x=1; x<5; x++){
						if( p == param->sock_fd_local[x]){
							
							/* setup datagram da ricevere,salvare */
							memset(&From, 0, sizeof(From));
							Fromlen=sizeof(struct sockaddr_in);
							
							/* RECVFROM */
							msglen = recvfrom ( socketfd, msg, (int)SIZEBUF, 0, (struct sockaddr*)&From, &Fromlen);
							if (msglen<0){
								char msgerror[1024];
								sprintf(msgerror,"recvfrom() failed [err %d] ", errno);
								perror(msgerror); 
								/* return(1); */
							}else{
								sprintf((char*)string_remote_ip_address,"%s",inet_ntoa(From.sin_addr));
								remote_port_number = ntohs(From.sin_port);
								stampa_pacchetto_ricevuto(msg, param->id, remote_port_number, tipo, param->sock_fd_local[x]);
								/*printf(_KBLU "ricevuto da socketfd %d , nel thread/LAN: %d , msg: \"%s\" len %d, from host %s, port %d\n", socketfd,
										(param->id + 1), msg, msglen, string_remote_ip_address, remote_port_number);*/
							}
						}
					}
				}
			}
		}pthread_mutex_unlock (&mutex);														
			/* fine gestione ricezione messaggio su socket appartenente a insieme read_fd_set */
	}		/* fine cliclo infinito for */
}			/* fine funzione principale thread br */
