#include "lib.h"
#include "extern.h"

/* spanning_tree procedure*/

/* crea la stringa di config spanning tree protocol per i bridge */
char *msg_cfg_stp(int id_root, int dist, int id_mitt){
	/* printf(_KCYN "id passato a funz : %d \n" _KNRM,id); */
	char	*mess;
	char	*type_msg="tipo_msg:spanning_tree:";
	char	*duepunti=":";
	char	id_br[5];
	char	id_br_mitt[5];
	char	c_dist[5];
	
	sprintf(id_br, "%d", id_root);
	
	sprintf(id_br_mitt, "%d", id_mitt);
	
	sprintf(c_dist, "%d", dist);
	sleep(1);
	
	mess=(char*)malloc(sizeof(char)*(strlen(duepunti)+strlen(id_br)+strlen(id_br_mitt)+strlen(c_dist)+strlen(type_msg)+2));
	strcpy(mess, type_msg);
	strcat(mess,id_br);
	strcat(mess,duepunti);
	strcat(mess,c_dist);
	strcat(mess,duepunti);
	strcat(mess,id_br_mitt);
	strcat(mess, "\0");
	
	return mess;
}

/*cerca il  bridge mittente del massaggio */
int mit_bridge(char *msg){
	int				br, x;
	char 			*ptr,*result;
	const char 		*s = ":";
	
	ptr=malloc(sizeof(char)*(strlen(msg)+2));
	memcpy(ptr, msg,((strlen(msg))+1));
	result = strtok(ptr , s);
	for (x = 0 ; x<4 ; x++){
		result = strtok(NULL , s);
		/*printf("result: %s , ciclo x=%d \n",result, x);*/
		}
	br=atoi(result);
	return br;
}

int id_temp_root_br(char *msg){
	int				br, x;
	char 			*ptr,*result;
	const char 		*s = ":";
	
	ptr=malloc(sizeof(char)*(strlen(msg)+2));
	memcpy(ptr, msg,((strlen(msg))+1));
	result = strtok(ptr , s);
	for (x = 0 ; x<2 ; x++){
		result = strtok(NULL , s);
		}
	br=atoi(result);
	return br;
}
