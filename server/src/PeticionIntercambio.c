#ifndef PETICION_INTERCAMBIO_H
#define PETICION_INTERCAMBIO_H

#include <stdio.h>
#include <string.h>
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

// Agrega una peticion de intercambio al archivo de peticiones
// Retorna 0 si la peticion se agrego correctamente, 1 si hubo un error
// CON EL FORMATO usuarioCreador|paisOf|jugadorOf|paisReq|jugadorReq|estado
int agregarPeticionIntercambio(PeticionIntercambio peticion)
{
    FILE *archivo = fopen(PETICIONES_INTERCAMBIO_FILE, "a");
    if (archivo == NULL)
    {
        return 1;
    }

    fprintf(archivo, "%s|%s|%s|%s|%s|%s\n", peticion.usuarioCreador, peticion.paisOf, peticion.jugadorOf, peticion.paisReq, peticion.jugadorReq, peticion.estado);
    fclose(archivo);

    return 0;
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

#endif // PETICION_INTERCAMBIO_H