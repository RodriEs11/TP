#ifndef ARCHIVOSHANDLER_H
#define ARCHIVOSHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "../include/ArchivosHandler.h"
#include "../include/util.h"

// Retorna el contenido de un archivo como un string
// Devuelve NULL si hay un error
char *leerArchivo(const char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo");
        return NULL;
    }

    fseek(archivo, 0, SEEK_END);
    long tamanoArchivo = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);

    char *contenido = (char *)malloc(tamanoArchivo + 1);
    if (contenido == NULL)
    {
        perror("Error al asignar memoria");
        fclose(archivo);
        return NULL;
    }

    fread(contenido, 1, tamanoArchivo, archivo);
    contenido[tamanoArchivo] = '\0';

    fclose(archivo);
    return contenido;
};

// Separa el contenido de un archivo en distintos strings por líneas
// Devuelve un array de strings, cada uno representando una línea del archivo
// El último elemento del array es NULL
// Devuelve NULL si hay un error
char **separarLineas(const char *nombreArchivo)
{

    char *contenido = leerArchivo(nombreArchivo);

    if (contenido == NULL)
    {
        return NULL;
    }

    size_t numLineas = 0;
    for (const char *p = contenido; *p; p++)
    {
        if (*p == '\n')
        {
            numLineas++;
        }
    }

    char **lineas = (char **)malloc((numLineas + 2) * sizeof(char *));
    if (lineas == NULL)
    {
        perror("Error al asignar memoria");
        return NULL;
    }

    size_t indiceLinea = 0;
    const char *inicioLinea = contenido;
    for (const char *p = contenido;; p++)
    {
        if (*p == '\n' || *p == '\0')
        {
            size_t longitudLinea = p - inicioLinea;
            lineas[indiceLinea] = (char *)malloc(longitudLinea + 1);
            if (lineas[indiceLinea] == NULL)
            {
                perror("Error al asignar memoria");
                for (size_t i = 0; i < indiceLinea; i++)
                {
                    free(lineas[i]);
                }
                free(lineas);
                return NULL;
            }
            strncpy(lineas[indiceLinea], inicioLinea, longitudLinea);
            lineas[indiceLinea][longitudLinea] = '\0';
            indiceLinea++;
            inicioLinea = p + 1;
        }
        if (*p == '\0')
        {
            break;
        }
    }
    lineas[indiceLinea] = NULL;

    return lineas;
};

// Escribe una línea al final de un archivo de texto
//// Devuelve 0 si tiene éxito, -1 si hay un error
int escribirArchivo(const char *nombreArchivo, const char *linea)
{
    FILE *archivo = fopen(nombreArchivo, "a");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo");
        return -1;
    }
    if (fprintf(archivo, "[%s] %s\n", obtenerFechaHoraActual(), linea) < 0)
    {
        perror("Error al escribir en el archivo");
        fclose(archivo);
        return -1;
    }

    fclose(archivo);
    return 0;
};

void escribirLog(char *logFile, const char *formato, ...)
{
    // Crear un buffer para almacenar la línea formateada
    char linea[256];

    // Inicializar la lista de argumentos
    va_list args;
    va_start(args, formato);

    // Usar vsnprintf para formatear la línea
    vsnprintf(linea, sizeof(linea), formato, args);
    va_end(args);

    escribirArchivo(logFile, (const char *)linea);
    printf("[%s] %s\n", obtenerFechaHoraActual(), linea);
}

#endif // ARCHIVOSHANDLER_H