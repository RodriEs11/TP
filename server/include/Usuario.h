#ifndef USUARIO_H
#define USUARIO_H

#include <stdbool.h>
#include <string.h>

#include "../../common/include/ArchivosHandler.h"

typedef struct
{
    char nombre[50];
    char password[50];
    char rol[50];
    bool activo;
} Usuario;

// Parsea un string con el formato "nombre|password|rol|activo" y lo convierte en un Usuario
Usuario parsearUsuario(const char *texto);

// Valida si un usuario existe en el archivo de autenticacion
// Retorna 1 si el usuario existe, 0 si no existe
int validarUsuario(char *nombre, char *password);

// obtener el rol del usuario
char *obtenerRol(char *nombre);

// obtener usuarios activos
Usuario *obtenerUsuariosActivos();

// obtener cantidad de usuarios activos
int obtenerUsuariosActivosCount();

// agregar un usuario
int agregarUsuario(Usuario usuario);

// Valida si un usuario existe en el archivo de autenticacion
// Retorna 1 si el usuario existe, 0 si no existe
int existeUsuario(char *nombre);

// Convertir un array de usuarios a un string
char *usuariosToString(Usuario *usuarios, int numUsuarios);

// Modificar el estado de una petición de intercambio
// Retorna 1 si tiene éxito, -1 si hay un error
// Cuando se elimina un usuario, busca una peticion a su nombre, y la cambia a CANCELADO
int modificarEstadoPeticionUsuario(const char *usuario);

// Eliminar un usuario
int eliminarUsuario(const char *nombreUsuario);

#endif // USUARIO_H