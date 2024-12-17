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
