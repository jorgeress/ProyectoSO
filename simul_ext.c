#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include "cabeceras.h"

#define LONGITUD_COMANDO 100

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2);
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombre);
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreantiguo, char *nombrenuevo);
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_DATOS *memdatos, char *nombre);
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, char *nombre, FILE *fich);
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino, FILE *fich);
void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);

int main() {
    char comando[LONGITUD_COMANDO];
    char orden[LONGITUD_COMANDO];
    char argumento1[LONGITUD_COMANDO];
    char argumento2[LONGITUD_COMANDO];
    int grabardatos = 0;
    FILE *fent;
    
    EXT_SIMPLE_SUPERBLOCK ext_superblock;
    EXT_BYTE_MAPS ext_bytemaps;
    EXT_BLQ_INODOS ext_blq_inodos;
    EXT_ENTRADA_DIR directorio[MAX_FICHEROS];
    EXT_DATOS memdatos[MAX_BLOQUES_DATOS];
    EXT_DATOS datosfich[MAX_BLOQUES_PARTICION];
    
    // Lectura del fichero completo de una sola vez
    fent = fopen("particion.bin", "r+b");
    fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent);
    memcpy(&ext_superblock, (EXT_SIMPLE_SUPERBLOCK *)&datosfich[0], SIZE_BLOQUE);
    memcpy(&directorio, (EXT_ENTRADA_DIR *)&datosfich[3], SIZE_BLOQUE);
    memcpy(&ext_bytemaps, (EXT_BLQ_INODOS *)&datosfich[1], SIZE_BLOQUE);
    memcpy(&ext_blq_inodos, (EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
    memcpy(&memdatos, (EXT_DATOS *)&datosfich[4], MAX_BLOQUES_DATOS * SIZE_BLOQUE);
    
    // Bucle de tratamiento de comandos
    for (;;) {
        do {
            printf(">> ");
            fflush(stdin);
            fgets(comando, LONGITUD_COMANDO, stdin);
        } while (ComprobarComando(comando, orden, argumento1, argumento2) != 0);
        
        if (strcmp(orden, "info") == 0) {
            printf("Comandos disponibles: info, bytemaps, dir, rename, imprimir, remove, copy, salir\n");
            continue;
        }
        
        if (strcmp(orden, "bytemaps") == 0) {
            Printbytemaps(&ext_bytemaps);
            continue;
        }
        
        if (strcmp(orden, "dir") == 0) {
            Directorio(&directorio, &ext_blq_inodos);
            continue;
        }
        
        if (strcmp(orden, "rename") == 0) {
            Renombrar(&directorio, &ext_blq_inodos, argumento1, argumento2);
            continue;
        }
        
        if (strcmp(orden, "imprimir") == 0) {
            Imprimir(&directorio, &ext_blq_inodos, &memdatos, argumento1);
            continue;
        }
        
        if (strcmp(orden, "remove") == 0) {
            Borrar(&directorio, &ext_blq_inodos, &ext_bytemaps, &ext_superblock, argumento1, fent);
            continue;
        }
        
        if (strcmp(orden, "copy") == 0) {
            Copiar(&directorio, &ext_blq_inodos, &ext_bytemaps, &ext_superblock, &memdatos, argumento1, argumento2, fent);
            continue;
        }
        
        if (strcmp(orden, "salir") == 0) {
            GrabarDatos(&memdatos, fent);
            fclose(fent);
            return 0;
        }
        
        // Si el comando no es reconocido
        printf("Error: Comando no reconocido\n");
        printf("Comandos disponibles: info, bytemaps, dir, rename, imprimir, remove, copy, salir\n");
    }
}

// Función que comprueba el comando
int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2) {
    int i = 0;
    char *pch;
    
    // Limpiar los argumentos
    memset(orden, 0, LONGITUD_COMANDO);
    memset(argumento1, 0, LONGITUD_COMANDO);
    memset(argumento2, 0, LONGITUD_COMANDO);
    
    // Comprobación de comando
    pch = strtok(strcomando, " \n");
    if (pch != NULL) {
        strcpy(orden, pch);
        pch = strtok(NULL, " \n");
        if (pch != NULL) {
            strcpy(argumento1, pch);
            pch = strtok(NULL, " \n");
            if (pch != NULL) {
                strcpy(argumento2, pch);
            }
        }
    }
    
    return 0;
}

// Implementación de funciones específicas como Printbytemaps, Directorio, etc.
// Estas funciones deben ser implementadas según lo que necesitas hacer con cada comando.

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps) {
    // Imprimir el bytemap
    printf("Bytemaps:\n");
    // Implementar la visualización de los bytemaps
}

void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos) {
    // Imprimir el directorio
    printf("Directorio:\n");
    // Implementar la visualización del directorio
}

int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreantiguo, char *nombrenuevo) {
    // Implementar el renombrado de archivo
    printf("Renombrando %s a %s\n", nombreantiguo, nombrenuevo);
    return 0;
}

int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_DATOS *memdatos, char *nombre) {
    // Implementar impresión de archivo
    printf("Imprimiendo %s\n", nombre);
    return 0;
}

int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, char *nombre, FILE *fich) {
    // Implementar borrado de archivo
    printf("Borrando archivo %s\n", nombre);
    return 0;
}

int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino, FILE *fich) {
    // Implementar copia de archivo
    printf("Copiando %s a %s\n", nombreorigen, nombredestino);
    return 0;
}
