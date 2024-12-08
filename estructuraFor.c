for (;;) {
    do {
        printf(">> ");
        fflush(stdin);
        fgets(comando, LONGITUD_COMANDO, stdin);
    } while (ComprobarComando(comando, orden, argumento1, argumento2) != 0);

    if (strcmp(orden, "dir") == 0) {
        Directorio(directorio, &ext_blq_inodos);
        continue;
    }
    if (strcmp(orden, "info") == 0) {
        LeeSuperBloque(&ext_superblock);
        continue;
    }
    if (strcmp(orden, "bytemaps") == 0) {
        Printbytemaps(&ext_bytemaps);
        continue;
    }
    if (strcmp(orden, "rename") == 0) {
        if (Renombrar(directorio, &ext_blq_inodos, argumento1, argumento2) == 0) {
            Grabarinodosydirectorio(directorio, &ext_blq_inodos, fent);
        }
        continue;
    }
    if (strcmp(orden, "remove") == 0) {
        if (Borrar(directorio, &ext_blq_inodos, &ext_bytemaps, &ext_superblock, argumento1, fent) == 0) {
            Grabarinodosydirectorio(directorio, &ext_blq_inodos, fent);
            GrabarByteMaps(&ext_bytemaps, fent);
            GrabarSuperBloque(&ext_superblock, fent);
        }
        continue;
    }
    if (strcmp(orden, "copy") == 0) {
        if (Copiar(directorio, &ext_blq_inodos, &ext_bytemaps, &ext_superblock, memdatos, argumento1, argumento2, fent) == 0) {
            Grabarinodosydirectorio(directorio, &ext_blq_inodos, fent);
            GrabarByteMaps(&ext_bytemaps, fent);
            GrabarSuperBloque(&ext_superblock, fent);
        }
        continue;
    }
    if (strcmp(orden, "salir") == 0) {
        GrabarDatos(memdatos, fent); // Grabar todos los datos antes de salir
        fclose(fent);
        return 0;
    }

    printf("Comando desconocido: %s\n", orden);
}
