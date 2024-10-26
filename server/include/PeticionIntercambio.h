#ifndef PETICION_INTERCAMBIO_H
#define PETICION_INTERCAMBIO_H

typedef struct
{
    char usuarioCreador[50];
    char paisOf[50];
    char jugadorOf[50];
    char paisReq[50];
    char jugadorReq[50];
    char estado[20];
} PeticionIntercambio;

int peticion_existe(const char *archivo, PeticionIntercambio peticion);
void convertir_a_cadena(PeticionIntercambio peticion, char *cadena);
int agregarPeticionIntercambio(PeticionIntercambio peticion);
int modificarEstadoPeticionIntercambio(PeticionIntercambio peticion, char *estado);
PeticionIntercambio *obtenerPeticionesIntercambio();
int obtenerPeticionesIntercambioCount();

#endif // PETICION_INTERCAMBIO_H