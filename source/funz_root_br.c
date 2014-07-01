#include "lib.h"
#include "extern.h"

/* prototipo del messaggio :
tipo_msg:setup_root_br:Sono il bridge:2:root:1 */

/* crea la stringa di */
char *msg_setup_root_br(int id, int root){
	/* printf(_KCYN "id passato a funz : %d \n" _KNRM,id); */
	char	*mess;
	char	*type_msg="tipo_msg:setup_root_br:";
	char	*frase1="Sono il bridge:";
	char	*frase2=":root:";
	char	id_br[5];
	char	stato[2];
	sprintf(stato,"%d",root);
	sprintf(id_br, "%d", id);
	sleep(1);
	mess=(char*)malloc(sizeof(char)*(strlen(frase1)+strlen(frase2)+strlen(id_br)+strlen(type_msg)+strlen(stato)+2));
	strcpy(mess, type_msg);
	strcat(mess, frase1);
	strcat(mess, id_br);
	strcat(mess, frase2);
	strcat(mess, stato);
	strcat(mess, "\0");
	
	return mess;
}

int stato_br_root(char *msg){
	int				stato;
	char 			*ptr,*result;
	const char 		*s = ":";
	char			*str="root";
	
	ptr=malloc(sizeof(char)*(strlen(msg)+2));
	memcpy(ptr, msg,((strlen(msg))+1));
	result = strtok(ptr , s);
	while (strcmp(result,str)){
		result = strtok(NULL , s);
		}
	result = strtok(NULL , s);
	stato=atoi(result);
	return stato;
}
