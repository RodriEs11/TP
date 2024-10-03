#ifndef USUARIO_H
#define USUARIO_H

#include <stdbool.h>
#include <string.h>

typedef struct {
    char nombre[50];
    char password[50];
    char rol[50];
    bool activo;
} Usuario;

// Parsea un string con el formato "nombre|password|rol|activo" y lo convierte en un Usuario
Usuario parsearUsuario(const char *texto)
{
    Usuario usuario;
    char buffer[256];
    strncpy(buffer, texto, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    char *token = strtok(buffer, "|");
    if (token != NULL)
    {
        strncpy(usuario.nombre, token, sizeof(usuario.nombre));
        usuario.nombre[sizeof(usuario.nombre) - 1] = '\0';
    }

    token = strtok(NULL, "|");
    if (token != NULL)
    {
        strncpy(usuario.password, token, sizeof(usuario.password));
        usuario.password[sizeof(usuario.password) - 1] = '\0';
    }

    token = strtok(NULL, "|");
    if (token != NULL)
    {
        strncpy(usuario.rol, token, sizeof(usuario.rol));
        usuario.rol[sizeof(usuario.rol) - 1] = '\0';
    }

    token = strtok(NULL, "|");
    if (token != NULL)
    {
        usuario.activo = (strcmp(token, "true") == 0) ? 1 : 0;
    }

    return usuario;
}

#endif // USUARIO_H