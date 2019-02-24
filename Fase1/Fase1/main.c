#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <libgen.h>
#include <time.h>
#include <math.h>
#include "STRUCT.h"

USERLOG userlog;
//-------------------------------Fin----------------------------
char tokenP[300];
char lexemaP[300];
char ControlGrupos[50000000];
char F;
char bitcero='0';
char bituno='1';
char Unionpath[500];
char COPIAUnionpath[500];

int msd=0;
int rmd=0;
int fdi=0;
int rep=0;
int mount=0;
int exec=0;
int mkfs=0;
int login=0;
int mkgrp=0;
int rmgrp=0;
int mkusr=0;
int rmusr=0;
int mkdir=0;
int mkfile=0;
int recovery=0;
int convert=0;
int chmod=0;
int cat=0;
int mv=0;
int rem=0;

int opcion_delete=0;

int estadoS=0;
int flag_global=1;
char direccion_global[400];

//------------------------------------------Variables para msdisk--------------------------
	int mkpath=0;
	int mksize=0;
	int tamanio=0;
	int opcionalUnit=0;
	char medida[30];
	char adress[400];

//------------------------------------------variables para fdisk----------------------------
	int tam_particion=0;
	int flag_espacio=0;
	int flag_unit=0;
	int flag_path=0;
	int flag_type=0;
	int flag_fit=0;
	int flag_delete=0;
	int flag_name=0;
	char typ[10];
	char nomb[100];
	char fitt[10];
	char deleteType[100];

//------------------------------------------VARIABLES PARA REP----------------------------------
	int flag_rep_path=0;
	int flag_rep_id=0;
	int flag_rep_name=0;
	int flag_name_type=0;
    char rep_ruta[400];

//------------------------------------------VARIABLES PARA MOUNT---------------------------------------------
	int flag_mount_path=0;
	int flag_mount_name=0;
	char rep_id[8];

//------------------------------------------VARIBALES PARA UNMOUNT---------------------------------------
	int unmount=0;


//------------------------------------------VARIABLES PARA MKFS--------------------------------------

	char typmkfs[40];
	char idmkfs[100];
	char fsmkfs[20];
	int flagtypemkfs=0;
	int flagfsmkfs=0;
	int flagidmkfs=0;

//------------------------------------------VARIABLES PARA LOGIN----------------------------------------
	int flag_login=0;
	int flag_idlogin=0;
	int flag_pwd=0;
	int flag_usr=0;
	int flag_root=0;
	char login_password[11];
	char login_user[11];
	char login_id[15];

//------------------------------------------VARIABLES PARA MKGRP----------------------------------------
	char mk_name[20];

//------------------------------------------VARIABLES PARA MKUSR------------------------------------------
    int flag_usr_usr=0;
    int flag_usr_pwd=0;
    int flag_usr_grp=0;

    char usr_usr[12];
    char usr_pass[12];
    char usr_grp[12];

//------------------------------------------VARIABLES PARA MKDIR-----------------------------------------
    int flag_mkdir_path=0;
    int flag_mkdir_p=0;
    char mk_path[150];

//------------------------------------------VARIABLES PARA MKFILE-------------------------------------------------
    char mkfile_path[150];
    int flag_mkfile_path=0;
    int flag_mkfile_p=0;
    int flag_mkfile_size=0;
    int flag_mkfile_cont=0;
    int mkfile_size=0;
    char mkfile_cont[150];

//------------------------------------------VARIABLES PARA REM----------------------------------------------------------------
 char rem_path[150];
//------------------------------------------VARIABLES PARA LOSS-----------------------------------
int loss=0;
char loss_id[50];


//------------------------------------------VARIABLES PARA CHMOD--------------------------------
int flag_ch_path=0;
char chmod_path[100];
int flag_ch_ugo=0;
int chmod_ugo=000;
int flag_ch_R=0;


//------------------------------------------VARIABLES PARA CAT-------------------------------------------
int num_cat=0;

//------------------------------------------VARIABLES PARA MV---------------------------------------------
int flag_mv_path=0;
char mv_path[100];
int flag_mv_dest=0;
char mv_dest[100];

/*
------------------------------------------------------METODOS PARA EL AUTOMATA
*/

//---------------------------------------------------VARIABLES GLOBLAES PARA EL ACCESO-------------------------------------
int inicio_bi=0;
int inicio_bb=0;
int inicio_blo=0;
int inicio_inode=0;
int inicio_super=0;
int inicio_journal=0;

int fin_bb=0;
int fin_bi=0;
int fin_inode=0;
int fin_blo=0;


int main()
{
	while(flag_global){
	    char entrada[400];
	    char *p=entrada;
	     estadoS=0;


	    printf("Ingrese sus sentencias \n");
	    scanf(" %400[^\n]", entrada);

	    concatenarPalabras(p,"Tokens.txt");
	    analisisSintactico("Tokens.txt");
	}
    return 0;
}

//----------------------------------------Metodo encargado de registrar tokens y lexemas------------------------------------
void concatenarPalabras(char* p,char direccion[400]){
    char palabra[400];
    int guardar=0;
    int flag_salto=0;
    int indexPalabra=0;
    int esnumero=0;
    int escomillas=0;
    int escomentario=0;
    int esletra=0;
    int essimbolo=0;
    int flag_espacio_espacio=0;
    int flag_recien=0;
	memset(palabra, 0, sizeof(palabra));
    FILE *fp;
    fp=fopen(direccion,"w");

    while(*p && *p!='\0'){
        if(*p==32 ){ //Espacios en blanco
        	if(flag_espacio_espacio==0 && flag_recien==0){
        		if(escomillas==1 || escomentario==1){
                palabra[indexPalabra]=*p;
                indexPalabra++;
                guardar=1;
                esnumero=0;
           	 }else{
                if(esnumero==1){
                    fprintf(fp,"%s,%s\n",palabra,"numero");
                }else{
                    fprintf(fp,"%s,%s\n",palabra,tipoDePalabra(palabra));
                }
                	memset(palabra, 0, sizeof(palabra));
               	 	palabra[0]='\0';
                	indexPalabra=0;
                	esletra=0;
            	}
            	flag_espacio_espacio=1;
            	flag_recien=0;
            	flag_salto=1;
        	}
            

        }else if(*p==62 ){//-----------------mayor-------------------------
        	if(escomentario==0){

	        	palabra[indexPalabra]=*p;
	            indexPalabra++;
	        	fprintf(fp,"%s,%s\n",palabra,tipoDePalabra(palabra));
	            memset(palabra, 0, sizeof(palabra));
	            essimbolo=0;
	            esletra=0;
	            guardar=0;
	            palabra[0]='\0';
	            indexPalabra=0;
	            flag_espacio_espacio=0;
	            flag_recien=1;
	        }
        }else if(*p>47 && *p<58 ){//-----------------Numeros-------------------------
              guardar=1;
            if(escomentario==1 || escomillas==1 || esletra==1){
                esnumero=0;
            }else{
                esnumero=1;
            }
            palabra[indexPalabra]=*p;
            indexPalabra++;
            flag_recien=0;
        }else if(*p==10 || *p==8 || *p==9 || *p==11 || *p==13 ){//---------------------nueva lineaS
        	
	    }        
        else{


	        	if(*p==34){
	                if(escomillas==0){
	                    escomillas=1;
	                }else{
	                    escomillas=0;
	                }
	            }else{
	            	if(*p==35){
	                escomentario=1;
	            }
	            	if(*p!=32 && *p!=10 && *p!=9){


	            	palabra[indexPalabra]=*p;
	            	 indexPalabra++;

	            	}	
	            }
	            	guardar=1;
	            	esnumero=0;
	            	esletra=1;  
	            	flag_espacio_espacio=0;   
	            	flag_recien=0;   
	            	flag_salto=0; 
	        	
        		
        	}
            
        p++;
    }
    
    if(guardar==1 ){
    	if(esnumero==1){
	        fprintf(fp,"%s,%s\n",palabra,"numero");
	        memset(palabra, 0, sizeof(palabra));
	        indexPalabra=0;
	        guardar=0;
	    }else if(escomentario==1){
		    fprintf(fp,"%s,%s\n",palabra,"comentario");
		    memset(palabra, 0, sizeof(palabra));
		    indexPalabra=0;
		    guardar=0;
	    }else if(flag_espacio_espacio!=1){
	        fprintf(fp,"%s,%s\n",palabra,tipoDePalabra(palabra));
	        memset(palabra, 0, sizeof(palabra));
	        indexPalabra=0;
	        guardar=0;
	    }else if(flag_salto!=1){
	        fprintf(fp,"%s,%s\n",palabra,tipoDePalabra(palabra));
	        memset(palabra, 0, sizeof(palabra));
	        indexPalabra=0;
	        guardar=0;
	    }
    	
        fprintf(fp,"%s,%s\n","finInstruccion","finInstruccion");
    }
    fclose(fp);
}


//-------------------------------------------Analisis Sintactico-----------------------------------------------------------
void analisisSintactico(char direccion[100]){
	char token[100]={};
	char lexema[100]={};
	char line[400];
	char *linea;
	FILE *archivo;
	archivo=fopen(direccion,"r");
	estadoS=0;


	while(fscanf(archivo, "%[^\n]%*c",line) == 1){
		linea = strtok(line, ",");
		strcpy(tokenP,linea);
		linea = strtok(NULL, ",");
		strcpy(lexemaP,linea);


		//----------------------------Saber que instruccion es------------------------
		if(strcmp(lexemaP, "mkdisk") == 0){
         	msd=1;
         	rmd=0;
         	fdi=0;
         	rep=0;
         	mount=0;
         	unmount=0;
         	exec=0;
         	mkfs=0;
         	login=0;
         	mkgrp=0;
            rmgrp=0;
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=0;
            chmod=0;
            cat=0;
            mv=0;
            rem=0;
    	}else if(strcmp(lexemaP, "rmdisk") == 0){
    		msd=0;
    		rmd=1;
    		fdi=0;
    		rep=0;
    		mount=0;
    		unmount=0;
    		exec=0;
    		mkfs=0;
    		login=0;
    		mkgrp=0;
            rmgrp=0;
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=0;
            chmod=0;
            cat=0;
            mv=0;
            rem=0;
    	}else if(strcmp(lexemaP, "fdisk") == 0){
    		strcpy(medida,"k");
    		strcpy(fitt,"wf");
    		tam_particion=0;
			flag_espacio=0;
			flag_unit=0;
			flag_path=0;
			flag_type=0;
			flag_fit=0;
			flag_delete=0;
			flag_name=0;

         	msd=0;
         	rmd=0;
         	fdi=1;
         	rep=0;
         	mount=0;
         	unmount=0;
         	exec=0;
         	mkfs=0;
         	login=0;
         	mkgrp=0;
            rmgrp=0;
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=0;
            chmod=0;
            cat=0;
            mv=0;
            rem=0;
    	}else if(strcmp(lexemaP, "rep") == 0){
         	msd=0;
         	rmd=0;
         	fdi=0;
         	rep=1;
         	mount=0;
         	unmount=0;
         	exec=0;
         	mkfs=0;
         	login=0;
         	mkgrp=0;
            rmgrp=0;
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=0;
            chmod=0;
            cat=0;
            mv=0;
            rem=0;
    	}else if(strcmp(lexemaP,"mount")==0){
          	msd=0;
         	rmd=0;
         	fdi=0;
         	rep=0;
         	mount=1;
         	unmount=0;   
         	exec=0;		
         	mkfs=0;
         	login=0;
         	mkgrp=0;
            rmgrp=0;
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=0;
            chmod=0;
            cat=0;
            mv=0;
            rem=0;
    	}else if(strcmp(lexemaP,"unmount")==0){
          	msd=0;
         	rmd=0;
         	fdi=0;
         	rep=0;
         	mount=0;
         	unmount=1;   
         	exec=0;	
         	mkfs=0;
         	login=0;
         	mkgrp=0;
            rmgrp=0;
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=0;
            chmod=0;
            cat=0;
            mv=0;
            rem=0;
    	}else if(strcmp(lexemaP,"exec")==0){
          	msd=0;
         	rmd=0;
         	fdi=0;
         	rep=0;
         	mount=0;
         	unmount=0;   
         	exec=1;	
         	mkfs=0;	
         	login=0;
         	mkgrp=0;
            rmgrp=0;
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=0;
            chmod=0;
            cat=0;
            mv=0;
            rem=0;
    	}else if(strcmp(lexemaP,"mkfs")==0){

            memset(typmkfs,0,sizeof(typmkfs));
            memset(idmkfs,0,sizeof(idmkfs));
            memset(fsmkfs,0,sizeof(fsmkfs));
            flagtypemkfs=0;
            flagfsmkfs=0;
            flagidmkfs=0;

          	msd=0;
         	rmd=0;
         	fdi=0;
         	rep=0;
         	mount=0;
         	unmount=0;   
         	exec=0;	
         	mkfs=1;	
         	login=0;
         	mkgrp=0;
            rmgrp=0;
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=0;
            chmod=0;
            cat=0;
            mv=0;
            rem=0;
    	}else if(strcmp(lexemaP,"login")==0){
          	msd=0;
         	rmd=0;
         	fdi=0;
         	rep=0;
         	mount=0;
         	unmount=0;   
         	exec=0;	
         	mkfs=0;	
         	login=1; 
         	mkgrp=0;   	
            rmgrp=0;	
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=0;
            chmod=0;
            cat=0;
            mv=0;
            rem=0;
    	}else if(strcmp(lexemaP,"logout")==0){
    		if(flag_login){
	    		flag_root=0;
	    		flag_login=0;	
    		}else{
    			printf("Error no existe ninguna sesion a la cual hacerle Logout \n");
    		}
    		msd=0;
         	rmd=0;
         	fdi=0;
         	rep=0;
         	mount=0;
         	unmount=0;   
         	exec=0;	
         	mkfs=0;	
         	login=0; 
         	mkgrp=0;  
            rmgrp=0;
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=0;
            chmod=0;
            mv=0;
            rem=0;
    	}else if(strcmp(lexemaP,"mkgrp")==0){
    		msd=0;
         	rmd=0;
         	fdi=0;
         	rep=0;
         	mount=0;
         	unmount=0;   
         	exec=0;	
         	mkfs=0;	
         	login=0; 
         	mkgrp=1; 
            rmgrp=0; 
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=0;
            chmod=0;
            cat=0;
            mv=0;
            rem=0;
    	}else if(strcmp(lexemaP,"rmgrp")==0){
            msd=0;
            rmd=0;
            fdi=0;
            rep=0;
            mount=0;
            unmount=0;   
            exec=0; 
            mkfs=0; 
            login=0; 
            mkgrp=0; 
            rmgrp=1; 
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=0;
            chmod=0;
            cat=0;
            mv=0;
            rem=0;
        }else if(strcmp(lexemaP,"mkusr")==0){

            flag_usr_usr=0;
            flag_usr_pwd=0;
            flag_usr_grp=0;

            msd=0;
            rmd=0;
            fdi=0;
            rep=0;
            mount=0;
            unmount=0;   
            exec=0; 
            mkfs=0; 
            login=0; 
            mkgrp=0; 
            rmgrp=0; 
            mkusr=1;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=0;
            chmod=0;
            cat=0;
            mv=0;
            rem=0;
        }else if(strcmp(lexemaP,"rmusr")==0){

            flag_usr_usr=0;
            flag_usr_pwd=0;
            flag_usr_grp=0;

            msd=0;
            rmd=0;
            fdi=0;
            rep=0;
            mount=0;
            unmount=0;   
            exec=0; 
            mkfs=0; 
            login=0; 
            mkgrp=0; 
            rmgrp=0; 
            mkusr=0;
            rmusr=1;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=0;
            chmod=0;
            cat=0;
            mv=0;
            rem=0;
        }else if(strcmp(lexemaP,"mkdir")==0){

            flag_mkdir_path=0;
            flag_mkdir_p=0;

            msd=0;
            rmd=0;
            fdi=0;
            rep=0;
            mount=0;
            unmount=0;   
            exec=0; 
            mkfs=0; 
            login=0; 
            mkgrp=0; 
            rmgrp=0; 
            mkusr=0;
            rmusr=0;
            mkdir=1;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=0;
            chmod=0;
            cat=0;
            mv=0;
            rem=0;
        }else if(strcmp(lexemaP,"mkfile")==0){

            memset(mkfile_path,0,sizeof(mkfile_path));
            flag_mkfile_path=0;
            flag_mkfile_p=0;
            flag_mkfile_size=0;
            flag_mkfile_cont=0;
            mkfile_size=0;
            memset(mkfile_cont,0,sizeof(mkfile_cont));
            msd=0;
            rmd=0;
            fdi=0;
            rep=0;
            mount=0;
            unmount=0;   
            exec=0; 
            mkfs=0; 
            login=0; 
            mkgrp=0; 
            rmgrp=0; 
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=1;
            loss=0;
            recovery=0;
            convert=0;
            chmod=0;
            cat=0;
            mv=0;
            rem=0;
        }else if(strcmp(lexemaP,"loss")==0){
            memset(loss_id,0,sizeof(loss_id));
            msd=0;
            rmd=0;
            fdi=0;
            rep=0;
            mount=0;
            unmount=0;   
            exec=0; 
            mkfs=0; 
            login=0; 
            mkgrp=0; 
            rmgrp=0; 
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=1;
            recovery=0;
            convert=0;
            chmod=0;
            cat=0;
            mv=0;
            rem=0;
        }else if(strcmp(lexemaP,"recovery")==0){
            memset(loss_id,0,sizeof(loss_id));
            msd=0;
            rmd=0;
            fdi=0;
            rep=0;
            mount=0;
            unmount=0;   
            exec=0; 
            mkfs=0; 
            login=0; 
            mkgrp=0; 
            rmgrp=0; 
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=1;
            convert=0;
            chmod=0;
            cat=0;
            mv=0;
            rem=0;
        }else if(strcmp(lexemaP,"convert")==0){
            memset(loss_id,0,sizeof(loss_id));
            msd=0;
            rmd=0;
            fdi=0;
            rep=0;
            mount=0;
            unmount=0;   
            exec=0; 
            mkfs=0; 
            login=0; 
            mkgrp=0; 
            rmgrp=0; 
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=1;
            chmod=0;
            cat=0;
            mv=0;
            rem=0;
        }else if(strcmp(lexemaP,"chmod")==0){
            memset(chmod_path,0,sizeof(chmod_path));
            flag_ch_R=0;
            flag_ch_ugo=0;
            flag_ch_path=0;


            msd=0;
            rmd=0;
            fdi=0;
            rep=0;
            mount=0;
            unmount=0;   
            exec=0; 
            mkfs=0; 
            login=0; 
            mkgrp=0; 
            rmgrp=0; 
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=0;
            chmod=1;
            cat=0;
            mv=0;
            rem=0;
        }else if(strcmp(lexemaP,"cat")==0){
            num_cat=0;

            msd=0;
            rmd=0;
            fdi=0;
            rep=0;
            mount=0;
            unmount=0;   
            exec=0; 
            mkfs=0; 
            login=0; 
            mkgrp=0; 
            rmgrp=0; 
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=0;
            chmod=0;
            cat=1;
            mv=0;
            rem=0;
        }else if(strcmp(lexemaP,"mv")==0){

            flag_mv_path=0;
            memset(mv_path,0,sizeof(mv_path));
            flag_mv_dest=0;
            memset(mv_dest,0,sizeof(mv_dest));

            num_cat=0;

            msd=0;
            rmd=0;
            fdi=0;
            rep=0;
            mount=0;
            unmount=0;   
            exec=0; 
            mkfs=0; 
            login=0; 
            mkgrp=0; 
            rmgrp=0; 
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=0;
            chmod=0;
            cat=0;
            mv=1;
            rem=0;
        }else if(strcmp(lexemaP,"rem")==0){

            memset(mv_path,0,sizeof(mv_path));

            num_cat=0;

            msd=0;
            rmd=0;
            fdi=0;
            rep=0;
            mount=0;
            unmount=0;   
            exec=0; 
            mkfs=0; 
            login=0; 
            mkgrp=0; 
            rmgrp=0; 
            mkusr=0;
            rmusr=0;
            mkdir=0;
            mkfile=0;
            loss=0;
            recovery=0;
            convert=0;
            chmod=0;
            cat=0;
            mv=0;
            rem=1;
        }

         



 		if(strcmp(tokenP,"exit")==0){
    		flag_global=0;
    		exit(1);
    	}else if(strcmp(lexemaP,"comentario")==0){
    		printf("Comentario: %s\n",tokenP);
    	}else if(msd){
    		automata_MsDisk();
    	}else if(rmd){
    		automata_rmdisk();
    	}else if(fdi){
    		automata_fdisk();
    	}else if(rep){
    		automata_rep();
    	}else if(mount){
    		automata_mount();
    	}else if(unmount){
    		automata_unmount();
    	}else if(exec){
    		automata_exec();
    	}else if(mkfs){
    		automata_mkfs();
    	}else if(login){
    		automata_login();
    	}else if(mkgrp){
    		automata_mkgrp();
    	}else if(rmgrp){
            automata_rmgrp();
        }else if(mkusr){
            automata_mkusr();
        }else if(rmusr){
            automata_rmusr();
        }else if(mkdir){
            automata_mkdir();
        }else if(mkfile){
            automata_mkfile();
        }else if(loss){
            automata_loss();
        }else if(recovery){
            automata_recovery();
        }else if(convert){
            automata_convert();
        }else if(chmod){
            automata_chmod();
        }else if(cat){
            automata_cat();
        }else if(mv){
            automata_mv();
        }else if(rem){
            automata_rem();
        }else if(strcmp(lexemaP,"finInstruccion")==0){

    	}else if(strcmp(lexemaP,"logout")==0){

        }else{
    		printf("Error no se reconoce el comando %s\n",tokenP);
    		break;
    	}

	}
}


const char* tipoDePalabra(char palabra[400]){
    if(strcmp(palabra, "mkdisk") == 0 || strcmp(palabra, "Mkdisk") == 0 || strcmp(palabra, "mkDisk") == 0 ){
        return "mkdisk";
    }else if(strcmp(palabra, "-size->") == 0 || strcmp(palabra, "-Size->") == 0 ){
        return "size";
    }else if(strcmp(palabra, "-unit->") == 0 || strcmp(palabra, "-Unit->") == 0 ){
        return "unit";
    }else if(strcmp(palabra, "-path->") == 0 || strcmp(palabra, "-Path->") == 0 ){
        return "path";
    }
    else if(strcmp(palabra, "rmdisk") == 0 || strcmp(palabra, "Rmdisk") == 0  || strcmp(palabra, "rmDisk") == 0 ){
        return "rmdisk";
    }else if(strcmp(palabra, "fdisk") == 0 || strcmp(palabra, "Fdisk") == 0  || strcmp(palabra, "fDisk") == 0 ){
        return "fdisk";
    }else if(strcmp(palabra, "-type->") == 0 || strcmp(palabra, "-Type->") == 0 ){
        return "type";
    }else if(strcmp(palabra, "-fit->") == 0 || strcmp(palabra, "-Fit->") == 0 ){
        return "fit";
    }else if(strcmp(palabra, "-delete->") == 0 || strcmp(palabra, "-Delete->") == 0 ){
        return "delete";
    }else if(strcmp(palabra, "-name->") == 0 || strcmp(palabra, "-Name->") == 0 ){
        return "name";
    }else if(strcmp(palabra, "add") == 0 || strcmp(palabra, "Add") == 0 ){
        return "add";
    }else if(strcmp(palabra, "mount") == 0 || strcmp(palabra, "Mount") == 0 ){
        return "mount";
    }else if(strcmp(palabra, "unmount") == 0 || strcmp(palabra, "Unmount") == 0 ){
        return "unmount";
    }else if(strcmp(palabra, "-id->") == 0 || strcmp(palabra, "-Id->") == 0 ){
        return "id";
    }else if(strcmp(palabra, "rep") == 0 || strcmp(palabra, "Rep") == 0 ){
        return "rep";
    }else if(strcmp(palabra, "Login") == 0 || strcmp(palabra, "login") == 0 ){
        return "login";
    }else if(strcmp(palabra, "-pwd->") == 0 || strcmp(palabra, "-Pwd->") == 0 ){
        return "pwd";
    }else if(strcmp(palabra, "-usr->") == 0 || strcmp(palabra, "-Usr->") == 0 ){
        return "usr";
    }else if(strcmp(palabra, "exec") == 0 || strcmp(palabra, "Exec") == 0 ){
        return "exec";
    }else if(strcmp(palabra, "logout") == 0 || strcmp(palabra, "Logout") == 0 ){
        return "logout";
    }else if(strcmp(palabra, "mkfs") == 0 || strcmp(palabra, "Mkfs") == 0 ){
        return "mkfs";
    }else if(strcmp(palabra, "mkgrp") == 0 || strcmp(palabra, "Mkgrp") == 0 ){
        return "mkgrp";
    }else if(strcmp(palabra, "-fs->") == 0 || strcmp(palabra, "-Fs->") == 0 ){
        return "fs";
    }else if(strcmp(palabra, "rmgrp") == 0 || strcmp(palabra, "Rmgrp") == 0 ){
        return "rmgrp";
    }else if(strcmp(palabra, "mkusr") == 0 || strcmp(palabra, "Mkusr") == 0 ){
        return "mkusr";
    }else if(strcmp(palabra, "-grp->") == 0 || strcmp(palabra, "-Grp->") == 0 ){
        return "grp";
    }else if(strcmp(palabra, "rmusr") == 0 || strcmp(palabra, "Rmusr") == 0 ){
        return "rmusr";
    }else if(strcmp(palabra, "mkdir") == 0 || strcmp(palabra, "Mkdir") == 0 ){
        return "mkdir";
    }else if(strcmp(palabra, "-p") == 0 || strcmp(palabra, "-P") == 0 ){
        return "p";
    }else if(strcmp(palabra, "mkfile") == 0 || strcmp(palabra, "Mkfile") == 0 ){
        return "mkfile";
    }else if(strcmp(palabra, "-cont->") == 0 || strcmp(palabra, "-Cont->") == 0 ){
        return "cont";
    }else if(strcmp(palabra, "-ruta->") == 0 || strcmp(palabra, "-Ruta->") == 0 ){
        return "ruta";
    }else if(strcmp(palabra, "Loss") == 0 || strcmp(palabra, "loss") == 0 ){
        return "loss";
    }else if(strcmp(palabra, "Recovery") == 0 || strcmp(palabra, "recovery") == 0 ){
        return "recovery";
    }else if(strcmp(palabra, "convert") == 0 || strcmp(palabra, "Convert") == 0 ){
        return "convert";
    }else if(strcmp(palabra, "chmod") == 0 || strcmp(palabra, "Chmod") == 0 ){
        return "chmod";
    }else if(strcmp(palabra, "-r") == 0 || strcmp(palabra, "-R") == 0 ){
        return "r";
    }else if(strcmp(palabra, "-ugo->") == 0 || strcmp(palabra, "-Ugo->") == 0 ){
        return "ugo";
    }else if(strcmp(palabra, "cat") == 0 || strcmp(palabra, "Cat") == 0 ){
        return "cat";
    }else if(strcmp(palabra, "mv") == 0 || strcmp(palabra, "MV") == 0 ){
        return "mv";
    }else if(strcmp(palabra, "-dest->") == 0 || strcmp(palabra, "-Dest->") == 0 ){
        return "dest";
    }else if(strcmp(palabra, "rem") == 0 || strcmp(palabra, "Rem") == 0 ){
        return "rem";
    }else{
        return "palabra";
    }
    
}


/*
-----------------------------------------Automata de MSDISK-----------------------------------------------
*/

void automata_MsDisk(){
	switch(estadoS){
		case 0:
			if(strcmp(lexemaP, "mkdisk") == 0 ){
         		estadoS=1;
    		}else{
    			printf(" Error no se reconoce el comando %s\n", tokenP);
    			estadoS=1000;
    		}
		break;

		case 1:
			if(strcmp(lexemaP, "size") == 0 ){
         		estadoS=2;
    		}else if(strcmp(tokenP, "finInstruccion") == 0){
  				if(mkpath){
  					if(mksize){
  						char *buffer="\0";
		  				msd=0;
		         		estadoS=0;
		         		char num[500];
		    			int tam_total=1024;
						crearArchivo(adress);
		    			
						MBR auxmbr;
		         		auxmbr.mbr_fecha_creacion=time(NULL);

		         		auxmbr.mbr_disk_signature=(int)time(NULL);
		         		
		         		if(opcionalUnit==0){
		         			auxmbr.mbr_tamanio=tamanio*1048576;
		         			tam_total=tamanio*1024;
		    				
		    			}else{
		    				if(strcmp(medida, "m") == 0){
		         				auxmbr.mbr_tamanio=tamanio*1048576;
		         				tam_total=tamanio*1024;
		    				}else{
		    					auxmbr.mbr_tamanio=tamanio*1024;
		    					tam_total=tamanio;
		    					
		    				}
		    			}


		    			for(int d=0;d<4;d++){
		    				auxmbr.mbr_partition[d].part_status ='\0';
				            auxmbr.mbr_partition[d].part_type = '\0';
				            auxmbr.mbr_partition[d].part_fit = '\0';
				            auxmbr.mbr_partition[d].part_start = -1;
				            auxmbr.mbr_partition[d].part_size = 0;
				            strcpy(auxmbr.mbr_partition[d].part_name,"");
		    			}

						opcionalUnit=0;
		    			
		    			char comando[1000];
		    			
		    			strcpy(comando,"dd if=/dev/zero of=\"");
		    			strcat(comando,adress);
		    			strcat(comando,"\"  bs=1024 count=");
		    			sprintf(num,"%d",tam_total);
		    			strcat(comando,num);
		    			system(comando);  
		    			FILE *NuevoDisco;
		    			NuevoDisco=fopen(adress,"rb+");    			
		    			fseek(NuevoDisco,0,SEEK_SET);
		    			fwrite(&auxmbr,sizeof(MBR),1,NuevoDisco);
		    			fclose(NuevoDisco);
		         		
  					}else{
  						printf("Falta size\n");
  						estadoS=0;
  						msd=0;
  					}

  				}else{
  					printf("Falta path\n");
  					estadoS=0;
  					msd=0;
  				}

    		}else if(strcmp(lexemaP, "path") == 0 ){
    			estadoS=3;
    		}else if(strcmp(lexemaP,"unit")==0){
    			estadoS=4;
    		}

		break;

		case 2:
			if(strcmp(lexemaP,"numero")==0){
				tamanio=atoi(tokenP);
				if(tamanio>0){
					estadoS=1;
					mksize=1;
				}else{
					printf("El numero tiene que ser mayor que cero\n");
				}
			}else{
				printf("Se esperaba un numero\n");
				estadoS=0;
				msd=0;
			}
		break;

		case 3:
			if(strcmp(lexemaP,"palabra")==0){
				mkpath=1;
				estadoS=1;
				strcpy(adress,tokenP);
			}else{
				printf("No se reconoce la direccion\n");
				estadoS=0;
				msd=0;
			}
		break;

		case 4:
			if(strcmp(lexemaP,"palabra")==0){
				if(strcmp(tokenP,"k")==0 || strcmp(tokenP,"K")==0 ){
					strcpy(medida,"k");
					estadoS=1;
					opcionalUnit=1;
				}else if(strcmp(tokenP,"m")==0 || strcmp(tokenP,"M")==0 ){
					strcpy(medida,"m");
					estadoS=1;
					opcionalUnit=1;
				}else{
					printf("Error no se reconoce el parametro\n");
					estadoS=0;
					msd=0;
				}
			}else{
				printf("Error no se reconoce el parametro\n");
				estadoS=0;
				msd=0;
			}
		break;

	}
}

/*---------------------------------------AUTOMATA RMDISK-----------------------------------------------------
*/
void automata_rmdisk(){
	switch(estadoS){
		case 0:
			if(strcmp(lexemaP, "rmdisk") == 0 ){
         		estadoS=1;
    		}else{
    			printf(" Error no se reconoce el comando %s\n", tokenP);
    			estadoS=1000;
    		}
		break;

		case 1:
			if(strcmp(lexemaP, "path") == 0 ){
         		estadoS=2;
    		}else{
    			printf(" Se esperaba path \n");
    			estadoS=1000;
    		}
		break;

		case 2:
			if(strcmp(lexemaP, "palabra") == 0 ){
         		estadoS=3;
         		strcpy(adress,tokenP);
    		}else{
    			printf(" Se esperaba una direccion \n");
    			estadoS=1000;
    		}
		break;

		case 3:
				rmd=0;
         		estadoS=0;
         		FILE *disco;

         		if(disco=fopen(adress,"r")){
         			int decision=2;
         			printf("Realmente desea eliminar el disco? 1.si 2.no\n");
         			scanf("%d",&decision);
         			if(decision==1){
         				char comando[1000];
    					strcpy(comando,"rm \"");
    					strcat(comando,adress);
    					strcat(comando,"\"");
    					system(comando);    			
         				printf("Disco elminado con exito\n");
         			}else{
         				printf("Cancelado con exito\n");
         			}
         			fclose(disco);
         		}else{
         			printf("No existe el disco a eliminar\n");
         		}
    		
    			
		break;
	}
}

/*---------------------------------------AUTOMATA FDISK-----------------------------------------------------
*/

void automata_fdisk(){

	switch(estadoS){
		case 0:
         		estadoS=1;
		break;

		case 1:
			if(strcmp(lexemaP, "size") == 0 ){
         		estadoS=2;
    		}else if(strcmp(lexemaP, "unit") == 0 ){
    			estadoS=3;
    		}else if(strcmp(lexemaP, "path") == 0 ){
    			estadoS=4;
    		}else if(strcmp(lexemaP, "type") == 0 ){
    			estadoS=5;
    		}else if(strcmp(lexemaP, "fit") == 0 ){
    			estadoS=6;
    		}else if(strcmp(lexemaP, "name") == 0 ){
    			estadoS=7;
    		}else if(strcmp(lexemaP,"delete")==0){
    			estadoS=8;
    			flag_delete=1;

    		}else if(strcmp(lexemaP, "finInstruccion") == 0){
    			if(flag_path){

    				if(flag_delete){
    					if(flag_name){

    						int index=-1;
    						int flag_exten=0;
    						int index_extendida=0;
    						FILE *disco;
    						if(disco=fopen(adress,"rb+")){
    							
    							char *buffer="\0";
    							fseek(disco,0,SEEK_SET);
    							MBR auxmbr;
    							fread(&auxmbr,sizeof(MBR),1,disco);
    							//---------------------------------------------Buscamos una particion Primaria-----------------------------------------
    							for(int i=0;i<4;i++){
    								if(strcmp(auxmbr.mbr_partition[i].part_name,nomb)==0 && auxmbr.mbr_partition[i].part_status!='1'){
    									index=i;
    									if(auxmbr.mbr_partition[i].part_type=='E'){
    										flag_exten=1;
    									}
    									break;
    								}else if(auxmbr.mbr_partition[i].part_type=='E' && auxmbr.mbr_partition[i].part_status!='1'){
    									index_extendida=i;
    								}
    							}
    							printf("Desea eliminar la particion 1. si 2.no?\n");
    							scanf("%d",&opcion_delete);
    							//----------------------------------------------Si encontro la particion------------------------------------------------
    							if(index!=-1 && opcion_delete==1){
    								//-----------------------------------------Si no es extendida------------------------------------------------------
    								if(flag_exten==0){
    									auxmbr.mbr_partition[index].part_status='1';
    									strcpy(auxmbr.mbr_partition[index].part_name,"\0");
    									fseek(disco,0,SEEK_SET);
    									fwrite(&auxmbr,sizeof(MBR),1,disco);
    									if(strcmp(deleteType,"full")==0){
    										fseek(disco,auxmbr.mbr_partition[index].part_start,SEEK_SET);
    										fwrite(&buffer,1,auxmbr.mbr_partition[index].part_size,disco);
    									}
    								     printf("Eliminada con exito\n");
    									 fseek(disco,0,SEEK_SET);
    									 fclose(disco);

    								}else{
    									int cant_logicas=0;
    									ebr auxebr;
    									fseek(disco,auxmbr.mbr_partition[index].part_start,SEEK_SET);
    									fread(&auxebr,sizeof(ebr),1,disco);
    									if(auxebr.part_size!=0){
    										fseek(disco,auxmbr.mbr_partition[index].part_start,SEEK_SET);
    										while((fread(&auxebr,sizeof(ebr),1,disco))!=0 && ftell(disco)<(auxmbr.mbr_partition[index].part_size+auxmbr.mbr_partition[index].part_start)){
													if(auxebr.part_next!=-1){
														if(auxebr.part_status!='1'){
															cant_logicas++;
														}
													}else{
														break;
													}
												}
    									}
    									if(cant_logicas==0){
    										auxmbr.mbr_partition[index].part_status='1';
    										strcpy(auxmbr.mbr_partition[index].part_name,"\0");
    										fseek(disco,0,SEEK_SET);
    										fwrite(&auxmbr,sizeof(MBR),1,disco);
    										if(strcmp(deleteType,"full")==0){
    											fseek(disco,auxmbr.mbr_partition[index].part_start,SEEK_SET);
    											fwrite(&buffer,1,auxmbr.mbr_partition[index].part_size,disco);
    										}
    								     	printf("Eliminada con exito\n");
    									 	fseek(disco,0,SEEK_SET);
    									 	fclose(disco);
    									}else{
    										printf("Error existen particiones logicas en la extendida, no se puede eliminar\n");
    									}

    								}
    							}else{
    								//--------------------------------------------Eliminar Logicas------------------------------------------------------------
    								int localizado=0;
    								ebr auxebr;
    									fseek(disco,auxmbr.mbr_partition[index_extendida].part_start,SEEK_SET);
    									fread(&auxebr,sizeof(ebr),1,disco);
    									if(auxebr.part_size!=0){
    										fseek(disco,auxmbr.mbr_partition[index_extendida].part_start,SEEK_SET);
    										while((fread(&auxebr,sizeof(ebr),1,disco))!=0 && ftell(disco)<(auxmbr.mbr_partition[index_extendida].part_size+auxmbr.mbr_partition[index_extendida].part_start)){
												if(auxebr.part_next!=-1){
													if(strcmp(auxebr.part_name,nomb)==0 && auxebr.part_status!='1'){
														localizado=1;
														break;
													}
												}else{
														break;
												}
											}
										}
										if(localizado){
											auxebr.part_status='1';
    										strcpy(auxebr.part_name,"\0");
    										fseek(disco,ftell(disco)-sizeof(ebr),SEEK_SET);
    										fwrite(&auxebr,sizeof(auxebr),1,disco);
    										if(strcmp(deleteType,"full")==0){
    											fwrite(&buffer,1,auxebr.part_size,disco);
    										}
    								     	printf("Eliminada con exito\n");
    									 	fseek(disco,0,SEEK_SET);
    									 	fclose(disco);

										}else{
											printf("Error no se encuentra la particion  a eliminar\n");
										}
    						}


    						}else{
    							printf("Error el disco a eliminar  no existe\n");
    						}



    					}else{
								estadoS=1000;
								printf("Hace falta name\n");
    					}
    				}else{
    					if(flag_espacio){
							if(flag_name){
								estadoS=0;
								fdi=0;

								FILE* disco;
						 		MBR auxmbr;
								if((disco=fopen(adress,"rb+"))!=NULL){
									fseek(disco,0,SEEK_SET);
									fread(&auxmbr,sizeof( MBR),1,disco);
									int numero=0;
									int flag_extendida=0;
									int flag_cantidad=0;
									int flag_guardar=0;
									int flag_tamanio=0;
									int flag_parti=0;
									int flag_logica=0;
									char *buffer="1";
									/*
									printf("Tamaio: %d\n",auxmbr.mbr_tamanio);
									printf("disk: %d\n",auxmbr.mbr_disk_signature);
									struct tm *tm;
									char fecha[100];
									tm=localtime(&auxmbr.mbr_fecha_creacion);
									strftime(fecha,100,"%d/%m/%Y",tm);
									printf("Hoy es %s\n",fecha );
									*/

							//---------------------------Ver que particion esta libre-----------------------------
									if(auxmbr.mbr_partition[0].part_start==-1 || (auxmbr.mbr_partition[0].part_size>=tam_particion && auxmbr.mbr_partition[0].part_status=='1') ){
										numero=0;
									}else if(auxmbr.mbr_partition[1].part_start==-1 || (auxmbr.mbr_partition[1].part_size>=tam_particion  && auxmbr.mbr_partition[1].part_status=='1') ){
										numero=1;
									}else if(auxmbr.mbr_partition[2].part_start==-1 || (auxmbr.mbr_partition[2].part_size>=tam_particion && auxmbr.mbr_partition[2].part_status=='1') ){
										numero=2;
									}else if(auxmbr.mbr_partition[3].part_start==-1 || (auxmbr.mbr_partition[3].part_size>=tam_particion && auxmbr.mbr_partition[3].part_status=='1') ){
										numero=3;
									}else{
										flag_cantidad=1;
									}

							//--------------------------------verificar particiones---------------------------------
									if(flag_cantidad==1 && strcmp(typ,"l")!=0){
										printf("Error ya existen 4 particiones no se puede crear otra elimine alguna para continuar\n");
									}else{
								//-----------------------------------Buscar nombre-----------------------------------------------
									if(buscarNombreLogicas(adress,nomb)==0){
										flag_cantidad=0;
										printf("------------------------Disco %s \n",adress );
									//----------------------------ver cuantas particiones extendidas y cuando es el espacio libre--------
										for(int i=0;i<4;i++){
											printf("Tamanio de particion: %d\n",auxmbr.mbr_partition[i].part_size);
											printf("Posicion de particion: %d\n",auxmbr.mbr_partition[i].part_start);
											flag_tamanio=flag_tamanio+auxmbr.mbr_partition[i].part_size;
											printf("Tipo %c\n\n\n", auxmbr.mbr_partition[i].part_status);
											if(auxmbr.mbr_partition[i].part_type=='E' && auxmbr.mbr_partition[i].part_type!='1'){
												flag_logica=i;
												flag_cantidad++;
											}
										}
										if(flag_cantidad>0 && strcmp(typ,"e")==0){
											printf("Error ya existe mas de una particion extendida\n");
											flag_guardar=0;
										}else if(flag_cantidad==0 && strcmp(typ,"l")==0){
											printf("Error no existe ninguna particion Extendida para guardar la logica\n");
										}else if(flag_cantidad==0 && strcmp(typ,"e")==0){
											flag_parti=1;
											flag_guardar=1;
											auxmbr.mbr_partition[numero].part_type='E';
										}else if(flag_cantidad==1 && strcmp(typ,"l")==0){
											//--------------------------------------------------GUARDAR LOGICA---------------------------------
											flag_guardar=1;
											flag_parti=2;											
										}else{
											flag_parti=0;
											flag_guardar=1;
											auxmbr.mbr_partition[numero].part_type='P';
										}



										if(flag_guardar==1){
											int tam=1024;

											if(strcmp(medida,"k")==0){
												tam=tam_particion*1024;
											}else if(strcmp(medida,"b")==0){
												tam=tam_particion;
											}else{
												tam=tam_particion*1024*1024;
											}
											if(tam<0){
												tam=tam*(-1);
											}
										



										//------------------------------------PARTICIONES LOGICAS----------------------------------
										if(flag_parti==2){
											ebr auxebr;
											int flag_logica_guardar=0;
											int tam_logicas=0;
											int contador=auxmbr.mbr_partition[flag_logica].part_start;
											fseek(disco,contador,SEEK_SET);
											fread(&auxebr,sizeof(ebr),1,disco);
											if(auxebr.part_size==0){
												if((auxmbr.mbr_partition[flag_logica].part_size)<(tam)){
													printf("tamaño de logica excede el libre de extendida\n");
												}else{ 
													auxebr.part_status='0';
													if(strcmp(fitt, "W") == 0){
														auxebr.part_fit='W';
													}else if(strcmp(fitt, "B") == 0){
														auxebr.part_fit='B';
													}else{
														auxebr.part_fit='F';
													}
													auxebr.part_start=ftell(disco)-sizeof(ebr);
													auxebr.part_size=tam;
													auxebr.part_next=-1;
													strcpy(auxebr.part_name,nomb);
													fseek(disco,auxmbr.mbr_partition[flag_logica].part_start,SEEK_SET);
													fwrite(&auxebr,sizeof(ebr),1,disco);	
												}
											}else{
												while(auxebr.part_next!=-1 && ftell(disco)<(auxmbr.mbr_partition[flag_logica].part_size+auxmbr.mbr_partition[flag_logica].part_start)){
													fseek(disco,auxebr.part_next,SEEK_SET);
													fread(&auxebr,sizeof(ebr),1,disco);
												}
												int ocupar=auxebr.part_size+tam;
												if((ocupar)>(auxmbr.mbr_partition[flag_logica].part_size)){
													printf("No se puede guardar excede el de la libre extendida\n");
												}else{
													auxebr.part_next=auxebr.part_start+auxebr.part_size;
													fseek(disco,ftell(disco)-sizeof(ebr),SEEK_SET);
													fwrite(&auxebr,sizeof(ebr),1,disco);
													fseek(disco,auxebr.part_start+auxebr.part_size,SEEK_SET);
													auxebr.part_status='0';
													if(strcmp(fitt, "W") == 0){
														auxebr.part_fit='W';
													}else if(strcmp(fitt, "B") == 0){
														auxebr.part_fit='B';
													}else{
														auxebr.part_fit='F';
													}
													auxebr.part_start=ftell(disco);
													auxebr.part_size=tam;
													auxebr.part_next=-1;
													strcpy(auxebr.part_name,nomb);
													fwrite(&auxebr,sizeof(ebr),1,disco);
												}
											}


			
											//--------------------------si no excede el tamanio------------------------------------
										}else {

											//-----------------------------------Particiones Primarias o Extendidas-----------------------
											printf(" Tamanio Libre:%d\n",(auxmbr.mbr_tamanio)-flag_tamanio);
											printf("Nuevo tamanio: %d\n",tam);
											if(((auxmbr.mbr_tamanio)-flag_tamanio)>tam){
												//-------------------------------tipo------------------------------------------------
												if(strcmp(fitt, "W") == 0){
													auxmbr.mbr_partition[numero].part_fit='W';
												}else if(strcmp(fitt, "B") == 0){
													auxmbr.mbr_partition[numero].part_fit='B';
												}else{
													auxmbr.mbr_partition[numero].part_fit='F';
												}
												//--------------------------Inicio-------------------------------------------------
												if(numero-1==-1){
													auxmbr.mbr_partition[numero].part_start=sizeof(auxmbr);

												}else{
													auxmbr.mbr_partition[numero].part_start=auxmbr.mbr_partition[numero-1].part_size+auxmbr.mbr_partition[numero-1].part_start;
												}
												auxmbr.mbr_partition[numero].part_size=tam;
												auxmbr.mbr_partition[numero].part_status='0';
												strcpy(auxmbr.mbr_partition[numero].part_name,nomb);
												fseek(disco,0,SEEK_SET);
												fwrite(&auxmbr,sizeof(MBR),1,disco);
												//-----------------------------------particion primaria--------------------------
												if(flag_parti==0){
													fseek(disco,auxmbr.mbr_partition[numero].part_start,SEEK_SET);
													for(int g=0;g<tam;g++){
															fwrite(&buffer,1,1,disco);
														}
												}else{
													fseek(disco,auxmbr.mbr_partition[numero].part_start,SEEK_SET);
													ebr auxebr;
												
													auxebr.part_fit='F';
													auxebr.part_status='0';
													auxebr.part_start=auxmbr.mbr_partition[numero].part_start;
													auxebr.part_size=0;
													auxebr.part_next=-1;
													strcpy(auxebr.part_name,"");
													fwrite(&auxebr,sizeof(ebr),1,disco);
													for(int g=0;g<(tam-sizeof(ebr));g++){
															
														fwrite(&buffer,1,1,disco);
													}
												}
												printf("Particion creada con exito\n");
											}else{
												printf("Error la particion a guardar excede el tamaño libre\n");
											}

										}

										
									}
								}else{
									printf("Ya existe una particion con ese nombre\n");
								}
							}
							
							fclose(disco);
							//}
						}else{
							estadoS=1000;
							printf("No existe el disco \n");
						}



							}else{
								estadoS=1000;
								printf("Hace falta name\n");
							}
					}else{
						estadoS=1000;
						printf("Hace falta size \n");
					}
    			}

				
			}else{
				estadoS=1000;
				printf("Hace falta el path\n");
			}		
    		}else{
    			printf(" No se reconoce el comodin %s\n", tokenP);
    			estadoS=1000;
    		}
		break;

		case 2:
			if(strcmp(lexemaP, "numero") == 0 ){
				tam_particion=atoi(tokenP);
         		if(tam_particion>0){
         			flag_espacio=1;
         			estadoS=1;
         		}else{
         			printf(" Solo numeros mayores a 0 \n");
         			estadoS=1000;
         		}
         		
    		}else{
    			printf(" Error no se reconoce el comando %s\n", tokenP);
    			estadoS=1000;
    		}
		break;

		case 3:
			if(strcmp(lexemaP, "palabra") == 0 ){
				if(strcmp(tokenP, "B") == 0 || strcmp(tokenP, "b") == 0 ){
					estadoS=1;
         			flag_unit=1;
         			strcpy(medida,"b");
				}else if(strcmp(tokenP, "K") == 0 || strcmp(tokenP, "k") == 0 ){
					estadoS=1;
	         		flag_unit=1;
	         		strcpy(medida,"k");
				}else if(strcmp(tokenP, "M") == 0 || strcmp(tokenP, "m") == 0 ){
					estadoS=1;
	         		flag_unit=1;
	         		strcpy(medida,"m");
				}else{
					estadoS=1000;
					printf(" No se reconoce el valor para unit%s\n", tokenP);
				}
         		
    		}else{
    			printf(" No se reconoce el comodin %s\n", tokenP);
    			estadoS=1000;
    		}
		break;

		case 4:
			if(strcmp(lexemaP, "palabra") == 0 ){
         		estadoS=1;
         		flag_path=1;
         		strcpy(adress,tokenP);
    		}else{
    			printf(" No se reconoce la ruta  %s\n", tokenP);
    			estadoS=1000;
    		}
		break;


		case 5:
			if(strcmp(lexemaP, "palabra") == 0 ){
				if(strcmp(tokenP, "P") == 0 || strcmp(tokenP, "p") == 0 ){
					estadoS=1;
         			flag_type=1;
         			strcpy(typ,"p");
				}else if((strcmp(tokenP, "E") == 0) || (strcmp(tokenP, "e") == 0) ){
					estadoS=1;
	         		flag_type=1;
	         		strcpy(typ,"e");
				}else if(strcmp(tokenP, "L") == 0 || strcmp(tokenP, "l") == 0 ){
					strcpy(typ,"l");
					estadoS=1;
	         		flag_type=1;
				}else{
					estadoS=1000;
					printf(" No se reconoce el valor %s\n", tokenP);
				}
         		
    		}else{
    			printf(" No se reconoce el comodin %s\n", tokenP);
    			estadoS=1000;
    		}
		break;

		case 6:
			if(strcmp(lexemaP, "palabra") == 0 ){
				if(strcmp(tokenP, "BF") == 0 || strcmp(tokenP, "bf") == 0 ){
					estadoS=1;
         			flag_fit=1;
         			strcpy(fitt,"B");
				}else if(strcmp(tokenP, "FF") == 0 || strcmp(tokenP, "ff") == 0 ){
					estadoS=1;
	         		flag_fit=1;
	         		strcpy(fitt,"F");
				}else if(strcmp(tokenP, "WF") == 0 || strcmp(tokenP, "wf") == 0 ){
					estadoS=1;
	         		flag_fit=1;
	         		strcpy(fitt,"W");
				}else{
					estadoS=1000;
					printf(" No se reconoce el valor para type %s\n", tokenP);
				}
         		
    		}else{
    			printf(" No se reconoce la palabra %s\n", tokenP);
    			estadoS=1000;
    		}
		break;

		case 7:
			if(strcmp(lexemaP, "palabra") == 0 ){
         			flag_name=1;
         			estadoS=1;
         			strcpy(nomb,tokenP);      		
    		}else{
    			printf(" Error no se reconoce el nombre%s\n", tokenP);
    			estadoS=1000;
    		}
		break;

		//-------------------------------------------------delete--------------------------------------------------------------------
		case 8:
			if(strcmp(tokenP,"Fast")==0 || strcmp(tokenP,"fast")==0){
				strcpy(deleteType,"fast");
				estadoS=1;
			}else if(strcmp(tokenP,"Full")==0 || strcmp(tokenP,"full")==0){
				strcpy(deleteType,"full");
				estadoS=1;
			}else{
				estadoS=0;
				fdi=0;
				printf("No se reconoce el comando: %s\n",tokenP);
			}
		break;

	}
}

/*---------------------------------------AUTOMATA REP-----------------------------------------------------
*/


void automata_rep(){
	switch(estadoS){

		case 0:
		 	estadoS=1;
		break;

		case 1:
			if(strcmp(lexemaP,"id")==0){
				estadoS=2;
				flag_rep_id=1;
			}else if(strcmp(lexemaP,"path")==0){
				estadoS=3;
				flag_rep_path=1;
			}else if(strcmp(lexemaP,"name")==0){
				estadoS=4;
				flag_rep_name=1;
			}else if(strcmp(lexemaP,"ruta")==0){
                estadoS=5;
            }else if(strcmp(lexemaP,"finInstruccion")==0){
				if(flag_rep_name && flag_rep_path && flag_rep_id){
					const char *get=getDireccion(rep_id);
					char jaDireccion[400];
					char *search=jaDireccion;
					char *search2=adress;
					strcpy(jaDireccion,get);
                    if(strcmp(jaDireccion,"no")!=0){
						char ext[30];
						char comando[400];


						strcpy(comando,"sudo mkdir -p \'");
						strcat(comando,getPath(search2));
						strcat(comando,"\'");
						system(comando);

						strcpy(comando,"sudo chmod -R 777 \'");
						strcat(comando,getPath(search2));
						strcat(comando,"\'");
						system(comando);

						strcpy(ext,extension(search2));
						
					if(flag_name_type==1){
						graficarDisco(jaDireccion,adress,ext);
					}else if(flag_name_type==2){
                        struct Nodo *aux=buscarMount(rep_id);
                        int index=buscarParticionP(jaDireccion,aux->nombre);
                        if(index!=-1){
                            MBR auxmbr;
                            FILE *disco;
                            disco=fopen(jaDireccion,"rb+");
                            fread(&auxmbr,sizeof(MBR),1,disco);
                            fseek(disco,auxmbr.mbr_partition[index].part_start,SEEK_SET);
                            superBloque super;
                            fread(&super,sizeof(superBloque),1,disco);
                            fclose(disco);
                            graficarInodos(jaDireccion,adress,ext,super.s_bm_inode_start,super.s_inode_start,super.s_bm_inode_start+super.s_inodes_count);
                        }else{
                            int index=buscarParticionL(jaDireccion,aux->nombre);
                            if(index!=-1){
                                ebr auxebr;
                                FILE *disco;
                                disco=fopen(jaDireccion,"rb+");
                                fseek(disco,index,SEEK_SET);
                                fread(&auxebr,sizeof(ebr),1,disco);
                                superBloque super;
                                fread(&super,sizeof(superBloque),1,disco);
                                fclose(disco);
                                graficarInodos(jaDireccion,adress,ext,super.s_bm_inode_start,super.s_inode_start,super.s_bm_inode_start+super.s_inodes_count);
                            }else{
                                printf("La particion no existe \n");
                                estadoS=0;
                                rep=0;
                            }
                        }
                    }else if(flag_name_type==3){
                        struct Nodo *aux=buscarMount(rep_id);
                        int index=buscarParticionP(jaDireccion,aux->nombre);
                        if(index!=-1){
                            MBR auxmbr;
                            FILE *disco;
                            disco=fopen(jaDireccion,"rb+");
                            fread(&auxmbr,sizeof(MBR),1,disco);
                            fseek(disco,auxmbr.mbr_partition[index].part_start,SEEK_SET);
                            superBloque super;
                            fread(&super,sizeof(superBloque),1,disco);
                            fclose(disco);
                            graficarBloques(jaDireccion,adress,ext,super.s_bm_block_start,super.s_block_start,super.s_bm_block_start+super.s_blocks_count);
                           }else{
                            int index=buscarParticionL(jaDireccion,aux->nombre);
                            if(index!=-1){
                                ebr auxebr;
                                FILE *disco;
                                disco=fopen(jaDireccion,"rb+");
                                fseek(disco,index,SEEK_SET);
                                fread(&auxebr,sizeof(ebr),1,disco);
                                superBloque super;
                                fread(&super,sizeof(superBloque),1,disco);
                                fclose(disco);
                                graficarBloques(jaDireccion,adress,ext,super.s_bm_block_start,super.s_block_start,super.s_bm_block_start+super.s_blocks_count);
                            }else{
                                printf("La particion no existe \n");
                                estadoS=0;
                                rep=0;
                            }
                        }

                    }else if(flag_name_type==4){
                        superBloque super;
                        FILE* disco;
                        disco=fopen(direccion_global,"rb+");
                        fseek(disco,inicio_super,SEEK_SET);
                        fread(&super,sizeof(superBloque),1,disco);
                        fclose(disco);
                        crearReporte(inicio_bb,super.s_blocks_count,adress);
                    }else if(flag_name_type==5){
                        superBloque super;
                        FILE* disco;
                        disco=fopen(direccion_global,"rb+");
                        fseek(disco,inicio_super,SEEK_SET);
                        fread(&super,sizeof(superBloque),1,disco);
                        fclose(disco);
                        crearReporte(inicio_bi,super.s_inodes_count,adress);
                    }else if(flag_name_type==6){
                        crearTree(adress,ext);
                    }else if(flag_name_type==7){
                        graficarSuper(adress,ext);

                    }else if(flag_name_type==8){
                        char *pa=rep_ruta;
                        reporteFile(pa,adress);

                    }else if(flag_name_type==9){
                        char *pa=rep_ruta;
                        reportePermisos(pa,adress,ext);

                    }else if(flag_name_type==10){
                        if(flagfsmkfs==0){
                            reportarJournaling(adress);
                        }else{
                            printf("No hay journaling a reportar\n");
                        }

                    }else{
						graficarMBR(jaDireccion,adress,ext);
					}
                }else{
                    printf("Error no se encuentra la particion\n");
                    estadoS=0;
                    rep=0;
                }
				}else{
					printf("Error no tiene los parametros suficientes\n");
				}
				estadoS=0;
				rep=0;
			}else{
				estadoS=10000;
				printf("No se reconoce el parametro %s\n",tokenP);
			}
		break;

		case 2:
			if(strcmp(lexemaP,"palabra")==0){
				estadoS=1;
				strcpy(rep_id,tokenP);
			}else{
				estadoS=1000;
				printf("No se reconoce el parametro\n");
			}
			break;
	

		case 3:
			if(strcmp(lexemaP,"palabra")==0){
				estadoS=1;
				strcpy(adress,tokenP);
			}else{
				estadoS=1000;
				printf("No se reconoce la direccion\n");
			}
			break;
	
		case 4:
			if(strcmp(tokenP,"mbr")==0){
				flag_name_type=0;
				estadoS=1;
			}else if(strcmp(tokenP,"disk")==0){
				flag_name_type=1;
				estadoS=1;
			}else if(strcmp(tokenP,"inode")==0){
                flag_name_type=2;
                estadoS=1;
            }else if(strcmp(tokenP,"block")==0){
                flag_name_type=3;
                estadoS=1;
            }else if(strcmp(tokenP,"bm_block")==0){
                flag_name_type=4;
                estadoS=1;
            }else if(strcmp(tokenP,"bm_inode")==0){
                flag_name_type=5;
                estadoS=1;
            }else if(strcmp(tokenP,"tree")==0){
                flag_name_type=6;
                estadoS=1;
            }else if(strcmp(tokenP,"sb")==0){
                flag_name_type=7;
                estadoS=1;

            }else if(strcmp(tokenP,"file")==0){
                flag_name_type=8;
                estadoS=1;
            }else if(strcmp(tokenP,"ls")==0){
                flag_name_type=9;
                estadoS=1;
            }else if(strcmp(tokenP,"Journaling")==0 || strcmp(tokenP,"journaling")==0 ){
                flag_name_type=10;
                estadoS=1;
            }else{
				estadoS=1000;
				printf("No se reconoce el paramentro para name\n");
			}
			break;

            case 5:
                if(strcmp(lexemaP,"palabra")==0){
                    memset(rep_ruta,0,sizeof(rep_ruta));
                    strcpy(rep_ruta,tokenP);
                    estadoS=1;
                }else{
                    printf("Se esperaba una direccion en ruta\n");
                    estadoS=1;
                    rep=0;
                }
            break;
	}
}


//---------------------------------------AUTOMATA MOUNT------------------------------------------------------------
void automata_mount(){
	switch(estadoS){
		case 0:
			estadoS=1;
			break;

		case 1:
			if(strcmp(lexemaP,"path")==0){
				flag_mount_path=1;
				estadoS=2;
			}else if(strcmp(lexemaP,"name")==0){
				flag_mount_name=1;
				estadoS=3;
			}else if(strcmp(lexemaP,"finInstruccion")==0){
				

					if(flag_mount_path==1 && flag_mount_name==1){
					int index=0;
					index=buscarParticionP(adress,nomb);
					if(index!=-1){
						FILE *disco;
						if(disco=fopen(adress,"rb+")){
							fseek(disco,0,SEEK_SET);
							MBR auxmbr;
							fread(&auxmbr,sizeof(MBR),1,disco);
							auxmbr.mbr_partition[index].part_status='2';
							fseek(disco,0,SEEK_SET);
							fwrite(&auxmbr,sizeof(MBR),1,disco);
					        fseek(disco,0,SEEK_SET);
					        fclose(disco);
							
							int nu=buscarletra("a",auxmbr.mbr_partition[index].part_name,adress);
							if(nu==-1){
								printf("Error ya la particion esta montada\n");
								estadoS=0;
								mount=0;
							}else{
								char *le=(char*)&nu;
								int nus=buscarnumero(1,"nombress.txt",adress);
								char bus[100];
								strcpy(bus,"vd");
								strcat(bus,le);
								sprintf(bus,"%s%d",bus,nus);
								struct Nodo *auxx=buscarMount(bus);
								if(auxx!=NULL){
									printf("La particion ya esta montada\n");
								}else{
									insertarNodo(nus,le,adress,nomb);
									printf("Nombre de la particion montada: vd%s%d\n",le,nus);
									mostrarcambio();
								}

								
							}
						}else{
								printf("Error no se encuentra el disco\n");
									estadoS=0;
									mount=0;
							}
					}else{

						int index=buscarParticionL(adress,nomb);
						if(index!=-1){
						FILE *disco;
							if(disco=fopen(adress,"rb+")){
								fseek(disco,index,SEEK_SET);
								ebr auxebr;
								fread(&auxebr,sizeof(ebr),1,disco);
								auxebr.part_status='2';
								fseek(disco,index,SEEK_SET);
								fwrite(&auxebr,sizeof(ebr),1,disco);
							    fseek(disco,0,SEEK_SET);
							    fclose(disco);

							    int nu=buscarletra("a",auxebr.part_name,adress);
								if(nu==-1){
									printf("Error ya la particion esta montada\n");
									estadoS=0;
									mount=0;
								}else{
									char *le=(char*)&nu;
									int nus=buscarnumero(1,"nombress.txt",adress);
								    char bus[100];
									strcpy(bus,"vd");
									strcat(bus,le);
									sprintf(bus,"%s%d",bus,nus);
									struct Nodo *auxx=buscarMount(bus);
									if(auxx!=NULL){
										printf("La particion ya esta montada\n");
									}else{
										insertarNodo(nus,le,adress,nomb);
										printf("Nombre de la particion montada: vd%s%d\n",le,nus);
										mostrarcambio();
									}
								}
							}else{
								printf("Error no se encuentra el disco\n");
									estadoS=0;
									mount=0;
								}
							}else{
								printf("Error no se encuentra el nombre a buscar\n");
								estadoS=0;
								mount=0;
								}
						}

										
				}else{
					printf("Faltan comodines \n");
					estadoS=1000;
				}



				//------------------------------------------Si encontro la particion la va a montar---------------------------------------------
				
			}else{
				printf("No se reconoce el comando %s\n",tokenP );
				estadoS=1000;
			}
			break;
	

		case 2:
			if(strcmp(lexemaP,"palabra")==0){
				strcpy(adress,tokenP);
				estadoS=1;
			}else{
				printf("Error no se reconoce la ruta\n");
				estadoS=1000;
			}
			break;


		case 3:
			if(strcmp(lexemaP,"palabra")==0){
				strcpy(nomb,tokenP);
				estadoS=1;
			}else{
				printf("Error no se reconoce el nombre\n");
				estadoS=1000;
			}
			break;
	}
}

//---------------------------------------AUTOMATA UNMOUNT-----------------------------------------------------------
void automata_unmount(){
	switch(estadoS){

		case 0:
			if(strcmp(tokenP,"unmount")==0){
				estadoS=1;
			}
			break;

		case 1:
			if(strcmp(lexemaP,"id")==0){
				estadoS=2;
			}else{
				estadoS=0;
				unmount=0;
				printf("Error se esperaba el comodin id\n");
			}
			break;
	
		case 2:
			if(strcmp(lexemaP,"palabra")==0){
				estadoS=3;
				strcpy(nomb,tokenP);
			}else{
				estadoS=0;
				unmount=0;
				printf("Se esperaba otro tipo de valor\n");
			}
			break;
	
		case 3:
			if(strcmp(tokenP,"finInstruccion")==0){
				estadoS=0;
				unmount=0;
				if(buscarMontado(nomb)==0){
					printf("Error no se encuentra esa unidad montada\n");
				}else{
					printf("Unidad desmontada con exito\n");
				}



			}else{
				printf("Error no se reconoce: %s\n",tokenP);
				estadoS=0;
				unmount=0;
			}
	}
}

//---------------------------------------AUTOMATA EXEC--------------------------------------------------------------
void automata_exec(){
	switch(estadoS){
		case 0:
			estadoS=1;
			break;

		case 1:
			if(strcmp(lexemaP,"path")==0){
				estadoS=2;
			}else{
				estadoS=0;
				exec=0;
				printf("Error se esperaba path\n");
			}
			break;
	
		case 2:
			if(strcmp(lexemaP,"palabra")==0){
				estadoS=3;
				strcpy(adress,tokenP);
			}else{
				estadoS=0;
				exec=0;
				printf("No se reconoce la direccion\n");
			}
			break;
	
		case 3:
			estadoS=0;
			exec=0;
			if(strcmp(lexemaP,"finInstruccion")==0){
				FILE *archivo;
				char str[1500];
				if(archivo=fopen(adress,"rb+")){
					char* line=NULL;
					size_t len=0;
					ssize_t read;
					while(fgets(str, 1500, archivo) != NULL){

							estadoS=0;
							memset(tokenP, 0, sizeof(tokenP));
							memset(lexemaP, 0, sizeof(lexemaP));
							memset(adress, 0, sizeof(adress));	
							memset(fitt, 0, sizeof(fitt));
						char *apun=str;
						if(*apun!=10 && *apun!='\0' && str[0]!=' '){
							concatenarPalabras(apun,"Tokens2.txt");
							analisisSintactico("Tokens2.txt");
						}

					}
				}else{
					printf("Error no existe la direccion\n");
				}
			}else{
				printf("No se reconoce el comando: %s\n",tokenP);
			}
	}
}


//---------------------------------------AUTOMATA MKFS-----------------------------------------------------------------
void automata_mkfs(){
	switch(estadoS){
		case 0:
			estadoS=1;
			break;
	
		case 1:
			if(strcmp(lexemaP,"id")==0){
				flagidmkfs=1;
				estadoS=2;
			}else if(strcmp(lexemaP,"type")==0){
				flagtypemkfs=1;
				estadoS=3;
			}else if(strcmp(lexemaP,"fs")==0){
				flagfsmkfs=1;
				estadoS=4;
			}else if(strcmp(lexemaP,"finInstruccion")==0){
				if(flagidmkfs){
					if(flagtypemkfs==0){
						strcpy(typmkfs,"full");
					}
					if(flagfsmkfs==0){
						strcpy(fsmkfs,"3fs");
					}

					struct Nodo *aux=buscarMount(idmkfs);
					if(aux){
                       int index=buscarParticionP(aux->direccion,aux->nombre);
                       if(index!=-1){
                            MBR auxmbr;
                            FILE *disco;
                            disco=fopen(aux->direccion,"rb+");
                            fread(&auxmbr,sizeof(MBR),1,disco);
                            if(flagfsmkfs==0){
                                iniciarFormateo3(auxmbr.mbr_partition[index].part_start,(auxmbr.mbr_partition[index].part_start+auxmbr.mbr_partition[index].part_size),auxmbr.mbr_partition[index].part_size,aux->direccion);
                            }else{
                                iniciarFormateo2(auxmbr.mbr_partition[index].part_start,(auxmbr.mbr_partition[index].part_start+auxmbr.mbr_partition[index].part_size),auxmbr.mbr_partition[index].part_size,aux->direccion);

                            }
                            fclose(disco);
                       }else{
                        index=buscarParticionL(aux->direccion,aux->nombre);
                        if(index!=-1){
                            ebr auxebr;
                            FILE *disco;
                            disco=fopen(aux->direccion,"rb+");
                            fseek(disco,index,SEEK_SET);
                            fread(&auxebr,sizeof(ebr),1,disco);
                            if(flagfsmkfs==0){
                                iniciarFormateo3(ftell(disco),ftell(disco)+auxebr.part_size-sizeof(ebr),auxebr.part_size-sizeof(ebr),aux->direccion);
                            }else{
                                iniciarFormateo2(ftell(disco),ftell(disco)+auxebr.part_size-sizeof(ebr),auxebr.part_size-sizeof(ebr),aux->direccion);

                            }
                            fclose(disco);
                        }
                       }
					}else{
						estadoS=0;
						mkfs=0;
						printf("Error no se encuentra la particion: %s para mkfs\n", idmkfs);
					}

				}else{
					estadoS=0;
					mkfs=0;
					printf("Error se esperaba id\n");
				}
			}else{
				estadoS=0;
				mkfs=0;
				printf("No se reconoce el comando: %s\n",tokenP);
			}
			break;

		case 2:
			if(strcmp(lexemaP,"palabra")==0){
				strcpy(idmkfs,tokenP);
				estadoS=1;
			}else{
				estadoS=0;
				mkfs=0;
				printf("Error no se reconoce el id \n");
			}
			break;

		case 3:
			if(strcmp(tokenP,"Fast")==0 || strcmp(tokenP,"fast")==0 || strcmp(tokenP,"Full")==0 || strcmp(tokenP,"full")==0){
				strcpy(typmkfs,tokenP);
				estadoS=1;
			}else{
				estadoS=0;
				mkfs=0;
				printf("No se reconoce el comando para type\n");
			}
			break;

		case 4:
			if(strcmp(tokenP,"2fs")==0 || strcmp(tokenP,"2Fs")==0 || strcmp(tokenP,"3fs")==0 || strcmp(tokenP,"3Fs")==0){
				if(strcmp(tokenP,"3fs")==0) flagfsmkfs=0;
                strcpy(fsmkfs,tokenP);
				estadoS=1;
			}else{
				estadoS=0;
				mkfs=0;
				printf("No se reconoce el sistema de archivos a formatear\n");
			}
			break;
	}
}

//---------------------------------------AUTOMATA LOGIN---------------------------------------------------------------
void automata_login(){
	switch(estadoS){
		case 0:
			estadoS=1;
			break;
	
		case 1:
			if(strcmp(lexemaP,"usr")==0){
				estadoS=2;
			}else if(strcmp(lexemaP,"pwd")==0){
				estadoS=3;
			}else if(strcmp(lexemaP,"id")==0){
				estadoS=4;
			}else if(strcmp(lexemaP,"finInstruccion")==0){
				if(flag_idlogin){
					if(flag_usr){
						if(flag_pwd){

							struct Nodo *aux=buscarMount(login_id);
							if(!flag_login){
								if(aux!=NULL){
									int resultado=Loguear(aux->nombre,aux->direccion,login_user,login_password);
									if(resultado==1){
										estadoS=0;
										login=0;
										printf("Usuario o contraseña no coinciden \n");
									}else if(resultado==0){
										estadoS=0;
										login=0;
										printf("La particion no ha sido formateada\n");
									}else{
										if(strcmp(login_user,"root")==0){
											flag_root=1;
										}
										flag_login=1;
                                        memset(direccion_global,0,sizeof(direccion_global));
                                        strcpy(direccion_global,aux->direccion);
										printf("Logueado con exito\n");
									}
								}else{
									estadoS=0;
									login=0;
									printf("Error no esta montada la particion a la que intenta loguearse \n");
								}
							}else{
								estadoS=0;
								login=0;
								printf("Ya existe una sesion activa por favor hacer un Logout antes\n" );
							}

						}else{
							estadoS=0;
							login=0;
							printf("Error falta el comodin pwd\n");
						}
					}else{
						estadoS=0;
						login=0;
						printf("Error falta el comodin usr\n");
					}
				}else{
					estadoS=0;
					login=0;
					printf("Error falta el comodin id\n");
				}
			}else{
				estadoS=0;
				login=0;
				printf("Error no se reconoce el comando: %s\n", tokenP);
			}
			break;

		case 2:
			if(strcmp(lexemaP,"palabra")==0){
				strcpy(login_user,tokenP);
				flag_usr=1;
				estadoS=1;
			}else{
				estadoS=0;
				login=0;
				printf("Error no se reconoce el valor:  %s\n",tokenP);
			}
			break;
	
		case 3:
			estadoS=1;
			strcpy(login_password,tokenP);
			flag_pwd=1;
			break;
	
		case 4:
			if(strcmp(lexemaP,"palabra")==0){
				estadoS=1;
				flag_idlogin=1;
				strcpy(login_id,tokenP);
			}else{
				estadoS=0;
				login=0;
				printf("No se reconoce el valor: %s\n",tokenP);
			}
			break;
	}
}

//---------------------------------------AUTOMATA MKGRP----------------------------------------------------------------
void automata_mkgrp(){
	switch(estadoS){
		case 0:
			estadoS=1;
			break;
	
		case 1:
			if(strcmp(lexemaP,"name")==0){
				estadoS=2;
			}else{
				estadoS=0;
				mkgrp=0;
				printf("Error se esperaba el comodin name\n");
			}
			break;
	
		case 2:
			if(strcmp(lexemaP,"palabra")==0){
				estadoS=3;
                memset(mk_name,0,sizeof(mk_name));
				strcpy(mk_name,tokenP);
			}else{
				estadoS=0;
				mkgrp=0;
				printf("Se esperaba una palabra\n");
			}
			break;
	
		case 3:
			if(strcmp(lexemaP,"finInstruccion")==0){
				if(flag_login){
					if(flag_root){
						if(buscarGrupo(mk_name)==-1){
							int nuevoid=buscarUltimoGrupo();
                            if(nuevoid!=-1){


    							nuevoid++;
                                FILE *disco;
                                disco=fopen(direccion_global,"r+b");
                                if(strlen(mk_name)<10){


                                    char temp[66]="\0";
                                    memset(temp,0,sizeof(temp));
                                    sprintf(temp,"%s%d",temp,nuevoid);
                                    strcat(temp,",G,");
                                    strcat(temp,mk_name);
                                    strcat(temp,"\n");
                                    

                                    int res=agregarGrupo(disco,temp);
                                    if(res){

                                        printf("Grupo agregado con exito \n");
                                    }
                                    fclose(disco);
                                    estadoS=0;
                                    mkgrp=0;
                                }else{
                                    printf("Error el nombre de grupo no puede exceder los 10 caracteres\n");
                                }
                            }
                            estadoS=0;
                            mkgrp=0;
						}else{
							estadoS=0;
							mkgrp=0;
							printf("Ya existe un grupo con ese nombre\n");
						}
					}else{
						estadoS=0;
						mkgrp=0;
						printf("Solo el usuario root puede ejecutar el comando mkgrp\n");
					}
				}else{
					estadoS=0;
					mkgrp=0;
					printf("Se necesita hacer un login antes\n");
				}
			}else{
				estadoS=0;
				mkgrp=0;
				printf("No se reconoce el comando\n");
			}
	}
}

//---------------------------------------AUTOMATA RMGRP------------------------------------------------------------------
void automata_rmgrp(){
    switch(estadoS){
        case 0:
            estadoS=1;
            break;

        case 1:
            if(strcmp(lexemaP,"name")==0){
                estadoS=2;
            }else{
                estadoS=0;
                rmgrp=0;
                printf("Error se esperaba el comodin name\n");
            }
            break;
    
        case 2:
            if(strcmp(lexemaP,"palabra")==0){
                estadoS=3;
                strcpy(mk_name,tokenP);
            }else{
                estadoS=0;
                rmgrp=0;
                printf("Se esperaba una palabra\n");
            }
            break;
    
        case 3:
            if(strcmp(lexemaP,"finInstruccion")==0){
                if(flag_login){
                    if(flag_root){
                        printf("buscnado: %s\n",mk_name);
                        if(buscarGrupo(mk_name)>-1){
                           eliminarGrupo(mk_name);
                           printf("Grupo eliminado con exito\n");
                        }else{
                            estadoS=0;
                            rmgrp=0;
                            printf("No existe un grupo con ese nombre\n");
                        }
                    }else{
                        estadoS=0;
                        mkgrp=0;
                        printf("Solo el usuario root puede ejecutar el comando mkgrp\n");
                    }
                }else{
                    estadoS=0;
                    mkgrp=0;
                    printf("Se necesita hacer un login antes\n");
                }
            }else{
                estadoS=0;
                mkgrp=0;
                printf("No se reconoce el comando\n");
            }
    }
}

//---------------------------------------AUTOMATA MKUSR----------------------------------------------------------------
void automata_mkusr(){
    switch(estadoS){

        case 0:
            estadoS=1;
        break;

        case 1:
            if(strcmp(lexemaP,"name")==0){
                estadoS=2;
                flag_usr_usr=1;
            }else if(strcmp(lexemaP,"pwd")==0){
                estadoS=3;
                flag_usr_pwd=1;
            }else if(strcmp(lexemaP,"grp")==0){
                estadoS=4;
                flag_usr_grp=1;
            }else if(strcmp(lexemaP,"finInstruccion")==0){
                if(flag_login){
                    if(flag_root){
                        if(flag_usr_usr){
                            if(flag_usr_grp){
                                if(flag_usr_pwd){
                                    if(buscarGrupo(usr_grp)>-1){
                                        if(buscarUsuario(usr_usr)==0){
                                            int numeroid=buscarnuevoID();
                                            numeroid++;
                                                if(strlen(usr_usr)<10 && strlen(usr_pass)<10){
                                                    char enviar[66];
                                                    memset(enviar,0,sizeof(enviar));

                                                    sprintf(enviar,"%d,U,%s,%s,%s\n",numeroid,usr_grp,usr_usr,usr_pass);
                                                    FILE *archivo;
                                                    archivo=fopen(direccion_global,"rb+");
                                                    int res=agregarGrupo(archivo,enviar);
                                                    estadoS=0;
                                                    mkusr=0;
                                                    printf("Usuario creado con exito\n");
                                                    fclose(archivo);
                                                }else{
                                                    printf("Error usuario y contraseña no pueden exceder los 10 caracteres\n");
                                                }
                                                
                                           
                                        }else{
                                            printf("El usuario ya existe\n");
                                        }
                                        
                                        
                                    }else{
                                        printf("Error no se encuentra el grupo\n");
                                        estadoS=0;
                                        mkusr=0;
                                    }
                                }else{
                                    printf("Error se esperaba el comodin pwd\n");
                                    estadoS=0;
                                    mkusr=0;
                                }
                            }else{
                                printf("Se esperaba el comodin grp\n");
                                estadoS=0;
                                mkusr=0;
                            }
                        }else{
                            printf("Se esperaba el comodin usr\n");
                            estadoS=0;
                            mkusr=0;
                        }
                    }else{
                        printf("Esta accion solo la puede ejecutar el usuario root\n");
                        estadoS=0;
                        mkusr=0;
                    }
                }else{
                    printf("Error no hay una sesion inciada\n");
                    estadoS=0;
                    mkusr=0;
                }
                
            }else{
                printf("Error no se reconoce el comando: %s\n",tokenP);
                estadoS=0;
                mkusr=0;
            }
        break;


        case 2:
            if(strcmp(lexemaP,"palabra")==0){
                estadoS=1;
                strcpy(usr_usr,tokenP);
            }else{
                printf("Error se esperaba una palabra\n");
                estadoS=0;
                mkusr=0;
            }
        break;

        case 3:
            strcpy(usr_pass,tokenP);
            estadoS =1;
        break;
    

        case 4:
            if(strcmp(lexemaP,"palabra")==0){
                strcpy(usr_grp,tokenP);
                estadoS=1;
            }else{
                estadoS=0;
                mkusr=0;
                printf("Error se esperaba una palabra\n");
            }
        break;
    }
}


//---------------------------------------AUTOMATA RMUSR----------------------------------------------------------------------
void automata_rmusr(){
    switch(estadoS){
        case 0:
            estadoS=1;
        break;

        case 1:
            if(strcmp(lexemaP,"usr")==0){
                estadoS=2;
            }else{
                printf("Error: se esperaba usr\n");
                estadoS=0;
                rmusr=0;
            }
        break;
    

        case 2:
            if(strcmp(lexemaP,"palabra")==0){
                strcpy(usr_usr,tokenP);
                estadoS=3;
            }else{
                estadoS=0;
                rmusr=0;
                printf("ERROR: se esperaba una palabra\n");
            }
        break;

        case 3:
            if(strcmp(lexemaP,"finInstruccion")==0){

                if(flag_login){
                    if(flag_root){
                        if(buscarUsuario(usr_usr)==1){
                            eliminarUsuario(usr_usr);
                            printf("Usuario eliminado con exito\n");
                            estadoS=0;
                            rmusr=0;
                        }else{
                            estadoS=0;
                            rmusr=0;
                            printf("ERROR: no existe el usuario\n");
                        }
                    }else{
                        estadoS=0;
                        rmusr=0;
                        printf("ERROR: solo el usuario root puede ejecutar esta accion\n");
                    }
                }else{
                    estadoS=0;
                    rmusr=0;
                    printf("ERROR: no hay una sesion iniciada\n");
                }

            }else{
                estadoS=0;
                rmusr=0;
                printf("ERROR: no se reconoce el comando %s\n",tokenP);
            }
        break;
    }
}

//--------------------------------------AUTOMATA MKDIR----------------------------------------------------------
void automata_mkdir(){
    switch(estadoS){
        case 0:
            estadoS=1;
        break;

        case 1:
            if(strcmp(lexemaP,"p")==0){
                estadoS=1;
                flag_mkdir_p=1;
            }else if(strcmp(lexemaP,"path")==0){
                estadoS=2;
                flag_mkdir_path=1;
            }else if(strcmp(lexemaP,"finInstruccion")==0){
                if(flag_login){
                    if(flag_mkdir_path){
                        char temp_mk[100];
                        memset(temp_mk,0,sizeof(temp_mk));
                        strcpy(temp_mk,mk_path);
                        char *pp=mk_path;
                        int res=CREARCARPETA(pp,flag_mkdir_p);
                        if(res==1){ 
                            printf("Carpeta creada con exito\n");
                            if(flagfsmkfs==0) guardarJournal(flag_mkdir_p,1,664,temp_mk,"vacio");
                            
                        }
                        else if(res==0) printf("La direccion ya existe\n");
                        else if(res==3) printf("No se tienen los permisos de escritura\n");
                        else if(res==2) printf("La direccion no existe y no se puede crear por falta del comando -p\n");
                        estadoS=0;
                        mkdir=0;
                    }else{
                        printf("Se esperaba path \n");
                        estadoS=0;
                        mkdir=0;
                    }
                }else{
                    printf("Error se necesita loguearse antes\n");
                    estadoS=0;
                    mkdir=0;
                }
            }else{
                printf("Error no se reconoce el comando: %s\n",tokenP);
                estadoS=0;
                mkdir=0;
            }
        break;

        case 2:
            if(strcmp(lexemaP,"palabra")==0){
                memset(mk_path,0,sizeof(mk_path));
                strcpy(mk_path,tokenP);
                estadoS=1;
            }else{
                printf("Se esperaba una direccion\n");
                estadoS=0;
                mkdir=0;
            }
        break;
    }
}


//--------------------------------------AUTOMATA MKFILE-----------------------------------------------------
void automata_mkfile(){
    switch(estadoS){
        case 0:
            estadoS=1;
        break;

        case 1:
            if(strcmp(lexemaP,"path")==0){
                estadoS=2;
                flag_mkfile_path=1;
            }else if(strcmp(lexemaP,"p")==0){
                estadoS=1;
                flag_mkfile_p=1;
            }else if(strcmp(lexemaP,"size")==0){
                estadoS=3;
            }else if(strcmp(lexemaP,"cont")==0){
                estadoS=4;
                flag_mkfile_cont=1;
            }else if(strcmp(lexemaP,"finInstruccion")==0){
                if(flag_login){
                    if(flag_mkfile_path){
                        char temp_mk[100];
                        memset(temp_mk,0,sizeof(temp_mk));
                        strcpy(temp_mk,mkfile_path);

                        char temp_mk2[100];
                        memset(temp_mk2,0,sizeof(temp_mk2));
                        strcpy(temp_mk2,mkfile_cont);

                        char *pa=mkfile_path;
                        char *co=mkfile_cont;

                        char *pa2=mkfile_path;
                        char *co2=mkfile_cont;

                        int res=CREARARCHIVO(pa,flag_mkfile_p,mkfile_size,co,0);

                        if(res==6){ 
                            printf("Archivo creado con exito\n");
                            if(flagfsmkfs==0) guardarJournal(flagfsmkfs,0,664,temp_mk,temp_mk2);
                        }
                        else if(res==5) printf("La direccion ya existe\n");
                        else if(res==3) printf("No se tienen los permisos de escritura\n");
                        else if(res==4) printf("La direccion no existe y no se puede crear por falta del comando -p\n");
                        else if(res==7) printf("No se puede abrir el archivo cat\n");
                        else if(res==9) printf("No se tienen los permisos de escritura\n");
                    }else{
                        printf("ERROR se necesita el comodin path\n");
                    }
                }else{
                    printf("ERROR se necesita una sesion inciada\n");
                }
                estadoS=0;
                mkfile=0;
            }
        break;

        case 2:
            if(strcmp(lexemaP,"palabra")==0){
                strcpy(mkfile_path,tokenP);
                estadoS=1;
            }else{
                printf("ERROR se esperaba una direccion\n");
                estadoS=0;
                mkfile=0;
            }
        break;

        case 3:
            if(strcmp(lexemaP,"numero")==0){
                if(atoi(tokenP)>-1){
                    estadoS=1;
                    mkfile_size=atoi(tokenP);
                }else{
                    printf("ERROR el numero tenia que ser positivo\n");
                    estadoS=0;
                    mkfile=0;
                }
            }else{
                printf("Se esperaba un numero\n");
                estadoS=0;
                mkfile=0;
            }
        break;
    

        case 4:
            if(strcmp(lexemaP,"palabra")==0){
                estadoS=1;
                strcpy(mkfile_cont,tokenP);
            }else{
                printf("Se esperaba una direccion\n");
                estadoS=0;
                mkfile=0;
            }
        break;
    }
}

//--------------------------------------AUTOMATA LOSS----------------------------------------------------------------
void automata_loss(){
    switch(estadoS){
        case 0:
            estadoS=1;
        break;

        case 1:
            if(strcmp(lexemaP,"id")==0){
                estadoS=2;
            }else{
                printf("Error no se reconoce el token: %s \n", tokenP);
                estadoS=0;
                loss=0;
            }
        break;
    
        case 2:
            strcpy(loss_id,tokenP);
            estadoS=3;
        break;
   
        case 3:
            if(strcmp(lexemaP,"finInstruccion")==0){
                if(flag_login){
                    if(flag_root){
                        if(!flagfsmkfs) metodoLoss(inicio_bi,fin_blo);
                        else printf("No se puede ejecutar en un sistema EXT2\n");
                    }else  printf("Se necesita permisos root para ejecutar la accion\n");
                }else printf("Se necesita iniciar sesion antes\n");
            }else{
                printf("No se reconoce el comando: %s\n",tokenP);
            }
            estadoS=0;
            loss=0;
        break;
    }
}


//--------------------------------------AUTOMATA RECOVERY----------------------------------------------------------------
void automata_recovery(){
    switch(estadoS){
        case 0:
            estadoS=1;
        break;

        case 1:
            if(strcmp(lexemaP,"id")==0){
                estadoS=2;
            }else{
                printf("Error no se reconoce el token: %s \n", tokenP);
                estadoS=0;
                loss=0;
            }
        break;
    
        case 2:
            strcpy(loss_id,tokenP);
            estadoS=3;
        break;
   
        case 3:
            if(strcmp(lexemaP,"finInstruccion")==0){
                if(flag_login){
                    if(flag_root){
                        if(!flagfsmkfs){

                            struct Nodo *aux=buscarMount(loss_id);
                            if(aux){
                               int index=buscarParticionP(aux->direccion,aux->nombre);
                               if(index!=-1){
                                    MBR auxmbr;
                                    FILE *disco;
                                    disco=fopen(aux->direccion,"rb+");
                                    fread(&auxmbr,sizeof(MBR),1,disco);
                                    iniciarFormateo3(auxmbr.mbr_partition[index].part_start,(auxmbr.mbr_partition[index].part_start+auxmbr.mbr_partition[index].part_size),auxmbr.mbr_partition[index].part_size,aux->direccion);
                                    fclose(disco);
                               }else{
                                index=buscarParticionL(aux->direccion,aux->nombre);
                                if(index!=-1){
                                    ebr auxebr;
                                    FILE *disco;
                                    disco=fopen(aux->direccion,"rb+");
                                    fseek(disco,index,SEEK_SET);
                                    fread(&auxebr,sizeof(ebr),1,disco);
                                    iniciarFormateo3(ftell(disco),ftell(disco)+auxebr.part_size-sizeof(ebr),auxebr.part_size-sizeof(ebr),aux->direccion);
                                    fclose(disco);
                                }
                               }
                            }



                            JOURNAL journal;
                            FILE* disco;
                            disco=fopen(direccion_global,"rb+");
                            fseek(disco,inicio_super+sizeof(superBloque),SEEK_SET);
                            
                            while(ftell(disco)<inicio_journal){
                                fread(&journal,sizeof(JOURNAL),1,disco);                                
                                if(journal.Journal_tipo_operacion==0 || journal.Journal_tipo_operacion==1){
                                    if(journal.Journal_tipo){
                                        char *pp=journal.Journal_nombre;
                                        int res=CREARCARPETA(pp,journal.Journal_tipo_operacion);
                                        if(res==1){ 
                                            printf("Carpeta creada con exito\n");
                                        }
                                        else if(res==0) printf("La direccion ya existe\n");
                                        else if(res==3) printf("No se tienen los permisos de escritura\n");
                                        else if(res==2) printf("La direccion no existe y no se puede crear por falta del comando -p\n");
                                    }else{
                                        char *pa=journal.Journal_nombre;
                                        char *co=journal.Journal_contenido;
                                        int res=CREARARCHIVO(pa,journal.Journal_tipo_operacion,20,co,0);
                                        if(res==6){ 
                                            printf("Archivo creado con exito\n");
                                        }
                                        else if(res==5) printf("La direccion ya existe\n");
                                        else if(res==3) printf("No se tienen los permisos de escritura\n");
                                        else if(res==4) printf("La direccion no existe y no se puede crear por falta del comando -p\n");
                                        else if(res==7) printf("No se puede abrir el archivo cat\n");
                                    }
                                    
                                }
                                
                            }
                        

                        }
                        else printf("No se puede ejecutar en un sistema EXT2\n");
                    }else  printf("Se necesita permisos root para ejecutar la accion\n");
                }else printf("Se necesita iniciar sesion antes\n");
            }else{
                printf("No se reconoce el comando: %s\n",tokenP);
            }
            estadoS=0;
            recovery=0;
        break;
    }
}

//--------------------------------------AUTOMATA CONVERT----------------------------------------------------------------
void automata_convert(){
    switch(estadoS){
        case 0:
            estadoS=1;
        break;

        case 1:
            if(strcmp(lexemaP,"id")==0){
                estadoS=2;
            }else{
                printf("Error no se reconoce el token: %s \n", tokenP);
                estadoS=0;
                loss=0;
            }
        break;
    
        case 2:
            strcpy(loss_id,tokenP);
            estadoS=3;
        break;
   
        case 3:
            if(strcmp(lexemaP,"finInstruccion")==0){
                if(flag_login){
                    if(flag_root){
                        if(flagfsmkfs){

                            struct Nodo *aux=buscarMount(loss_id);
                            if(aux){
                               int index=buscarParticionP(aux->direccion,aux->nombre);
                               if(index!=-1){
                                    MBR auxmbr;
                                    FILE *disco;
                                    disco=fopen(aux->direccion,"rb+");
                                    fread(&auxmbr,sizeof(MBR),1,disco);
                                    iniciarFormateo3(auxmbr.mbr_partition[index].part_start,(auxmbr.mbr_partition[index].part_start+auxmbr.mbr_partition[index].part_size),auxmbr.mbr_partition[index].part_size,aux->direccion);
                                    fclose(disco);
                               }else{
                                index=buscarParticionL(aux->direccion,aux->nombre);
                                if(index!=-1){
                                    ebr auxebr;
                                    FILE *disco;
                                    disco=fopen(aux->direccion,"rb+");
                                    fseek(disco,index,SEEK_SET);
                                    fread(&auxebr,sizeof(ebr),1,disco);
                                    iniciarFormateo3(ftell(disco),ftell(disco)+auxebr.part_size-sizeof(ebr),auxebr.part_size-sizeof(ebr),aux->direccion);
                                    fclose(disco);
                                }
                               }
                               printf("Conversion a EXT3 correctamente, el disco se formateo\n");
                               flagfsmkfs=1;
                            }
                        }else printf("No se puede convertir por que ya es una EXT3\n");
                    }else  printf("Se necesita permisos root para ejecutar la accion\n");
                }else printf("Se necesita iniciar sesion antes\n");
            }else{
                printf("No se reconoce el comando: %s\n",tokenP);
            }
            estadoS=0;
            recovery=0;
        break;
    }
}

//--------------------------------------AUTOMATA CHMOD---------------------------------------------------------------------------

void automata_chmod(){
    switch(estadoS){
        case 0:
            estadoS=1;
        break;

        case 1:
            if(strcmp(lexemaP,"path")==0){
                flag_ch_path=1;
                estadoS=2;
            }else if(strcmp(lexemaP,"ugo")==0){
                flag_ch_ugo=1;
                estadoS=3;
            }else if(strcmp(lexemaP,"r")==0){
                flag_ch_R=1;
                estadoS=1;
            }else if(strcmp(lexemaP,"finInstruccion")==0){
                if(flag_login){
                    if(flag_ch_path){
                        if(flag_ch_ugo){
                            int numeracion=0;
                            FILE* disco;
                            disco=fopen(direccion_global,"rb+");
                            int ExisteCarpeta = BUSCARCARPETAOARCHIVO(disco,chmod_path,inicio_super,&numeracion);
                            if(ExisteCarpeta){
                                inodeTable inodo;
                                char byte='0';
                                fseek(disco,inicio_inode+(sizeof(inodeTable)*numeracion),SEEK_SET);
                                fread(&inodo,sizeof(inodeTable),1,disco);
                                int permisos=chmod_ugo;
                                 int milla=(permisos/1000);
                                int usuari=(permisos-(milla*1000))/100;
                                int gru=(permisos-(milla*1000+usuari*100))/10;
                                int otros=permisos-(milla*1000+usuari*100+gru*10);
                                if((usuari>-1 && usuari<8) && (gru>-1 && gru<8) && (otros>-1 && otros<8)){
                                    if(flag_root || userlog.id_user==inodo.i_uid){
                                         if(flag_ch_R){
                                            cambiarpermisos(disco,numeracion,permisos);
                                         }else{
                                            inodo.i_perm=permisos;
                                            fseek(disco,inicio_inode+(sizeof(inodeTable)*numeracion),SEEK_SET);
                                            fwrite(&inodo,sizeof(inodeTable),1,disco);
                                        }
                                        printf("Permisos cambiados exitosamente\n");
                                    }else printf("Para cambiar los permisos debe ser usuario root o ser dueño de la carpeta\n");  
                                }else printf("ERROR los permisos tienen que estar en un rango[0-7]\n");       
                            fclose(disco);
                            }else printf("Error no existe el disco que quiere acceder\n");
                        }else printf("Error se necesita comodin ugo\n");
                    }else printf("Error se necesita comodin path\n");
                }else printf("Error se necesita iniciar sesion antes\n");
                estadoS=0;
                chmod=0;
            }else{
                printf("Error no se reconoce el comando: %s\n",tokenP);
                estadoS=0;
                chmod=0;
            }
        break;


        case 2:
            strcpy(chmod_path,tokenP);
            estadoS=1;
        break;

        case 3:
            chmod_ugo=atoi(tokenP);
            estadoS=1;
        break;
    }
}


//--------------------------------------AUTOMATA CAT---------------------------------------------------------------------
void automata_cat(){
    switch(estadoS){
        case 0:
            estadoS=1;
        break;

        case 1:
            if(strcmp(lexemaP,"palabra")==0){
                char *p=tokenP;
                num_cat=buscarNumeroPalabra(p);
                if(num_cat>-1){ 
                    estadoS=2;
                }
                else{
                    printf("Error no se acepta ese numero para fileN\n");
                    estadoS=0;
                    cat=0;
                }
            }else if(strcmp(lexemaP,"finInstruccion")==0){
                struct Lista *aux=primeroLista;
                if(aux!=NULL){
                    printf("----COMANDO CAT-------\n");
                    while(aux!=NULL){
                        printf("%s\n",aux->cadena);
                        free(aux);
                        aux=aux->siguiente;
                    }
                    primeroLista=NULL;
                }else printf("ERROR almenos debe haber un FileN\n");
                estadoS=0;
                cat=0;
            }else{
                estadoS=0;
                cat=0;
                printf("ERROR no se reconoce el comodin %s\n",tokenP);   
            }
        break;

        case 2:
        {
            int numeracion=0;
            FILE *disco;
            disco=fopen(direccion_global,"rb+");
            int ExisteCarpeta = BUSCARCARPETAOARCHIVO(disco,tokenP,inicio_super,&numeracion);
            if(ExisteCarpeta){
                inodeTable inodo;
                char cade[1000];
                memset(cade,0,sizeof(cade));
                strcpy(cade,"");
                fseek(disco,inicio_inode+(sizeof(inodeTable)*numeracion),SEEK_SET);
                fread(&inodo,sizeof(inodeTable),1,disco);
                int per=permisosdeLectura(inodo.i_perm,inodo.i_uid==userlog.id_user,inodo.i_gid==userlog.id_grup);
                if(per){
                    for(int i=0;i<12;i++){
                        if(inodo.i_block[i]!=-1){
                            bloqueArchivo archivo;
                            fseek(disco,inicio_blo+(sizeof(bloqueCarpetas)*inodo.i_block[i]),SEEK_SET);
                            fread(&archivo,sizeof(bloqueCarpetas),1,disco);
                            strcat(cade,archivo.b_content);
                        }else break;
                    }
                    strcat(cade,"\n");
                    int res=insertarOrdenado(num_cat,cade);
                    if(!res) printf("Ya existe un archivo en esa posicion\n");
                }else printf("No se tienen los suficientes permisos de Lectura\n");
            }else{
                printf("Error no se encuentra la direccion: %s\n",tokenP);
            }
            fclose(disco);
            estadoS=1;
        }

    }
}


//--------------------------------------AUTOAMTA MV---------------------------------------------------------------------------------
void automata_mv(){
    switch(estadoS){

        case 0:
            estadoS=1;
        break;

        case 1:
            if(strcmp(lexemaP,"path")==0){
                estadoS=2;
                flag_mv_path=1;
            }else if(strcmp(lexemaP,"dest")==0){
                estadoS=3;
                flag_mv_dest=1;
            }else if(strcmp(lexemaP,"finInstruccion")==0){
                if(flag_login){
                    if(flag_mv_path){
                        if(flag_mv_dest){
                            int numeracion=0;
                            int numeracion2=0;

                            char temp_dest[100];
                            memset(temp_dest,0,sizeof(temp_dest));
                            strcpy(temp_dest,mv_dest);

                            char temp_path[100];
                            memset(temp_path,0,sizeof(temp_path));
                            strcpy(temp_path,mv_path);


                            FILE* disco;
                            disco=fopen(direccion_global,"rb+");
                            int existe=BUSCARCARPETAOARCHIVO(disco,mv_path,inicio_super,&numeracion);
                            int auxn=numeracion;
                            int existe2=BUSCARCARPETAOARCHIVO(disco,mv_dest,inicio_super,&numeracion2);
                            if(existe){
                                if(existe2){
                                    int perm=permisosLecturaRecursividad(disco,numeracion);
                                    if(perm){
                                        inodeTable inodo;
                                        fseek(disco,inicio_inode+(sizeof(inodeTable)*numeracion2),SEEK_SET);
                                        fread(&inodo,sizeof(inodeTable),1,disco);
                                        int perm2=permisosDeEscritura(inodo.i_perm,inodo.i_uid==userlog.id_user,inodo.i_gid==userlog.id_grup);
                                        if(perm2){
                                            fseek(disco,inicio_inode+(sizeof(inodeTable)*numeracion),SEEK_SET);
                                            inodeTable iaux;
                                            fread(&iaux,sizeof(inodeTable),1,disco);
                                            if(iaux.i_type!=49){
                                                bloqueCarpetas caux;
                                                fseek(disco,inicio_blo+(sizeof(bloqueCarpetas)*iaux.i_block[0]),SEEK_SET);
                                                fread(&caux,sizeof(bloqueCarpetas),1,disco);
                                                caux.b_content[1].b_inodo=numeracion2;
                                                fseek(disco,inicio_blo+(sizeof(bloqueCarpetas)*iaux.i_block[0]),SEEK_SET);
                                                fwrite(&caux,sizeof(bloqueCarpetas),1,disco); 
                                            }


                                            

                                            int guardado=0;
                                            int num=0;
                                            int auxapu=0;
                                            int ind=0;

                                            char temp_nomb[20];
                                            memset(temp_nomb,0,sizeof(temp_nomb));


                                            int perm3=BUSCARCARPETAOARCHIVO2(disco,temp_path,inicio_super,&num,&ind);
                                            bloqueCarpetas carpeta;
                                            fseek(disco,inicio_blo+(sizeof(bloqueCarpetas)*num),SEEK_SET);
                                            fread(&carpeta,sizeof(bloqueCarpetas),1,disco);
                                            auxapu=carpeta.b_content[ind].b_inodo;
                                            strcpy(temp_nomb,carpeta.b_content[ind].b_name);
                                            memset(carpeta.b_content[ind].b_name,0,sizeof(carpeta.b_content[ind].b_name));
                                            carpeta.b_content[ind].b_inodo=-1;
                                            fseek(disco,inicio_blo+(sizeof(bloqueCarpetas)*num),SEEK_SET);
                                            fwrite(&carpeta,sizeof(bloqueCarpetas),1,disco);

                                            for(int i=0;i<12;i++){
                                                if(inodo.i_block[i]!=-1 && guardado==0){
                                                    char byte='0';
                                                    bloqueCarpetas carp;
                                                    fseek(disco,inicio_bb+(sizeof(char)*inodo.i_block[i]),SEEK_SET);
                                                    byte=fgetc(disco);
                                                    if(byte=='1'){
                                                      fseek(disco,inicio_blo+(sizeof(bloqueCarpetas)*inodo.i_block[i]),SEEK_SET);  
                                                      fread(&carp,sizeof(bloqueCarpetas),1,disco);
                                                      for(int j=0;j<4;j++){
                                                        if(carp.b_content[j].b_inodo==-1){
                                                            carp.b_content[j].b_inodo=auxapu;
                                                            strcpy(carp.b_content[j].b_name,temp_nomb);
                                                            fseek(disco,inicio_blo+(sizeof(bloqueCarpetas)*inodo.i_block[i]),SEEK_SET);
                                                            fwrite(&carp,sizeof(bloqueCarpetas),1,disco);
                                                            printf("Movido con exito\n" );
                                                            guardado=1;
                                                            break;
                                                        }
                                                      }
                                                    }
                                                }else{

                                                    if(guardado==0){

                                                        char auxbituno='1';
                                                        int bitlibre2 = buscarPrimerBloque(disco,inicio_super,2,F);
                                                        inodo.i_block[i]=bitlibre2;
                                                        fseek(disco,inicio_inode+(sizeof(inodeTable)*numeracion2),SEEK_SET);
                                                        fwrite(&inodo,sizeof(inodeTable),1,disco);

                                                        bloqueCarpetas NueC;
                                                        NueC.b_content[0].b_inodo =numeracion;
                                                        strcpy(NueC.b_content[0].b_name,temp_nomb);
                                                        NueC.b_content[1].b_inodo =-1;
                                                        strcpy(NueC.b_content[1].b_name,"");
                                                        NueC.b_content[2].b_inodo =-1;
                                                        strcpy(NueC.b_content[2].b_name,"");
                                                        NueC.b_content[3].b_inodo =-1;
                                                        strcpy(NueC.b_content[3].b_name,"");
                                                        fseek(disco,inicio_blo +(sizeof(bloqueCarpetas)*bitlibre2),SEEK_SET);
                                                        fwrite(&NueC,sizeof(bloqueCarpetas),1,disco);
                                                        fseek(disco,inicio_bb + (bitlibre2*sizeof(char)),SEEK_SET);
                                                        fwrite(&auxbituno,sizeof(char),1,disco);
                                                        fseek(disco,inicio_super,SEEK_SET);
                                                        superBloque sb;
                                                        fread(&sb,sizeof(superBloque),1,disco);
                                                        sb.s_free_block_count = sb.s_free_block_count - 1;
                                                        sb.s_first_blo = sb.s_first_blo +1;
                                                        fseek(disco,inicio_super,SEEK_SET);
                                                        fwrite(&sb,sizeof(superBloque),1,disco);



                                                        guardado=1;
                                                        printf("Se ha movido con exito\n");
                                                        break;
                                                    }
                                                }
                                            }
                                            if(!guardado) printf("No se ha podido mover el archivo\n");




                                        }else printf("No hay permisos de escritura en path\n");


                                    }else printf("Alguna carpeta no posee permisos de Lectura no se pueden mover\n");
                                }else printf("No existe la direccion de dest\n");
                            }else printf("No existe la direccion de path\n");
                            
                            fclose(disco);
                        }else printf("Se necesita el comando dest\n");
                    }else printf("Se necesita el comando path\n");
                }else printf("Se necesita haber iniciado sesion\n");
                estadoS=0;
                mv=0;
            }else{
                estadoS=0;
                mv=0;
                printf("ERROR: no se reconoce el siguiente token: %s\n",tokenP );
            }
        break;


        case 2:
            strcpy(mv_path,tokenP);
            estadoS=1;
        break;

        case 3:
            strcpy(mv_dest,tokenP);
            estadoS=1;
        break;
    }
}

//-------------------------------------AUTOMATA REM---------------------------------------------------------------------------------
void automata_rem(){
    switch(estadoS){

        case 0:
            estadoS=1;
        break;

        case 1:
            if(strcmp(lexemaP,"path")==0){
                estadoS=2;
            }else{
                estadoS=0;
                rem=0;
                printf("ERROR: no se reconoce el siguiente token: %s\n",tokenP );
            }
        break;


        case 2:
            strcpy(mv_path,tokenP);
            estadoS=3;
        break;

        case 3:
            if(strcmp(lexemaP,"finInstruccion")==0){
                if(flag_login){
                    if(flag_mv_path){
                        
                            int numeracion=0;
                            int numeracion2=0;
                            int num=0;
                            int ind=0;

                            char temp_dest[100];
                            memset(temp_dest,0,sizeof(temp_dest));
                            strcpy(temp_dest,mv_path);

                            char temp_path[100];
                            memset(temp_path,0,sizeof(temp_path));
                            strcpy(temp_path,mv_path);


                            FILE* disco;
                            disco=fopen(direccion_global,"rb+");
                            int existe=BUSCARCARPETAOARCHIVO(disco,mv_path,inicio_super,&numeracion);
                            int auxn=numeracion;
                            if(existe){
                                    int perm=permisosEscrituraRecursividad(disco,numeracion);
                                    if(perm){
                                        int perm3=BUSCARCARPETAOARCHIVO2(disco,temp_path,inicio_super,&num,&ind);
                                            bloqueCarpetas carpeta;
                                            fseek(disco,inicio_blo+(sizeof(bloqueCarpetas)*num),SEEK_SET);
                                            fread(&carpeta,sizeof(bloqueCarpetas),1,disco);
                                            eliminarRecursividad(disco,carpeta.b_content[ind].b_inodo);
                                            memset(carpeta.b_content[ind].b_name,0,sizeof(carpeta.b_content[ind].b_name));
                                            carpeta.b_content[ind].b_inodo=-1;
                                            fseek(disco,inicio_blo+(sizeof(bloqueCarpetas)*num),SEEK_SET);
                                            fwrite(&carpeta,sizeof(bloqueCarpetas),1,disco);
                                            printf("ELIMINADO CON EXITO\n");
                                    }else printf("Alguna carpeta no posee permisos de Escritura no se pueden eliminar\n");
                            }else printf("No existe la direccion de path\n");
                            
                            fclose(disco);
                    }else printf("Se necesita el comando path\n");
                }else printf("Se necesita haber iniciado sesion\n");
                estadoS=0;
                mv=0;
            }
        break;
    }
}


int buscarNombreLogicas(char direccion[400],char nombre[100]){
	int d=-1;
	int retorno=0;
	FILE *disco;
	disco=fopen(direccion,"rb+");
	fseek(disco,0,SEEK_SET);
	MBR auxmbr;
	fread(&auxmbr,sizeof(MBR),1,disco);
	for(int i=0;i<4;i++){
		if(auxmbr.mbr_partition[i].part_status!='1'){
			if(strcmp(auxmbr.mbr_partition[i].part_name,nombre)==0 ){
				retorno=1;
			}else if(auxmbr.mbr_partition[i].part_type=='E'){
				d=i;
			}
		}
	}
	if(d!=-1){
		fseek(disco,auxmbr.mbr_partition[d].part_start,SEEK_SET);
		ebr auxebr;
		while((fread(&auxebr,sizeof(ebr),1,disco))!=0 && ftell(disco)<(auxmbr.mbr_partition[d].part_size+auxmbr.mbr_partition[d].part_start)){
		    	if(strcmp(auxebr.part_name,nombre)==0){
		    		retorno=1;
		    		break;
		    	}
		    if(auxebr.part_next!=-1){
			}else{
				break;
			}
		}
	}
	return retorno;
}




void crearArchivo(char direccion[400]){
						char *search2=direccion;
						char ext[30];
						char comando[400];
						
						strcpy(comando,"sudo mkdir -p \'");
						strcat(comando,getPath(search2));
						strcat(comando,"\'");
						system(comando);

						strcpy(comando,"sudo chmod -R 777 \'");
						strcat(comando,getPath(search2));
						strcat(comando,"\'");
						system(comando);

						FILE *arch;
						char buffer[500];
    					if(arch=fopen(direccion,"wb")){
         				fclose(arch);
         			}
}

void mostrarcambio(){
	struct Nodo *aux=primero;
	printf("----------------------Particiones montadas------------------------\n");
	while(aux!=NULL){
		printf("Nombre:%s\nIdentificador:vd%s%d\n",aux->nombre,aux->letra,aux->numero);
		aux=aux->siguiente;
	}
}

int buscarParticionP(char direccion[400],char nombre[100]){
	int retorno=-1;
	FILE *disco;
	if(disco=fopen(direccion,"rb+")){
		fseek(disco,0,SEEK_SET);
		MBR auxmbr;
		fread(&auxmbr,sizeof(MBR),1,disco);
		for(int i=0;i<4;i++){
			if(auxmbr.mbr_partition[i].part_status!='1'){
			if(strcmp(auxmbr.mbr_partition[i].part_name,nombre)==0  ){
                F=auxmbr.mbr_partition[i].part_fit;
				retorno=i;
			}
			}
		}
	}
	return retorno;
}

int buscarParticionL(char direccion[400],char nombre[100]){


	int d=-1;
	int retorno=-1;
	FILE *disco;
	if(disco=fopen(direccion,"rb+")){
		fseek(disco,0,SEEK_SET);
		MBR auxmbr;
		fread(&auxmbr,sizeof(MBR),1,disco);
		for(int i=0;i<4;i++){
			if(auxmbr.mbr_partition[i].part_type=='E'){
				d=i;
			}
		}
		if(d!=-1){
			fseek(disco,auxmbr.mbr_partition[d].part_start,SEEK_SET);
			ebr auxebr;
			while((fread(&auxebr,sizeof(ebr),1,disco))!=0 && ftell(disco)<(auxmbr.mbr_partition[d].part_size+auxmbr.mbr_partition[d].part_start)){
			    	if(strcmp(auxebr.part_name,nombre)==0){
                        F=auxebr.part_fit;
			    		retorno=ftell(disco)-sizeof(ebr);
			    		break;
			    	}
			    if(auxebr.part_next!=-1){

				}else{
					break;
				}
			}
		}
	}
	return retorno;
}

int buscarletra(char* letra,char direccion[400],char temp[400]){
	struct Nodo* aux=primero;
	int retorno=(int)*letra;
	char vd[100]={};
	char* letter=letra;
	char dire[400]={};
	char nam[400]={};
	char line[400];
	char *linea;
	while(aux!=NULL){
		letter=aux->letra;
		strcpy(dire,aux->direccion);
		strcpy(nam,aux->nombre);
		if(strcmp(direccion,nam)==0){
			return -1;
		}else{
			if(strcmp(temp,dire)==0){
				retorno=(int)*letter;
			}else if(retorno<=(int)*letter){
				retorno=retorno+1;
			}
		}
		
		aux=aux->siguiente;
	}
	return retorno;
	
}

int buscarnumero(int numero, char direccion[400],char temp[400]){
	struct Nodo* aux=primero;
	int retorno=1;
	char vd[100]={};
	char dire[400]={};
	char nam[400]={};
	char line[400];
	char *linea;
	while(aux!=NULL){
		numero=aux->numero;
		strcpy(dire,aux->direccion);
		if(strcmp(temp,dire)==0 && retorno==numero){
			retorno++;
		}
		
		aux=aux->siguiente;
	}
	return retorno;
}

const char * getDireccion(char id[10]){
	struct Nodo *aux=primero;

	char *re="no";
	while(aux!=NULL){
		
		if(*aux->letra==id[2]){
			re=aux->direccion;
			return re;
		}
		aux=aux->siguiente;
	}
	return "no";
}


void graficarDisco(char direccion[400],char destino[400],char extension[40]){
	double cant_libre=0;

	FILE *disco;
	int flag_libres=1;
	FILE *graph;
	int sub_total=0;
	if(disco=fopen(direccion,"r")){
		//-------------------------------------------Grafoca
		graph=fopen("grafica.dot","w");
		fprintf(graph,"digraph G{\nNodo[shape=box,label=<<TABLE BORDER=\"0\" CELLBORDER=\"1\" WIDTH=\"500\"  HEIGHT=\"200\" >\n<TR>\n<TD HEIGHT=\"200\" WIDTH=\"50\">MBR</TD>\n");
		MBR auxmbr;
		fseek(disco,0,SEEK_SET);
		fread(&auxmbr,sizeof(MBR),1,disco);
		int total=auxmbr.mbr_tamanio;
		for(int i=0;i<4;i++){
			int parcial=auxmbr.mbr_partition[i].part_size;
			sub_total=parcial+sub_total;
			if(auxmbr.mbr_partition[i].part_start==-1){
				flag_libres=0;
				parcial=total-(auxmbr.mbr_partition[i-1].part_size+auxmbr.mbr_partition[i-1].part_start);
				double porcent=(double)(parcial*100)/total;
				cant_libre=cant_libre+porcent;
				double porcent2=(double)(porcent*450)/100;
				fprintf(graph, "<TD WIDTH=\"%f\" HEIGHT=\"200\">LIBRE<br/>Ocupado: %f%c</TD>\n",porcent2,porcent,'%');
				break;
			}else{
				double porcent=(double)(parcial*100)/total;
				cant_libre=cant_libre+porcent;				
				double porcent2=(double)(porcent*450)/100;
				if(auxmbr.mbr_partition[i].part_status!='1'){

					if(auxmbr.mbr_partition[i].part_type=='E'){
						fprintf(graph, "<TD  HEIGHT=\"200\" WIDTH=\"%f\">\n<TABLE BORDER=\"0\"  HEIGHT=\"200\" WIDTH=\"%f\" CELLBORDER=\"1\">\n<TR>\n<TD HEIGHT=\"50\" WIDTH=\"%f\">Extendida</TD>\n</TR>\n<TR>\n",porcent,porcent,porcent);
					
						fseek(disco,auxmbr.mbr_partition[i].part_start,SEEK_SET);
						ebr auxebr;
						while((fread(&auxebr,sizeof(ebr),1,disco))!=0 && ftell(disco)<(auxmbr.mbr_partition[i].part_size+auxmbr.mbr_partition[i].part_start)){
								parcial=auxebr.part_size;
								 porcent=(double)(parcial*100)/total;
								 if(porcent!=0){
								 	if(auxebr.part_status!='1'){
								 		fprintf(graph, "<TD HEIGHT=\"150\">EBR </TD>\n");
										fprintf(graph, "<TD HEIGHT=\"150\" >LOGICA <br/> Ocupado: %f%c</TD>\n",porcent,'%');
								 	}else{
								 		fprintf(graph, "<TD HEIGHT=\"150\" >LIBRE <br/> Ocupado: %f%c</TD>\n",porcent,'%');
								 	}
									 	
					    			fseek(disco,auxebr.part_next,SEEK_SET);
					    			if(auxebr.part_next!=-1){
									}else{
										parcial=(auxmbr.mbr_partition[i].part_size+auxmbr.mbr_partition[i].part_start)-(auxebr.part_size+auxebr.part_start);
										 porcent=(double)(parcial*100)/total;
										fprintf(graph, "<TD HEIGHT=\"150\" >LIBRE <br/> Ocupado: %f%c</TD>\n",porcent,'%');
										break;
									}
								 }
					    		
							}
							fprintf(graph, "<TD>1\n</TD></TR>\n</TABLE>\n</TD>\n");
					
					}else{
						fprintf(graph, "<TD HEIGHT=\"200\" WIDTH=\"%f\">PRIMARIA <br/> Ocupado: %f%c</TD>\n",porcent2,porcent,'%');
					}
				}else{
					fprintf(graph, "<TD HEIGHT=\"200\" WIDTH=\"%f\">LIBRE <br/> Ocupado: %f%c</TD>\n",porcent2,porcent,'%');
				}
			}
		}

		fprintf(graph, "<TD HEIGHT=\"200\">LIBRE <br/> Ocupado: %f%c</TD>\n",100-cant_libre,'%');

		
		fclose(disco);
		fprintf(graph,"</TR>\n</TABLE>>];\n}");
		fclose(graph);
		char comando[400];
		strcpy(comando,"dot -T");
		strcat(comando,extension);
		strcat(comando," grafica.dot -o ");
		strcat(comando,destino);
		system(comando);

	}else{
		printf("Error el disco no existe\n");
	}
}


char *filename(char *str) {
	char palabra[400];
    char* result="";
    int index=0;
    while(*str){
    	if(*str==47){
    		memset(palabra, 0, sizeof(palabra));
    		index=0;
    	}else if(*str==46){
    		result=palabra;
    		printf("Palabra %s\n", result);
    		break;
    	}else{
    		palabra[index]=*str;
    		index++;
    	}
    	*str++;
    }
    return result;
}

char *extension(char *str) {
	char palabra[400];
    char* result="";
    int flag=0;
    int index=0;
    while(*str){
    	if(flag){
    		palabra[index]=*str;
    		index++;
    	}
		if(*str==46){
			memset(palabra, 0, sizeof(palabra));
			index=0;
			flag=1;
    	}

    	*str++;
    }
    result=palabra;
    return result;
}

char *getPath(char *str) {
	char palabra[400];
    char* result="";
    int index=0;
    int contador=0;
    int parar;
    while(*str){
    	if(*str==47){
    		contador=0;
			palabra[index]=*str;
    		index++;
    	}else if(*str==46){
    		parar=index-contador-1;
    		for(int i=parar;i<index;i++){
    			palabra[i]='\0';
    		}
    		break;
    	}else{
    		palabra[index]=*str;
    		index++;
    		contador++;
    	}
    	*str++;
    }
    result=palabra;
    return result;
}


void insertarNodo(int numero,char* letra,char direccion[400],char nombre[400]){
	struct Nodo* nuevo=(struct Nodo*) malloc(sizeof(struct Nodo));
	nuevo->numero=numero;
	nuevo->letra=(char*)malloc(strlen(letra+1));
	strcpy(nuevo->letra,letra);
	strcpy(nuevo->direccion,direccion);
	strcpy(nuevo->nombre,nombre);
	nuevo->siguiente=NULL;
	nuevo->anterior=NULL;

	if(primero==NULL){
		primero=ultimo=nuevo;
	}else{
		ultimo->siguiente=nuevo;
		nuevo->anterior=ultimo;
		ultimo=nuevo;
	}
}


int buscarMontado(char nombre[400]){

	struct Nodo *aux=primero;
	char temp[400];
	strcpy(temp,"vd");
	strcat(temp,aux->letra);
	sprintf(temp,"%s%d",temp,aux->numero);
	if(strcmp(nombre,temp)==0){
		primero=aux->siguiente;
		free(aux);
		unParticionP(aux->direccion,aux->nombre);
	    unParticionL(aux->direccion,aux->nombre);
		return 1;
	}else{
		 aux=ultimo;
		char temp[400];
		strcpy(temp,"vd");
		strcat(temp,aux->letra);
		sprintf(temp,"%s%d",temp,aux->numero);
		if(strcmp(nombre,temp)==0){
			ultimo=aux->anterior;
			ultimo->siguiente=NULL,
			free(aux);
			 unParticionP(aux->direccion,aux->nombre);
	  		 unParticionL(aux->direccion,aux->nombre);
			return 1;
		}else{
			aux=primero;
			while(aux!=NULL){
				char temp[400];
				strcpy(temp,"vd");
				strcmp(temp,aux->letra);
				sprintf(temp,"%s%d",temp,aux->numero);
				if(strcmp(temp,nombre)==0){
					struct Nodo *aux2=aux->anterior;
					aux2->siguiente=aux->siguiente;
					aux->siguiente->anterior=aux2;
					free(aux);
					unParticionP(aux->direccion,aux->nombre);
	  				unParticionL(aux->direccion,aux->nombre);	
					return 1;
				}
				aux=aux->siguiente;
			}
		}
	}
	return 0;

}

struct Nodo* buscarMount(char nombre[400]){

	struct Nodo *aux=primero;
	while(aux!=NULL){
		char comparar[400];
		strcpy(comparar,"vd");
		strcat(comparar,aux->letra);
		sprintf(comparar,"%s%d",comparar,aux->numero);
		if(strcmp(comparar,nombre)==0){
			return aux;
		}
		aux=aux->siguiente;
	}
	return NULL;
}

void  unParticionP(char direccion[400],char nombre[100]){
	int retorno=-1;
	FILE *disco;
	if(disco=fopen(direccion,"rb+")){
		fseek(disco,0,SEEK_SET);
		MBR auxmbr;
		fread(&auxmbr,sizeof(MBR),1,disco);
		for(int i=0;i<4;i++){
			if(strcmp(auxmbr.mbr_partition[i].part_name,nombre)==0){
				auxmbr.mbr_partition[i].part_status='0';
				fseek(disco,0,SEEK_SET);
				fwrite(&auxmbr,sizeof(MBR),1,disco);
				fclose(disco);
				return;
			}
		}
	}
}

void unParticionL(char direccion[400],char nombre[100]){


	int d=-1;
	int retorno=-1;
	FILE *disco;
	if(disco=fopen(direccion,"rb+")){


		fseek(disco,0,SEEK_SET);
		MBR auxmbr;
		fread(&auxmbr,sizeof(MBR),1,disco);
		for(int i=0;i<4;i++){
			if(auxmbr.mbr_partition[i].part_type=='E'){
				d=i;
			}
		}
		if(d!=-1){
			fseek(disco,auxmbr.mbr_partition[d].part_start,SEEK_SET);
			ebr auxebr;
			while((fread(&auxebr,sizeof(ebr),1,disco))!=0 && ftell(disco)<(auxmbr.mbr_partition[d].part_size+auxmbr.mbr_partition[d].part_start)){
			    	if(strcmp(auxebr.part_name,nombre)==0){
			    		auxebr.part_status='0';
			    		fseek(disco,ftell(disco)-sizeof(ebr),SEEK_SET);
			    		fwrite(&auxebr,sizeof(ebr),1,disco);
			    		fclose(disco);
			    		return ;
			    	}
			    if(auxebr.part_next!=-1){
				}else{
					return ;
				}
			}
		}
	}
}

void graficarMBR(char direccion[400],char destino[400],char extension[40]){
	int index_extendida=-1;
	FILE *disco;
	int index_ebr=0;
	FILE *graph;
	int sub_total=0;
	if(disco=fopen(direccion,"r")){
		//-------------------------------------------Grafoca
		graph=fopen("grafica.dot","w");
		fprintf(graph,"digraph G{ \nsubgraph cluster{\n label=\"MBR\"");
		fprintf(graph,"\nNodo[shape=box,label=<<TABLE BORDER=\"0\" CELLBORDER=\"1\" WIDTH=\"300\"  HEIGHT=\"200\" >\n" );
		fprintf(graph,"<TR ><TD WIDTH=\"150\" ><b>Nombre</b></TD><TD WIDTH=\"150\"><b>Valor</b></TD></TR>\n" );
		MBR auxmbr;
		fseek(disco,0,SEEK_SET);
		fread(&auxmbr,sizeof(MBR),1,disco);
		int total=auxmbr.mbr_tamanio;
		fprintf(graph,"<TR><TD><b>mbr_tamaño</b></TD><TD>%d</TD></TR>\n",total);
		struct tm *tm;
		char fecha[100];
		tm=localtime(&auxmbr.mbr_fecha_creacion);
		strftime(fecha,100,"%d/%m/%y %H:%S",tm);
		fprintf(graph,"<TR><TD><b>mbr_fecha_creacion</b></TD><TD>%s</TD></TR>\n",fecha);
		fprintf(graph,"<TR><TD><b>mbr_disk_signature</b></TD><TD>%d</TD></TR>\n",auxmbr.mbr_disk_signature);

		
		for(int i=0;i<4;i++){
			int parcial=auxmbr.mbr_partition[i].part_size;
			sub_total=parcial+auxmbr.mbr_partition[i].part_start;
			if(auxmbr.mbr_partition[i].part_start==-1){
				break;
			}else{
				double porcent=(double)(parcial*100)/total;
				double porcent2=(double)(porcent*450)/100;
				if(auxmbr.mbr_partition[i].part_status!='1'){

					if(auxmbr.mbr_partition[i].part_type=='E'){
						index_extendida=i;	
					}
					char status[3];
					sprintf(status,"%c",auxmbr.mbr_partition[i].part_status);
					if(strcmp(status,"")==0) strcpy(status,"0");
					fprintf(graph, "<TR><TD><b>part_status_%d</b></TD><TD>%s</TD></TR>\n",i,status);
					fprintf(graph, "<TR><TD><b>part_type_%d</b></TD><TD><b>%c</b></TD></TR>\n",i,auxmbr.mbr_partition[i].part_type);
					fprintf(graph, "<TR><TD><b>part_fit_%d</b></TD><TD><b>%c</b></TD></TR>\n",i,auxmbr.mbr_partition[i].part_fit);
					fprintf(graph, "<TR><TD><b>part_start_%d</b></TD><TD>%d</TD></TR>\n",i,auxmbr.mbr_partition[i].part_start);
					fprintf(graph, "<TR><TD><b>part_size_%d</b></TD><TD>%d</TD></TR>\n",i,auxmbr.mbr_partition[i].part_size);
					fprintf(graph, "<TR><TD><b>part_name_%d</b></TD><TD>%s</TD></TR>\n",i,auxmbr.mbr_partition[i].part_name);

					
				}
			}
		}
	
		fprintf(graph,"</TABLE>>];\n}");

		if(index_extendida!=-1){
			fseek(disco,auxmbr.mbr_partition[index_extendida].part_start,SEEK_SET);
			ebr auxebr;
			while((fread(&auxebr,sizeof(ebr),1,disco))!=0 && ftell(disco)<(auxmbr.mbr_partition[index_extendida].part_size+auxmbr.mbr_partition[index_extendida].part_start)){
				if(auxebr.part_status!='1'){
					fprintf(graph,"subgraph ebr_%d{\n label=\"EBR_%d\"\n",index_ebr,index_ebr);
					fprintf(graph,"\nNodo_%d[shape=box,label=<<TABLE BORDER=\"0\" CELLBORDER=\"1\" WIDTH=\"300\"  HEIGHT=\"200\" >\n",index_ebr);
					fprintf(graph,"<TR ><TD WIDTH=\"150\" ><b>Nombre</b></TD><TD WIDTH=\"150\"><b>Valor</b></TD></TR>\n" );
					char status[3];
						sprintf(status,"%c",auxebr.part_status);
						if(strcmp(status,"")==0) strcpy(status,"0");
					fprintf(graph, "<TR><TD><b>part_status_1</b></TD><TD>%s</TD></TR>\n",status);
					fprintf(graph, "<TR><TD><b>part_fit_1</b></TD><TD><b>%c</b></TD></TR>\n",auxebr.part_fit);
					fprintf(graph, "<TR><TD><b>part_start_1</b></TD><TD>%d</TD></TR>\n",auxebr.part_start);
					fprintf(graph, "<TR><TD><b>part_size_1</b></TD><TD>%d</TD></TR>\n",auxebr.part_size);
					fprintf(graph, "<TR><TD><b>part_next_1</b></TD><TD>%d</TD></TR>\n",auxebr.part_next);
					fprintf(graph, "<TR><TD><b>part_name_1</b></TD><TD>%s</TD></TR>\n",auxebr.part_name);
					fprintf(graph,"</TABLE>>];\n}\n");
					index_ebr++;
				}

				if(auxebr.part_next!=-1){
						fseek(disco,auxebr.part_next,SEEK_SET);
				}else{
			
					break;
				}
			
				
			}

		}
		fclose(disco);


		fprintf(graph, "}\n");
		fclose(graph);
		char comando[400];
		strcpy(comando,"dot -T");
		strcat(comando,extension);
		strcat(comando," grafica.dot -o ");
		strcat(comando,destino);
		system(comando);

	}else{
		printf("Error el disco no existe\n");
	}
}


int Loguear(char id[20],char direccion[100],char user[11],char password[11]){
        int index=buscarParticionP(direccion,id);
        superBloque super;
        inodeTable inodo;
        if(index!=-1){
            MBR auxmbr;
            FILE *disco;
            disco=fopen(direccion,"rb+");
            fread(&auxmbr,sizeof(MBR),1,disco);
            fseek(disco,auxmbr.mbr_partition[index].part_start,SEEK_SET);
            inicio_super=ftell(disco);
            fread(&super,sizeof(superBloque),1,disco);
            fseek(disco,super.s_inode_start+sizeof(inodeTable),SEEK_SET);
            fread(&inodo,sizeof(inodeTable),1,disco);
            fseek(disco,super.s_inode_start+sizeof(inodeTable),SEEK_SET);
            inodo.i_atime=time(NULL);
            fwrite(&inodo,sizeof(inodeTable),1,disco);
            fclose(disco);
            return buscarUsuarioL(super.s_inode_start+sizeof(inodeTable),super.s_block_start,user,password,direccion);
            }else{
                index=buscarParticionL(direccion,id);
                    if(index!=-1){
                        ebr auxebr;
                        FILE *disco;
                        disco=fopen(direccion,"rb+");
                        fseek(disco,index+sizeof(ebr),SEEK_SET);
                        inicio_super=ftell(disco);
                        fread(&super,sizeof(superBloque),1,disco);
                        fseek(disco,super.s_inode_start+sizeof(inodeTable),SEEK_SET);
                        fread(&inodo,sizeof(inodeTable),1,disco);
                        fseek(disco,super.s_inode_start+sizeof(inodeTable),SEEK_SET);
                        inodo.i_atime=time(NULL);
                        fwrite(&inodo,sizeof(inodeTable),1,disco);
                        fclose(disco);
                        return buscarUsuarioL(super.s_inode_start+sizeof(inodeTable),super.s_block_start,user,password,direccion);
                    }    
                    return 0;   
            }
    
	   return 0;
}

int buscarUsuarioL(int inicioI,int InicioB,char i_user[12],char i_pass[12],char direccion[100]){
    FILE *disco;
    FILE *discoaux;

    if(disco=fopen(direccion,"rb+")){
        char cadena[300]="\0";
        discoaux=fopen(direccion,"rb+");
        inodeTable inodo;
        fseek(disco,inicioI,SEEK_SET);
        fread(&inodo,sizeof(inodeTable),1,disco);
        memset(ControlGrupos,0,sizeof(ControlGrupos));
        for(int i=0;i<15;i++){
            if(inodo.i_block[i]!=-1){
                char fase[64]="";
                memset(fase,0,sizeof(fase));
                bloqueArchivo archivo;
                fseek(discoaux,InicioB,SEEK_SET);
                fread(&archivo,sizeof(bloqueArchivo),1,discoaux);
                for(int j=0;j<inodo.i_block[i];j++){
                    fread(&archivo,sizeof(bloqueArchivo),1,discoaux);
                }
                strcat(cadena,archivo.b_content);
            }
        }
        char *end_str;
        char *ptr=strtok_r(cadena,"\n",&end_str);
            while(ptr!=NULL){
                char id[2];
                char tipo[2];
                char grup[12];
                char user[12];
                char pass[12];
                char *end_token;
                char *token2=strtok_r(ptr,",",&end_token);
                strcpy(id,token2);
                if(strcmp(id,"0")!=0){
                    token2=strtok_r(NULL,",",&end_token);
                    strcpy(tipo,token2);
                    if(strcmp(tipo,"U")==0){
                        token2=strtok_r(NULL,",",&end_token);
                        token2=strtok_r(NULL,",",&end_token);
                        strcpy(user,token2);
                        token2=strtok_r(NULL,",",&end_token);
                        strcpy(pass,token2);
                        if(strcmp(user,i_user)==0 && strcmp(pass,i_pass)==0){ 
                            if(strcmp(i_user,"root")==0){
                                userlog.id_user=1;
                                userlog.id_grup=1;

                            }else{
                                userlog.id_user=atoi(id);
                                userlog.id_grup=buscarNumeroGrupo(i_user);

                            }
                            memset(userlog.usuario,0,sizeof(userlog.usuario));
                            strcpy(userlog.usuario,user);
                            return 2;
                        }
                    }
                }
                
                ptr=strtok_r(NULL,"\n",&end_str);
            }
        fclose(disco);
        fclose(discoaux);
    }
    return 1;
}

int buscarUltimoGrupo(){
    int mk_id=-1;
    FILE *disco;
    FILE *discoaux;
    if(disco=fopen(direccion_global,"rb+")){
        char cadena[300]="\0";
        discoaux=fopen(direccion_global,"rb+");
        inodeTable inodo;
        fseek(disco,inicio_inode+sizeof(inodeTable),SEEK_SET);
        fread(&inodo,sizeof(inodeTable),1,disco);
        for(int i=0;i<15;i++){
            if(inodo.i_block[i]!=-1){
                bloqueArchivo archivo;
                fseek(discoaux,inicio_blo,SEEK_SET);
                fread(&archivo,sizeof(bloqueArchivo),1,discoaux);
                for(int j=0;j<inodo.i_block[i];j++){
                    fread(&archivo,sizeof(bloqueArchivo),1,discoaux);
                }
                strcat(cadena,archivo.b_content);
            }
        }
        char *end_str;
        char *ptr=strtok_r(cadena,"\n",&end_str);
            while(ptr!=NULL){
                char id[2];
                char tipo[2];
                char grup[12];
                char *end_token;
                char *token2=strtok_r(ptr,",",&end_token);
                strcpy(id,token2);
                if(strcmp(id,"0")!=0){
                    if(strlen(end_token)>2){
                        token2=strtok_r(NULL,",",&end_token);
                        strcpy(tipo,token2);
                        if(strcmp(tipo,"G")==0){
                            mk_id=atoi(id);
                        }
                    }
                }
                
                ptr=strtok_r(NULL,"\n",&end_str);
            }
        fclose(disco);
        fclose(discoaux);
    }
    return mk_id;
}

int buscarGrupo(char nombre[12]){
	FILE *disco;
    FILE *discoaux;
    int re=-1;
    if(disco=fopen(direccion_global,"rb+")){
        char cadena[300]="\0";
        discoaux=fopen(direccion_global,"rb+");
        inodeTable inodo;
        fseek(disco,inicio_inode+sizeof(inodeTable),SEEK_SET);
        fread(&inodo,sizeof(inodeTable),1,disco);
        for(int i=0;i<15;i++){
            if(inodo.i_block[i]!=-1){
                bloqueArchivo archivo;
                fseek(discoaux,inicio_blo,SEEK_SET);
                fread(&archivo,sizeof(bloqueArchivo),1,discoaux);
                for(int j=0;j<inodo.i_block[i];j++){
                    fread(&archivo,sizeof(bloqueArchivo),1,discoaux);
                }
                strcat(cadena,archivo.b_content);
            }
        }
        char *end_str;
        char *ptr=strtok_r(cadena,"\n",&end_str);
            while(ptr!=NULL){
                char id[2];
                char tipo[2];
                char grup[12];
                char *end_token;
                char *token2=strtok_r(ptr,",",&end_token);
                strcpy(id,token2);
                if(strcmp(id,"0")!=0){

                    token2=strtok_r(NULL,",",&end_token);
                    strcpy(tipo,token2);
                    if(strcmp(tipo,"G")==0){
                        token2=strtok_r(NULL,",",&end_token);
                        strcpy(grup,token2);
                        if(strcmp(grup,nombre)==0) return atoi(id);                        
                    }
                }
                
                ptr=strtok_r(NULL,"\n",&end_str);
            }
        fclose(disco);
        fclose(discoaux);
    }
    return re;
}


void eliminarGrupo(char *nombre){
    inodeTable inodo;
    bloqueArchivo archivo;
    bloqueArchivo aux;
    FILE *disco;
    int columna=0;
    disco=fopen(direccion_global,"rb+");

    int idaux=0;
    int existe=0;
    char actual;
    char palabra[50]={0};
    char tipoU;
    char grupo[11];
    int posicion=0;


    memset(ControlGrupos,0,sizeof(ControlGrupos));
    fseek(disco,inicio_inode+sizeof(inodeTable),SEEK_SET);
    fread(&inodo,sizeof(inodeTable),1,disco);
    char bu[12];
    memset(bu,0,sizeof(bu));
    strcpy(bu,nombre);
    existe=buscarGrupo(bu);
    if(existe>-1){
        for(int i=0;i<12;i++){
            if(inodo.i_block[i]!=-1){
                char fase[64] = "";
                memset(fase,0,sizeof(fase));
                fseek(disco,inicio_blo+(sizeof(bloqueArchivo)*i),SEEK_SET);
                fread(&archivo,sizeof(bloqueArchivo),1,disco);
                int tamanio=strlen(archivo.b_content);
                for(int j=0;j<tamanio;j++){
                    char aux[4]={0};
                    sprintf(aux,"%c",archivo.b_content[j]);
                    strcat(fase,aux);
                }
                strcat(ControlGrupos,fase);
            }else{
                break;
            }
        }

        for(int i=0;i<12;i++){
            memset(grupo,0,sizeof(grupo));
            fseek(disco,inicio_blo+(sizeof(bloqueArchivo)*inodo.i_block[i]),SEEK_SET);
            fread(&archivo,sizeof(bloqueArchivo),1,disco);
            for(int j=0;j<64;j++){
                actual=archivo.b_content[j];
                if(actual!='\n'){
                    if(tipoU=='G'){
                        if(idaux!=0){
                            strcpy(grupo,palabra);
                            if(strcmp(grupo,nombre)==0 && idaux==existe){
                                if(idaux>9){
                                    if(posicion==1){
                                        fseek(disco,inicio_blo+(sizeof(bloqueArchivo)*inodo.i_block[i-1]),SEEK_SET);
                                        fread(&aux,sizeof(bloqueArchivo),1,disco);
                                        aux.b_content[63]='0';
                                        fseek(disco,inicio_blo+(sizeof(bloqueArchivo)*inodo.i_block[i-1]),SEEK_SET);
                                        fwrite(&aux,sizeof(bloqueArchivo),1,disco);
                                        fseek(disco,inicio_blo+(sizeof(bloqueArchivo)*inodo.i_block[i]),SEEK_SET);
                                        fread(&aux,sizeof(bloqueArchivo),1,disco);
                                        archivo.b_content[posicion-1]='0';
                                        fseek(disco,inicio_blo+(sizeof(bloqueArchivo)*inodo.i_block[i]),SEEK_SET);
                                        fwrite(&archivo,sizeof(bloqueArchivo),1,disco);
                                    }else{
                                        archivo.b_content[posicion-1]='0';
                                        archivo.b_content[posicion]='0';
                                        fseek(disco,inicio_blo+(sizeof(bloqueArchivo)*inodo.i_block[i]),SEEK_SET);
                                        fwrite(&archivo,sizeof(bloqueArchivo),1,disco);
                                    }
                                }else{
                                    if(posicion==0){
                                        fseek(disco,inicio_blo+(sizeof(bloqueArchivo)*inodo.i_block[i-1]),SEEK_SET);
                                        fread(&aux,sizeof(bloqueArchivo),1,disco);
                                        aux.b_content[63]='0';
                                        fseek(disco,inicio_blo+(sizeof(bloqueArchivo)*inodo.i_block[i-1]),SEEK_SET);
                                        fwrite(&aux,sizeof(bloqueArchivo),1,disco);
                                    }else{
                                        archivo.b_content[posicion-1]='0';
                                        fseek(disco,inicio_blo+(sizeof(bloqueArchivo)*inodo.i_block[i]),SEEK_SET);
                                        fwrite(&archivo,sizeof(bloqueArchivo),1,disco);
                                    }
                                }
                                break;
                            }
                        }
                    }else if(tipoU=='U'){
                        memset(palabra,0,sizeof(palabra));
                        memset(grupo,0,sizeof(grupo));
                    }
                    columna=0;
                    memset(palabra,0,sizeof(palabra));
                    memset(grupo,0,sizeof(grupo)); 
                }else if(actual!=','){
                    strncat(palabra,&actual,1);
                }else if(actual=','){
                    if(columna==0){
                        columna++;
                        idaux=strtol(palabra,NULL,10);
                        posicion=j;
                    }else if(columna==1){
                        columna++;
                        tipoU=palabra[0];
                    }else if(columna==2){
                        columna++;
                    }else if(columna==3){
                        columna++;
                    }
                    memset(palabra,0,sizeof(palabra));
                }
                
            }
        }
    }else{
        printf("Error no existe el grupo\n");
    }
    
    fclose(disco);
   
}


int buscarNumeroGrupo(char nombre[12]){
    FILE *disco;
    FILE *discoaux;
    if(disco=fopen(direccion_global,"rb+")){
        char cadena[300]="\0";
        discoaux=fopen(direccion_global,"rb+");
        inodeTable inodo;
        fseek(disco,inicio_inode+sizeof(inodeTable),SEEK_SET);
        fread(&inodo,sizeof(inodeTable),1,disco);
        for(int i=0;i<15;i++){
            if(inodo.i_block[i]!=-1){
                bloqueArchivo archivo;
                fseek(discoaux,inicio_blo,SEEK_SET);
                fread(&archivo,sizeof(bloqueArchivo),1,discoaux);
                for(int j=0;j<inodo.i_block[i];j++){
                    fread(&archivo,sizeof(bloqueArchivo),1,discoaux);
                }
                strcat(cadena,archivo.b_content);
            }
        }
        char *end_str;
        char *ptr=strtok_r(cadena,"\n",&end_str);
            while(ptr!=NULL){
                char id[2];
                char tipo[2];
                char usr[12];
                char gru[12];
                char *end_token;
                char *token2=strtok_r(ptr,",",&end_token);
                strcpy(id,token2);
                if(strcmp(id,"0")!=0){

                    token2=strtok_r(NULL,",",&end_token);
                    strcpy(tipo,token2);
                    if(strcmp(tipo,"U")==0){
                        token2=strtok_r(NULL,",",&end_token);
                        strcpy(gru,token2);
                        token2=strtok_r(NULL,",",&end_token);
                        strcpy(usr,token2);
                        if(strcmp(usr,nombre)==0) return buscarGrupo(gru);
                        
                    }
                }
                
                ptr=strtok_r(NULL,"\n",&end_str);
            }
        fclose(disco);
        fclose(discoaux);
    }
    return 0;
}

int buscarnuevoID(){
    FILE *disco;
    FILE *discoaux;
    int re=0;
    if(disco=fopen(direccion_global,"rb+")){
        char cadena[300]="\0";
        discoaux=fopen(direccion_global,"rb+");
        inodeTable inodo;
        fseek(disco,inicio_inode+sizeof(inodeTable),SEEK_SET);
        fread(&inodo,sizeof(inodeTable),1,disco);
        for(int i=0;i<15;i++){
            if(inodo.i_block[i]!=-1){
                bloqueArchivo archivo;
                fseek(discoaux,inicio_blo,SEEK_SET);
                fread(&archivo,sizeof(bloqueArchivo),1,discoaux);
                for(int j=0;j<inodo.i_block[i];j++){
                    fread(&archivo,sizeof(bloqueArchivo),1,discoaux);
                }
                strcat(cadena,archivo.b_content);
            }
        }
        char *end_str;
        char *ptr=strtok_r(cadena,"\n",&end_str);
            while(ptr!=NULL){
                char id[2];
                char tipo[2];
                char grup[12];
                char *end_token;
                char *token2=strtok_r(ptr,",",&end_token);
                strcpy(id,token2);
                if(strcmp(id,"0")!=0){

                    token2=strtok_r(NULL,",",&end_token);
                    strcpy(tipo,token2);
                    if(strcmp(tipo,"U")==0){
                        re++;                       
                    }
                }
                
                ptr=strtok_r(NULL,"\n",&end_str);
            }
        fclose(disco);
        fclose(discoaux);
    }
    return re;
}


int buscarUsuario(char nombre[12]){

    FILE *disco;
    FILE *discoaux;
    if(disco=fopen(direccion_global,"rb+")){
        char cadena[300]="\0";
        discoaux=fopen(direccion_global,"rb+");
        inodeTable inodo;
        fseek(disco,inicio_inode+sizeof(inodeTable),SEEK_SET);
        fread(&inodo,sizeof(inodeTable),1,disco);
        for(int i=0;i<15;i++){
            if(inodo.i_block[i]!=-1){
                bloqueArchivo archivo;
                fseek(discoaux,inicio_blo,SEEK_SET);
                fread(&archivo,sizeof(bloqueArchivo),1,discoaux);
                for(int j=0;j<inodo.i_block[i];j++){
                    fread(&archivo,sizeof(bloqueArchivo),1,discoaux);
                }
                strcat(cadena,archivo.b_content);
            }
        }
        char *end_str;
        char *ptr=strtok_r(cadena,"\n",&end_str);
            while(ptr!=NULL){
                char id[2];
                char tipo[2];
                char usr[12];
                char *end_token;
                char *token2=strtok_r(ptr,",",&end_token);
                strcpy(id,token2);
                if(strcmp(id,"0")!=0){

                    token2=strtok_r(NULL,",",&end_token);
                    strcpy(tipo,token2);
                    if(strcmp(tipo,"U")==0){
                        token2=strtok_r(NULL,",",&end_token);
                        token2=strtok_r(NULL,",",&end_token);
                        strcpy(usr,token2);
                        if(strcmp(usr,nombre)==0) return 1;
                        
                    }
                }
                
                ptr=strtok_r(NULL,"\n",&end_str);
            }
        fclose(disco);
        fclose(discoaux);
    }
    return 0;
}


void eliminarUsuario(char nombre[12]){
    FILE *archivo;
    FILE *temp;
    temp=fopen("temporal.txt","w+");

    char line[400];
    char *linea;
    char stat[2];
    char tipoU2[2]="\0";
    char us[10];
    char pass[10];
    char namegroup[10]="\0";

    int flag_estado=0;
    
    if(archivo=fopen(direccion_global,"rb+")){
    
        while(fscanf(archivo, "%[^\n]%*c",line) == 1){            
            linea = strtok(line, ",");
            strcpy(stat,linea);
    
            linea = strtok(NULL, ",");
            strcpy(tipoU2,linea);
    
            linea=strtok(NULL,",");
            strcpy(namegroup,linea);
    
            if(strcmp(tipoU2,"U")==0){
                linea=strtok(NULL,",");
                strcpy(us,linea); 
    
                linea=strtok(NULL,",");
                strcpy(pass,linea); 

                if(strcmp(us,nombre)==0){
                     fprintf(temp,"0,%s,%s,%s,%s\n",tipoU2,namegroup,us,pass);
                }else{
                    fprintf(temp,"%s,%s,%s,%s,%s\n",stat,tipoU2,namegroup,us,pass);
                }
            }else{

                fprintf(temp,"%s,%s,%s\n",stat,tipoU2,namegroup);       
            }
            
        }

       fclose(archivo);
       fclose(temp);
        temp=fopen("temporal.txt","rb+");
        archivo=fopen(direccion_global,"w+");
        while(fscanf(temp, "%[^\n]%*c",line) == 1){ 
            fprintf(archivo, "%s\n",line);
        }
        fclose(archivo);
        fclose(temp);
      
    }
}

void iniciarFormateo2(int inicio,int fin,int tamanio,char direccion[100]){

    int n = (tamanio - sizeof(superBloque))/(4 + sizeof(inodeTable) + 3*sizeof(bloqueArchivo));
    int num_struc=floor(n);
    int num_bloques=3*num_struc;

    superBloque super;
    super.s_filesystem_type=2;
    super.s_inodes_count=num_struc;
    super.s_blocks_count=num_bloques;
    super.s_free_inodes_count=num_struc-2;
    super.s_free_block_count=num_bloques-2;
    super.s_mtime=time(NULL);
    super.s_umtime = 0;
    super.s_mnt_count = 0;
    super.s_magic=0xEF53;
    super.s_inode_size=sizeof(inodeTable);
    super.s_block_size=sizeof(bloqueArchivo);
    super.s_first_ino=2;
    super.s_first_blo=2;
    super.s_bm_inode_start=inicio+sizeof(superBloque);
    super.s_bm_block_start=inicio+sizeof(superBloque)+num_struc;
    super.s_inode_start=inicio+sizeof(superBloque)+num_struc+num_bloques;
    super.s_block_start=inicio+sizeof(superBloque)+num_struc+num_bloques+(sizeof(inodeTable)*num_struc);


    inicio_bi=super.s_bm_inode_start;
    inicio_bb=super.s_bm_block_start;
    inicio_blo=super.s_block_start;
    inicio_inode=super.s_inode_start;
    fin_blo=inicio_blo+super.s_block_size;
    fin_inode=inicio_inode+super.s_inode_size;
    fin_bi=inicio_bi+super.s_inodes_count;
    fin_bb=inicio_blo+super.s_blocks_count;
    inicio_journal=inicio_super+sizeof(superBloque);


    inodeTable inodo;
    bloqueCarpetas bloque;

    printf("EXT2\n");
    FILE *disco;
    char buffer='0';
    char buffer2='1';
    char buffer3='2';
    if(disco=fopen(direccion,"rb+")){
        fseek(disco,inicio,SEEK_SET);
        fwrite(&super,sizeof(superBloque),1,disco);
        //---------------------------------bits de inodos------------------------------------------
        
        for(int i=0;i<num_struc;i++){
            fseek(disco,super.s_bm_inode_start+(i*sizeof(char)),SEEK_SET);
            fwrite(&buffer,sizeof(char),1,disco);
        }

        //--------------------------- inodos para / y user.txt-----------------------------------------------
        for(int i=0;i<2;i++){
            fseek(disco,super.s_bm_inode_start+(i*sizeof(char)),SEEK_SET);
            fwrite(&buffer2,sizeof(char),1,disco);
        }
        //----------------------------bits para bloques------------------
        for(int i=0;i<num_bloques;i++){
            fseek(disco,super.s_bm_block_start+(i*sizeof(char)),SEEK_SET);
            fwrite(&buffer,sizeof(char),1,disco);
        }
        //--------------------------- bloques para / y user.txt-----------------------------------------------
        fseek(disco,super.s_bm_block_start,SEEK_SET);
        fwrite(&buffer2,sizeof(char),1,disco);
        fwrite(&buffer3,sizeof(char),1,disco);
        //--------------------------INODO PARA CARPETA ROOT---------------------
        inodo.i_uid=1;
        inodo.i_gid=1;
        inodo.i_size=0;
        inodo.i_atime=time(NULL);
        inodo.i_ctime=time(NULL);
        inodo.i_mtime=time(NULL);
        inodo.i_block[0]=0;
        for(int i=1;i<15;i++){
            inodo.i_block[i]=-1;
        }
        inodo.i_type='0';
        inodo.i_perm=664;
        fseek(disco,super.s_inode_start,SEEK_SET);        
        fwrite(&inodo,sizeof(inodeTable),1,disco);

        //-----------------------------------PRIMER BLOQUE------------------------

        strcpy(bloque.b_content[0].b_name,".");
        bloque.b_content[0].b_inodo=0;

        strcpy(bloque.b_content[1].b_name,"..");
        bloque.b_content[1].b_inodo=0;

        strcpy(bloque.b_content[2].b_name,"user.txt");
        bloque.b_content[2].b_inodo=1;

        strcpy(bloque.b_content[3].b_name,".");
        bloque.b_content[3].b_inodo=-1;

        fseek(disco,super.s_block_start,SEEK_SET);
        fwrite(&bloque,sizeof(bloqueCarpetas),1,disco);
        

        //--------------------INODO PARA USER.TXT--------------------------



        inodo.i_uid=1;
        inodo.i_gid=1;
        inodo.i_size=27;
        inodo.i_atime=time(NULL);
        inodo.i_ctime=time(NULL);
        inodo.i_mtime=time(NULL);
        inodo.i_block[0]=1;

        for(int i=1;i<15;i++){
            inodo.i_block[i]=-1;
        }

        inodo.i_type='1';
        inodo.i_perm=755;

        fseek(disco,super.s_inode_start+sizeof(inodeTable),SEEK_SET);
        fwrite(&inodo,sizeof(inodeTable),1,disco);

        //------------------------------Bloque para user.txt

        bloqueArchivo archivo;
        memset(archivo.b_content,0,sizeof(archivo.b_content));
        strcpy(archivo.b_content,"1,G,root\n1,U,root,root,123\n");
        fseek(disco,super.s_block_start+sizeof(bloqueCarpetas),SEEK_SET);
        fwrite(&archivo,sizeof(bloqueArchivo),1,disco);
        
        fclose(disco);
    }
}

void iniciarFormateo3(int inicio,int fin,int tamanio,char direccion[100]){
    int n = (tamanio - sizeof(superBloque))/(4 + sizeof(inodeTable) + 3*sizeof(bloqueArchivo) + sizeof(JOURNAL));

    int num_struc=floor(n);
    int num_bloques=3*num_struc;

    superBloque super;
    super.s_filesystem_type=3;
    super.s_inodes_count=num_struc;
    super.s_blocks_count=num_bloques;
    super.s_free_inodes_count=num_struc-2;
    super.s_free_block_count=num_bloques-2;
    super.s_mtime=time(NULL);
    super.s_umtime = 0;
    super.s_mnt_count = 0;
    super.s_magic=0xEF53;
    super.s_inode_size=sizeof(inodeTable);
    super.s_block_size=sizeof(bloqueArchivo);
    super.s_first_ino=2;
    super.s_first_blo=2;
    super.s_bm_inode_start=inicio+sizeof(superBloque)+(sizeof(JOURNAL)*num_struc);
    super.s_bm_block_start=inicio+sizeof(superBloque)+num_struc+(sizeof(JOURNAL)*num_struc);
    super.s_inode_start=inicio+sizeof(superBloque)+num_struc+num_bloques+(sizeof(JOURNAL)*num_struc);
    super.s_block_start=inicio+sizeof(superBloque)+(sizeof(JOURNAL)*num_struc)+num_struc+num_bloques+(sizeof(inodeTable)*num_struc);


    inicio_bi=super.s_bm_inode_start;
    inicio_bb=super.s_bm_block_start;
    inicio_blo=super.s_block_start;
    inicio_inode=super.s_inode_start;
    fin_blo=inicio_blo+super.s_block_size;
    fin_inode=inicio_inode+super.s_inode_size;
    fin_bi=inicio_bi+super.s_inodes_count;
    fin_bb=inicio_blo+super.s_blocks_count;
    inicio_journal=inicio_super+sizeof(superBloque);

    inodeTable inodo;
    bloqueCarpetas bloque;

    printf("EXT3\n");
    FILE *disco;
    char buffer='0';
    char buffer2='1';
    char buffer3='2';
    if(disco=fopen(direccion,"rb+")){
        fseek(disco,inicio,SEEK_SET);
        fwrite(&super,sizeof(superBloque),1,disco);
        //---------------------------------bits de inodos------------------------------------------
        
        for(int i=0;i<num_struc;i++){
            fseek(disco,super.s_bm_inode_start+(i*sizeof(char)),SEEK_SET);
            fwrite(&buffer,sizeof(char),1,disco);
        }

        //--------------------------- inodos para / y user.txt-----------------------------------------------
        for(int i=0;i<2;i++){
            fseek(disco,super.s_bm_inode_start+(i*sizeof(char)),SEEK_SET);
            fwrite(&buffer2,sizeof(char),1,disco);
        }
        //----------------------------bits para bloques------------------
        for(int i=0;i<num_bloques;i++){
            fseek(disco,super.s_bm_block_start+(i*sizeof(char)),SEEK_SET);
            fwrite(&buffer,sizeof(char),1,disco);
        }
        //--------------------------- bloques para / y user.txt-----------------------------------------------
        fseek(disco,super.s_bm_block_start,SEEK_SET);
        fwrite(&buffer2,sizeof(char),1,disco);
        fwrite(&buffer3,sizeof(char),1,disco);
        //--------------------------INODO PARA CARPETA ROOT---------------------
        inodo.i_uid=1;
        inodo.i_gid=1;
        inodo.i_size=0;
        inodo.i_atime=time(NULL);
        inodo.i_ctime=time(NULL);
        inodo.i_mtime=time(NULL);
        inodo.i_block[0]=0;
        for(int i=1;i<15;i++){
            inodo.i_block[i]=-1;
        }
        inodo.i_type='0';
        inodo.i_perm=664;
        fseek(disco,super.s_inode_start,SEEK_SET);        
        fwrite(&inodo,sizeof(inodeTable),1,disco);

        //-----------------------------------PRIMER BLOQUE------------------------

        strcpy(bloque.b_content[0].b_name,".");
        bloque.b_content[0].b_inodo=0;

        strcpy(bloque.b_content[1].b_name,"..");
        bloque.b_content[1].b_inodo=0;

        strcpy(bloque.b_content[2].b_name,"user.txt");
        bloque.b_content[2].b_inodo=1;

        strcpy(bloque.b_content[3].b_name,".");
        bloque.b_content[3].b_inodo=-1;

        fseek(disco,super.s_block_start,SEEK_SET);
        fwrite(&bloque,sizeof(bloqueCarpetas),1,disco);
        

        //--------------------INODO PARA USER.TXT--------------------------



        inodo.i_uid=1;
        inodo.i_gid=1;
        inodo.i_size=27;
        inodo.i_atime=time(NULL);
        inodo.i_ctime=time(NULL);
        inodo.i_mtime=time(NULL);
        inodo.i_block[0]=1;

        for(int i=1;i<15;i++){
            inodo.i_block[i]=-1;
        }

        inodo.i_type='1';
        inodo.i_perm=755;

        fseek(disco,super.s_inode_start+sizeof(inodeTable),SEEK_SET);
        fwrite(&inodo,sizeof(inodeTable),1,disco);

        //------------------------------Bloque para user.txt

        bloqueArchivo archivo;
        memset(archivo.b_content,0,sizeof(archivo.b_content));
        strcpy(archivo.b_content,"1,G,root\n1,U,root,root,123\n");
        fseek(disco,super.s_block_start+sizeof(bloqueCarpetas),SEEK_SET);
        fwrite(&archivo,sizeof(bloqueArchivo),1,disco);
        
        fclose(disco);
    }
}


void graficarInodos(char direccion[400],char destino[400],char extension[40],int biS,int iS,int biE){
    int numeroInode=0;
    char  buff;
    FILE *disco;
    FILE *discoaux;
    int flag_libres=1;
    FILE *graph;
    int sub_total=0;
    if(disco=fopen(direccion,"r")){
        //-------------------------------------------Grafoca
        graph=fopen("grafica.dot","w");
        fprintf(graph,"digraph G{\n");
        discoaux=fopen(direccion,"r");
        inodeTable inode;
       fseek(discoaux,biS,SEEK_SET);
        fseek(disco,iS,SEEK_SET);       
        while(ftell(discoaux)<biE){
            buff=fgetc(discoaux);
            fread(&inode,sizeof(inodeTable),1,disco);           
            if(buff=='1'){
                fprintf(graph,"subgraph inode_%d{\n",numeroInode);
                fprintf(graph, "nodo_%d [ shape=none,",numeroInode);
                fprintf(graph, "label=< <TABLE> <TR> <TD COLSPAN=\"2\"> Inodo %d </TD></TR>\n",numeroInode);
                fprintf(graph, "<TR> <TD> i_uid </TD> <TD> %d </TD></TR>\n",inode.i_uid);
                fprintf(graph, "<TR> <TD> i_gid </TD> <TD> %d </TD></TR>\n",inode.i_gid);
                fprintf(graph, "<TR> <TD> i_size </TD> <TD> %d </TD></TR>\n",inode.i_size);
                fprintf(graph, "<TR> <TD> i_gid </TD> <TD> %d </TD></TR>\n",inode.i_gid);
                struct tm *tm;
                char fecha[100];
                tm=localtime(&inode.i_atime);
                strftime(fecha,100,"%d/%m/%y %H:%S",tm);
                fprintf(graph, "<TR> <TD> i_atime </TD> <TD> %s </TD></TR>\n",fecha);
                tm=localtime(&inode.i_ctime);
                strftime(fecha,100,"%d/%m/%y %H:%S",tm);
                fprintf(graph, "<TR> <TD> i_ctime </TD> <TD> %s </TD></TR>\n",fecha);
                tm=localtime(&inode.i_mtime);
                strftime(fecha,100,"%d/%m/%y %H:%S",tm);
                fprintf(graph, "<TR> <TD> i_mtime </TD> <TD> %s </TD></TR>\n",fecha);
                for(int i=0;i<15;i++){
                    fprintf(graph, "<TR> <TD> i_block_%d </TD> <TD> %d </TD></TR>\n",i,inode.i_block[i]);
                }
                fprintf(graph, "<TR> <TD> i_type </TD> <TD> %c </TD></TR>\n",inode.i_type);
                fprintf(graph, "<TR> <TD> i_perm </TD> <TD> %d </TD></TR>\n",inode.i_perm);
                fprintf(graph, "</TABLE>>]\n");
                fprintf(graph,"}\n");
            }
            
            numeroInode++;
        }
        
        fclose(disco);
        fprintf(graph,"\n}");
        fclose(graph);
        char comando[400];
        strcpy(comando,"dot -T");
        strcat(comando,extension);
        strcat(comando," grafica.dot -o ");
        strcat(comando,destino);
        system(comando);
        fclose(discoaux);

    }else{
        printf("Error el disco no existe\n");
    }
}

void graficarBloques(char direccion[400],char destino[400],char extension[40],int biS,int iS,int biE){
    int numerobloque=0;
    char  buff;
    FILE *disco;
    FILE *discoaux;
    int flag_libres=1;
    FILE *graph;
    int sub_total=0;
    if(disco=fopen(direccion,"r")){
        //-------------------------------------------Grafoca
        graph=fopen("grafica.dot","w");
        fprintf(graph,"digraph G{\n");
        discoaux=fopen(direccion,"r");
        bloqueArchivo archivo;
        bloqueCarpetas carpeta;
        bloquedeApuntadores apuntador;

        fseek(discoaux,inicio_bb,SEEK_SET);
        fseek(disco,inicio_blo,SEEK_SET);       
        while(ftell(discoaux)<=biE){
            buff=fgetc(discoaux);
            fseek(disco,inicio_blo+(sizeof(bloqueArchivo)*numerobloque),SEEK_SET);
            if(buff=='1'){
                fread(&carpeta,sizeof(bloqueCarpetas),1,disco);
                fprintf(graph,"subgraph inode_%d{\n",numerobloque);
                fprintf(graph, "nodo_%d [ shape=none,",numerobloque);
                fprintf(graph, "label=< <TABLE> <TR> <TD COLSPAN=\"2\"> Bloque de Carpeta %d </TD></TR>\n",numerobloque);
                fprintf(graph, "<TR> <TD> b_name </TD> <TD> b_inode </TD></TR>\n");
                for(int i=0;i<4;i++){
                    fprintf(graph, "<TR> <TD> %s </TD> <TD> %d </TD></TR>\n",carpeta.b_content[i].b_name,carpeta.b_content[i].b_inodo);
                }
                fprintf(graph, "</TABLE>>]\n");
                fprintf(graph,"}\n");
            }else if(buff=='2'){
                fread(&archivo,sizeof(bloqueCarpetas),1,disco);
                fprintf(graph,"subgraph inode_%d{\n",numerobloque);
                fprintf(graph, "nodo_%d [ shape=none,",numerobloque);
                fprintf(graph, "label=< <TABLE> <TR> <TD COLSPAN=\"2\"> Bloque de Archivo %d </TD></TR>\n",numerobloque);
                fprintf(graph, "<TR> <TD COLSPAN=\"2\"> %s </TD></TR>\n",archivo.b_content);
                fprintf(graph, "</TABLE>>]\n");
                fprintf(graph,"}\n");
            }
            
            numerobloque++;
        }
        
        fclose(disco);
        fprintf(graph,"\n}");
        fclose(graph);
        char comando[400];
        strcpy(comando,"dot -T");
        strcat(comando,extension);
        strcat(comando," grafica.dot -o ");
        strcat(comando,destino);
        system(comando);

    }else{
        printf("Error el disco no existe\n");
    }
}

int agregarGrupo(FILE* disco,char nombre[66]){
    inodeTable inodo;
    inodeTable inodoUsuario;
    bloqueArchivo archivo;
    superBloque super;
    int direccion_inodo=0;
    int direccion_bloque=0;
    int posicion=0;
    char cadena_obtenida[400];
    int usando=0;
    int espaciolibre=0;
    int tam_actual=0;

    memset(ControlGrupos,0,sizeof(ControlGrupos));
    fseek(disco,inicio_inode+sizeof(inodeTable),SEEK_SET);
    fread(&inodo,sizeof(inodeTable),1,disco);
    for(int i=0;i<12;i++){
        if(inodo.i_block[i]!=-1){
            char fase[64]="";
            memset(fase,0,sizeof(fase));
            fseek(disco,inicio_blo+(sizeof(bloqueArchivo)*inodo.i_block[i]),SEEK_SET);
            fread(&archivo,sizeof(bloqueArchivo),1,disco);
            int tamanio=strlen(archivo.b_content);
            for(int j=0;j<tamanio;j++){
                char aux[4]={0};
                sprintf(aux,"%c",archivo.b_content[j]);
                strcat(fase,aux);
            }
            strcat(ControlGrupos,fase);
            posicion=inodo.i_block[i];
        }else break;
    }
    memset(cadena_obtenida,0,sizeof(cadena_obtenida));
    fseek(disco,inicio_blo+(sizeof(bloqueArchivo)*posicion),SEEK_SET);
    fread(&archivo,sizeof(bloqueArchivo),1,disco);
    usando=strlen(archivo.b_content);
    espaciolibre=64-usando;
    strcpy(cadena_obtenida,nombre);
    tam_actual=strlen(cadena_obtenida);
    if(tam_actual<=espaciolibre){
        strcat(archivo.b_content,cadena_obtenida);
        fseek(disco,inicio_blo+(sizeof(bloqueArchivo)*posicion),SEEK_SET);
        fwrite(&archivo,sizeof(bloqueArchivo),1,disco);
        fseek(disco,inicio_inode+sizeof(inodeTable),SEEK_SET);
        fread(&inodoUsuario,sizeof(inodeTable),1,disco);
        inodoUsuario.i_size=inodoUsuario.i_size+tam_actual;
        inodoUsuario.i_mtime=time(NULL);
        fseek(disco,inicio_inode+sizeof(inodeTable),SEEK_SET);
        fwrite(&inodoUsuario,sizeof(inodeTable),1,disco);
        return 1;
    }else{
        char fase1[40]="";
        char fase2[40]="";
        memset(fase1,0,sizeof(fase1));
        memset(fase2,0,sizeof(fase2));
        int registros=0;
        for(registros=0;registros<espaciolibre;registros++){
            char aux[4]={0};
            sprintf(aux,"%c",cadena_obtenida[registros]);
            strcat(fase1,aux);
        }
        strcat(archivo.b_content,fase1);
        for(registros=registros;registros<tam_actual;registros++){
            char aux[4]={0};
            sprintf(aux,"%c",cadena_obtenida[registros]);
            strcat(fase2,aux);
        }
        fseek(disco,inicio_blo+(sizeof(bloqueArchivo)*posicion),SEEK_SET);
        fwrite(&archivo,sizeof(bloqueArchivo),1,disco);
        bloqueArchivo archAux;
        strcpy(archAux.b_content,fase2);
        int bitlibe=buscarPrimerBloque(disco,inicio_super,2,F);
        fseek(disco,inicio_blo+(sizeof(bloqueArchivo)*bitlibe),SEEK_SET);
        fwrite(&archAux,sizeof(bloqueArchivo),1,disco);
        fseek(disco,inicio_bb+(bitlibe*sizeof(char)),SEEK_SET);
        fputc('2',disco);
        fseek(disco,inicio_inode+sizeof(inodeTable),SEEK_SET);
        fread(&inodoUsuario,sizeof(inodeTable),1,disco);
        inodoUsuario.i_size=inodoUsuario.i_size+tam_actual;
        inodoUsuario.i_mtime=time(NULL);
        inodoUsuario.i_block[posicion]=bitlibe;
        fseek(disco,inicio_inode+sizeof(inodeTable),SEEK_SET);
        fwrite(&inodoUsuario,sizeof(inodeTable),1,disco);
        fseek(disco,inicio_super,SEEK_SET);
        fread(&super,sizeof(superBloque),1,disco);
        super.s_first_blo=super.s_first_blo+1;
        super.s_free_block_count=super.s_free_block_count-1;
        fseek(disco,inicio_super,SEEK_SET);
        fwrite(&super,sizeof(superBloque),1,disco);
        return 1;
    }
    return 1;
}

int buscarPrimerBloque(FILE *disco,int start,int bitUso,char FIT){
    char bitTemporal='0';
    superBloque super;
    int tam_for=0;
    int inicio_bitmap=0;

    int bit_libre=-1;
    int existe=0;
    int cant_libres=0;
    int temp_libres=-1;

    char nombreBit[100];
    fseek(disco,start,SEEK_SET);
    fread(&super,sizeof(superBloque),1,disco);
    
    //----------------------------------TIPO DE BLOQUE A BUSCAR----------------------
    if(bitUso==1){
        tam_for=super.s_inodes_count;
        inicio_bitmap=inicio_bi;
        strcpy(nombreBit,"INODOS");
    }else if(bitUso==2){
        tam_for=super.s_blocks_count;
        inicio_bitmap=inicio_bb;
        strcpy(nombreBit,"BLOQUES");
    }
    //------------------------------TIPO DE FIT A UTILIZAR---------------------------
    if(FIT=='F'){
        for(int i=0;i<tam_for;i++){
            fseek(disco,inicio_bitmap+(i*sizeof(char)),SEEK_SET);
            bitTemporal=fgetc(disco);
            if(bitTemporal=='0'){
                bit_libre=i;
                existe=1;
                return bit_libre;
            }
        }

        if(!existe){
            printf("ya no existe bloques libres\n");
            return -1;
        }
    }else if(FIT=='B'){

        for(int i=0;i<tam_for;i++){
            fseek(disco,inicio_bitmap+(i*sizeof(char)),SEEK_SET);
            bitTemporal=fgetc(disco);
            if(bitTemporal=='0'){
                cant_libres++;
                existe=1;
                if(i+1==tam_for){
                    if(temp_libres==-1 || temp_libres==0){
                        temp_libres=cant_libres;
                    }else{
                        if(temp_libres>cant_libres){
                            temp_libres=cant_libres;
                        }
                    }
                    cant_libres=0;
                }
            }else if(bitTemporal=='1'){
                if(temp_libres==-1 || temp_libres==0){
                        temp_libres=cant_libres;
                    }else{
                        if(temp_libres>cant_libres){
                            temp_libres=cant_libres;
                        }
                    }
                    cant_libres=0;
            }
        }

        for(int i=0;i<tam_for;i++){
            fseek(disco,inicio_bitmap+(i*sizeof(char)),SEEK_SET);
            bitTemporal=fgetc(disco);
            if(bitTemporal=='0'){
                cant_libres++;
                if(i+1==tam_for){
                    return (i+1)-cant_libres;
                }
            }else if(bitTemporal=='1'){
                if(temp_libres==cant_libres){
                    temp_libres=cant_libres;
                    return (i+1)-cant_libres;
                }
                cant_libres=0;
            }
        }

        if(!existe){
            printf("Error ya no hay bits libres \n" );
            return -1;
        }


    }else if(FIT=='W'){
        for(int i=0;i<tam_for;i++){
            fseek(disco,inicio_bitmap+(i*sizeof(char)),SEEK_SET);
            bitTemporal=fgetc(disco);
            if(bitTemporal=='0'){
                cant_libres++;
                existe=1;
                if(i+1==tam_for){
                    if(temp_libres==-1 || temp_libres==0){
                        temp_libres=cant_libres;
                    }else{
                        if(temp_libres>cant_libres){
                            temp_libres=cant_libres;
                        }
                    }
                    cant_libres=0;
                }
            }else if(bitTemporal=='1'){
                if(temp_libres==-1 || temp_libres==0){
                        temp_libres=cant_libres;
                    }else{
                        if(temp_libres<cant_libres){
                            temp_libres=cant_libres;
                        }
                    }
                    cant_libres=0;
            }
        }

        for(int i=0;i<tam_for;i++){
            fseek(disco,inicio_bitmap+(i*sizeof(char)),SEEK_SET);
            bitTemporal=fgetc(disco);

            if(bitTemporal=='0'){
                cant_libres++;
                if(i+1==tam_for){
                    return (i+1)-cant_libres;
                }
            }else if(bitTemporal=='1'){
                if(temp_libres==cant_libres){
                    temp_libres=cant_libres;
                    return (i+1)-cant_libres;
                }
                cant_libres=0;
            }
        }

        if(!existe){
            printf("Error ya no hay bloques libres\n");
            return -1;
        }

    }
}

int CREARCARPETA(char *path,int indicadorp){
    superBloque sb;
    FILE* DiscoEnUso;
     int encontrado = 0;
    int byte_inicio = 0;
    int a = 0;
    int numeracion = 0;
    char name[40];
    char nombreCarpeta[85];
    char path2[500];
    char path3[500];
    memset(path2,0,sizeof(path2));
    strcpy(path2,path);
    memset(path3,0,sizeof(path3));
    strcpy(path3,path);
    memset(nombreCarpeta,0,sizeof(nombreCarpeta));
    memset(path,0,sizeof(path));
    memset(name,0,sizeof(name));
    char numeroid;
    char letraid;
    char busquedaid;
    char busquedaid2;
    char fit;
    strcpy(nombreCarpeta,basename(path2));

        DiscoEnUso = fopen(direccion_global,"rb+");
        fseek(DiscoEnUso,0,SEEK_SET);
        fseek(DiscoEnUso,inicio_super,SEEK_SET);
        fread(&sb,sizeof(superBloque),1,DiscoEnUso);

        int ExisteCarpeta = BUSCARCARPETAOARCHIVO(DiscoEnUso,path,inicio_super,&numeracion);
        if(ExisteCarpeta == 1){
            return 0;
        }else if(ExisteCarpeta == 0){
            int Retorno = NUEVACARPETA(DiscoEnUso,F,userlog,indicadorp,inicio_super,path3);
            if(Retorno == 4){
                return 2;
            }else if(Retorno==9){
                return 3;
            }
        }
    fclose(DiscoEnUso);
    return 1;
}


int BUSCARCARPETAOARCHIVO(FILE *DiscoEnUSo,char* path,int inicio_particion,int *numeracion){
    inodeTable InodoUso;
    bloqueCarpetas BC;
    superBloque sb;
    char *listapath[100];
    memset(listapath,0,sizeof(listapath));
    const char s[2] = "\/";
    char *token;
    token= malloc(sizeof(char));
    token = strtok(path, s);
    int contador=0;
    int contador2=0;
    int LugarInododo = 0;
    while(token != NULL)
    {
        listapath[contador]=malloc(200);
        strcpy(listapath[contador],token);
        //printf("LISTA POSICION %d: %s\n", contador,listapath[contador]);
        contador=contador+1;
        token = strtok(NULL, s);
    }
    fseek(DiscoEnUSo,inicio_particion,SEEK_SET);
    fread(&sb,sizeof(superBloque),1,DiscoEnUSo);
     LugarInododo = sb.s_inode_start;
    for(int contador2 = 0; contador2 < contador; contador2++){
        fseek(DiscoEnUSo,LugarInododo,SEEK_SET);
        fread(&InodoUso,sizeof(inodeTable),1,DiscoEnUSo);
        int siguiente =0;
        int iblock = 0;
        for(iblock = 0; iblock < 12; iblock++){
            if(InodoUso.i_block[iblock] != -1){
                int movimiento = LugarInodoBloque(DiscoEnUSo,InodoUso.i_block[iblock],inicio_particion,2);
                fseek(DiscoEnUSo,movimiento,SEEK_SET);
                fread(&BC,sizeof(bloqueCarpetas),1,DiscoEnUSo);
                int iblock2 = 0;;
                for(iblock2 = 0; iblock2 < 4; iblock2++){
                    if((contador2 == contador - 1)&&(strcasecmp(BC.b_content[iblock2].b_name,listapath[contador2]) == 0)){
                        *numeracion = BC.b_content[iblock2].b_inodo;
                        return 1;
                    }else if((contador2 != contador - 1)&&(strcasecmp(BC.b_content[iblock2].b_name,listapath[contador2]) == 0)){
                        LugarInododo = LugarInodoBloque(DiscoEnUSo,BC.b_content[iblock2].b_inodo,inicio_particion,1);
                        siguiente = 1;
                        break;
                    }
                    if(siguiente == 1){
                        break;
                    }
                }
                if(siguiente == 1){
                    break;
                }
            }

        }
    }
    return 0;
}


int LugarInodoBloque(FILE *DiscoEnUSo,int posicion,int inicio_particion,int tipo){
    superBloque sb;
    int resultado = 0;
    fseek(DiscoEnUSo,inicio_particion,SEEK_SET);
    fread(&sb,sizeof(superBloque),1,DiscoEnUSo);
    if(tipo==1){
       resultado = sb.s_inode_start + (sizeof(inodeTable)*posicion);
    }else if(tipo == 2){
       resultado = sb.s_block_start + (sizeof(bloqueCarpetas)*posicion);
    }
    return resultado;
}

int NUEVACARPETA(FILE *DiscoEnUso,char fit,USERLOG UsuarioActual,int indicadorp,int inicio_particion,char* path){
    inodeTable InodoUso,OTROBA;
    bloqueCarpetas BC,BC2,NueC;
    superBloque sb;
    char LarutaDelaCarpeta[500];
    char copiaPath[500];
    char copiaPath2[500];
    char copiaPath3[500];
    char *listapath2[500];
    char NombreCarpeta[84];
    memset(LarutaDelaCarpeta,0,sizeof(LarutaDelaCarpeta));
    memset(copiaPath,0,sizeof(copiaPath));
    memset(copiaPath2,0,sizeof(copiaPath2));
    memset(copiaPath3,0,sizeof(copiaPath3));
    memset(listapath2,0,sizeof(listapath2));
    strcpy(copiaPath,path);
    strcpy(copiaPath2,path);
    strcpy(copiaPath3,path);
    strcpy(LarutaDelaCarpeta,dirname(copiaPath));
    strcpy(NombreCarpeta,basename(copiaPath2));
    const char s[2] = "\/";
    char *token;
    token= malloc(sizeof(char));
    token = strtok(path, s);
    int contador=0;
    int contador2=0;
    int iblock =0;
    int iblock2 =0;
    int comprobacionRuta = 0;
    int LugarInododo = 0;
    while(token != NULL)
    {
        listapath2[contador]=malloc(200);
        strcpy(listapath2[contador],token);
        //printf("LISTA POSICION %d: %s\n", contador,listapath2[contador]);
        contador=contador+1;
        token = strtok(NULL, s);
    }
    fseek(DiscoEnUso,inicio_particion,SEEK_SET);
    fread(&sb,sizeof(superBloque),1,DiscoEnUso);
    if (contador == 1){
        int mover = 0;
        int mover2 = 0;
        int libre = 0;
        fseek(DiscoEnUso,sb.s_inode_start+ (sizeof(inodeTable)*LugarInododo),SEEK_SET);
        fread(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
        for(iblock=0;iblock<12;iblock++){
            if(InodoUso.i_block[iblock] != -1){
                fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueCarpetas)*InodoUso.i_block[iblock]),SEEK_SET);
                fread(&BC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                for(iblock2=0;iblock2<4;iblock2++){
                    if(BC.b_content[iblock2].b_inodo != -1){
                        libre = 0;
                    }else{
                        libre = 1;
                        mover = iblock2;
                        mover2 = iblock;
                        break;
                    }
                }
            }
        }
        if(libre == 1){
            int per=permisosDeEscritura(InodoUso.i_perm,userlog.id_user==InodoUso.i_uid,userlog.id_grup==InodoUso.i_gid);
            if(per==1 || flag_root){
                char auxbituno = '1';
                int bitLibre = 0;
                int bitlibre2 = 0;
                int a =0;
                bitLibre = buscarPrimerBloque(DiscoEnUso,inicio_super,1,F);
                BC.b_content[mover].b_inodo = bitLibre;
                strcpy(BC.b_content[mover].b_name,NombreCarpeta);
                fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueCarpetas)*InodoUso.i_block[mover2]),SEEK_SET);
                fwrite(&BC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                OTROBA.i_uid = UsuarioActual.id_user;
                OTROBA.i_gid = UsuarioActual.id_grup;
                OTROBA.i_size= 0;
                OTROBA.i_atime = time(NULL);
                OTROBA.i_ctime = time(NULL);
                OTROBA.i_mtime = time(NULL);
                bitlibre2 = buscarPrimerBloque(DiscoEnUso,inicio_super,2,fit);
                OTROBA.i_block[0] = bitlibre2;
                for(a = 1;a<15;a++){
                    OTROBA.i_block[a] = -1;
                }
                OTROBA.i_type = '0';
                OTROBA.i_perm = 664;
                fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                BC2.b_content[0].b_inodo =bitLibre;
                strcpy(BC2.b_content[0].b_name,".");
                BC2.b_content[1].b_inodo =LugarInododo;
                strcpy(BC2.b_content[1].b_name,"..");
                BC2.b_content[2].b_inodo =-1;
                strcpy(BC2.b_content[2].b_name,"");
                BC2.b_content[3].b_inodo =-1;
                strcpy(BC2.b_content[3].b_name,"");
                fseek(DiscoEnUso,sb.s_block_start +(sizeof(bloqueCarpetas)*bitlibre2),SEEK_SET);
                fwrite(&BC2,sizeof(bloqueCarpetas),1,DiscoEnUso);
                fseek(DiscoEnUso,sb.s_bm_block_start + (bitlibre2*sizeof(char)),SEEK_SET);
                fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                sb.s_free_block_count = sb.s_free_block_count - 1;
                sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                sb.s_first_ino = sb.s_first_ino +1;
                sb.s_first_blo = sb.s_first_blo +1;
                fseek(DiscoEnUso,inicio_super,SEEK_SET);
                fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);
                strcpy(Unionpath,copiaPath2);
                return 6;
            }else return 9;
        }else if (libre == 0){
            char auxbituno = '1';
            int bitLibre = 0;
            int bitlibre2 = 0;
            int segui = 0;
            int a = 0;
            bitlibre2 = buscarPrimerBloque(DiscoEnUso,inicio_super,2,fit);
            fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
            fread(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
            for(int segui=0;segui <12;segui++){
                if(InodoUso.i_block[segui]==-1){
                    iblock2 = segui;
                    break;
                }
            }
            int per=permisosDeEscritura(InodoUso.i_perm,userlog.id_user==InodoUso.i_uid,userlog.id_grup==InodoUso.i_gid);
            if(per==1 || flag_root){
                InodoUso.i_block[iblock2]=bitlibre2;
                fseek(DiscoEnUso,sb.s_inode_start,SEEK_SET);
                fwrite(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
                fseek(DiscoEnUso,sb.s_bm_block_start + (bitlibre2*sizeof(char)),SEEK_SET);
                fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                bitLibre = buscarPrimerBloque(DiscoEnUso,inicio_super,1,fit);
                NueC.b_content[0].b_inodo =bitLibre;
                strcpy(NueC.b_content[0].b_name,NombreCarpeta);
                NueC.b_content[1].b_inodo =-1;
                strcpy(NueC.b_content[1].b_name,"");
                NueC.b_content[2].b_inodo =-1;
                strcpy(NueC.b_content[2].b_name,"");
                NueC.b_content[3].b_inodo =-1;
                strcpy(NueC.b_content[3].b_name,"");
                fseek(DiscoEnUso,sb.s_block_start +(sizeof(bloqueCarpetas)*bitlibre2),SEEK_SET);
                fwrite(&NueC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                OTROBA.i_uid = UsuarioActual.id_user;
                OTROBA.i_gid = UsuarioActual.id_grup;
                OTROBA.i_size= 0;
                OTROBA.i_atime = time(NULL);
                OTROBA.i_ctime = time(NULL);
                OTROBA.i_mtime = time(NULL);
                bitlibre2 = buscarPrimerBloque(DiscoEnUso,inicio_super,2,fit);
                OTROBA.i_block[0] = bitlibre2;
                for(a = 1;a<15;a++){
                    OTROBA.i_block[a] = -1;
                }
                OTROBA.i_type = '0';
                OTROBA.i_perm = 664;
                fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                BC2.b_content[0].b_inodo =bitLibre;
                strcpy(BC2.b_content[0].b_name,".");
                BC2.b_content[1].b_inodo =LugarInododo;
                strcpy(BC2.b_content[1].b_name,"..");
                BC2.b_content[2].b_inodo =-1;
                strcpy(BC2.b_content[2].b_name,"");
                BC2.b_content[3].b_inodo =-1;
                strcpy(BC2.b_content[3].b_name,"");
                fseek(DiscoEnUso,sb.s_bm_block_start + (bitlibre2*sizeof(char)),SEEK_SET);
                fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                fseek(DiscoEnUso,sb.s_block_start +(sizeof(bloqueCarpetas)*bitlibre2),SEEK_SET);
                fwrite(&BC2,sizeof(bloqueCarpetas),1,DiscoEnUso);
                fseek(DiscoEnUso,inicio_super,SEEK_SET);
                fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                sb.s_free_block_count = sb.s_free_block_count - 2;
                sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                sb.s_first_blo = sb.s_first_blo +2;
                sb.s_first_ino = sb.s_first_ino +1;
                fseek(DiscoEnUso,inicio_super,SEEK_SET);
                fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);
                strcpy(Unionpath,copiaPath2);
            return 6;
            }else return 9;
        }

    }else if (contador >1){
        int SaberExiste = BUSCARCARPETAOARCHIVO(DiscoEnUso,LarutaDelaCarpeta,inicio_particion,&LugarInododo);
        if(SaberExiste == 0){
            if(indicadorp == 0){
                return 4;
            }else{
                memset(Unionpath,0,sizeof(Unionpath));
                for(contador2 = 0;contador2<contador;contador2++){
                    strcat(Unionpath, "\/");
                    strcat(Unionpath,listapath2[contador2]);
                    strcpy(COPIAUnionpath,Unionpath);
                    int lovio = BUSCARCARPETAOARCHIVO(DiscoEnUso,Unionpath,inicio_particion,&LugarInododo);
                    if(lovio == 0){
                        NUEVACARPETA(DiscoEnUso,fit,UsuarioActual,0,inicio_particion,COPIAUnionpath);
                    }
                }
            }
        }else if(SaberExiste == 1){
            int mover = 0;
            int mover2 = 0;
            int libre = 0;
            fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
            fread(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
            for(iblock=0;iblock<12;iblock++){
                if(InodoUso.i_block[iblock] != -1){
                    fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueCarpetas)*InodoUso.i_block[iblock]),SEEK_SET);
                    fread(&BC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                    if(iblock==0){
                        for(iblock2=2;iblock2<4;iblock2++){
                            if(BC.b_content[iblock2].b_inodo != -1){
                                libre = 0;
                            }else{
                                libre = 1;
                                mover = iblock2;
                                mover2 = iblock;
                                break;
                            }
                        } 
                    }else{
                        for(iblock2=0;iblock2<4;iblock2++){
                            if(BC.b_content[iblock2].b_inodo != -1){
                                libre = 0;
                            }else{
                                libre = 1;
                                mover = iblock2;
                                mover2 = iblock;
                                break;
                            }
                        }
                    }

                }
            }
            if(libre == 1){
                int per=permisosDeEscritura(InodoUso.i_perm,userlog.id_user==InodoUso.i_uid,userlog.id_grup==InodoUso.i_gid);
                if(per || flag_root){
                    char auxbituno = '1';
                    int bitLibre = 0;
                    int bitlibre2 = 0;
                    int a =0;
                    bitLibre = buscarPrimerBloque(DiscoEnUso,inicio_particion,1,fit);
                    BC.b_content[mover].b_inodo = bitLibre;
                    strcpy(BC.b_content[mover].b_name,NombreCarpeta);
                    fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueCarpetas)*InodoUso.i_block[mover2]),SEEK_SET);
                    fwrite(&BC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                    OTROBA.i_uid = UsuarioActual.id_user;
                    OTROBA.i_gid = UsuarioActual.id_grup;
                    OTROBA.i_size= 0;
                    OTROBA.i_atime = time(NULL);
                    OTROBA.i_ctime = time(NULL);
                    OTROBA.i_mtime = time(NULL);
                    bitlibre2 = buscarPrimerBloque(DiscoEnUso,inicio_particion,2,fit);
                    OTROBA.i_block[0] = bitlibre2;
                    for(a = 1;a<15;a++){
                        OTROBA.i_block[a] = -1;
                    }
                    OTROBA.i_type = '0';
                    OTROBA.i_perm = 664;
                    fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                    fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                    BC2.b_content[0].b_inodo =bitLibre;
                    strcpy(BC2.b_content[0].b_name,".");
                    BC2.b_content[1].b_inodo =LugarInododo;
                    strcpy(BC2.b_content[1].b_name,"..");
                    BC2.b_content[2].b_inodo =-1;
                    strcpy(BC2.b_content[2].b_name,"");
                    BC2.b_content[3].b_inodo =-1;
                    strcpy(BC2.b_content[3].b_name,"");
                    fseek(DiscoEnUso,sb.s_block_start +(sizeof(bloqueCarpetas)*bitlibre2),SEEK_SET);
                    fwrite(&BC2,sizeof(bloqueCarpetas),1,DiscoEnUso);
                    fseek(DiscoEnUso,sb.s_bm_block_start + (bitlibre2*sizeof(char)),SEEK_SET);
                    fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                    fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                    fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                    fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                    fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                    sb.s_free_block_count = sb.s_free_block_count - 1;
                    sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                    sb.s_first_ino = sb.s_first_ino +1;
                    sb.s_first_blo = sb.s_first_blo +1;
                    fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                    fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);
                    strcpy(Unionpath,copiaPath2);
                    return 6;
                }else return 9;
            }else if (libre == 0){
                int per=permisosDeEscritura(InodoUso.i_perm,userlog.id_user==InodoUso.i_uid,userlog.id_grup==InodoUso.i_gid);
                if(per || flag_root){
                    char auxbituno = '1';
                    int bitLibre = 0;
                    int bitlibre2 = 0;
                    int segui = 0;
                    int a = 0;
                    bitlibre2 = buscarPrimerBloque(DiscoEnUso,inicio_particion,2,fit);
                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                    fread(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
                    for(segui=0;segui <12;segui++){
                        if(InodoUso.i_block[segui]==-1){
                            iblock2 = segui;
                            break;
                        }
                    }
                    InodoUso.i_block[iblock2]=bitlibre2;
                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                    fwrite(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
                    fseek(DiscoEnUso,sb.s_bm_block_start + (bitlibre2*sizeof(char)),SEEK_SET);
                    fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                    bitLibre = buscarPrimerBloque(DiscoEnUso,inicio_particion,1,fit);
                    NueC.b_content[0].b_inodo =bitLibre;
                    strcpy(NueC.b_content[0].b_name,NombreCarpeta);
                    NueC.b_content[1].b_inodo =-1;
                    strcpy(NueC.b_content[1].b_name,"");
                    NueC.b_content[2].b_inodo =-1;
                    strcpy(NueC.b_content[2].b_name,"");
                    NueC.b_content[3].b_inodo =-1;
                    strcpy(NueC.b_content[3].b_name,"");
                    fseek(DiscoEnUso,sb.s_block_start +(sizeof(bloqueCarpetas)*bitlibre2),SEEK_SET);
                    fwrite(&NueC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                    fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                    fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                    OTROBA.i_uid = UsuarioActual.id_user;
                    OTROBA.i_gid = UsuarioActual.id_grup;
                    OTROBA.i_size= 0;
                    OTROBA.i_atime = time(NULL);
                    OTROBA.i_ctime = time(NULL);
                    OTROBA.i_mtime = time(NULL);
                    bitlibre2 = buscarPrimerBloque(DiscoEnUso,inicio_particion,2,fit);
                    OTROBA.i_block[0] = bitlibre2;
                    for(a = 1;a<15;a++){
                        OTROBA.i_block[a] = -1;
                    }
                    OTROBA.i_type = '0';
                    OTROBA.i_perm = 664;
                    fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                    fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                    BC2.b_content[0].b_inodo =bitLibre;
                    strcpy(BC2.b_content[0].b_name,".");
                    BC2.b_content[1].b_inodo =LugarInododo;
                    strcpy(BC2.b_content[1].b_name,"..");
                    BC2.b_content[2].b_inodo =-1;
                    strcpy(BC2.b_content[2].b_name,"");
                    BC2.b_content[3].b_inodo =-1;
                    strcpy(BC2.b_content[3].b_name,"");
                    fseek(DiscoEnUso,sb.s_bm_block_start + (bitlibre2*sizeof(char)),SEEK_SET);
                    fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                    fseek(DiscoEnUso,sb.s_block_start +(sizeof(bloqueCarpetas)*bitlibre2),SEEK_SET);
                    fwrite(&BC2,sizeof(bloqueCarpetas),1,DiscoEnUso);
                    fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                    fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                    sb.s_free_block_count = sb.s_free_block_count - 2;
                    sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                    sb.s_first_blo = sb.s_first_blo +2;
                    sb.s_first_ino = sb.s_first_ino +1;
                    fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                    fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);
                    strcpy(Unionpath,copiaPath2);
                    return 6;
                }else return 9;
            }
        }
    }

}


int CREARARCHIVO(char* pa_path,int indicadorp,int size,char* cont,int c){
    int encontrado = 0;
    int byte_inicio = 0;
    int a = 0;
    int numeracion = 0;
    char path[150];
    char name[40];
    char pa_path2[500];
    char pa_path3[500];
    char pa_path32[500];
    memset(pa_path2,0,sizeof(pa_path2));
    strcpy(pa_path2,pa_path);
    memset(pa_path3,0,sizeof(pa_path3));
    strcpy(pa_path3,pa_path);

    memset(pa_path32,0,sizeof(pa_path32));
    strcpy(pa_path32,pa_path3);
    FILE* DiscoEnUso;
    superBloque sb;
    memset(path,0,sizeof(path));
    memset(name,0,sizeof(name));
    char numeroid;
    char letraid;
    char busquedaid;
    char busquedaid2;
    char fit;

        /*Verifica que el Disco exista*/
        DiscoEnUso = fopen(direccion_global,"rb+");
        fseek(DiscoEnUso,0,SEEK_SET);
        fseek(DiscoEnUso,inicio_super,SEEK_SET);
        fread(&sb,sizeof(superBloque),1,DiscoEnUso);

        int ExisteArchivo = BUSCARCARPETAOARCHIVO(DiscoEnUso,pa_path,inicio_super,&numeracion);
        if(ExisteArchivo == 1){
             return 5;
        }else if(ExisteArchivo == 0){
            int Retorno = NUEVOARCHIVO(DiscoEnUso,F,userlog,indicadorp,inicio_super,pa_path3,size,cont);
            if(indicadorp){
                int re2=NUEVOARCHIVO(DiscoEnUso,F,userlog,0,inicio_super,pa_path32,size,cont);
            }
            if(Retorno == 4){
                return 4;
            }else if(Retorno == 7){
                return 7;
            }
        }
        
    fclose(DiscoEnUso);
    return 6;
}


int NUEVOARCHIVO(FILE *DiscoEnUso,char fit,USERLOG UsuarioActual,int indicadorp,int inicio_particion,char* pampa,int size,char* cont){
    inodeTable InodoUso,OTROBA,InodoUso2;
    bloqueCarpetas BC,BC2,NueC;
    superBloque sb;
    char LarutaDelaCarpeta[500];
    char copiaPath[500];
    char copiaPath2[500];
    char copiaPath3[500];
    char *listapath2[500];
    char NombreCarpeta[84];
    memset(LarutaDelaCarpeta,0,sizeof(LarutaDelaCarpeta));
    memset(copiaPath,0,sizeof(copiaPath));
    memset(copiaPath2,0,sizeof(copiaPath2));
    memset(copiaPath3,0,sizeof(copiaPath3));
    memset(listapath2,0,sizeof(listapath2));
    strcpy(copiaPath,pampa);
    strcpy(copiaPath2,pampa);
    strcpy(copiaPath3,pampa);
    strcpy(LarutaDelaCarpeta,dirname(copiaPath));
    strcpy(NombreCarpeta,basename(copiaPath2));
    const char s[2] = "\/";
    char *token;
    token= malloc(sizeof(char));
    token = strtok(pampa, s);
    int contador=0;
    int contador2=0;
    int iblock =0;
    int iblock2 =0;
    int Indicadorcont = 0;
    int LugarInododo = 0;
    while(token != NULL)
    {
        listapath2[contador]=malloc(200);
        strcpy(listapath2[contador],token);
        //printf("LISTA POSICION %d: %s\n", contador,listapath2[contador]);
        contador=contador+1;
        token = strtok(NULL, s);
    }
    char *llenador = NULL;
    if(strcasecmp(cont,"") != 0){
        FILE *ArchivoEnUso = fopen(cont, "r");
        if(ArchivoEnUso == NULL){
            return 7;
        }
        size = 0;
        fseek(ArchivoEnUso, 0, SEEK_END);
        size = ftell(ArchivoEnUso);
        rewind(ArchivoEnUso);
        llenador = malloc((size + 1) * sizeof(*llenador));
        fread(llenador, size, 1, ArchivoEnUso);
        llenador[size] = '\0';
        printf("%s\n", llenador);
        Indicadorcont = 1;
    }

    fseek(DiscoEnUso,inicio_particion,SEEK_SET);
    fread(&sb,sizeof(superBloque),1,DiscoEnUso);
    if (contador == 1){
        int mover = 0;
        int mover2 = 0;
        int libre = 0;
        fseek(DiscoEnUso,sb.s_inode_start+ (sizeof(inodeTable)*LugarInododo),SEEK_SET);
        fread(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
        for(iblock=0;iblock<12;iblock++){
            if(InodoUso.i_block[iblock] != -1){
                fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueCarpetas)*InodoUso.i_block[iblock]),SEEK_SET);
                fread(&BC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                for(iblock2=0;iblock2<4;iblock2++){
                    if(BC.b_content[iblock2].b_inodo != -1){
                        libre = 0;
                    }else{
                        libre = 1;
                        mover = iblock2;
                        mover2 = iblock;
                        break;
                    }
                }
            }
        }
        if(libre == 1){
            int per=permisosDeEscritura(InodoUso.i_perm,userlog.id_user==InodoUso.i_uid,userlog.id_grup==InodoUso.i_gid);
            if(per || flag_root){
                char auxbituno = '2';
                int bitLibre = 0;
                int bitlibre2 = 0;
                int a =0;
                bitLibre = buscarPrimerBloque(DiscoEnUso,inicio_particion,1,fit);
                BC.b_content[mover].b_inodo = bitLibre;
                strcpy(BC.b_content[mover].b_name,NombreCarpeta);
                fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueCarpetas)*InodoUso.i_block[mover2]),SEEK_SET);
                fwrite(&BC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                OTROBA.i_uid = UsuarioActual.id_user;
                OTROBA.i_gid = UsuarioActual.id_grup;
                OTROBA.i_size= 0;
                OTROBA.i_atime = time(NULL);
                OTROBA.i_ctime = time(NULL);
                OTROBA.i_mtime = time(NULL);
                for(a = 0;a<15;a++){
                    OTROBA.i_block[a] = -1;
                }
                OTROBA.i_type = '1';
                OTROBA.i_perm = 664;
                fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                fwrite(&bituno,sizeof(char),1,DiscoEnUso);
                int verArchivo = BUSCARCARPETAOARCHIVO(DiscoEnUso,copiaPath3,inicio_particion,&LugarInododo);
                if(verArchivo == 1){
                    memset(Unionpath,0,sizeof(Unionpath));
                    fseek(DiscoEnUso,sb.s_inode_start+(sizeof(inodeTable)*LugarInododo),SEEK_SET);
                    fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                    double aux = 0;
                    int cantidadBloques = 1;
                    int index = 0;
                    int conIndex = 0;
                    int charNum = 0;
                    int caracteres = size;
                    if(size != 0){
                        int x = 0;
                        aux = (double)size/64;
                        cantidadBloques = (int)ceil(aux);
                        bloqueArchivo nBA;

                        for(x = 0; x < cantidadBloques; x++){
                            char auxbit = '2';
                            int bloqueActual = buscarPrimerBloque(DiscoEnUso,inicio_particion,2,fit);
                            fseek(DiscoEnUso,sb.s_bm_block_start+(sizeof(char)*bloqueActual),SEEK_SET);
                            fwrite(&auxbit,sizeof(char),1,DiscoEnUso);
                            memset(nBA.b_content,0,sizeof(nBA.b_content));
                            if(caracteres > 64){
                                for(index = 0; index < 64; index++){
                                    if(Indicadorcont == 0){
                                        nBA.b_content[index] = charNum + '0';
                                        charNum++;
                                        if(charNum == 10){
                                            charNum = 0;
                                        }
                                    }else if(Indicadorcont == 1){
                                        nBA.b_content[index] = llenador[conIndex];
                                        conIndex ++;
                                    }
                                }

                                fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                InodoUso2.i_block[x] = bloqueActual;
                                InodoUso2.i_size = size;
                                fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueArchivo)*bloqueActual),SEEK_SET);
                                fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);
                                caracteres = caracteres - 64;
                            }else{

                                for(index = 0; index < caracteres; index++){
                                    if(Indicadorcont == 0){
                                        nBA.b_content[index] = charNum + '0';
                                        charNum++;
                                        if(charNum == 10){
                                            charNum = 0;
                                        }
                                    }else if(Indicadorcont == 1){
                                        nBA.b_content[index] = llenador[conIndex];
                                        conIndex ++;
                                    }
                                }
                                fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                InodoUso2.i_block[x] = bloqueActual;
                                InodoUso2.i_size = size;
                                fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                fseek(DiscoEnUso,sb.s_block_start+(bloqueActual * sizeof(bloqueArchivo)),SEEK_SET);
                                fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);

                            }

                        }

                        fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                        fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                        sb.s_free_block_count = sb.s_free_block_count - cantidadBloques;
                        sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                        sb.s_first_ino = sb.s_first_ino +1;
                        sb.s_first_blo = sb.s_first_blo +cantidadBloques;
                        fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                        fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);

                    }
                }
                strcpy(Unionpath,copiaPath2);
                return 6;
            }else return 9;
        }else if (libre == 0){
            char auxbituno = '2';
            int bitLibre = 0;
            int bitlibre2 = 0;
            int segui = 0;
            int a = 0;
            bitlibre2 = buscarPrimerBloque(DiscoEnUso,inicio_particion,2,fit);
            fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
            fread(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
            int per=permisosDeEscritura(InodoUso.i_perm,userlog.id_user==InodoUso.i_uid,userlog.id_grup==InodoUso.i_gid);
            if(per || flag_root){
                for(segui=0;segui <12;segui++){
                    if(InodoUso.i_block[segui]==-1){
                        iblock2 = segui;
                        break;
                    }
                }
                InodoUso.i_block[iblock2]=bitlibre2;
                fseek(DiscoEnUso,sb.s_inode_start,SEEK_SET);
                fwrite(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
                fseek(DiscoEnUso,sb.s_bm_block_start + (bitlibre2*sizeof(char)),SEEK_SET);
                fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                bitLibre = buscarPrimerBloque(DiscoEnUso,inicio_particion,1,fit);
                NueC.b_content[0].b_inodo =bitLibre;
                strcpy(NueC.b_content[0].b_name,NombreCarpeta);
                NueC.b_content[1].b_inodo =-1;
                strcpy(NueC.b_content[1].b_name,"");
                NueC.b_content[2].b_inodo =-1;
                strcpy(NueC.b_content[2].b_name,"");
                NueC.b_content[3].b_inodo =-1;
                strcpy(NueC.b_content[3].b_name,"");
                fseek(DiscoEnUso,sb.s_block_start +(sizeof(bloqueCarpetas)*bitlibre2),SEEK_SET);
                fwrite(&NueC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                fwrite(&bituno,sizeof(char),1,DiscoEnUso);
                OTROBA.i_uid = UsuarioActual.id_user;
                OTROBA.i_gid = UsuarioActual.id_grup;
                OTROBA.i_size= 0;
                OTROBA.i_atime = time(NULL);
                OTROBA.i_ctime = time(NULL);
                OTROBA.i_mtime = time(NULL);
                for(a = 0;a<15;a++){
                    OTROBA.i_block[a] = -1;
                }
                fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                fwrite(&bituno,sizeof(char),1,DiscoEnUso);
                int verArchivo = BUSCARCARPETAOARCHIVO(DiscoEnUso,copiaPath3,inicio_particion,&LugarInododo);
                if(verArchivo == 1){
                    memset(Unionpath,0,sizeof(Unionpath));
                    fseek(DiscoEnUso,sb.s_inode_start+(sizeof(inodeTable)*LugarInododo),SEEK_SET);
                    fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                    double aux = 0;
                    int cantidadBloques = 1;
                    int index = 0;
                    int conIndex = 0;
                    int charNum = 0;
                    int caracteres = size;
                    if(size != 0){
                        int x = 0;
                        aux = (double)size/64;
                        cantidadBloques = (int)ceil(aux);
                        bloqueArchivo nBA;

                        for(x = 0; x < cantidadBloques; x++){
                            char auxbit = '2';
                            int bloqueActual = buscarPrimerBloque(DiscoEnUso,inicio_particion,2,fit);
                            fseek(DiscoEnUso,sb.s_bm_block_start+(sizeof(char)*bloqueActual),SEEK_SET);
                            fwrite(&auxbit,sizeof(char),1,DiscoEnUso);
                            memset(nBA.b_content,0,sizeof(nBA.b_content));
                            if(caracteres > 64){
                                for(index = 0; index < 64; index++){
                                    if(Indicadorcont == 0){
                                        nBA.b_content[index] = charNum + '0';
                                        charNum++;
                                        if(charNum == 10){
                                            charNum = 0;
                                        }
                                    }else if(Indicadorcont == 1){
                                        nBA.b_content[index] = llenador[conIndex];
                                        conIndex ++;
                                    }
                                }

                                fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                InodoUso2.i_block[x] = bloqueActual;
                                InodoUso2.i_size = size;
                                fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueArchivo)*bloqueActual),SEEK_SET);
                                fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);
                                caracteres = caracteres - 64;
                            }else{

                                for(index = 0; index < caracteres; index++){
                                    if(Indicadorcont == 0){
                                        nBA.b_content[index] = charNum + '0';
                                        charNum++;
                                        if(charNum == 10){
                                            charNum = 0;
                                        }
                                    }else if(Indicadorcont == 1){
                                        nBA.b_content[index] = llenador[conIndex];
                                        conIndex ++;
                                    }
                                }
                                fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                InodoUso2.i_block[x] = bloqueActual;
                                InodoUso2.i_size = size;
                                fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                fseek(DiscoEnUso,sb.s_block_start+(bloqueActual * sizeof(bloqueArchivo)),SEEK_SET);
                                fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);

                            }

                        }
                    }

                fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                sb.s_free_block_count = sb.s_free_block_count - cantidadBloques;
                sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                sb.s_first_blo = sb.s_first_blo +cantidadBloques;
                sb.s_first_ino = sb.s_first_ino +1;
                fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);
                }
            }else return 9;

        }
            strcpy(Unionpath,copiaPath2);
            return 6;

    }else if (contador >1){
        int SaberExiste = BUSCARCARPETAOARCHIVO(DiscoEnUso,LarutaDelaCarpeta,inicio_particion,&LugarInododo);
        if(SaberExiste == 0){
            if(indicadorp == 0){
                return 4;
            }else{
                memset(Unionpath,0,sizeof(Unionpath));
                for(contador2 = 0;contador2<contador-1;contador2++){
                    strcat(Unionpath, "\/");
                    strcat(Unionpath,listapath2[contador2]);
                    strcpy(COPIAUnionpath,Unionpath);
                    int lovio = BUSCARCARPETAOARCHIVO(DiscoEnUso,Unionpath,inicio_particion,&LugarInododo);
                    if(lovio == 0){
                        NUEVACARPETA(DiscoEnUso,fit,UsuarioActual,0,inicio_particion,COPIAUnionpath);
                    }
                }
            }
        }else if(SaberExiste == 1){
            int mover = 0;
            int mover2 = 0;
            int libre = 0;
            fseek(DiscoEnUso,sb.s_inode_start+ (sizeof(inodeTable)*LugarInododo),SEEK_SET);
            fread(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
            for(iblock=0;iblock<12;iblock++){
                if(InodoUso.i_block[iblock] != -1){
                    fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueCarpetas)*InodoUso.i_block[iblock]),SEEK_SET);
                    fread(&BC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                    if(iblock==0){
                        for(iblock2=2;iblock2<4;iblock2++){
                            if(BC.b_content[iblock2].b_inodo != -1){
                                libre = 0;
                            }else{
                                libre = 1;
                                mover = iblock2;
                                mover2 = iblock;
                                break;
                            }
                        } 
                    }else{
                        for(iblock2=0;iblock2<4;iblock2++){
                            if(BC.b_content[iblock2].b_inodo != -1){
                                libre = 0;
                            }else{
                                libre = 1;
                                mover = iblock2;
                                mover2 = iblock;
                                break;
                            }
                        }
                    }
                }
            }
            if(libre == 1){
                int per=permisosDeEscritura(InodoUso.i_perm,userlog.id_user==InodoUso.i_uid,userlog.id_grup==InodoUso.i_gid);
                if(per || flag_root){
                    char auxbituno = '2';
                    int bitLibre = 0;
                    int bitlibre2 = 0;
                    int a =0;
                    bitLibre = buscarPrimerBloque(DiscoEnUso,inicio_particion,1,fit);
                    BC.b_content[mover].b_inodo = bitLibre;
                    strcpy(BC.b_content[mover].b_name,NombreCarpeta);
                    fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueCarpetas)*InodoUso.i_block[mover2]),SEEK_SET);
                    fwrite(&BC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                    OTROBA.i_uid = UsuarioActual.id_user;
                    OTROBA.i_gid = UsuarioActual.id_grup;
                    OTROBA.i_size= 0;
                    OTROBA.i_atime = time(NULL);
                    OTROBA.i_ctime = time(NULL);
                    OTROBA.i_mtime = time(NULL);
                    for(a = 0;a<15;a++){
                        OTROBA.i_block[a] = -1;
                    }
                    OTROBA.i_type = '1';
                    OTROBA.i_perm = 664;
                    fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                    fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                    fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                    fwrite(&bituno,sizeof(char),1,DiscoEnUso);
                    int verArchivo = BUSCARCARPETAOARCHIVO(DiscoEnUso,copiaPath3,inicio_particion,&LugarInododo);
                    if(verArchivo == 1){
                        memset(Unionpath,0,sizeof(Unionpath));
                        fseek(DiscoEnUso,sb.s_inode_start+(sizeof(inodeTable)*LugarInododo),SEEK_SET);
                        fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                        double aux = 0;
                        int cantidadBloques = 1;
                        int index = 0;
                        int conIndex = 0;
                        int charNum = 0;
                        int caracteres = size;
                        if(size != 0){
                            int x = 0;
                            aux = (double)size/64;
                            cantidadBloques = (int)ceil(aux);
                            bloqueArchivo nBA;

                            for(x = 0; x < cantidadBloques; x++){
                                char auxbit = '2';
                                int bloqueActual = buscarPrimerBloque(DiscoEnUso,inicio_particion,2,fit);
                                fseek(DiscoEnUso,sb.s_bm_block_start+(sizeof(char)*bloqueActual),SEEK_SET);
                                fwrite(&auxbit,sizeof(char),1,DiscoEnUso);
                                memset(nBA.b_content,0,sizeof(nBA.b_content));
                                if(caracteres > 64){
                                    for(index = 0; index < 64; index++){
                                        if(Indicadorcont == 0){
                                            nBA.b_content[index] = charNum + '0';
                                            charNum++;
                                            if(charNum == 10){
                                                charNum = 0;
                                            }
                                        }else if(Indicadorcont == 1){
                                            nBA.b_content[index] = llenador[conIndex];
                                            conIndex ++;
                                        }
                                    }

                                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                    fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                    InodoUso2.i_block[x] = bloqueActual;
                                    InodoUso2.i_size = size;
                                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                    fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                    fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueArchivo)*bloqueActual),SEEK_SET);
                                    fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);
                                    caracteres = caracteres - 64;
                                }else{

                                    for(index = 0; index < caracteres; index++){
                                        if(Indicadorcont == 0){
                                            nBA.b_content[index] = charNum + '0';
                                            charNum++;
                                            if(charNum == 10){
                                                charNum = 0;
                                            }
                                        }else if(Indicadorcont == 1){
                                            nBA.b_content[index] = llenador[conIndex];
                                            conIndex ++;
                                        }
                                    }
                                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                    fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                    InodoUso2.i_block[x] = bloqueActual;
                                    InodoUso2.i_size = size;
                                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                    fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                    fseek(DiscoEnUso,sb.s_block_start+(bloqueActual * sizeof(bloqueArchivo)),SEEK_SET);
                                    fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);

                                }

                            }

                            fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                            fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                            sb.s_free_block_count = sb.s_free_block_count - cantidadBloques;
                            sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                            sb.s_first_ino = sb.s_first_ino +1;
                            sb.s_first_blo = sb.s_first_blo +cantidadBloques;
                            fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                            fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);

                        }
                    }
                    strcpy(Unionpath,copiaPath2);
                    return 6;
                }else return 9;
            }else if (libre == 0){
                char auxbituno = '2';
                int bitLibre = 0;
                int bitlibre2 = 0;
                int segui = 0;
                int a = 0;
                bitlibre2 = buscarPrimerBloque(DiscoEnUso,inicio_particion,2,fit);
                fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                fread(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
                int per=permisosDeEscritura(InodoUso.i_perm,userlog.id_user==InodoUso.i_uid,userlog.id_grup==InodoUso.i_gid);
                if(per || flag_root){
                    for(segui=0;segui <12;segui++){
                        if(InodoUso.i_block[segui]==-1){
                            iblock2 = segui;
                            break;
                        }
                    }
                    InodoUso.i_block[iblock2]=bitlibre2;
                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                    fwrite(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
                    fseek(DiscoEnUso,sb.s_bm_block_start + (bitlibre2*sizeof(char)),SEEK_SET);
                    fwrite(&bituno,sizeof(char),1,DiscoEnUso);
                    bitLibre = buscarPrimerBloque(DiscoEnUso,inicio_particion,1,fit);
                    NueC.b_content[0].b_inodo =bitLibre;
                    strcpy(NueC.b_content[0].b_name,NombreCarpeta);
                    NueC.b_content[1].b_inodo =-1;
                    strcpy(NueC.b_content[1].b_name,"");
                    NueC.b_content[2].b_inodo =-1;
                    strcpy(NueC.b_content[2].b_name,"");
                    NueC.b_content[3].b_inodo =-1;
                    strcpy(NueC.b_content[3].b_name,"");
                    fseek(DiscoEnUso,sb.s_block_start +(sizeof(bloqueCarpetas)*bitlibre2),SEEK_SET);
                    fwrite(&NueC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                    fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                    fwrite(&bituno,sizeof(char),1,DiscoEnUso);
                    OTROBA.i_uid = UsuarioActual.id_user;
                    OTROBA.i_gid = UsuarioActual.id_grup;
                    OTROBA.i_size= 0;
                    OTROBA.i_atime = time(NULL);
                    OTROBA.i_ctime = time(NULL);
                    OTROBA.i_mtime = time(NULL);
                    for(a = 0;a<15;a++){
                        OTROBA.i_block[a] = -1;
                    }
                    OTROBA.i_type = '1';
                    OTROBA.i_perm = 664;
                    fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                    fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                    fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                    fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                    int verArchivo = BUSCARCARPETAOARCHIVO(DiscoEnUso,copiaPath3,inicio_particion,&LugarInododo);
                    if(verArchivo == 1){
                        memset(Unionpath,0,sizeof(Unionpath));
                        fseek(DiscoEnUso,sb.s_inode_start+(sizeof(inodeTable)*LugarInododo),SEEK_SET);
                        fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                        double aux = 0;
                        int cantidadBloques = 1;
                        int index = 0;
                        int conIndex = 0;
                        int charNum = 0;
                        int caracteres = size;
                        if(size != 0){
                            int x = 0;
                            aux = (double)size/64;
                            cantidadBloques = (int)ceil(aux);
                            bloqueArchivo nBA;

                            for(x = 0; x < cantidadBloques; x++){
                                char auxbit = '2';
                                int bloqueActual = buscarPrimerBloque(DiscoEnUso,inicio_particion,2,fit);
                                fseek(DiscoEnUso,sb.s_bm_block_start+(sizeof(char)*bloqueActual),SEEK_SET);
                                fwrite(&auxbit,sizeof(char),1,DiscoEnUso);
                                memset(nBA.b_content,0,sizeof(nBA.b_content));
                                if(caracteres > 64){
                                    for(index = 0; index < 64; index++){
                                        if(Indicadorcont == 0){
                                            nBA.b_content[index] = charNum + '0';
                                            charNum++;
                                            if(charNum == 10){
                                                charNum = 0;
                                            }
                                        }else if(Indicadorcont == 1){
                                            nBA.b_content[index] = llenador[conIndex];
                                            conIndex ++;
                                        }
                                    }

                                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                    fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                    InodoUso2.i_block[x] = bloqueActual;
                                    InodoUso2.i_size = size;
                                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                    fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                    fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueArchivo)*bloqueActual),SEEK_SET);
                                    fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);
                                    caracteres = caracteres - 64;
                                }else{

                                    for(index = 0; index < caracteres; index++){
                                        if(Indicadorcont == 0){
                                            nBA.b_content[index] = charNum + '0';
                                            charNum++;
                                            if(charNum == 10){
                                                charNum = 0;
                                            }
                                        }else if(Indicadorcont == 1){
                                            nBA.b_content[index] = llenador[conIndex];
                                            conIndex ++;
                                        }
                                    }
                                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                    fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                    InodoUso2.i_block[x] = bloqueActual;
                                    InodoUso2.i_size = size;
                                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                    fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                    fseek(DiscoEnUso,sb.s_block_start+(bloqueActual * sizeof(bloqueArchivo)),SEEK_SET);
                                    fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);

                                }

                            }
                        }

                    fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                    fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                    sb.s_free_block_count = sb.s_free_block_count - cantidadBloques;
                    sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                    sb.s_first_blo = sb.s_first_blo +cantidadBloques;
                    sb.s_first_ino = sb.s_first_ino +1;
                    fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                    fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);
                    }
                }else return 9;

            }
                strcpy(Unionpath,copiaPath2);
                return 6;

        }
    }
}

void crearReporte(int inicio,int final,char direccion[400]){
    FILE *disco;
    FILE *report;
    char byte;
    char sat='\n';
    int salto=0;
    report=fopen(direccion,"w+");
    fseek(report,0,SEEK_SET);
    disco=fopen(direccion_global,"rb+");
    for(int i=0;i<final;i++){
        fseek(disco,inicio+(sizeof(char)*i),SEEK_SET);
        byte=fgetc(disco);
        if(byte!='0') byte='1';
        fputc(byte,report);
        if(salto==19){
            salto=0;
            fputc(sat,report);
        }else salto++;
    }
    fclose(disco);
    fclose(report);
}

void crearTree(char direccion[400],char extension[40]){
    
    int numeroInode=0;
    char  buff;
    char buff2;
    FILE *disco;
    FILE *discoaux;
    int flag_libres=1;
    FILE *graph;
    int sub_total=0;
    if(disco=fopen(direccion_global,"r")){
        //-------------------------------------------Grafoca
        graph=fopen("grafica.dot","w");
        fprintf(graph,"digraph G{ rankdir=\"LR\"\n");
        discoaux=fopen(direccion_global,"r");
        inodeTable inode;
        bloqueCarpetas carpeta;
        bloqueArchivo archivo;
       fseek(discoaux,inicio_bi,SEEK_SET);
        fseek(disco,inicio_inode,SEEK_SET);       
        while(ftell(discoaux)<inicio_bb){
            buff=fgetc(discoaux);
            fseek(disco,inicio_inode+(sizeof(inodeTable)*numeroInode),SEEK_SET);
            fread(&inode,sizeof(inodeTable),1,disco);           
            if(buff!='0'){
                fprintf(graph,"subgraph inode_%d{\n",numeroInode);
                fprintf(graph, "inodo_%d [ shape=none,",numeroInode);
                fprintf(graph, "label=< <TABLE> <TR> <TD COLSPAN=\"2\"> Inodo %d </TD></TR>\n",numeroInode);
                fprintf(graph, "<TR> <TD> i_uid </TD> <TD> %d </TD></TR>\n",inode.i_uid);
                fprintf(graph, "<TR> <TD> i_gid </TD> <TD> %d </TD></TR>\n",inode.i_gid);
                fprintf(graph, "<TR> <TD> i_size </TD> <TD> %d </TD></TR>\n",inode.i_size);
                fprintf(graph, "<TR> <TD> i_gid </TD> <TD> %d </TD></TR>\n",inode.i_gid);
                struct tm *tm;
                char fecha[100];
                tm=localtime(&inode.i_atime);
                strftime(fecha,100,"%d/%m/%y %H:%S",tm);
                fprintf(graph, "<TR> <TD> i_atime </TD> <TD> %s </TD></TR>\n",fecha);
                tm=localtime(&inode.i_ctime);
                strftime(fecha,100,"%d/%m/%y %H:%S",tm);
                fprintf(graph, "<TR> <TD> i_ctime </TD> <TD> %s </TD></TR>\n",fecha);
                tm=localtime(&inode.i_mtime);
                strftime(fecha,100,"%d/%m/%y %H:%S",tm);
                fprintf(graph, "<TR> <TD> i_mtime </TD> <TD> %s </TD></TR>\n",fecha);
                for(int i=0;i<15;i++){
                    fprintf(graph, "<TR> <TD> i_block_%d </TD> <TD> %d </TD></TR>\n",i,inode.i_block[i]);
                }
                fprintf(graph, "<TR> <TD> i_type </TD> <TD> %c </TD></TR>\n",inode.i_type);
                fprintf(graph, "<TR> <TD> i_perm </TD> <TD> %d </TD></TR>\n",inode.i_perm);
                fprintf(graph, "</TABLE>>]\n");
                fprintf(graph,"}\n");
                for(int i=0;i<15;i++){
                    if(inode.i_block[i]!=-1){
                        fprintf(graph, "inodo_%d->bloque_%d;\n",numeroInode,inode.i_block[i]);
                        fseek(disco,inicio_bb+(sizeof(char)*inode.i_block[i]),SEEK_SET);
                        buff2=fgetc(disco);
                        if(buff2=='1'){
                            fseek(disco,inicio_blo+(sizeof(bloqueCarpetas)*inode.i_block[i]),SEEK_SET);
                            fread(&carpeta,sizeof(bloqueCarpetas),1,disco);
                            fprintf(graph, "bloque_%d [ shape=none,",inode.i_block[i]);
                            fprintf(graph, "label=< <TABLE> <TR> <TD COLSPAN=\"2\"> Bloque de Carpeta %d </TD></TR>\n",inode.i_block[i]);
                            fprintf(graph, "<TR> <TD> b_name </TD> <TD> b_inode </TD></TR>\n");
                            for(int j=0;j<4;j++){
                                fprintf(graph, "<TR> <TD> %s </TD> <TD> %d </TD></TR>\n",carpeta.b_content[j].b_name,carpeta.b_content[j].b_inodo);
                            }
                            fprintf(graph, "</TABLE>>];\n");
                            for(int j=0;j<4;j++){
                                if((strcmp(carpeta.b_content[j].b_name,".")!=0 && strcmp(carpeta.b_content[j].b_name,"..")!=0 ) && carpeta.b_content[j].b_inodo!=-1){
                                    fprintf(graph, "bloque_%d->inodo_%d;\n",inode.i_block[i],carpeta.b_content[j].b_inodo);
                                }
                            }
                        }else if(buff2=='2'){
                            fseek(disco,inicio_blo+(sizeof(bloqueCarpetas)*inode.i_block[i]),SEEK_SET);
                            fread(&archivo,sizeof(bloqueCarpetas),1,disco);
                            fprintf(graph, "bloque_%d [ shape=none,",inode.i_block[i]);
                            fprintf(graph, "label=< <TABLE> <TR> <TD COLSPAN=\"2\"> Bloque de Archivo %d </TD></TR>\n",inode.i_block[i]);
                            fprintf(graph, "<TR> <TD COLSPAN=\"2\"> %s </TD></TR>\n",archivo.b_content);
                            fprintf(graph, "</TABLE>>]\n");
                        }
                    }
                }
            }
            
            numeroInode++;
        }
        
        fclose(disco);
        fprintf(graph,"\n}");
        fclose(graph);
        char comando[400];
        strcpy(comando,"dot -T");
        strcat(comando,extension);
        strcat(comando," grafica.dot -o ");
        strcat(comando,direccion);
        system(comando);
        fclose(discoaux);

    }else{
        printf("Error el disco no existe\n");
    }
}

void graficarSuper(char destino[400],char extension[20]){
    superBloque super;

    FILE* disco;
    FILE* graph;
    disco=fopen(direccion_global,"r");
    fseek(disco,inicio_super,SEEK_SET);
    
    fread(&super,sizeof(superBloque),1,disco);

    graph=fopen("grafica.dot","w");
        fprintf(graph,"digraph G{\n");
                fprintf(graph,"subgraph super{\n");
                fprintf(graph, "nodo [ shape=none,");
                fprintf(graph, "label=< <TABLE> <TR> <TD COLSPAN=\"2\"> SUPERBLOQUE </TD></TR>\n");
                fprintf(graph, "<TR> <TD> Nombre </TD> <TD> Valor </TD></TR>\n");
                fprintf(graph, "<TR> <TD> s_inodes_count </TD> <TD> %d </TD></TR>\n",super.s_inodes_count);
                fprintf(graph, "<TR> <TD> s_blocks_count </TD> <TD> %d </TD></TR>\n",super.s_blocks_count);
                fprintf(graph, "<TR> <TD> s_free_block_count </TD> <TD> %d </TD></TR>\n",super.s_free_block_count);
                fprintf(graph, "<TR> <TD> s_free_inodes_count </TD> <TD> %d </TD></TR>\n",super.s_free_inodes_count);

                struct tm *tm;
                char fecha[100];
                tm=localtime(&super.s_mtime);
                strftime(fecha,100,"%d/%m/%y %H:%S",tm);
                fprintf(graph, "<TR> <TD> s_mtime </TD> <TD> %s </TD></TR>\n",fecha);


                tm=localtime(&super.s_umtime);
                strftime(fecha,100,"%d/%m/%y %H:%S",tm);
                fprintf(graph, "<TR> <TD> s_umtime </TD> <TD> %s </TD></TR>\n",fecha);
                
                fprintf(graph, "<TR> <TD> s_mnt_count </TD> <TD> %d </TD></TR>\n",super.s_mnt_count);
                fprintf(graph, "<TR> <TD> s_magic </TD> <TD> %d </TD></TR>\n",super.s_magic);
                fprintf(graph, "<TR> <TD> s_inode_size </TD> <TD> %d </TD></TR>\n",super.s_inode_size);
                fprintf(graph, "<TR> <TD> s_block_size </TD> <TD> %d </TD></TR>\n",super.s_block_size);
                fprintf(graph, "<TR> <TD> s_first_ino </TD> <TD> %d </TD></TR>\n",super.s_first_ino);
                fprintf(graph, "<TR> <TD> s_first_blo </TD> <TD> %d </TD></TR>\n",super.s_first_blo);
                fprintf(graph, "<TR> <TD> s_bm_inode_start </TD> <TD> %d </TD></TR>\n",super.s_bm_inode_start);
                fprintf(graph, "<TR> <TD> s_block_start </TD> <TD> %d </TD></TR>\n",super.s_block_start);

                fprintf(graph, "</TABLE>>]\n");
                fprintf(graph,"}\n");
            
    fprintf(graph,"\n}");
    char comando[400];
    strcpy(comando,"dot -T");
    strcat(comando,extension);
    strcat(comando," grafica.dot -o ");
    strcat(comando,destino);
    system(comando);            
        
    fclose(graph);
  
    fclose(disco);
}

void reporteFile(char* pa_path,char destino[400]){
    
    FILE* disco;
    FILE* archivo;
    inodeTable inodo;

    disco=fopen(direccion_global,"rb+");
    archivo=fopen(destino,"w+");
    char cont[1000];
    memset(cont,0,sizeof(cont));
    strcpy(cont,"");
    int numeracion=0;
    int resu=0;
    resu=BUSCARCARPETAOARCHIVO(disco,pa_path,inicio_super,&numeracion);
    if(resu==1){
        fseek(disco,inicio_inode+(sizeof(inodeTable)*numeracion),SEEK_SET);
        fread(&inodo,sizeof(inodeTable),1,disco);
        for(int i=0;i<15;i++){
            if(inodo.i_block[i]!=-1){
                bloqueArchivo barchivo;
                fseek(disco,inicio_blo+(sizeof(bloqueCarpetas)*inodo.i_block[i]),SEEK_SET);
                fread(&barchivo,sizeof(bloqueArchivo),1,disco);
                strcat(cont,barchivo.b_content);
            }else break;
        }
        fprintf(archivo, "%s\n",cont);
    }else printf("No existe el archivo para hacer el reporte\n");
    fclose(archivo);
    fclose(disco);
}

int permisosDeEscritura(int permisos,int flu,int flg){
    int milla=(permisos/1000);
    int usuari=(permisos-(milla*1000))/100;
    int gru=(permisos-(milla*1000+usuari*100))/10;
    int otros=permisos-(milla*1000+usuari*100+gru*10);
    if((usuari==2 || usuari==3 || usuari==6 || usuari==7) && flu) return 1;
    else{
        if((gru==2 || gru==3 || gru==6 || gru==7) && flg) return 1;
        else{
            if(otros==2 || otros==3 || otros==6 || otros==7) return 1;
            else return 0;
        }
    }
    return 0;
}

void reportePermisos(char* pa_path,char destino[400],char extension[20]){
    char byte='0';
    FILE* disco;
    FILE* graph;
    inodeTable inodo;

    disco=fopen(direccion_global,"rb+");
    graph=fopen("grafica.dot","w");

    char cont[1000];
    memset(cont,0,sizeof(cont));
    strcpy(cont,"");
    int numeracion=0;
    int resu=0;
    resu=BUSCARCARPETAOARCHIVO(disco,pa_path,inicio_super,&numeracion);
    
    if(resu==1){
        fprintf(graph,"digraph G{\n");
        fprintf(graph, "nodo [ shape=none,");
        fprintf(graph, "label=< <TABLE>\n");
        fprintf(graph,"<TR> <TD>PERMISOS</TD><TD>OWNER</TD><TD>GRUPO</TD><TD>SIZE</TD><TD>FECHA</TD><TD>HORA</TD><TD>TIPO</TD><TD>NAME</TD></TR>\n");
        fseek(disco,inicio_inode+(sizeof(inodeTable)*numeracion),SEEK_SET);
        fread(&inodo,sizeof(inodeTable),1,disco);
        for(int i=0;i<15;i++){
            if(inodo.i_block[i]!=-1){

                bloqueArchivo barchivo;
                bloqueCarpetas carp;
                inodeTable auxInodo;
                fseek(disco,inicio_blo+(sizeof(bloqueArchivo)*inodo.i_block[i]),SEEK_SET);
                fread(&carp,sizeof(bloqueArchivo),1,disco);
                
                for(int j=2;j<4;j++){
                    if(carp.b_content[j].b_inodo!=-1){
                        fseek(disco,inicio_inode+(sizeof(inodeTable)*carp.b_content[j].b_inodo),SEEK_SET);
                        fread(&auxInodo,sizeof(inodeTable),1,disco);
                        int milla=(auxInodo.i_perm/1000);
                        int usuari=(auxInodo.i_perm-(milla*1000))/100;
                        int gru=(auxInodo.i_perm-(milla*1000+usuari*100))/10;
                        int otros=auxInodo.i_perm-(milla*1000+usuari*100+gru*10);

                        char perm[50];
                        memset(perm,0,sizeof(perm));
                        if(usuari==0) strcpy(perm,"- - -");
                        else if(usuari==1) strcpy(perm, "- - x");
                        else if(usuari==2) strcpy(perm,"- w -");
                        else if(usuari==3) strcpy(perm,"- w x");
                        else if(usuari==4) strcpy(perm,"r - -");
                        else if(usuari==5) strcpy(perm,"r - x");
                        else if(usuari==6) strcpy(perm,"r w -");
                        else strcpy(perm,"r w x");

                        if(gru==0) strcat(perm,"- - -");
                        else if(gru==1) strcat(perm, "- - x");
                        else if(gru==2) strcat(perm,"- w -");
                        else if(gru==3) strcat(perm,"- w x");
                        else if(gru==4) strcat(perm,"r - -");
                        else if(gru==5) strcat(perm,"r - x");
                        else if(gru==6) strcat(perm,"r w -");
                        else strcat(perm,"r w x");

                        if(otros==0) strcat(perm,"- - -");
                        else if(otros==1) strcat(perm, "- - x");
                        else if(otros==2) strcat(perm,"- w -");
                        else if(otros==3) strcat(perm,"- w x");
                        else if(otros==4) strcat(perm,"r - -");
                        else if(otros==5) strcat(perm,"r - x");
                        else if(otros==6) strcat(perm,"r w -");
                        else strcat(perm,"r w x");
                        USERLOG temp;
                        temp=buscarUsuarioPropi(auxInodo.i_uid);
                        fprintf(graph,"<TR> <TD>%s</TD><TD>%s</TD><TD>%s</TD><TD>%d</TD>",perm,temp.usuario,temp.grupo,inodo.i_size);
                        
                         struct tm *tm;
                        char fecha[100];
                        tm=localtime(&auxInodo.i_atime);
                        strftime(fecha,100,"%d/%m/%y",tm);

                        strftime(fecha,100,"%H:%S",tm);

                        fprintf(graph, "<TD>%s</TD><TD>%s</TD>",fecha,fecha );

                        fseek(disco,inicio_bb+(sizeof(char)*carp.b_content[j].b_inodo),SEEK_SET);
                        byte=fgetc(disco);
                        if(byte=='1'){
                            fprintf(graph, "<TD>Carpeta</TD><TD>%s</TD></TR>",carp.b_content[j].b_name);
                        }else if(byte=='2'){
                            fprintf(graph, "<TD>Archivo</TD><TD>%s</TD></TR>\n",carp.b_content[j].b_name);
                        }
                        
                    }
                }
                
                
                
                   
            }else break;
        }
        fprintf(graph, "</TABLE>>];\n");
        fprintf(graph,"\n}");
    }else printf("No existe el archivo para hacer el reporte\n");
    
    fclose(graph);
    char comando[400];
    strcpy(comando,"dot -T");
    strcat(comando,extension);
    strcat(comando," grafica.dot -o ");
    strcat(comando,destino);
    system(comando);                   
    fclose(disco);
}


USERLOG buscarUsuarioPropi(int idv){
    FILE *disco;
    FILE *discoaux;
    if(disco=fopen(direccion_global,"rb+")){
        char cadena[300]="\0";
        discoaux=fopen(direccion_global,"rb+");
        inodeTable inodo;
        fseek(disco,inicio_inode+(sizeof(inodeTable)),SEEK_SET);
        fread(&inodo,sizeof(inodeTable),1,disco);
        memset(ControlGrupos,0,sizeof(ControlGrupos));
        for(int i=0;i<15;i++){
            if(inodo.i_block[i]!=-1){
                char fase[64]="";
                memset(fase,0,sizeof(fase));
                bloqueArchivo archivo;
                fseek(discoaux,inicio_blo,SEEK_SET);
                fread(&archivo,sizeof(bloqueArchivo),1,discoaux);
                for(int j=0;j<inodo.i_block[i];j++){
                    fread(&archivo,sizeof(bloqueArchivo),1,discoaux);
                }
                strcat(cadena,archivo.b_content);
            }
        }
        char *end_str;
        char *ptr=strtok_r(cadena,"\n",&end_str);
            while(ptr!=NULL){
                char id[2];
                char tipo[2];
                char grup[12];
                char user[12];
                char pass[12];
                char *end_token;
                char *token2=strtok_r(ptr,",",&end_token);
                strcpy(id,token2);

                if(strcmp(id,"0")!=0){
                    token2=strtok_r(NULL,",",&end_token);
                    strcpy(tipo,token2);
                    if(strcmp(tipo,"U")==0){
                        token2=strtok_r(NULL,",",&end_token);
                        strcpy(grup,token2);
                        token2=strtok_r(NULL,",",&end_token);
                        strcpy(user,token2);
                        token2=strtok_r(NULL,",",&end_token);
                        if((atoi(id)==idv)){ 
                            USERLOG temp;
                            memset(temp.usuario,0,sizeof(temp.usuario));
                            strcpy(temp.usuario,user);
                            memset(temp.grupo,0,sizeof(temp.grupo));
                            strcpy(temp.grupo,grup);
                            return temp;
                        }
                    }
                }
                
                ptr=strtok_r(NULL,"\n",&end_str);
            }
        fclose(disco);
        fclose(discoaux);
    }
    return userlog;
}


void guardarJournal(int accion,int tipo,int permisos,char nom[50],char conte[100]){
    JOURNAL journal;
    journal.Journal_tipo_operacion=accion;
    journal.Journal_tipo=tipo;
    strcpy(journal.Journal_nombre,nom);
    strcpy(journal.Journal_contenido,conte);
    journal.Journal_fecha=time(NULL);
    strcpy(journal.Journal_propietario,userlog.usuario);
    journal.Journal_Permisos=permisos;
    
    FILE* disco;
    disco=fopen(direccion_global,"rb+");
    fseek(disco,inicio_journal,SEEK_SET);
    fwrite(&journal,sizeof(JOURNAL),1,disco);
    inicio_journal+=sizeof(JOURNAL);
    fclose(disco);
    
}

void metodoLoss(int inicio,int final){
    FILE* disco;
    disco=fopen(direccion_global,"rb+");
    char byte='0';
    fseek(disco,inicio,SEEK_SET);
    for(int i=inicio;i<final;i++){
        fputc(byte,disco);
    }
    fclose(disco);
    printf("FATAL ERROR: PERDIDA DE DATOS \n");
}


void cambiarpermisos(FILE* disco,int pos,int permisos){
    inodeTable inodo;
    bloqueCarpetas carpeta;
    char byte='0';
    fseek(disco,inicio_inode+(sizeof(inodeTable)*pos),SEEK_SET);
    fread(&inodo,sizeof(inodeTable),1,disco);
    inodo.i_perm=permisos;
    fseek(disco,inicio_inode+(sizeof(inodeTable)*pos),SEEK_SET);
    fwrite(&inodo,sizeof(inodeTable),1,disco);
    for(int i=0;i<15;i++){
        if(inodo.i_block[i]!=-1){
            fseek(disco,inicio_bb+(sizeof(char)*inodo.i_block[i]),SEEK_SET);
            byte=fgetc(disco);
            if(byte=='1'){
                fseek(disco,inicio_blo+(sizeof(bloqueCarpetas)*inodo.i_block[i]),SEEK_SET);
                fread(&carpeta,sizeof(bloqueCarpetas),1,disco);
                for(int j=0;j<4;j++){
                    if(carpeta.b_content[j].b_inodo!=-1 && strcmp(carpeta.b_content[j].b_name,".")!=0 &&  strcmp(carpeta.b_content[j].b_name,"..")!=0){
                        cambiarpermisos(disco,carpeta.b_content[j].b_inodo,permisos);    

                    }
                }
            }
        }
    }
}

int buscarNumeroPalabra(char *p){
    while(*p){
        if(*p>47 && *p<58) return atoi(p);
        *p++;
    }

    return -1;
}

int insertarOrdenado(int id,char cadena[300]){
    struct Lista* nuevo=(struct Lista*)malloc(sizeof(struct Lista));
    nuevo->id=id;
    nuevo->anterior=NULL;
    nuevo->siguiente=NULL;
    strcpy(nuevo->cadena,cadena);
    if(primeroLista==NULL){
        primeroLista=nuevo;
        ultimoLista=nuevo;
        return 1;
    }else{
        if(primeroLista->id>id){
            nuevo->siguiente=primeroLista;
            primeroLista->anterior=nuevo;
            primeroLista=nuevo;
            return 1;
        }else if(ultimoLista->id<id){
            ultimoLista->siguiente=nuevo;
            nuevo->anterior=ultimoLista;
            ultimoLista=nuevo;
            return 1;
        }else{
            struct Lista* aux=primeroLista;
            while(aux!=NULL){
                if(aux->id==id) return 0;
                else if(aux->id>id) break;
                else aux=aux->siguiente;
            }
            struct Lista* temp=aux->anterior;
            nuevo->anterior=temp;
            temp->siguiente=nuevo;
            nuevo->siguiente=aux;
            aux->anterior=nuevo;
            return 1;

        }

    }
}


int permisosdeLectura(int permisos,int flu,int flg){
    int milla=(permisos/1000);
    int usuari=(permisos-(milla*1000))/100;
    int gru=(permisos-(milla*1000+usuari*100))/10;
    int otros=permisos-(milla*1000+usuari*100+gru*10);
    if((usuari>3) && flu) return 1;
    else{
        if((gru>3) && flg) return 1;
        else{
            if(otros>3) return 1;
            else return 0;
        }
    }
    return 0;
}

void reportarJournaling(char destino[400]){
    FILE* arch;
    arch=fopen(destino,"w");
    fprintf(arch, "OPERACION,TIPO,NOMBRE,CONTENIDO,FECHA,PROPIETARIO,PERMISOS\n" );

                            JOURNAL journal;
                            FILE* disco;
                            disco=fopen(direccion_global,"rb+");
                            fseek(disco,inicio_super+sizeof(superBloque),SEEK_SET);
                            
                            while(ftell(disco)<inicio_journal){
                                fread(&journal,sizeof(JOURNAL),1,disco);                                
                                  struct tm *tm;
                                    char fecha[100];
                                    tm=localtime(&journal.Journal_fecha);
                                    strftime(fecha,100,"%d/%m/%y %H:%S",tm);
                                    fprintf(arch, "%d,%d,%s,%s,%s,%s,%d\n",journal.Journal_tipo_operacion,journal.Journal_tipo,journal.Journal_nombre,journal.Journal_contenido,fecha,journal.Journal_propietario,journal.Journal_Permisos);  
                                
                                
                            }

    fclose(disco);
    fclose(arch);

}


int permisosLecturaRecursividad(FILE* disco,int numeracion){
    
    inodeTable inodo;

    fseek(disco,inicio_inode+(sizeof(inodeTable)*numeracion),SEEK_SET);
    fread(&inodo,sizeof(inodeTable),1,disco);
    int perm=permisosdeLectura(inodo.i_perm,inodo.i_uid==userlog.id_user,inodo.i_gid==userlog.id_grup);
    if(perm){
        int retto=1;
        if(!inodo.i_type){


            for(int i=0;i<12;i++){
                if(inodo.i_block[i]!=-1){
                    char byte='0';
                    fseek(disco,inicio_bb+(sizeof(char)*inodo.i_block[i]),SEEK_SET);
                    byte=fgetc(disco);
                    if(byte=='1'){
                        bloqueCarpetas carpeta;
                        fseek(disco,inicio_blo+(sizeof(bloqueCarpetas)*inodo.i_block[i]),SEEK_SET);
                        fread(&carpeta,sizeof(bloqueCarpetas),1,disco);
                        for(int j=0;j<4;j++){
                            if(carpeta.b_content[j].b_inodo!=-1){
                                if(strcmp(carpeta.b_content[j].b_name,".")!=0 && strcmp(carpeta.b_content[j].b_name,"..")!=0){
                                retto= retto && permisosLecturaRecursividad(disco,carpeta.b_content[j].b_inodo); 
                                }
                            }else return retto;
                        }
                    }else{
                        fseek(disco,inicio_inode+(sizeof(inodeTable)*inodo.i_block[i]),SEEK_SET);
                        inodeTable inodoAux;
                        fread(&inodoAux,sizeof(inodeTable),1,disco);
                        return permisosdeLectura(inodo.i_perm,inodo.i_uid==userlog.id_user,inodo.i_gid==userlog.id_grup);
                    }
                }else return retto;
            }
            return retto;
        }else permisosdeLectura(inodo.i_perm,inodo.i_uid==userlog.id_user,inodo.i_gid==userlog.id_grup);
    }else return 0;
}

int permisosEscrituraRecursividad(FILE* disco,int numeracion){
    
    inodeTable inodo;

    fseek(disco,inicio_inode+(sizeof(inodeTable)*numeracion),SEEK_SET);
    fread(&inodo,sizeof(inodeTable),1,disco);
    int perm=permisosDeEscritura(inodo.i_perm,inodo.i_uid==userlog.id_user,inodo.i_gid==userlog.id_grup);
    if(perm){
        int retto=1;
        if(!inodo.i_type){


            for(int i=0;i<12;i++){
                if(inodo.i_block[i]!=-1){
                    char byte='0';
                    fseek(disco,inicio_bb+(sizeof(char)*inodo.i_block[i]),SEEK_SET);
                    byte=fgetc(disco);
                    if(byte=='1'){
                        bloqueCarpetas carpeta;
                        fseek(disco,inicio_blo+(sizeof(bloqueCarpetas)*inodo.i_block[i]),SEEK_SET);
                        fread(&carpeta,sizeof(bloqueCarpetas),1,disco);
                        for(int j=0;j<4;j++){
                            if(carpeta.b_content[j].b_inodo!=-1){
                                if(strcmp(carpeta.b_content[j].b_name,".")!=0 && strcmp(carpeta.b_content[j].b_name,"..")!=0){
                                retto= retto && permisosEscrituraRecursividad(disco,carpeta.b_content[j].b_inodo); 
                                }
                            }else return retto;
                        }
                    }else{
                        fseek(disco,inicio_inode+(sizeof(inodeTable)*inodo.i_block[i]),SEEK_SET);
                        inodeTable inodoAux;
                        fread(&inodoAux,sizeof(inodeTable),1,disco);
                        return permisosDeEscritura(inodo.i_perm,inodo.i_uid==userlog.id_user,inodo.i_gid==userlog.id_grup);
                    }
                }else return retto;
            }
            return retto;
        }else permisosDeEscritura(inodo.i_perm,inodo.i_uid==userlog.id_user,inodo.i_gid==userlog.id_grup);
    }else return 0;
}


void eliminarRecursividad(FILE* disco,int numeracion){
    
    inodeTable inodo;
    char borrar='0';

    fseek(disco,inicio_inode+(sizeof(inodeTable)*numeracion),SEEK_SET);
    fread(&inodo,sizeof(inodeTable),1,disco);
    
        int retto=1;
        if(!inodo.i_type){
            for(int i=0;i<12;i++){
                if(inodo.i_block[i]!=-1){
                    char byte='0';
                    fseek(disco,inicio_bb+(sizeof(char)*inodo.i_block[i]),SEEK_SET);
                    byte=fgetc(disco);
                    if(byte=='1'){
                        bloqueCarpetas carpeta;
                        fseek(disco,inicio_blo+(sizeof(bloqueCarpetas)*inodo.i_block[i]),SEEK_SET);
                        fread(&carpeta,sizeof(bloqueCarpetas),1,disco);
                        for(int j=0;j<4;j++){
                            if(carpeta.b_content[j].b_inodo!=-1){
                                if(strcmp(carpeta.b_content[j].b_name,".")!=0 && strcmp(carpeta.b_content[j].b_name,"..")!=0){
                                    eliminarRecursividad(disco,carpeta.b_content[j].b_inodo);
                                }
                            }
                        }
                        fseek(disco,inicio_bb+(sizeof(char)*inodo.i_block[i]),SEEK_SET);
                        fputc(borrar,disco);
                    }
                }
            }
            fseek(disco,inicio_bi+(sizeof(char)*numeracion),SEEK_SET);
            fputc(borrar,disco);
        }else {

            for(int i=0;i<15;i++){
                if(inodo.i_block[i]!=-1){
                    fseek(disco,inicio_blo+(sizeof(char)*inodo.i_block[i]),SEEK_SET); 
                    fputc(borrar,disco); 
                }else break;
            }

            fseek(disco,inicio_bi+(sizeof(char)*numeracion),SEEK_SET);
            fputc(borrar,disco);
        }
  
}


int BUSCARCARPETAOARCHIVO2(FILE *DiscoEnUSo,char* path,int inicio_particion,int *numeracion,int *index){
    inodeTable InodoUso;
    bloqueCarpetas BC;
    superBloque sb;
    char *listapath[100];
    memset(listapath,0,sizeof(listapath));
    const char s[2] = "\/";
    char *token;
    token= malloc(sizeof(char));
    token = strtok(path, s);
    int contador=0;
    int contador2=0;
    int LugarInododo = 0;
    while(token != NULL)
    {
        listapath[contador]=malloc(200);
        strcpy(listapath[contador],token);
        //printf("LISTA POSICION %d: %s\n", contador,listapath[contador]);
        contador=contador+1;
        token = strtok(NULL, s);
    }
    fseek(DiscoEnUSo,inicio_particion,SEEK_SET);
    fread(&sb,sizeof(superBloque),1,DiscoEnUSo);
     LugarInododo = sb.s_inode_start;
    for(int contador2 = 0; contador2 < contador; contador2++){
        fseek(DiscoEnUSo,LugarInododo,SEEK_SET);
        fread(&InodoUso,sizeof(inodeTable),1,DiscoEnUSo);
        int siguiente =0;
        int iblock = 0;
        for(iblock = 0; iblock < 12; iblock++){
            if(InodoUso.i_block[iblock] != -1){
                int movimiento = LugarInodoBloque(DiscoEnUSo,InodoUso.i_block[iblock],inicio_particion,2);
                fseek(DiscoEnUSo,movimiento,SEEK_SET);
                fread(&BC,sizeof(bloqueCarpetas),1,DiscoEnUSo);
                int iblock2 = 0;;
                for(iblock2 = 0; iblock2 < 4; iblock2++){
                    if((contador2 == contador - 1)&&(strcasecmp(BC.b_content[iblock2].b_name,listapath[contador2]) == 0)){
                        *numeracion = InodoUso.i_block[iblock];
                        *index=iblock2;
                        return 1;
                    }else if((contador2 != contador - 1)&&(strcasecmp(BC.b_content[iblock2].b_name,listapath[contador2]) == 0)){
                        LugarInododo = LugarInodoBloque(DiscoEnUSo,BC.b_content[iblock2].b_inodo,inicio_particion,1);
                        siguiente = 1;
                        break;
                    }
                    if(siguiente == 1){
                        break;
                    }
                }
                if(siguiente == 1){
                    break;
                }
            }

        }
    }
    return 0;
}