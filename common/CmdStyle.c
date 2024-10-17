#ifndef CMDSTYLE_H
#define CMDSTYLE_H

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdbool.h>

#include "../server/include/Rutas.h"
#include "include/ArchivosHandler.h"

enum Color
{
    RED,
    GREEN,
    BLUE,
    YELLOW,
    DEFAULT
};

void printColoredText(enum Color color, char *texto, ...)
{
    const char *colorCodes[] = {
        "\033[0;31m", // RED
        "\033[0;32m", // GREEN
        "\033[0;34m", // BLUE
        "\033[0;33m", // YELLOW
        "\033[0m"     // DEFAULT
    };

    va_list args;          // Declara una lista de argumentos
    va_start(args, texto); // Inicializa la lista de argumentos

    printf("%s", colorCodes[color]); // Imprime el código de color
    vprintf(texto, args);            // Imprime el texto formateado
    printf("\033[0m");             // Restablece el color
    va_end(args);                    // Cierra la lista de argumentos
}

void loadingAnimation(int duration)
{
    const char *loadingChars = "|/-\\";
    int i;

    for (i = 0; i < duration; i++)
    {
        // Calcular el índice del carácter de carga
        printf("\r%c  ", loadingChars[i % 4]); // \r para volver al principio de la línea
        fflush(stdout);                        // Asegurarse de que se imprima inmediatamente
        usleep(100000);                        // Espera 100 ms
    }
    printf("\r   \r"); // Limpia la línea
}

#endif // CMDSTYLE_H