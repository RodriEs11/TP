#ifndef USUARIO_H
#define USUARIO_H

#include <stdbool.h>
#include <string.h>

#include "../../common/include/ArchivosHandler.h"


typedef struct {
    char nombre[50];
    char password[50];
    char rol[50];
    bool activo;
} Usuario;

// Parsea un string con el formato "nombre|password|rol|activo" y lo convierte en un Usuario
Usuario parsearUsuario(const char *texto);


// Valida si un usuario existe en el archivo de autenticacion
// Retorna 1 si el usuario existe, 0 si no existe
int validarUsuario(char* nombre, char* password);

// obtener el rol del usuario
char* obtenerRol(char* nombre);

#endif // USUARIO_H