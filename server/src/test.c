#include <stdio.h>


#include "../include/Usuario.h"
#include "../include/Figurita.h"
#include "../include/Rutas.h"

#include "../../common/include/ArchivosHandler.h"
#include "../../common/include/util.h"




int main(int argc, char *argv[])
{


    char** lineas = separarLineas(FIGURITAS_FILE);

    Figurita* figuritas = obtenerFiguritas();

  

    printf("Figurita  %s\n",figuritas[0].jugador);
    printf("Figurita  %s\n", figuritas[1].jugador);
    printf("Figurita  %d", obtenerFiguritasCount());

    system("pause");
    
    return 0;
 
};


