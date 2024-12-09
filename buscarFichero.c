int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombre) {
    for (int i = 0; i < MAX_FICHEROS; i++) {
        // Verificar si el nombre coincide y si el inodo no es NULL
        if (directorio[i].dir_inodo != NULL_INODO && strcmp(directorio[i].dir_nfich, nombre) == 0) {
            return i;  // Retornar índice del archivo encontrado
        } 
    }
    return -1;  // Retornar -1 si no se encontró
}
