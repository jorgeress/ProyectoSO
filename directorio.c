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
