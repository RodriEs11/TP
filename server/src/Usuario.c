#ifndef USUARIO_H
#define USUARIO_H

#include <stdbool.h>
#include <string.h>

#include "../include/Rutas.h"

#include "../../common/include/ArchivosHandler.h"

typedef struct Usuario
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
    int count = 0;
    char **lineas = separarLineas(AUTENTICACION_FILE);
    Usuario *usuarios = malloc(sizeof(Usuario) * 100);

    for (int i = 0; lineas[i] != NULL; i++)
    {
        usuarios[i] = parsearUsuario(lineas[i]);
        if (usuarios[i].activo)
        {
            count++;
        }
    }

    free(usuarios);
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

char *usuariosToString(Usuario *usuarios, int numUsuarios)
{
    if (usuarios == NULL || numUsuarios == 0)
    {
        char *noUsuarios = malloc(25);
        strcpy(noUsuarios, "No hay usuarios activos");
        return noUsuarios;
    }

    size_t bufferSize = 1024;
    char *buffer = malloc(bufferSize);
    buffer[0] = '\0';

    for (int i = 0; i < numUsuarios; i++)
    {
        char temp[512];
        snprintf(temp, sizeof(temp), "Nombre: %s, Rol: %s, Activo: %s\n", usuarios[i].nombre, usuarios[i].rol, usuarios[i].activo ? "true" : "false");

        // Verifica si el buffer tiene suficiente espacio
        if (strlen(buffer) + strlen(temp) + 1 > bufferSize)
        {
            bufferSize *= 2;
            buffer = realloc(buffer, bufferSize);
            if (buffer == NULL)
            {
                free(buffer);
                return NULL;
            }
        }

        strcat(buffer, temp);
    }

    return buffer;
};
int modificarEstadoPeticionUsuario(const char *nombre_usuario)
{
    FILE *archivo_original = fopen(PETICIONES_INTERCAMBIO_FILE, "r");
    FILE *archivo_temporal = fopen("temp.txt", "w");

    if (archivo_original == NULL || archivo_temporal == NULL)
    {
        perror("Error al abrir el archivo");
        return -1;
    }

    char linea[256];
    int usuario_encontrado = 0; // Variable para controlar si encontramos el usuario

    while (fgets(linea, sizeof(linea), archivo_original) != NULL)
    {
        // Copia la línea original para modificarla si es necesario
        char linea_modificada[256];
        strcpy(linea_modificada, linea);

        // Verificar si la línea comienza con el nombre de usuario
        char *usuario = strtok(linea, "|");
        if (usuario != NULL && strcmp(usuario, nombre_usuario) == 0)
        {

            usuario_encontrado = 1; // Se encontró el usuario

            // Busca el estado en la línea
            char *estado = strrchr(linea_modificada, '|');
            if (estado != NULL)
            {
                // Cambiar PENDIENTE a CANCELADO
                estado++; // Mover el puntero al inicio del estado
                if (strcmp(estado, "PENDIENTE\n") == 0)
                {
                    strcpy(estado, "CANCELADA\n"); // Cambiar a CANCELADO
                }
            }
        }

        // Escribir la línea (modificada o no) en el archivo temporal
        fprintf(archivo_temporal, "%s", linea_modificada);
    }
    fclose(archivo_original);
    fclose(archivo_temporal);

    // Reemplazar el archivo original con el temporal
    remove(PETICIONES_INTERCAMBIO_FILE);
    rename("temp.txt", PETICIONES_INTERCAMBIO_FILE);

    
    return usuario_encontrado ? 1 : -1;
}
// Función para eliminar un usuario por nombre
int eliminarUsuario(const char *nombreUsuario)
{
    FILE *archivo_original = fopen(AUTENTICACION_FILE, "r");
    FILE *archivo_temp = fopen("temp.txt", "w");

    if (archivo_original == NULL || archivo_temp == NULL)
    {
        perror("Error al abrir los archivos");
        return -1;
    }

    char linea[256];
    int encontrado = 0;

    while (fgets(linea, sizeof(linea), archivo_original))
    {
        // Comprobar si la línea empieza con el nombre de usuario
        if (strncmp(linea, nombreUsuario, strlen(nombreUsuario)) != 0 || linea[strlen(nombreUsuario)] != '|')
        {
            fputs(linea, archivo_temp); // Escribir la línea en el archivo temporal
        }
        else
        {
            encontrado = 1; // Se encontró el usuario a eliminar
        }
    }

    fclose(archivo_original);
    fclose(archivo_temp);

    // Reemplazar el archivo original por el temporal
    if (encontrado)
    {
        remove(AUTENTICACION_FILE);             // Eliminar el archivo original
        rename("temp.txt", AUTENTICACION_FILE); // Renombrar el archivo temporal
    }
    else
    {
        remove("temp.txt"); // Si no se encontró, eliminar el archivo temporal
        return -1;
    }

    return 1;
}

#endif // USUARIO_H