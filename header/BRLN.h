
typedef struct BRIDGE{
	short int 				id;
	/* numero di connessioni con le lan */
	unsigned short int 		n_port;
	/* fd del socket locale per comunicare con le lan */
	short int 				sock_fd_local[MAX_NUM_LAN];
	/* porta udp locale per i socket che comuincano con le lan */
	unsigned short int 		port_br_local[MAX_NUM_LAN];
	/* array di puntatori a stringhe IP */
	char 					ip_lan[MAX_NUM_LAN];
	/* porte delle lan a cui i br sono connessi */
	unsigned short int 		port_lan[MAX_NUM_LAN];
	/* socket default di ascolto */
	short int 				sock_fd;
	
} BRIDGE;

typedef struct LAN{
	char 				*IP;
	short int 			id;
	unsigned short int 	n_port;
	unsigned short int 	l_port_in[MAX_NUM_BRIDGE];
	unsigned short int 	l_port_br[MAX_NUM_BRIDGE];
	short int 			sock_fd_local[MAX_NUM_LAN];
	int 				br_id[MAX_NUM_BRIDGE];		/* bridge a cui si collega la lan */
	
} LAN;
