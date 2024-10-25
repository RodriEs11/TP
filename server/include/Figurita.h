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

int obtenerUltimaFiguritaId();
int agregarFigurita(Figurita figurita);
void actualizarFigurita(Figurita figurita);
void eliminarFigurita(int id);
Figurita* obtenerFiguritas();
int obtenerFiguritasCount();
Figurita obtenerFiguritaPorId(int id);
Figurita* obtenerFiguritasPorUsuario(char* usuario);
char *figuritasToString(Figurita *figuritas, int numFiguritas);



#endif // FIGURITA_H
