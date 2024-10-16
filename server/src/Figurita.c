#include "../include/Figurita.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Parsea un string con el formato "id;usuario;pais;jugador;disponible" y lo convierte en una Figurita
Figurita parsearFigurita(const char* texto) {
    Figurita figurita;
    char buffer[256];
    strncpy(buffer, texto, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    char* token = strtok(buffer, ";");
    if (token != NULL) {
        figurita.id = atoi(token);
    }

    token = strtok(NULL, ";");
    if (token != NULL) {
        strncpy(figurita.usuario, token, sizeof(figurita.usuario));
        figurita.usuario[sizeof(figurita.usuario) - 1] = '\0';
    }

    token = strtok(NULL, ";");
    if (token != NULL) {
        strncpy(figurita.pais, token, sizeof(figurita.pais));
        figurita.pais[sizeof(figurita.pais) - 1] = '\0';
    }

    token = strtok(NULL, ";");
    if (token != NULL) {
        strncpy(figurita.jugador, token, sizeof(figurita.jugador));
        figurita.jugador[sizeof(figurita.jugador) - 1] = '\0';
    }

    token = strtok(NULL, ";");
    if (token != NULL) {
        figurita.disponible = atoi(token);
    }

    return figurita;
}