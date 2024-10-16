#include <stdio.h>


#include "../include/Usuario.h"
#include "../include/ArchivosHandler.h"
#include "../include/util.h"
#include "../include/Figurita.h"

#include "../include/Rutas.h"


int main(int argc, char *argv[])
{


    char** lineas = separarLineas(FIGURITAS_FILE);

    Figurita* figuritas = obtenerFiguritas();

  

    printf("Figurita  %s\n",figuritas[0].jugador);
    printf("Figurita  %s\n", figuritas[1].jugador);
    printf("Figurita  %d", obtenerFiguritasCount());

    return 0;
 
};


