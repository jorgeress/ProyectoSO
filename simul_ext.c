#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include "cabeceras.h"

#define LONGITUD_COMANDO 100

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2);
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,char *nombre);
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);
//int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreantiguo, char *nombrenuevo);
//int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_DATOS *memdatos, char *nombre);
//int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, char *nombre,  FILE *fich);
//int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich);
void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);

int main()
{
         char *comando[LONGITUD_COMANDO];
         char *orden[LONGITUD_COMANDO];
         char *argumento1[LONGITUD_COMANDO];
         char *argumento2[LONGITUD_COMANDO];
         int i,j;
         unsigned long int m;
     EXT_SIMPLE_SUPERBLOCK ext_superblock;
     EXT_BYTE_MAPS ext_bytemaps;
     EXT_BLQ_INODOS ext_blq_inodos;
     EXT_ENTRADA_DIR directorio[MAX_FICHEROS];
     EXT_DATOS memdatos[MAX_BLOQUES_DATOS];
     EXT_DATOS datosfich[MAX_BLOQUES_PARTICION];
     int entradadir;
     int grabardatos;
     FILE *fent;
     // Lectura del fichero completo de una sola vez

     fent = fopen("particion.bin","r+b");
     fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent);
     memcpy(&ext_superblock,(EXT_SIMPLE_SUPERBLOCK *)&datosfich[0], SIZE_BLOQUE);
     memcpy(&directorio,(EXT_ENTRADA_DIR *)&datosfich[3], SIZE_BLOQUE);
     memcpy(&ext_bytemaps,(EXT_BLQ_INODOS *)&datosfich[1], SIZE_BLOQUE);
     memcpy(&ext_blq_inodos,(EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
     memcpy(&memdatos,(EXT_DATOS *)&datosfich[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);

     // Buce de tratamiento de comandos
     for (;;){
                 do {
                 printf (">> ");
                 fflush(stdin);
                 fgets(comando, LONGITUD_COMANDO, stdin);
                 } while (ComprobarComando(comando,orden,argumento1,argumento2) !=0);
             if (strcmp(orden,"dir")==0) {
            Directorio(&directorio,&ext_blq_inodos);
            continue;
            }
            if (strcmp(orden,"info")==0) {
            LeeSuperBloque(&ext_superblock);
            continue;
            }
            if (strcmp(orden,"bytemaps")==0) {
            Printbytemaps(&ext_bytemaps);
            continue;
            }
         // Escritura de metadatos en comandos rename, remove, copy     
         Grabarinodosydirectorio(&directorio,&ext_blq_inodos,fent);
         GrabarByteMaps(&ext_bytemaps,fent);
         GrabarSuperBloque(&ext_superblock,fent);
         if (grabardatos)
           GrabarDatos(&memdatos,fent);
         grabardatos = 0;
         //Si el comando es salir se habrán escrito todos los metadatos
         //faltan los datos y cerrar
         if (strcmp(orden,"salir")==0){
            GrabarDatos(&memdatos,fent);
            fclose(fent);
            return 0;
         }
     }
int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2) {
    int n = sscanf(strcomando, "%s %s %s", orden, argumento1, argumento2);
    if (n == 0) return -1;  // Comando vacío o no válido
    return 0;  // Comando válido
}

int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombre) {
    for (int i = 0; i < MAX_FICHEROS; i++) {
        // Verificar si el nombre coincide y si el inodo no es NULL
        if (directorio[i].dir_inodo != NULL_INODO && strcmp(directorio[i].dir_nfich, nombre) == 0) {
            return i;  // Retornar índice del archivo encontrado
        }
    }
    return -1;  // Retornar -1 si no se encontró
}

void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich) {
    fseek(fich, SIZE_BLOQUE * 2, SEEK_SET); // Posicionar en el bloque 2 (inodos)
    fwrite(inodos, sizeof(EXT_BLQ_INODOS), 1, fich); // Escribir inodos
    fseek(fich, SIZE_BLOQUE * 3, SEEK_SET); // Posicionar en el bloque 3 (directorio)
    fwrite(directorio, SIZE_BLOQUE, 1, fich); // Escribir directorio
}

void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich) {
    fseek(fich, SIZE_BLOQUE, SEEK_SET); // Posicionar en el bloque 1 (bytemaps)
    fwrite(ext_bytemaps, sizeof(EXT_BYTE_MAPS), 1, fich);
}

void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich) {
    fseek(fich, 0, SEEK_SET); // Posicionar en el bloque 0 (superbloque)
    fwrite(ext_superblock, sizeof(EXT_SIMPLE_SUPERBLOCK), 1, fich);
}

void GrabarDatos(EXT_DATOS *memdatos, FILE *fich) {
    fseek(fich, SIZE_BLOQUE * PRIM_BLOQUE_DATOS, SEEK_SET); // Posicionar en el bloque 4 (datos)
    fwrite(memdatos, sizeof(EXT_DATOS), MAX_BLOQUES_DATOS, fich); // Escribir todos los datos
}

}
