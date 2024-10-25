#ifndef MENU_H
#define MENU_H
#include <WinSock2.h>

#include "../../common/include/CmdStyle.h"
#include "../../common/include/Util.h"

void mostrarMenuPrincipal(struct sockaddr_in server, struct hostent *host)
{
    system("cls");
    printColoredText(RED, "Modo offline\n");

    printf("\n\n----- Menu Principal -----\n");
    printf("1. Configurar host\n");
    printf("2. Conectarse\n");

    // mostrar numero de host y puerto como referencia, si es nulo mostrar mensaje de error
    if (host != NULL)
    {

        printColoredText(GREEN, "\nHost: %s", inet_ntoa(server.sin_addr));
        printColoredText(GREEN, "\nPuerto: %d\n\n", ntohs(server.sin_port));
    }
    else
    {
        printColoredText(RED, "\nHost no configurado\n\n");
    }

    printf("0. Salir\n");
    printf("--------------------------\n");
}

void mostrarMenuConexion(struct sockaddr_in server)
{

    system("cls");
    printColoredText(GREEN, "Conectado a %s\n", inet_ntoa(server.sin_addr));

    printColoredText(DEFAULT, "\n\nMenu Principal \\\n");

    printf("\n\n----- Menu de Conexion -----\n");
    printf("1. Figuritas\n");
    printf("2. Insertar usuario\n");
    printf("3. Baja de usuario\n");
    printf("4. Ver registro de actividades\n");
    printf("5. Peticion de intercambio\n");

    printf("\n0. Cerrar sesion\n");
    printf("--------------------------\n");
}

void mostrarMenuFiguritas(struct sockaddr_in server)
{

    system("cls");
    printColoredText(GREEN, "Conectado a %s\n", inet_ntoa(server.sin_addr));

    printColoredText(DEFAULT, "\n\nMenu Principal \\ Figuritas\n");
    printf("\n\n----- Figuritas -----\n");
    printf("1. Insertar figurita\n");
    printf("2. Ver figuritas\n");

    printf("\n0. Volver\n");
    printf("--------------------------\n");
}

void mostrarMenuInsertarFigurita(struct sockaddr_in server, char *nombre, char *pais, int disponible)
{
    system("cls");
    printColoredText(GREEN, "Conectado a %s\n", inet_ntoa(server.sin_addr));

    printColoredText(DEFAULT, "\n\nMenu Principal \\ Figuritas \\ Insertar Figurita\n");
    printf("\n\n\n----- Insertar Figurita -----\n\n");

    printf("Nombre: ");

    if (strlen(nombre) != 0)
    {
        printColoredText(GREEN, "%s\n", nombre);
    }
    else
    {
        printColoredText(RED, "Requerido\n");
    }

    printf("Pais: ");
    if (strlen(pais) != 0)
    {
        printColoredText(GREEN, "%s\n", pais);
    }
    else
    {
        printColoredText(RED, "Requerido\n");
    }

    printf("Disponible: ");
    if (disponible != -1)
    {
        printColoredText(GREEN, "%s\n", disponible == 1 ? "Si" : "No");
    }
    else
    {
        printColoredText(RED, "Requerido\n");
    }

    printf("\n--------------------------\n");
}

void mostrarMenuBajaUsuario(struct sockaddr_in server)
{
    system("cls");
    printColoredText(GREEN, "Conectado a %s\n", inet_ntoa(server.sin_addr));

    printColoredText(DEFAULT, "\n\nMenu Principal \\ Baja de Usuario\n");
    printf("\n\n\n----- Baja de Usuario -----\n\n");
}

void mostrarMenuPeticionIntercambio(struct sockaddr_in server, char *nombreFigurita, char *paisFigurita, char *nombreFiguritaRecibir, char *paisFiguritaRecibir)
{
    system("cls");
    printColoredText(GREEN, "Conectado a %s\n", inet_ntoa(server.sin_addr));

    printColoredText(DEFAULT, "\n\nMenu Principal \\ Peticion de Intercambio\n");
    printf("\n\n\n----- Peticion de Intercambio -----\n\n");

    printf("Nombre de la figurita a intercambiar: ");
    if (strlen(nombreFigurita) != 0)
    {
        printColoredText(GREEN, "%s\n", nombreFigurita);
    }
    else
    {
        printColoredText(RED, "Requerido\n");
    }

    printf("Pais de la figurita a intercambiar: ");
    if (strlen(paisFigurita) != 0)
    {
        printColoredText(GREEN, "%s\n", paisFigurita);
    }
    else
    {
        printColoredText(RED, "Requerido\n");
    }

    printf("Nombre de la figurita a recibir: ");
    if (strlen(nombreFiguritaRecibir) != 0)
    {
        printColoredText(GREEN, "%s\n", nombreFiguritaRecibir);
    }
    else
    {
        printColoredText(RED, "Requerido\n");
    }

    printf("Pais de la figurita a recibir: ");
    if (strlen(paisFiguritaRecibir) != 0)
    {
        printColoredText(GREEN, "%s\n", paisFiguritaRecibir);
    }
    else
    {
        printColoredText(RED, "Requerido\n");
    }

    printf("\n--------------------------\n");
}

#endif // MENU_H