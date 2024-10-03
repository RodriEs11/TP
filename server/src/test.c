#include <stdio.h>


#include "../include/Usuario.h"
#include "../include/ArchivosHandler.h"



int main(int argc, char *argv[])
{

    char** lineas = separarLineas("../autenticacion.txt");

    for (int i = 0; lineas[i] != NULL; i++) {
        printf("Linea %d: %s\n", i, lineas[i]);
    }   

    escribirArchivo("prueba.log", "Hola mundo");
    escribirArchivo("prueba.log", "Hola mundo222");
    escribirArchivo("prueba.log", "Hola mundo253252352");
    escribirArchivo("prueba.log", "Hola mundo343434");

    //Usuario usuario = parsearUsuario(texto);

   // printf("Nombre: %s\n", usuario.nombre);
   // printf("Contrasena: %s\n", usuario.password);
   // printf("Rol: %s\n", usuario.rol);
   // printf("Activo: %d\n", usuario.activo);

    return 0;

};


