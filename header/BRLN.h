
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
	/* indica bridge attivo o spento */
	int						state_l;
	/* indica stato root del bridge */
	int						state_r;
	
} BRIDGE;

typedef struct LAN{
	char 				*IP;
	short int 			id;
	/* numero di connessioni con i bridge */
	unsigned short int 	n_port;
	/* porte socket delle lan */
	unsigned short int 	l_port_in[MAX_NUM_BRIDGE];
	/* porte socket dei bridge */
	unsigned short int 	l_port_br[MAX_NUM_BRIDGE];
	/* socket_fd dei socket della lan */
	short int 			sock_fd_local[MAX_NUM_LAN];
	/* id dei bridge a cui la lan è connessa */
	int 				br_id[10];
	
} LAN;

