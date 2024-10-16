#ifndef FIGURITA_H
#define FIGURITA_H

// Definición de la estructura
typedef struct
{
    int id;
    char usuario[50];
    char pais[50];
    char jugador[50];
    int disponible;
} Figurita;

// Parsea un string con el formato "id;usuario;pais;jugador;disponible" y lo convierte en una Figurita
Figurita parsearFigurita(const char *texto);


#endif // FIGURITA_H
