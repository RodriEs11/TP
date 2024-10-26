#ifndef PETICION_INTERCAMBIO_H
#define PETICION_INTERCAMBIO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/PeticionIntercambio.h"
#include "../include/Rutas.h"

typedef struct
{
    char usuarioCreador[50];
    char paisOf[50];
    char jugadorOf[50];
    char paisReq[50];
    char jugadorReq[50];
    char estado[20];
} PeticionIntercambio;

// Función auxiliar para convertir la estructura en una cadena de formato compatible
void convertir_a_cadena(PeticionIntercambio peticion, char *cadena)
{
    sprintf(cadena, "%s|%s|%s|%s|%s|%s",
            peticion.usuarioCreador,
            peticion.paisOf,
            peticion.jugadorOf,
            peticion.paisReq,
            peticion.jugadorReq,
            peticion.estado);
}

// Función para verificar si una línea con los datos de la petición ya existe en el archivo
int peticion_existe(const char *archivo, PeticionIntercambio peticion)
{
    FILE *file = fopen(archivo, "r");
    if (!file)
    {
        perror("No se pudo abrir el archivo");
        return 0;
    }

    char buffer[256];
    char peticion_cadena[256];
    convertir_a_cadena(peticion, peticion_cadena);

    int existe = 0;
    while (fgets(buffer, 256, file))
    {
        buffer[strcspn(buffer, "\n")] = 0; // Eliminar salto de línea en buffer
        if (strcmp(buffer, peticion_cadena) == 0)
        {
            existe = 1;
            break;
        }
    }

    fclose(file);
    return existe;
}

// Agrega una peticion de intercambio al archivo de peticiones
// Retorna 1 si la peticion se agrego correctamente, -1 si hubo un error
// Retorna 2 si la peticion ya existe
// CON EL FORMATO usuarioCreador|paisOf|jugadorOf|paisReq|jugadorReq|estado
int agregarPeticionIntercambio(PeticionIntercambio peticion)
{
    FILE *archivo = fopen(PETICIONES_INTERCAMBIO_FILE, "a");
    if (archivo == NULL)
    {
        return -1;
    }

    // agrega validacion si ya existe la peticion
    if (peticion_existe(PETICIONES_INTERCAMBIO_FILE, peticion))
    {
        return 2;
    }

    fprintf(archivo, "%s|%s|%s|%s|%s|%s\n", peticion.usuarioCreador, peticion.paisOf, peticion.jugadorOf, peticion.paisReq, peticion.jugadorReq, peticion.estado);
    fclose(archivo);

    return 1;
}

// Modifica el estado de una peticion de intercambio
// Retorna 0 si la peticion se modifico correctamente, 1 si hubo un error
int modificarEstadoPeticionIntercambio(const char *usuarioCreador, const char *paisOf, const char *jugadorOf, const char *nuevoEstado)
{
    FILE *archivo = fopen(PETICIONES_INTERCAMBIO_FILE, "r+");
    if (archivo == NULL)
    {
        return 1;
    }

    char linea[256];
    long pos;
    while (fgets(linea, sizeof(linea), archivo))
    {
        pos = ftell(archivo);
        char usuario[50], paisO[50], jugadorO[50], paisR[50], jugadorR[50], estado[50];
        sscanf(linea, "%49[^|]|%49[^|]|%49[^|]|%49[^|]|%49[^|]|%49[^\n]", usuario, paisO, jugadorO, paisR, jugadorR, estado);

        if (strcmp(usuario, usuarioCreador) == 0 && strcmp(paisO, paisOf) == 0 && strcmp(jugadorO, jugadorOf) == 0)
        {
            fseek(archivo, pos - strlen(linea), SEEK_SET);
            fprintf(archivo, "%s|%s|%s|%s|%s|%s\n", usuario, paisO, jugadorO, paisR, jugadorR, nuevoEstado);
            fclose(archivo);
            return 0;
        }
    }

    fclose(archivo);
    return 1;
}

PeticionIntercambio *obtenerPeticionesIntercambio()
{
    FILE *archivo = fopen(PETICIONES_INTERCAMBIO_FILE, "r");
    if (archivo == NULL)
    {
        return NULL;
    }

    PeticionIntercambio *peticiones = NULL;
    size_t count = 0;
    char linea[256];

    while (fgets(linea, sizeof(linea), archivo))
    {
        peticiones = realloc(peticiones, (count + 1) * sizeof(PeticionIntercambio));
        if (peticiones == NULL)
        {
            fclose(archivo);
            return NULL;
        }

        sscanf(linea, "%49[^|]|%49[^|]|%49[^|]|%49[^|]|%49[^|]|%19[^\n]",
               peticiones[count].usuarioCreador,
               peticiones[count].paisOf,
               peticiones[count].jugadorOf,
               peticiones[count].paisReq,
               peticiones[count].jugadorReq,
               peticiones[count].estado);

        count++;
    }

    fclose(archivo);
    return peticiones;
}

int obtenerPeticionesIntercambioCount()
{
    FILE *archivo = fopen(PETICIONES_INTERCAMBIO_FILE, "r");
    if (archivo == NULL)
    {
        return 0;
    }

    int count = 0;
    char linea[256];
    while (fgets(linea, sizeof(linea), archivo))
    {
        count++;
    }

    fclose(archivo);
    return count;
}
#endif // PETICION_INTERCAMBIO_H