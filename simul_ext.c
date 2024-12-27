#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include "cabeceras.h"
#include <stdlib.h>

#define LONGITUD_COMANDO 100

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2);
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,char *nombre);
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreantiguo, char *nombrenuevo);
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_DATOS *memdatos, char *nombre);
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, char *nombre,  FILE *fich);
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich);
void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);

int main()
{
      char *comando = (char *)malloc(LONGITUD_COMANDO * sizeof(char));
    char *orden = (char *)malloc(LONGITUD_COMANDO * sizeof(char));
    char *argumento1 = (char *)malloc(LONGITUD_COMANDO * sizeof(char));
    char *argumento2 = (char *)malloc(LONGITUD_COMANDO * sizeof(char));

    if (!comando || !orden || !argumento1 || !argumento2) {
        fprintf(stderr, "Error al asignar memoria\n");
        return 1;
    }
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
     memcpy(&directorio, (EXT_ENTRADA_DIR *)&datosfich[3], sizeof(directorio));
     memcpy(&ext_bytemaps,(EXT_BLQ_INODOS *)&datosfich[1], SIZE_BLOQUE);
     memcpy(&ext_blq_inodos,(EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
     memcpy(&memdatos,(EXT_DATOS *)&datosfich[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);

     // Buce de tratamiento de comandos
for (;;) {
    do {
        printf(">> ");
        fflush(stdin);
        fgets(comando, LONGITUD_COMANDO, stdin);
    } while (ComprobarComando(comando, orden, argumento1, argumento2) != 0);

    if (strcmp(orden, "dir") == 0) {
        Directorio(directorio, &ext_blq_inodos);
        printf("Funcion directorio\n");
        continue;
    }
    if (strcmp(orden, "info") == 0) {
        LeeSuperBloque(&ext_superblock);
        printf("Funcion info\n");
        continue;
    }
    if (strcmp(orden, "bytemaps") == 0) {
        Printbytemaps(&ext_bytemaps);
        printf("Funcion bytemaps");
        continue;
    }
    if (strcmp(orden, "rename") == 0) {
        if (Renombrar(directorio, &ext_blq_inodos, argumento1, argumento2) == 0) {
         Grabarinodosydirectorio(directorio, &ext_blq_inodos, fent);
        }
        printf("Funcion rename");
        continue;
    }
    if (strcmp(orden, "remove") == 0) {
        if (Borrar(directorio, &ext_blq_inodos, &ext_bytemaps, &ext_superblock, argumento1, fent) == 0) {
            Grabarinodosydirectorio(directorio, &ext_blq_inodos, fent);
            GrabarByteMaps(&ext_bytemaps, fent);
            GrabarSuperBloque(&ext_superblock, fent);
        }
        printf("Funcion remove");
        continue;
    }
    if (strcmp(orden, "copy") == 0) {
        if (Copiar(directorio, &ext_blq_inodos, &ext_bytemaps, &ext_superblock, memdatos, argumento1, argumento2, fent) == 0) {
            Grabarinodosydirectorio(directorio, &ext_blq_inodos, fent);
            GrabarByteMaps(&ext_bytemaps, fent);
            GrabarSuperBloque(&ext_superblock, fent);
        }
        printf("Funcion copy");
        continue;
    }

  if (strcmp(orden, "imprimir") == 0) {
    if (Imprimir(directorio, &ext_blq_inodos, memdatos, argumento1) == 0) {
        printf("Archivo impreso con éxito.\n");
    }
    continue;
}
    if (strcmp(orden, "salir") == 0) {
        //GrabarDatos(memdatos, fent); // Grabar todos los datos antes de salir
        fclose(fent);
        return 0;
    }

    printf("Comando desconocido: %s\n", orden);
}

     }

int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2){
    int i;
    sprintf(argumento1,"%s",""); // Inicializamos a vacío
    sprintf(argumento2,"%s","");
    sprintf(orden,"%s","");
    sscanf(strcomando,"%s %s %s",orden, argumento1, argumento2);
    for(i = 0; orden[i]; i++){
        orden[i] = tolower(orden[i]);
    }

    // printf("%s %s %s\n",orden,argumento1,argumento2);
    if ((strcmp(orden,"dir")!=0) && (strcmp(orden,"info")!=0) && (strcmp(orden,"rename")!=0) &&
        (strcmp(orden,"copy")!=0) && (strcmp(orden,"remove")!=0) &&
        (strcmp(orden,"imprimir")!=0) && (strcmp(orden,"salir")!=0) &&
        (strcmp(orden,"bytemaps")!=0)) {
        printf("ERROR: Comando ilegal [bytemaps,copy,dir,info,imprimir,rename,remove,salir]\n");
        return -1;
    }
    return 0;
}

void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup)
{
    printf("Bloque %d Bytes\n", psup -> s_block_size);
    printf("inodos partición = %d\n", psup -> s_inodes_count);
    printf("inodos libres = %d\n", psup -> s_free_inodes_count);
    printf("Bloques partición = %d\n", psup -> s_blocks_count);
    printf("Bloques libres = %d\n", psup -> s_free_blocks_count);
    printf("Primer bloque de datos = %d\n", psup -> s_first_data_block);
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
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombre) {
    for (int i = 0; i < MAX_FICHEROS; i++) {
        // Verificar si el nombre coincide y si el inodo no es NULL
        if (directorio[i].dir_inodo != NULL_INODO && strcmp(directorio[i].dir_nfich, nombre) == 0) {
            return i;  // Retornar índice del archivo encontrado
        } 
    }
    return -1;  // Retornar -1 si no se encontró
}
void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps) {
    printf("Mapa de bits de bloques:\n");
    for (int i = 0; i < MAX_BLOQUES_PARTICION; i++) {
        printf("%d", ext_bytemaps->bmap_bloques[i]);
        if ((i + 1) % 8 == 0) printf(" "); // Espaciado cada 8 bits
    }
    printf("\n");

    printf("Mapa de bits de inodos:\n");
    for (int i = 0; i < MAX_INODOS; i++) {
        printf("%d", ext_bytemaps->bmap_inodos[i]);
        if ((i + 1) % 8 == 0) printf(" "); // Espaciado cada 8 bits
    }
    printf("\n");
}
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos) {
    printf("Directorio de archivos:\n");
    printf("Nombre de archivo       Tamaño (bytes)\n");
    printf("----------------------  --------------\n");

    for (int i = 0; i < MAX_FICHEROS; i++) {
        if (directorio[i].dir_inodo != NULL_INODO) { // Verificar si la entrada es válida
            int inodo_index = directorio[i].dir_inodo;
            unsigned int size_fichero = inodos->blq_inodos[inodo_index].size_fichero;
            printf("%-22s %14u\n", directorio[i].dir_nfich, size_fichero);
        }
    }
}

int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreantiguo, char *nombrenuevo) {
    int indice_antiguo = BuscaFich(directorio, inodos, nombreantiguo);
    if (indice_antiguo == -1) {
        printf("\nERROR: Archivo con nombre '%s' no encontrado.\n", nombreantiguo);
        return -1;
    }
    if (BuscaFich(directorio, inodos, nombrenuevo) != -1) {
        printf("ERROR: Ya existe un archivo con el nombre '%s'.\n", nombrenuevo);
        return -1;
    }
    strncpy(directorio[indice_antiguo].dir_nfich, nombrenuevo, LEN_NFICH);
    printf("\nArchivo '%s' renombrado a '%s'.\n", nombreantiguo, nombrenuevo);
    return 0;
}

int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_DATOS *memdatos, char *nombre) {
    // Buscar el archivo en el directorio
    int indice = BuscaFich(directorio, inodos, nombre);
    if (indice == -1) {
        printf("ERROR: Archivo '%s' no encontrado.\n", nombre);
        return -1;
    }

    printf("Archivo encontrado: %s (índice: %d)\n", nombre, indice);

    // Obtener el índice del inodo
    int inodo_index = directorio[indice].dir_inodo;
    EXT_SIMPLE_INODE *inodo = &inodos->blq_inodos[inodo_index];

    printf("Inodo asociado: %d\n", inodo_index);
    printf("Bloques del inodo: ");
    for (int i = 0; i < MAX_NUMS_BLOQUE_INODO; i++) {
        if (inodo->i_nbloque[i] == NULL_BLOQUE) break;
        printf("%d ", inodo->i_nbloque[i]);
    }
    printf("\n");

    // Imprimir los datos asociados a los bloques del inodo
    for (int i = 0; i < MAX_NUMS_BLOQUE_INODO; i++) {
        if (inodo->i_nbloque[i] == NULL_BLOQUE) break;
        printf("Contenido del bloque %d:\n", i);
        printf("%.*s", SIZE_BLOQUE, (char *)&memdatos[inodo->i_nbloque[i]]);
    }
    printf("\n");

    return 0;
}

int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, char *nombre, FILE *fich) {
    int indice = BuscaFich(directorio, inodos, nombre);
    if (indice == -1) {
        printf("\nERROR: Archivo '%s' no encontrado.\n", nombre);
        return -1;
    }

    int inodo_index = directorio[indice].dir_inodo;
    EXT_SIMPLE_INODE *inodo = &inodos->blq_inodos[inodo_index];

    for (int i = 0; i < MAX_NUMS_BLOQUE_INODO; i++) {
        if (inodo->i_nbloque[i] == NULL_BLOQUE) break;
        ext_bytemaps->bmap_bloques[inodo->i_nbloque[i]] = 0;
        ext_superblock->s_free_blocks_count++;
    }

    ext_bytemaps->bmap_inodos[inodo_index] = 0;
    ext_superblock->s_free_inodes_count++;
    memset(inodo, 0xFF, sizeof(EXT_SIMPLE_INODE));
    strcpy(directorio[indice].dir_nfich, "");
    directorio[indice].dir_inodo = NULL_INODO;

    printf("\nArchivo '%s' eliminado.\n", nombre);
    return 0;
}

int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino, FILE *fich) {
    int indice_origen = BuscaFich(directorio, inodos, nombreorigen);
    if (indice_origen == -1) {
        printf("\nERROR: Archivo origen '%s' no encontrado.\n", nombreorigen);
        return -1;
    }
    if (BuscaFich(directorio, inodos, nombredestino) != -1) {
        printf("\nERROR: Archivo destino '%s' ya existe.\n", nombredestino);
        return -1;
    }

    int inodo_libre = -1;
    for (int i = 0; i < MAX_INODOS; i++) {
        if (ext_bytemaps->bmap_inodos[i] == 0) {
            inodo_libre = i;
            break;
        }
    }
    if (inodo_libre == -1) {
        printf("\nERROR: No hay inodos libres.\n");
        return -1;
    }

    EXT_SIMPLE_INODE *inodo_origen = &inodos->blq_inodos[directorio[indice_origen].dir_inodo];
    EXT_SIMPLE_INODE *inodo_destino = &inodos->blq_inodos[inodo_libre];

    ext_bytemaps->bmap_inodos[inodo_libre] = 1;
    ext_superblock->s_free_inodes_count--;
    inodo_destino->size_fichero = inodo_origen->size_fichero;

    for (int i = 0; i < MAX_NUMS_BLOQUE_INODO; i++) {
        if (inodo_origen->i_nbloque[i] == NULL_BLOQUE) break;

        int bloque_libre = -1;
        for (int j = 0; j < MAX_BLOQUES_PARTICION; j++) {
            if (ext_bytemaps->bmap_bloques[j] == 0) {
                bloque_libre = j;
                break;
            }
        }
        if (bloque_libre == -1) {
            printf("\nERROR: No hay bloques libres.\n");
            return -1;
        }

        ext_bytemaps->bmap_bloques[bloque_libre] = 1;
        ext_superblock->s_free_blocks_count--;
        inodo_destino->i_nbloque[i] = bloque_libre;
        memcpy(&memdatos[bloque_libre], &memdatos[inodo_origen->i_nbloque[i]], SIZE_BLOQUE);
    }

    for (int i = 0; i < MAX_FICHEROS; i++) {
        if (directorio[i].dir_inodo == NULL_INODO) {
            strcpy(directorio[i].dir_nfich, nombredestino);
            directorio[i].dir_inodo = inodo_libre;
            printf("\nArchivo '%s' copiado a '%s'.\n", nombreorigen, nombredestino);
            return 0;
        }
    }

    printf("\nERROR: No hay espacio en el directorio.\n");
    return -1;
}
