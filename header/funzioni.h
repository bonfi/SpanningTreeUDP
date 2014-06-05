short int quale_lan(char *msg);

char *risp_msg_port(int porta);

char *msg_setup_link(int id);

int create_socket(unsigned short int porta);

void stampa_pacchetto_ricevuto(char *msg, int id_dispositivo, int port, char tipo, short int socket);

void stampa_pacchetto_trasmesso(char *msg, int id_dispositivo, int port, char tipo, short int socket);

void send_msg(short int socket_fd,unsigned short int porta, char *msg, int id_disp, char tipo_disp);

void stampa_tabella(BRIDGE *br);
