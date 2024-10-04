// SERVER
#include <stdio.h>
#include <WinSock2.h>

#include "../include/ArchivosHandler.h"

char sendBuff[512], recvBuff[512];

#define PORT 6000
#define LOG_FILE "server.log"

// enviar mensaje
void enviarMensaje(SOCKET conn_socket, const char *mensaje)
{
    send(conn_socket, mensaje, strlen(mensaje), 0);
}

// recibir mensaje

void recibirMensaje(SOCKET conn_socket)
{
    int bytes = recv(conn_socket, recvBuff, sizeof(recvBuff), 0);
    recvBuff[bytes] = '\0';
    printf("Mensaje recibido: %s\n", recvBuff);
}

void escribirLog(const char *formato, ...)
{
    // Crear un buffer para almacenar la línea formateada
    char linea[256]; // Ajusta el tamaño según tus necesidades

    // Inicializar la lista de argumentos
    va_list args;
    va_start(args, formato);

    // Usar vsnprintf para formatear la línea
    vsnprintf(linea, sizeof(linea), formato, args);
    va_end(args);

    escribirArchivo(LOG_FILE, (const char *)linea);
}

int main(int argc, char *argv[])
{
    WSADATA wsaData;
    WORD wVersionRequerida = MAKEWORD(2, 2);

    SOCKET conn_socket, comm_socket;

    struct sockaddr_in server;
    struct sockaddr_in client;
    struct hostent *host;
    int respuesta, stsize;

    respuesta = WSAStartup(wVersionRequerida, &wsaData);

    if (respuesta)
    {
        printf("No se pudo iniciar Winsock\n");
        escribirLog("No se pudo iniciar Winsock");
        getchar();
        return respuesta;
    }

    // Ip del servidor
    host = (struct hostent *)gethostbyname("localhost");

    if (host == NULL)
    {
        printf("No se pudo resolver el host\n");
        escribirLog("No se pudo resolver el host");
        getchar();
        WSACleanup();
        return WSAGetLastError();
    }

    // Crear socket

    // AF_INET = TCP/IP ipv4
    conn_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (conn_socket == INVALID_SOCKET)
    {
        printf("No se pudo crear el socket\n");
        escribirLog("No se pudo crear el socket");
        getchar();
        WSACleanup();
        return WSAGetLastError();
    }

    memset(&server, 0, sizeof(server));
    memcpy(&server.sin_addr.s_addr, host->h_addr, host->h_length);

    server.sin_family = AF_INET;   // TCP / UDP
    server.sin_port = htons(PORT); // Puerto

    // Se asocia el socket con la direccion del servidor
    respuesta = bind(conn_socket, (struct sockaddr *)&server, sizeof(server));
    escribirLog("Asociando el socket con la direccion del servidor: %s:%d", inet_ntoa(server.sin_addr), PORT);

    if (respuesta == SOCKET_ERROR)
    {
        printf("Error al asociar el socket con la direccion del servidor\n");
        escribirLog("Error al asociar el socket con la direccion del servidor");
        closesocket(conn_socket);
        WSACleanup();
        return WSAGetLastError();
    }

    if (listen(conn_socket, 1) == SOCKET_ERROR)
    {
        printf("Error al habilitar conexiones entrantes\n");
        escribirLog("Error al habilitar conexiones entrantes");
        closesocket(conn_socket);
        WSACleanup();
        getchar();
        return WSAGetLastError();
    }

    // Aceptar conexiones
    printf("***** SERVER ******\n");
    escribirLog("***** INICIANDO SERVER ******");

    while (1)
    {
        char buffer[100];

        printf("Esperando conexiones en el puerto %d\n", PORT);
        escribirLog("Esperando conexiones en el puerto %d", PORT);

        stsize = sizeof(struct sockaddr);
        comm_socket = accept(conn_socket, (struct sockaddr *)&client, &stsize);

        escribirLog("Conexion entrante desde: %s", inet_ntoa(client.sin_addr));

        if (comm_socket == INVALID_SOCKET)
        {
            printf("Error al aceptar la conexion\n");
            escribirLog("Error al aceptar la conexion");
            closesocket(conn_socket);
            WSACleanup();
            getchar();
            return WSAGetLastError();
        }

        printf("Conexion entrante desde: %s\n", inet_ntoa(client.sin_addr));
        escribirLog("Conexion entrante desde: %s", inet_ntoa(client.sin_addr));

        recibirMensaje(comm_socket);
        escribirLog("Mensaje recibido: %s", recvBuff);

        // Enviar mensaje
        enviarMensaje(comm_socket, "Hola querido cliente, soy el servidor");
        escribirLog("Mensaje enviado: %s", "Hola querido cliente, soy el servidor");

        // Cerrar socket de comunicacion
        closesocket(comm_socket);
        escribirLog("Cerrando socket de comunicacion");
    }

    return 0;
}
