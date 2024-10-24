#ifndef USUARIO_H
#define USUARIO_H

#include <stdbool.h>
#include <string.h>

#include "../include/Rutas.h"

#include "../../common/include/ArchivosHandler.h"

typedef struct
{
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
};

// Valida si un usuario existe en el archivo de autenticacion
// Retorna 1 si el usuario existe, 0 si no existe
int validarUsuario(char *nombre, char *password)
{
    char **lineas = separarLineas(AUTENTICACION_FILE);
    Usuario *usuarios = malloc(sizeof(Usuario) * 100);
    for (int i = 0; lineas[i] != NULL; i++)
    {
        usuarios[i] = parsearUsuario(lineas[i]);
        if (strcmp(usuarios[i].nombre, nombre) == 0 && strcmp(usuarios[i].password, password) == 0)
        {
            return 1;
        }
    }
    return 0;
};

// obtener el rol del usuario
char *obtenerRol(char *nombre)
{
    char **lineas = separarLineas(AUTENTICACION_FILE);
    Usuario *usuarios = malloc(sizeof(Usuario) * 100);
    for (int i = 0; lineas[i] != NULL; i++)
    {
        usuarios[i] = parsearUsuario(lineas[i]);
        if (strcmp(usuarios[i].nombre, nombre) == 0)
        {
            return usuarios[i].rol;
        }
    }
    return "No se encontro el usuario";
};

// obtener usuarios activos

Usuario *obtenerUsuariosActivos()
{
    char **lineas = separarLineas(AUTENTICACION_FILE);
    Usuario *usuarios = malloc(sizeof(Usuario) * 100);
    Usuario *usuariosActivos = malloc(sizeof(Usuario) * 100);
    int count = 0;

    for (int i = 0; lineas[i] != NULL; i++)
    {
        usuarios[i] = parsearUsuario(lineas[i]);
        if (usuarios[i].activo)
        {
            usuariosActivos[count++] = usuarios[i];
        }
    }

    usuariosActivos = realloc(usuariosActivos, sizeof(Usuario) * count);

    return usuariosActivos;
};

// obtener cantidad de usuarios activos
int obtenerUsuariosActivosCount()
{
    Usuario *usuarios = obtenerUsuariosActivos();
    int count = 0;
    while (usuarios[count].nombre[0] != '\0')
    {
        count++;
    }
    return count;
}


// agregar un usuario
// Retorna 0 si tiene éxito, -1 si hay un error

int agregarUsuario(Usuario usuario)
{
    char buffer[256];
    sprintf(buffer, "%s|%s|%s|%s", usuario.nombre, usuario.password, usuario.rol, usuario.activo ? "true" : "false");
    return escribirArchivoSinFecha(AUTENTICACION_FILE, buffer);
};


// Valida si un usuario existe en el archivo de autenticacion
// Retorna 1 si el usuario existe, 0 si no existe
int existeUsuario(char *nombre)
{
    char **lineas = separarLineas(AUTENTICACION_FILE);
    Usuario *usuarios = malloc(sizeof(Usuario) * 100);
    for (int i = 0; lineas[i] != NULL; i++)
    {
        usuarios[i] = parsearUsuario(lineas[i]);
        if (strcmp(usuarios[i].nombre, nombre) == 0)
        {
            return 1;
        }
    }
    return 0;
};
#endif // USUARIO_H