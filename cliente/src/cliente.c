// CLIENTE
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <stdbool.h>

#include "../include/Menu.h"

#include "../../common/include/CmdStyle.h"
#include "../../common/include/Util.h"
#include "../../common/include/Requests.h"

char sendBuff[512], recvBuff[512];

WSADATA wsaData;
SOCKET conn_socket;
struct sockaddr_in server;
struct hostent *host;

void enviarMensajeAServidor(SOCKET conn_socket, const char *mensaje)
{
    send(conn_socket, mensaje, strlen(mensaje), 0);
}

int recibirMensaje(SOCKET conn_socket)
{
    int bytes = recv(conn_socket, recvBuff, sizeof(recvBuff), 0);
    recvBuff[bytes] = '\0'; // Agrega un caracter no nulo al final del buffer

    return bytes;
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

int insertarFigurita(SOCKET conn_socket)
{
    char nombre[50], pais[50], disponible[50];

    mostrarMenuInsertarFigurita(server, "", "", -1);

    // Enviar solicitud de inserción de figurita
    enviarMensajeAServidor(conn_socket, INSERTAR_FIGURITA);

    // Introduce el nombre de la figurita
    recibirMensaje(conn_socket);
    printColoredText(BLUE, "[Server] ");
    printColoredText(DEFAULT, "%s", recvBuff);
    scanf("%s", sendBuff);
    strcpy(nombre, sendBuff);
    enviarMensajeAServidor(conn_socket, sendBuff);
    mostrarMenuInsertarFigurita(server, nombre, "", -1);

    // Introduce el pais de la figurita
    recibirMensaje(conn_socket);
    printColoredText(BLUE, "[Server] ");
    printColoredText(DEFAULT, "%s", recvBuff);
    scanf("%s", sendBuff);
    strcpy(pais, sendBuff);
    enviarMensajeAServidor(conn_socket, sendBuff);
    mostrarMenuInsertarFigurita(server, nombre, pais, -1);

    // Introduce si la figurita esta disponible
    recibirMensaje(conn_socket);
    printColoredText(BLUE, "[Server] ");
    printColoredText(DEFAULT, "%s", recvBuff);
    scanf("%s", sendBuff);
    strcpy(disponible, sendBuff);
    enviarMensajeAServidor(conn_socket, sendBuff);

    recibirMensaje(conn_socket);
    mostrarMenuInsertarFigurita(server, nombre, pais, atoi(disponible));

    printColoredText(BLUE, "[Server] ");
    printColoredText(DEFAULT, "%s\n", recvBuff);
    system("pause");
}

int verFiguritas(SOCKET conn_socket)
{

    enviarMensajeAServidor(conn_socket, VER_FIGURITAS);

    recibirMensaje(conn_socket);
    printColoredText(BLUE, "[Server] ");
    printColoredText(DEFAULT, "%s\n", recvBuff);
    system("pause");
}

void opcionFiguritas()
{
    int opcion;
    do
    {
        // Menu de figuritas
        // 1. Insertar figurita
        // 2. Ver figuritas
        // 3. Buscar figurita
        // 4. Eliminar figurita
        // 0. Volver

        mostrarMenuFiguritas(server);

        printf("Selecciona una opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            insertarFigurita(conn_socket);
            break;

        case 2:
            verFiguritas(conn_socket);
            break;

        case 3:
            break;

        case 4:
            break;

        case 0:
            // Volver
            break;

        default:
            printf("Opcion no valida. Intenta de nuevo.\n");
        }

    } while (opcion != 0);
}

void opcionInsertarUsuario()
{

    enviarMensajeAServidor(conn_socket, INSERTAR_USUARIO);
    recibirMensaje(conn_socket); // Recibir mensaje de solicitud de nombre de usuario
    printColoredText(BLUE, "[Server] ");
    printColoredText(DEFAULT, "%s", recvBuff);
    scanf("%s", sendBuff);
    enviarMensajeAServidor(conn_socket, sendBuff);

    recibirMensaje(conn_socket); // Recibir mensaje de solicitud de password
    printColoredText(BLUE, "[Server] ");
    printColoredText(DEFAULT, "%s", recvBuff);
    ocultarInput(sendBuff);
    enviarMensajeAServidor(conn_socket, sendBuff);

    recibirMensaje(conn_socket);
    printColoredText(BLUE, "\n[Server] ");
    printColoredText(DEFAULT, "%s\n", recvBuff);

    system("pause");
}
void opcionBajaUsuario()
{
    // TODO: Implementar
}
void opcionPeticionIntercambio()
{

    char nombreOf[50];
    char paisOf[50];
    char nombreReq[50];
    char paisReq[50];
  
    mostrarMenuPeticionIntercambio(server, "", "", "", "");
    enviarMensajeAServidor(conn_socket, INSERTAR_PETICION_INTERCAMBIO);

    // Introduce el nombre de la figurita a intercambiar
    recibirMensaje(conn_socket);
    printColoredText(BLUE, "[Server] ");
    printColoredText(DEFAULT, "%s", recvBuff);
    scanf("%s", sendBuff);
    strcpy(nombreOf, sendBuff);
    enviarMensajeAServidor(conn_socket, sendBuff);
    mostrarMenuPeticionIntercambio(server, nombreOf, "", "", "");

    // Introduce el nombre del pais de la figurita a intercambiar
    recibirMensaje(conn_socket);
    printColoredText(BLUE, "[Server] ");
    printColoredText(DEFAULT, "%s", recvBuff);
    scanf("%s", sendBuff);
    strcpy(paisOf, sendBuff);
    enviarMensajeAServidor(conn_socket, sendBuff);
    mostrarMenuPeticionIntercambio(server, nombreOf, paisOf, "", "");

    // Introduce el nombre de la figurita a recibir
    recibirMensaje(conn_socket);
    printColoredText(BLUE, "[Server] ");
    printColoredText(DEFAULT, "%s", recvBuff);
    scanf("%s", sendBuff);
    strcpy(nombreReq, sendBuff);
    enviarMensajeAServidor(conn_socket, sendBuff);
    mostrarMenuPeticionIntercambio(server, nombreOf, paisOf, nombreReq, "");

    // Introduce el nombre del pais de la figurita a recibir
    recibirMensaje(conn_socket);
    printColoredText(BLUE, "[Server] ");
    printColoredText(DEFAULT, "%s", recvBuff);
    scanf("%s", sendBuff);
    strcpy(paisReq, sendBuff);
    enviarMensajeAServidor(conn_socket, sendBuff);
    mostrarMenuPeticionIntercambio(server, nombreOf, paisOf, nombreReq, paisReq);

    recibirMensaje(conn_socket);
    printColoredText(BLUE, "[Server] ");
    printColoredText(DEFAULT, "%s\n", recvBuff);

    system("pause");
}
int opcionConexionAlSocket()
{

    if (host == NULL)
    {
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

    printColoredText(GREEN, "Conexion establecida con %s\n", inet_ntoa(server.sin_addr));

    // Enviar mensaje
    printf("Introduce tu nombre de usuario: ");
    scanf("%s", sendBuff);
    send(conn_socket, sendBuff, sizeof(sendBuff), 0);

    printf("Introduce tu password: ");
    ocultarInput(sendBuff);
    send(conn_socket, sendBuff, sizeof(sendBuff), 0);

    recv(conn_socket, recvBuff, sizeof(recvBuff), 0);

    if (strstr(recvBuff, "Usuario validado") != NULL)
    {
        usuarioValidado = true;
    }
    else
    {
        printColoredText(RED, "\nUsuario o password invalido\n");
        system("pause");
        return -1;
    }

    int opcion;
    do
    {
        // Menu de conexion
        // Opciones de conexion
        // 1. Figuritas
        // 2. Insertar usuario
        // 3. Baja de usuario
        // 4. Ver registro de actividades
        // 5. Peticion de intercambio
        // 0. Cerrar sesion
        mostrarMenuConexion(server);

        printf("Selecciona una opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            opcionFiguritas();
            break;
        case 2:
            opcionInsertarUsuario();
            break;
        case 3:
            opcionBajaUsuario();
            break;
        case 4:

            break;
        case 5:
            opcionPeticionIntercambio();
            break;
        case 0:
            break;
        default:
            printf("Opcion no valida. Intenta de nuevo.\n");
        }

    } while (opcion != 0);

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
        // Menu principal
        // 1. Configurar host
        // 2. Conectarse
        // 0. Salir
        // Si no se ha configurado el host, mostrar mensaje de error
        // Mostrar host y puerto como referencia
        // Si se ha configurado el host, mostrar mensaje de exito

        mostrarMenuPrincipal(server, host);
        printf("Selecciona una opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
            // Configurar host
        case 1:
            opcionConfigurarSocket();
            break;
            // Conectarse
        case 2:
            configurarSocket("localhost", 6000);
            opcionConexionAlSocket();
            break;

        case 0:
            // Salir
            break;
        default:
            printf("Opcion no valida. Intenta de nuevo.\n");
        }

    } while (opcion != 0);

    return 0;
}