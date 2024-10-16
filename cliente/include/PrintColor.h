#ifndef PRINTCOLOR_H
#define PRINTCOLOR_H

#include <stdio.h>

enum Color {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    DEFAULT
};

void printColoredText(enum Color color, char *texto, ...);


#endif // PRINTCOLOR_H