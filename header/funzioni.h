int quale_lan(char *msg);

char *risp_msg_port(int porta);

char *msg_setup_link(int id);

int crea_socket(unsigned short int porta);

void stampa_pacchetto_ricevuto(char *msg, int id_dispositivo, int port, char tipo);

void stampa_pacchetto_trasmesso(char *msg, int id_dispositivo, int port, char tipo);

void invia_msg(short int socket_fd,unsigned short int porta, char *msg);

