#include "lib.h"
#include "extern.h"

void *create_lan(void *parametri){
	char					tipo='L';
	int 					br_id;
	struct sockaddr_in		From;
	char					string_remote_ip_address[100];
	unsigned short int		remote_port_number, local_port_number;
	unsigned short int		msglen;
	short int				sock_fd_tmp, p, x, x1, br, ris, sock_fd_br[10];
	short int 				socketfd;
	unsigned int			Fromlen;
	char					*msg;
	int						fdmax;
	fd_set					write_fd_set, read_fd_set, service_fd_set;
	
	pthread_mutex_lock (&mutex);																
	/*setto la grandezza del messaggio da ricevere con la recvfrom */
	/*printf("malloc dentro il thread lan\n");*/
	
	if((msg=malloc(sizeof(char)*(SIZEBUF)))==NULL){
		printf("malloc() failed [err %d] ", errno);
		pthread_exit (NULL);}
	
	LAN *param = (LAN *)parametri;
	pthread_mutex_unlock (&mutex);																
	if (DEBUG) printf("sono il thread/LAN: %d e ho: %d link\n", (param->id), param->n_port);
	
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
			param->sock_fd_local[param->br_id[x+1]]=socketfd;
			
			/* aggiungo il socket default nel "set di ascolto" / "insieme di fd di ascolto" della select */
			FD_SET(socketfd,&service_fd_set);
			
			if (socketfd > (fdmax-1)){
				fdmax=socketfd + 1;}
			/* mi collego al rispettivo bridge, creando prima il messaggio di setup_connection */
			msg=msg_setup_link(param->id);
			if (DEBUG){
				printf("id LAN è : %d \n",param->id); 
				printf("id BR destinazione: %d \n", param->br_id[x+1]);}
			/*  e poi spedendo il messaggio */
			send_msg((param->sock_fd_local[param->br_id[x+1]]), port_br[param->br_id[x+1]], msg, param->id, tipo);
			sleep(1);
			pthread_mutex_unlock (&mutex);														
			
			if (DEBUG) printf("LAN: %d inizio ricezione pacchetti setup link #: %d\n",param->id, x);
			
			/* setup datagram da ricevere,salvare */
			memset(&From, 0, sizeof(From));
			Fromlen=sizeof(struct sockaddr_in);
			
			/* RECVFROM */
			msglen = recvfrom ( (param->sock_fd_local[param->br_id[x+1]]), msg, (int)SIZEBUF, 0, (struct sockaddr*)&From, &Fromlen);
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
			ris = quale_porta(msg);
			if (ris==-1){ printf("Errore nel messaggio: porta non valida \n");}
			else{ param->l_port_br[param->br_id[x+1]]=ris;}
		}
	}
	
	for(;;){
		/* resto in ascolto sui socket creati */
		read_fd_set=service_fd_set;
		
		ris=select(fdmax,&read_fd_set,NULL,NULL,NULL);
		
		if(ris<0){
			if (errno!=EINTR){
				printf(_KRED "Error in select: errno diverso da EINTR \n" _KNRM);
			}
		}
		/* se ris>0 ho ricevuto qualcosa in read_fd_set */
		if (ris>0){
			/* in che socket ho ricevuto qualcosa? */
			for(p=0; p<fdmax; p++){
				
				if((FD_ISSET(p,&read_fd_set))!=0){
					/* il socket_fd p e' gia' stato creato */
					for( x=0; x<(MAX_NUM_BRIDGE-1); x++){
						if( p == param->sock_fd_local[x]){
							/* setup datagram da ricevere,salvare */
							memset(&From, 0, sizeof(From));
							Fromlen=sizeof(struct sockaddr_in);
							
							/* RECVFROM */
							msglen = recvfrom (param->sock_fd_local[x], msg, (int)SIZEBUF, 0, (struct sockaddr*)&From, &Fromlen);
							if (msglen<0){
								char msgerror[1024];
								sprintf(msgerror,"recvfrom() failed [err %d] ", errno);
								perror(msgerror); 
								/* return(1); */
							}else{sprintf((char*)string_remote_ip_address,"%s",inet_ntoa(From.sin_addr));
								remote_port_number = ntohs(From.sin_port);
								if (DBG_MSG_UDP) stampa_pacchetto_ricevuto(msg, param->id, remote_port_number, tipo, param->sock_fd_local[x]);
								
								/*pthread_mutex_lock (&mutex);														*/
								/* il messaggio ricevuto è di tipo 'funz_spanning_tree' */
								if (quale_tipo_msg(msg)==2){
									br=mit_bridge(msg);
									
									x1=0;
									while (param->br_id[x1]!=br){
										x1++;}
									if (DEBUG){
										printf(_KYEL"sono la lan: %d - il messaggio proviene dal br: %d\n", param->id, br);
										printf(_KYEL"quindi dalla porta: %d \n"_KNRM,param->l_port_br[param->br_id[x1]]);}
									for (ris=0; ris<=(param->num_br); ris++){
										if ((param->l_port_br[ris]!=0) && (param->l_port_br[ris]!=remote_port_number) && (param->sock_fd_local[ris]!=-1)){
											send_msg(param->sock_fd_local[ris], param->l_port_br[ris], msg, param->id, tipo);
										}
									}
								}
								if (quale_tipo_msg(msg)==3){
									x1=is_msg_closeudp(msg);
									
									if (x1==1){
										if (param->n_port==1) {printf("lan: %d - NON chiudo l'ultima connessione a un bridge\n",param->id);}
										else {
											printf("lan: %d -- IN TEORIA DOVREI CHIUDERE LA CONNESSIONE CON:%d \n",param->id, remote_port_number);
											sleep(1);
											for (ris=0; ris<=(param->num_br); ris++){
												if ((param->l_port_br[ris]!=0) && (param->l_port_br[ris]==remote_port_number)){
													param->l_port_br[ris]=0;
													FD_CLR(param->sock_fd_local[ris],&service_fd_set);
													param->sock_fd_local[ris]=-1;
													fdmax--;
													param->n_port--;
												}
											}
										}
									}
								}
								/*pthread_mutex_unlock (&mutex);														*/
							}
						}
					}
				}
			}
		}
			/* fine gestione ricezione messaggio su socket appartenente a insieme read_fd_set */
	}		/* fine cliclo infinito for */
}			/* fine funzione principale thread br */

