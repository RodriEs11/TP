// CLIENTE
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <stdbool.h>
#include <windows.h>

#include "../include/PrintColor.h"

char sendBuff[512], recvBuff[512];

WSADATA wsaData;
SOCKET conn_socket;
struct sockaddr_in server;
struct hostent *host;

void setColor(int textColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, textColor);
}

int configurarSocket(char *direccion, int puerto)
{
    int respuesta;
    // Inicializar Winsock
    respuesta = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (respuesta != 0)
    {
        printf("Error al inicializar Winsock");
        getchar();
        return -1;
    }

    // Configurar ip
    host = (struct hostent *)gethostbyname(direccion);
    if (host == NULL)
    {
        printColoredText(RED,"No se pudo resolver el host\n");
        system("pause");
        WSACleanup();
        return WSAGetLastError();
    }

    // Crear socket
    conn_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (conn_socket == INVALID_SOCKET)
    {
        printColoredText(RED, "No se pudo crear el socket\n");
         system("pause");
        WSACleanup();
        return WSAGetLastError();
    }

    // Configurar servidor
    memset(&server, 0, sizeof(server));
    memcpy(&server.sin_addr.s_addr, host->h_addr, host->h_length);

    server.sin_family = AF_INET;
    server.sin_port = htons(puerto);

}

void mostrarMenu()
{

    printf("\n\n----- Menu Principal -----\n");
    printf("1. Configurar host\n");
    printf("2. Conectarse\n");
    printf("3. Opcion 3\n");

    // mostrar numero de host y puerto como referencia, si es nulo mostrar mensaje
    if (host != NULL)
    {

        printColoredText(GREEN, "\nHost: %s", inet_ntoa(server.sin_addr));
        printColoredText(GREEN, "Puerto: %d\n", ntohs(server.sin_port));
    }
    else
    {
        printColoredText(RED, "\nHost no configurado\n");
    }

    printf("0. Salir\n");
    printf("--------------------------\n");
}

int conexionAlSocket()
{
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    conn_socket = socket(AF_INET, SOCK_STREAM, 0);

    bool usuarioValidado = false;

    // Conexion al servidor
    if (connect(conn_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Error al conectar al servidor\n");
        closesocket(conn_socket);
        return -1;
    }

    printf("Conexion establecida con %s\n", inet_ntoa(server.sin_addr));

    // Enviar mensaje
    printf("Introduce tu nombre de usuario: ");
    scanf("%s", sendBuff);
    send(conn_socket, sendBuff, sizeof(sendBuff), 0);

    printf("Introduce tu password: ");
    scanf("%s", sendBuff);
    send(conn_socket, sendBuff, sizeof(sendBuff), 0);

    recv(conn_socket, recvBuff, sizeof(recvBuff), 0);
    printf("Respuesta del servidor: %s\n", recvBuff);

    if (strstr(recvBuff, "Usuario validado") != NULL)
    {
        usuarioValidado = true;
    }

    while (usuarioValidado)
    {
        printf("Introduce un mensaje: ");
        scanf("%s", sendBuff);
        send(conn_socket, sendBuff, sizeof(sendBuff), 0);

        recv(conn_socket, recvBuff, sizeof(recvBuff), 0);
        printf("Respuesta del servidor: %s\n", recvBuff);

        if (strstr(recvBuff, "Cliente desconectado") != NULL)
        {
            break;
        }
    }

    closesocket(conn_socket);
}

int main(int argc, char *argv[])
{
    printf("***** CLIENTE ******\n");

    bool usuarioValidado = false;

    int opcion;

    do
    {

        system("cls");
        mostrarMenu();
        printf("Selecciona una opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            char direccion[50];
            int puerto;
            printf("\nIntroduce la direccion del servidor: ");
            scanf("%s", &direccion);
            printf("Introduce el puerto: ");
            scanf("%d", &puerto);

            configurarSocket(direccion, puerto);
            break;
        case 2:
            conexionAlSocket();
            break;
        case 0:
            printf("Saliendo del programa.\n");
            break;
        default:
            printf("Opcion no valida. Intenta de nuevo.\n");
        }

    } while (opcion != 0);

    return 0;
}