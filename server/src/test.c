#include <stdio.h>


#include "../include/Usuario.h"
#include "../include/ArchivosHandler.h"
#include "../include/util.h"



int main(int argc, char *argv[])
{

    char** lineas = separarLineas("../autenticacion.txt");

    for (int i = 0; lineas[i] != NULL; i++) {
        printf("Linea %d: %s\n", i, lineas[i]);
    }   

   int usuarioValidado = validarUsuario("rodrigo", "rodrigo123");

   printf("Usuario validado: %d\n", usuarioValidado);

   printf("fecha y hora: %s\n", obtenerFechaHoraActual());
 

    return 0;
 
};


