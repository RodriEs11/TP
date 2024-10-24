#ifndef PETICION_INTERCAMBIO_H
#define PETICION_INTERCAMBIO_H

typedef struct {
    char usuarioCreador[50];
    char paisOf[50];
    char jugadorOf[50];
    char paisReq[50];
    char jugadorReq[50];
    char estado[20];
} PeticionIntercambio;

int agregarPeticionIntercambio(PeticionIntercambio peticion);
int modificarEstadoPeticionIntercambio(PeticionIntercambio peticion, char *estado);


#endif // PETICION_INTERCAMBIO_H 