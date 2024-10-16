#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

char *obtenerFechaHoraActual()
{
    time_t current_time;
    struct tm *local_time;
    struct timespec ts;
    char *buffer = malloc(100 * sizeof(char)); // Asignar memoria para el string

    if (buffer == NULL)
    {
        perror("Error al asignar memoria");
        return NULL;
    }

    // Obtener el tiempo actual
    current_time = time(NULL);

    // Convertir a tiempo local
    local_time = localtime(&current_time);

    // Obtener milisegundos
    clock_gettime(CLOCK_REALTIME, &ts);
    long milliseconds = ts.tv_nsec / 1000000; // Convertir nanosegundos a milisegundos

    // Formatear la fecha y hora en el buffer incluyendo milisegundos
    snprintf(buffer, 100, "%02d/%02d/%04d %02d:%02d:%02d.%03ld",
             local_time->tm_mday,        // Día del mes
             local_time->tm_mon + 1,     // Mes (0-11, por eso se suma 1)
             local_time->tm_year + 1900, // Año (desde 1900)
             local_time->tm_hour,        // Hora (0-23)
             local_time->tm_min,         // Minutos (0-59)
             local_time->tm_sec,         // Segundos (0-59)
             milliseconds                // Milisegundos
    );

    return buffer;
}

char *mayusculas(const char *texto)
{
    char *mayusculas = malloc(100 * sizeof(char)); // Asignar memoria para el string
    if (mayusculas == NULL)
    {
        perror("Error al asignar memoria");
        return NULL;
    }
    int i = 0;

    // Convertir cada carácter a mayúscula
    while (texto[i] != '\0' && i < 99) // Asegúrate de que el tamaño sea suficiente para tu texto
    {
        mayusculas[i] = toupper(texto[i]);
        i++;
    }
    mayusculas[i] = '\0'; // Terminar la cadena

    // Imprimir el texto en mayúsculas
    return mayusculas;
}

#endif // UTIL_H
