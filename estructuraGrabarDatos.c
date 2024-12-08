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
