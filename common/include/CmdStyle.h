#ifndef CMDSTYLE_H
#define CMDSTYLE_H

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdbool.h>

#include "../../server/include/Rutas.h"

enum Color
{
    RED,
    GREEN,
    BLUE,
    YELLOW,
    DEFAULT
};

void printColoredText(enum Color color, char *texto, ...);
void loadingAnimation(int duration);

#endif // CMDSTYLE_H