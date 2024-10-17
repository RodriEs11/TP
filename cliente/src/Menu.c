#ifndef MENU_H
#define MENU_H
#include <WinSock2.h>

#include "../../common/include/CmdStyle.h"
#include "../../common/include/Util.h"

void mostrarMenuPrincipal(struct sockaddr_in server, struct hostent *host)
{
    system("cls");
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
    printf("3. Buscar figurita\n");
    printf("4. Eliminar figurita\n");

    printf("\n0. Volver\n");
    printf("--------------------------\n");
}

void mostrarMenuInsertarFigurita(struct sockaddr_in server, char *nombre, char *pais, int disponible)
{
    system("cls");
    printColoredText(GREEN, "Conectado a %s\n", inet_ntoa(server.sin_addr));

    printColoredText(DEFAULT, "\n\nMenu Principal \\ Figuritas \\ Insertar Figurita\n");
    printf("\n\n\n----- Insertar Figurita -----\n");

    printf("1. Nombre: ");

    if (strlen(nombre) != 0)
    {
        printColoredText(GREEN, "%s\n", nombre);
    }
    else
    {
        printColoredText(RED, "Requerido\n");
    }

    printf("2. Pais: ");
    if (strlen(pais) != 0)
    {
        printColoredText(GREEN, "%s\n", pais);
    }
    else
    {
        printColoredText(RED, "Requerido\n");
    }

    printf("3. Disponible: ");
    if (disponible != -1)
    {
        printColoredText(GREEN, "%s\n", disponible == 1 ? "Si" : "No");
    }
    else
    {
        printColoredText(RED, "Requerido\n");
    }
    printf("\n9. Insertar\n");
    printf("\n0. Volver\n");
    printf("--------------------------\n");
}

#endif // MENU_H