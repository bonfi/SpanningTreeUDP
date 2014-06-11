#include "lib.h"
#include "extern.h"


/* void *PrintHello(void *p_index)
{
	//Glob+=3;
	sleep(1);
	printf("\n index %d: Hello World! Glob=%d\n", *((int*)p_index), Glob);
	free(p_index);
	pthread_exit (NULL);
} */

void *create_br(void *parametri){
	pthread_mutex_lock (&mutex);																			
	char					tipo='B';
	int 					lan;
	int						cont=0;
	struct sockaddr_in		From;
	char					string_remote_ip_address[100];
	unsigned short int		remote_port_number, local_port_number;
	int						msglen;
	short int				socketfd, sock_fd_tmp, p, x, n, ris, br, root_msg, sock_fd_br[10];
	unsigned int			Fromlen;
	char					*msg;
	unsigned short int		required_remote_port_number;
	char					string_required_remote_ip_address[100];
	
	int						fdmax;
	fd_set					write_fd_set, read_fd_set, service_fd_set;
	
	BRIDGE *param = (BRIDGE *)parametri;
	if (DEBUG) printf("sono il thread/bridge: %d e ho: %d link", (param->id), param->n_port);
	
	FD_ZERO(&write_fd_set);
	FD_ZERO(&read_fd_set);
	FD_ZERO(&service_fd_set);
	
	local_port_number = port_br[param->id];
	
	socketfd=create_socket(local_port_number);
	
	/* mi salvo il socket di ascolto sulla porta di default */
	param->sock_fd=socketfd;
	/* aggiungo il socket default nel "set di ascolto" / "insieme di fd di ascolto" della select */
	/* FD_SET(socketfd,&write_fd_set); */ /* pensavo di scrivere subito i messaggi */
	FD_SET(socketfd,&service_fd_set);
	fdmax=socketfd+1;
	
	/*setto la grandezza del messaggio da ricevere con la recvfrom */
	if((msg=malloc(sizeof(char)*(SIZEBUF)))==NULL){
		printf("malloc() failed [err %d] ", errno);
		pthread_exit (NULL);}
	pthread_mutex_unlock (&mutex);																		
	
	
	
	for(;;){
		/* resto in ascolto sulla porta port_br[(param->id)] di default per ogni bridge, all'inizio */
		/* copio il set di ascolto dei fd dei socket */
		read_fd_set=service_fd_set;
		
		ris=select(fdmax,&read_fd_set,NULL,NULL,NULL);
		if(ris<0){
			if (errno!=EINTR){
				printf(_KRED "Error in select: errno diverso da EINTR \n");
			}
		}if(ris==-1){ printf(_KRED "Error in select \n");}
		/* se ris>0 ho ricevuto qualcosa in read_fd_set */
		if (ris>0){
			
			/* ho ricevuto un msg nel socket_br principale? */
			if (FD_ISSET(param->sock_fd,&read_fd_set)){
			/* ----------------------------------------------------------------------------------------------------*/
				/* setup datagram da ricevere,salvare */
				memset(&From, 0, sizeof(From));
				Fromlen=sizeof(struct sockaddr_in);
				
				/* RECVFROM */
				msglen = recvfrom (param->sock_fd, msg, (int)SIZEBUF, 0, (struct sockaddr*)&From, &Fromlen);
				if (msglen<0){
					char msgerror[1024];
					printf(_KRED "ERRORE recvfrom del br: %d\n" _KNRM, param->id);
					sprintf(msgerror,"recvfrom() failed [err %d] ", errno);
					perror(msgerror);
					/* return(1); */
				}else{
					sprintf((char*)string_remote_ip_address,"%s",inet_ntoa(From.sin_addr));
					remote_port_number = ntohs(From.sin_port);
					stampa_pacchetto_ricevuto(msg, param->id, remote_port_number, tipo, param->sock_fd);
			/* ------------------------------------------------------------------------------------------------------*/
					/* analizzo il messaggio ricevuto: è di tipo setup_link ? */
					if (quale_tipo_msg(msg)!=1) {printf("Errore:nella porta deafult_br ricevo solo msg di setup_link\n");}
					else{
						/* analizzo il messaggio ricevuto per capire che lan e' */
						lan=quale_lan(msg);
						if(lan == -1){ printf("Errore nel messaggio: LAN non valido\n");}
						else {
							local_port_number = port_br[param->id] + lan;
							param->port_br_local[lan]=local_port_number;
							if (param->port_lan[lan] != remote_port_number){
								printf(_KRED "Errore setup link: (porta lan %d::Num lan %d) non coincide \n" _KNRM,param->port_lan[lan], lan );
							}else{
								/* creo un nuovo socket per comunicare con la nuova lan */
								sock_fd_tmp = create_socket(local_port_number);
								
								param->sock_fd_local[lan]=sock_fd_tmp;
								/* aggiungo il socket default e il socket appena creato nel "set di ascolto" della select */
								FD_SET(sock_fd_tmp,&service_fd_set);
								
								if (sock_fd_tmp > (fdmax-1)){
									fdmax=sock_fd_tmp + 1;
								}
							/* -----------------------------------------------------------------------------------*/
								/* comunico alla lan in quale porta mi deve trasmettere i successivi messaggi,
								la lan dovra' cambiare porta per comunicare attraverso il bridge */
							/* -----------------------------------------------------------------------------------*/
								/* creo il messaggio da spedire*/
								msg=risp_msg_port(local_port_number);
								
								send_msg(sock_fd_tmp, remote_port_number, msg, param->id, tipo);
								/*stampo tabella del bridge perchè modificata */
								stampa_tabella(param);
								/*pthread_cond_signal(&cond);*/
								cont++;
								
								if(cont == param->n_port){
									pthread_mutex_lock (&mutex);
									/* aspetto il segnale dal main */
									pthread_cond_wait(&cond, &mutex);
									
									msg=msg_setup_root_br(param->id, param->state_r);
									
									for (x=1; x<=(MAX_NUM_LAN-1); x++){
										if (param->port_lan[x]!=NULL){
											send_msg(param->sock_fd_local[x], param->port_lan[x], msg, param->id, tipo);
											}
										}
									pthread_mutex_unlock (&mutex);
								}
							}
						}
					}
				} /* fine gestione recizione messaggio su socket deafaul bridge 
					e creazione nuovo socket per la nuova lan connessa */
			/* ------------------------------------------------------------------------------------ */
			/* se non ho ricevuto msg sul socket default, in che socket ho ricevuto qualcosa? */
			}else{
				/*numero di socket che hanno ricevuto qualcosa */
				n=0;
				if (DEBUG){printf("ricevuto msg dal bridge: %d\n", param->id);}
				for(p=0; p<=fdmax; p++){
																										
					if((p!=param->sock_fd) && ((FD_ISSET(p,&read_fd_set))!=0) ){
						/* il socket_fd p e' gia' stato creato */
						for( x=1; x<=(MAX_NUM_LAN-1); x++){
							if( p == param->sock_fd_local[x] ){
								
								/* setup datagram da ricevere,salvare */
								memset(&From, 0, sizeof(From));
								Fromlen=sizeof(struct sockaddr_in);
								
								/* RECVFROM */
								msglen = recvfrom ( param->sock_fd_local[x], msg, (int)SIZEBUF, 0, (struct sockaddr*)&From, &Fromlen);
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
								n=n+1;
								/* il messaggio ricevuto è di tipo 'setup_root_br' */
								if (quale_tipo_msg(msg)==2){
									br=quale_bridge(msg);
									if (DEBUG){
										printf("sono il br: %d - il messaggio proviene dal br: %d\n", param->id, br);
										}
									root_msg=stato_br_root(msg);
									if (root_msg==1 && param->state_r==1){
										if (br < (param->id)){
											param->state_r=0;
											printf(_KCYN"sono il br: %d e non sono più root \n" _KNRM);
										}
									}
								}
							}
							if(n==ris){ break;}
						}
					}
																											
				}
			}	/* fine else {} */
		}	/* fine gestione ricezione messaggio su socket appartenente a insieme read_fd_set */
	}	/* fine cliclo infinito for */
}	/* fine funzione principale thread br */


