#ifndef STRUCT_H
#define STRUCT_H
/*--------------------------------------STRUCST PARA GUARDAR INFORMACION DE DISCOS--------------------------------*/
typedef struct {
	char part_status;
	char part_type;
	char part_fit;
	int part_start;
	int part_size;	
	char part_name[16];
} Partition;

typedef struct {
	int mbr_tamanio;
	time_t mbr_fecha_creacion;
	int mbr_disk_signature;
	Partition mbr_partition[4];

} MBR;



typedef struct{
	char part_status;
	char part_fit;
	int part_start;
	int part_size;
	int part_next;
	char part_name[16];
}ebr;

struct Nodo{
	char* letra;
	int numero;
	char  direccion[400];
	char  nombre[100];
	struct Nodo *siguiente;
	struct Nodo *anterior;
};

struct Nodo *primero=NULL;
struct Nodo *ultimo=NULL;



/*--------------------------------------------------------FIN------------------------------------------------------*/

//-------------------------------------------------STRUCTS PARA SISTEMA DE ARCHIVOS----------------------------------
typedef struct{
    int s_filesystem_type;
    int s_inodes_count;
    int s_blocks_count;
    int s_free_block_count;
    int s_free_inodes_count;
    time_t s_mtime;
    time_t s_umtime;
    int s_mnt_count;
    int s_magic;
    int s_inode_size;
    int s_block_size;
    int s_first_ino;
    int s_first_blo;
    int s_bm_inode_start;
    int s_bm_block_start;
    int s_inode_start;
    int s_block_start;
}superBloque;

typedef struct{
    int i_uid;
    int i_gid;
    int i_size;
    time_t i_atime;
    time_t i_ctime;
    time_t i_mtime;
    int i_block[15];
    char i_type;
    int i_perm;
}inodeTable;

typedef struct
{
    char b_name[12];
    int b_inodo;
}content;


typedef struct{
    content b_content[4];
}bloqueCarpetas;

typedef struct{
    char b_content[64];
}bloqueArchivo;

typedef struct{
    int b_pointers[16] ;
}bloquedeApuntadores;

typedef struct{
    int Journal_tipo_operacion;
    int Journal_tipo;
    char Journal_nombre[50];
    char Journal_contenido[100];
    time_t Journal_fecha;
    char Journal_propietario[16];
    int Journal_Permisos;
}JOURNAL;

typedef struct{
    int id_user;
    int id_grup;
    char usuario[12];
    char password [12];
    char grupo[12];
}USERLOG;


struct Lista{

    int id;
    char cadena[400];

    struct  Lista *siguiente; 
    struct Lista* anterior;

};

struct Lista* primeroLista=NULL;
struct Lista* ultimoLista=NULL;


void concatenarPalabras(char *p,char direccion[400]);
const char* tipoDePalabra(char palabra[400]);
void analisisSintactico(char direccion[100]);

void crearArchivo(char direccion[400]);


int buscarNombreLogicas(char direccion[400],char nombre[100]);
int buscarParticionP(char direccion[400],char nombre[100]);
int buscarParticionL(char direccion[400],char nombre[100]);

void unParticionP(char direccion[400],char nombre[100]);
void unParticionL(char direccion[400],char nombre[100]);


int buscarletra(char* letra,char direccion[400],char temp[400]);
int buscarnumero(int numero, char direccion[400],char temp[400]);

struct Nodo* buscarMount(char nombre[400]);


const char* getDireccion(char id[10]);

void graficarDisco(char direccion[400],char destino[400],char extension[30]);
void graficarMBR(char direccion[400],char destino[400],char extension[30]);
void insertarNodo(int numero,char* letra,char direccion[400],char nombre[400]);
int buscarGrupo(char nombre[12]);
int buscarUltimoGrupo();
void eliminarGrupo(char *nombre);
void eliminarUsuario(char nombre[12]);
int buscarNumeroGrupo(char nombre[12]);
int buscarUsuario(char nombre[12]);
void iniciarFormateo2(int inicio,int fin,int tam,char direccion[100]);
void iniciarFormateo3(int inicio,int fin,int tam,char direccion[100]);
void graficarInodos(char direccion[400],char destino[400],char extension[30],int biS,int iS,int biE);
void graficarBloques(char direccion[400],char destino[400],char extension[30],int biS,int iS,int biE);
int buscarUsuarioL(int inicioI,int InicioB,char i_user[12],char i_pass[12],char direccion[100]);
int agregarGrupo(FILE* disco,char nombre[64]);
int crearCarpeta(FILE* disco,char fit,USERLOG usuariolog,int indicadorp,char* path);
void crearReporte(int inicio,int final,char direccion[400]);
void crearTree(char direccion[400],char extension[20]);
void graficarSuper(char destino[400],char extesion[20]);
void reporteFile(char* pa_path,char destino[400]);
void reportePermisos(char *pa_path,char destino[400],char extension[20]);
/*

-----------------------------Void de Automatas-------------------------------

*/

void mostrarcambio();

void automata_rep();
void automata_MsDisk();
void automata_rmdisk();
void automata_fdisk();
void automata_mount();
void automata_unmount();
void automata_exec();
void automata_mkfs();
void automata_login();
void automata_mkgrp();
void automata_rmgrp();
void automata_mkusr();
void automata_rmusr();
void automata_mkdir();
void automata_mkfile();
void automata_loss();
void automata_recovery();
void automata_convert();
void automata_chmod();
void automata_cat();
void automata_mv();
void automata_rem();


char *filename(char *str);
char *extension(char *str);
char *getPath(char *str);

int buscarMontado(char nombre[400]);

int Loguear(char id[20],char direccion[100],char user[11],char password[11]);
int buscarPrimerBloque(FILE *disco,int start,int bitUso,char FIT);
int CREARCARPETA(char *path,int indicadorp);
int BUSCARCARPETAOARCHIVO(FILE *DiscoEnUSo,char* path,int inicio_particion,int *numeracion);
int LugarInodoBloque(FILE *DiscoEnUSo,int posicion,int inicio_particion,int tipo);
int NUEVACARPETA(FILE *DiscoEnUso,char fit,USERLOG UsuarioActual,int indicadorp,int inicio_particion,char* path);

int CREARARCHIVO(char* pa_path,int indicadorp,int size,char* cont,int c);
int NUEVOARCHIVO(FILE *DiscoEnUso,char fit,USERLOG UsuarioActual,int indicadorp,int inicio_particion,char* pampa,int size,char* cont);
int permisosDeEscritura(int permisos,int flu,int flg);
USERLOG buscarUsuarioPropi(int id);
int buscarnuevoID();

void guardarJournal(int accion,int tipo,int permisos,char nom[150],char conte[100]);
void metodoLoss(int inicio,int final);

void cambiarpermisos(FILE* disco,int pos,int permisos);

int buscarNumeroPalabra(char *p);
int insertarOrdenado(int id,char cadena[300]);

int permisosdeLectura(int permisos,int flu,int flg);

void reportarJournaling(char destino[400]);

int permisosLecturaRecursividad(FILE* disco,int numeracion);
int permisosEscrituraRecursividad(FILE* disco,int numeracion);
void eliminarRecursividad(FILE* disco,int numeracion);


int BUSCARCARPETAOARCHIVO2(FILE *DiscoEnUSo,char* path,int inicio_particion,int *numeracion,int *index);



#endif