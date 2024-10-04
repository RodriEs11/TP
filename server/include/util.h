#ifndef UTIL_H
#define UTIL_H


#include <stdio.h>
#include <time.h>
#include <stdlib.h>


char* obtenerFechaHoraActual() {
    time_t current_time;
    struct tm *local_time;
    char *buffer = malloc(100 * sizeof(char));  // Asignar memoria para el string
    
    if (buffer == NULL) {
        perror("Error al asignar memoria");
        return NULL;
    }

    // Obtener el tiempo actual
    current_time = time(NULL);

    // Convertir a tiempo local
    local_time = localtime(&current_time);

    // Formatear la fecha y hora en el buffer
    snprintf(buffer, 100, "%02d/%02d/%04d %02d:%02d:%02d",
        local_time->tm_mday,       // Día del mes
        local_time->tm_mon + 1,    // Mes (0-11, por eso se suma 1)
        local_time->tm_year + 1900,// Año (desde 1900)
        local_time->tm_hour,       // Hora (0-23)
        local_time->tm_min,        // Minutos (0-59)
        local_time->tm_sec         // Segundos (0-59)
    );

    return buffer;
}



#endif // UTIL_H
