#include <stdio.h>


#include "../include/Usuario.h"
#include "../include/ArchivosHandler.h"
#include "../include/util.h"
#include "../include/Figurita.h"



int main(int argc, char *argv[])
{

    char** lineas = separarLineas("../figuritas.txt");

    Figurita* figuritas = malloc(sizeof(Figurita) * 100);
    for (int i = 0; lineas[i] != NULL; i++)
    {
        figuritas[i] = parsearFigurita(lineas[i]);
        printf("Figurita %d: %d %s %s %s %d\n", i, figuritas[i].id, figuritas[i].usuario, figuritas[i].pais, figuritas[i].jugador, figuritas[i].disponible);
    }


    return 0;
 
};


