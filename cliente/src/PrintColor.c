#ifndef PRINTCOLOR_H
#define PRINTCOLOR_H

#include <stdio.h>
#include <stdarg.h>

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
    printf("\033[0m\n");             // Restablece el color
    va_end(args);                    // Cierra la lista de argumentos
}
#endif // PRINTCOLOR_H