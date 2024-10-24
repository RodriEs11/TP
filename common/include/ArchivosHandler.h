#ifndef ARCHIVOSHANDLER_H
#define ARCHIVOSHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

// Retorna el contenido de un archivo como un string
// Devuelve NULL si hay un error
char *leerArchivo(const char *nombreArchivo);

// Separa el contenido de un archivo en distintos strings por líneas
// Devuelve un array de strings, cada uno representando una línea del archivo
// El último elemento del array es NULL
// Devuelve NULL si hay un error
char **separarLineas(const char *nombreArchivo);

// Escribe una línea al final de un archivo de texto
//// Devuelve 0 si tiene éxito, -1 si hay un error
int escribirArchivo(const char *nombreArchivo, const char *linea);
int escribirArchivoSinFecha(const char *nombreArchivo, const char *linea);

// Escribe un mensaje en el archivo de log especificado en LOG_FILE
void escribirLog(char *logFile, const char *formato, ...);

#endif // ARCHIVOSHANDLER_H