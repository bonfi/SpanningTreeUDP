int quale_tipo_msg(char *msg);

int create_socket(short int porta);

void stampa_pacchetto_ricevuto(char *msg, int id_dispositivo, int port, char tipo, short int socket);

void stampa_pacchetto_trasmesso(char *msg, int id_dispositivo, int port, char tipo, short int socket);

void send_msg(short int socket_fd, short int porta, char *msg, int id_disp, char tipo_disp);

void stampa_tabella(BRIDGE *br);
