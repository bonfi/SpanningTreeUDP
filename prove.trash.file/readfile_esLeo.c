#include <stdio.h>
#include <stdlib.h>
#include "BRLN.h"

#define config_txt "Config.txt"
 
int main()
{
   int BUFF_CHAR=10;
   char ch, tmp[BUFF_CHAR], br_id[5][10];
   char Br[]="BRIDGE";
   char Ln[]="LAN";
   FILE *fp, *fl;
   int i=0;
   int z,ris;
   // printf("Enter the name of file you wish to see\n");
   // gets(config_txt);
	
	printf("%s\n",config_txt);
 
   fp = fopen(config_txt,"r"); // read mode
	fl=fp;
   if( fp == NULL )
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }
 
   printf("The contents of %s file are :\n", config_txt);
 
   while( ( ch = fgetc(fp) ) != EOF ){
      if(ch == '('){
	    fgets(tmp,BUFF_CHAR,fp);
	    printf("%s",tmp);
	  }
	  if ( strcmp(tmp,Br)==1 ){
		 printf("è il blocco bridge");
		 
	  }
         if(ch != ')')
      {
            //tmp = ch;
	    //printf("%c",tmp);
	//printf("aaa%c", par1);
            //i=i+1;
         }
         /*tmp[i]='\0';*/
     /* for (z=0; z<i; z++)*/
      	//printf("%s\n",tmp);
      /*printf("%c",ch);*/
   }
	
	printf("\n");
   fclose(fp);
   return 0;
}
