// CLIENTE
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <stdbool.h>


#include "../../common/include/CmdStyle.h"
#include "../../common/include/Util.h"

char sendBuff[512], recvBuff[512];

WSADATA wsaData;
SOCKET conn_socket;
struct sockaddr_in server;
struct hostent *host;


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
        printColoredText(RED, "No se pudo resolver el host\n");
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

int opcionConexionAlSocket()
{
    
    
    if (host == NULL) {
        printColoredText(RED, "Host no configurado. Por favor, configure el host primero.");
        system("pause");
        return -1;
    }
    

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    conn_socket = socket(AF_INET, SOCK_STREAM, 0);

    bool usuarioValidado = false;

    // Conexion al servidor
    if (connect(conn_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printColoredText(RED, "Error al conectar al servidor\n");
        system("pause");
        closesocket(conn_socket);
        return -1;
    }

    loadingAnimation(10);
    printColoredText(GREEN, "Conexion establecida con %s\n", inet_ntoa(server.sin_addr));

    // Enviar mensaje
    printf("Introduce tu nombre de usuario: ");
    scanf("%s", sendBuff);
    send(conn_socket, sendBuff, sizeof(sendBuff), 0);

    printf("Introduce tu password: ");
    ocultarInput(sendBuff);
    send(conn_socket, sendBuff, sizeof(sendBuff), 0);


    recv(conn_socket, recvBuff, sizeof(recvBuff), 0);
    printColoredText(BLUE, "\nRespuesta del servidor: ");
    printf("%s\n", recvBuff);

    if (strstr(recvBuff, "Usuario validado") != NULL)
    {
        usuarioValidado = true;
    }

    while (usuarioValidado)
    {
        printf("\nIntroduce un mensaje: ");
        scanf("%s", sendBuff);
        send(conn_socket, sendBuff, sizeof(sendBuff), 0);

        recv(conn_socket, recvBuff, sizeof(recvBuff), 0);
        printColoredText( BLUE,"Respuesta del servidor: ");
        printf("%s", recvBuff);

        if (strstr(recvBuff, "Cliente desconectado") != NULL)
        {
            break;
        }
    }

    closesocket(conn_socket);
}

void opcionConfigurarSocket()
{
    char direccion[100];
    int puerto;

    printf("Introduce la direccion del servidor: ");
    scanf("%s", direccion);
    printf("Introduce el puerto: ");
    scanf("%d", &puerto);

    configurarSocket(direccion, puerto);
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

            opcionConfigurarSocket();
            break;
        case 2:
            configurarSocket("localhost", 6000);
            opcionConexionAlSocket();
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