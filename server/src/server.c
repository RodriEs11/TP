// SERVER
#include <stdio.h>
#include <WinSock2.h>

#include "../include/Rutas.h"
#include "../include/Usuario.h"

#include "../../common/include/ArchivosHandler.h"
#include "../../common/include/CmdStyle.h"

char sendBuff[512], recvBuff[512];

WSADATA wsaData;
WORD wVersionRequerida = MAKEWORD(2, 2);

SOCKET conn_socket, comm_socket;

struct sockaddr_in server;
struct sockaddr_in client;
struct hostent *host;
int respuesta, stsize;

#define PORT 6000

void enviarMensajeACliente(SOCKET conn_socket, const char *mensaje)
{
    send(conn_socket, mensaje, strlen(mensaje), 0);
}

int recibirMensaje(SOCKET conn_socket)
{
    int bytes = recv(conn_socket, recvBuff, sizeof(recvBuff), 0);
    recvBuff[bytes] = '\0'; // Agrega un caracter no nulo al final del buffer

    return bytes;
}

int configurarSocket()
{

    respuesta = WSAStartup(wVersionRequerida, &wsaData);

    if (respuesta)
    {
        printf("No se pudo iniciar Winsock\n");
        escribirLog(LOG_FILE, "No se pudo iniciar Winsock");
        return respuesta;
    }

    // Ip del servidor
    host = (struct hostent *)gethostbyname("localhost");

    if (host == NULL)
    {
        printf("No se pudo resolver el host\n");
        escribirLog(LOG_FILE, "No se pudo resolver el host");
        WSACleanup();
        return WSAGetLastError();
    }

    // Crear socket

    // AF_INET = TCP/IP ipv4
    conn_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (conn_socket == INVALID_SOCKET)
    {
        escribirLog(LOG_FILE, "No se pudo crear el socket");
        WSACleanup();
        return WSAGetLastError();
    }

    memset(&server, 0, sizeof(server));
    memcpy(&server.sin_addr.s_addr, host->h_addr, host->h_length);

    server.sin_family = AF_INET;   // TCP / UDP
    server.sin_port = htons(PORT); // Puerto

    // Se asocia el socket con la direccion del servidor
    respuesta = bind(conn_socket, (struct sockaddr *)&server, sizeof(server));
    escribirLog(LOG_FILE, "Asociando el socket con la direccion del servidor: %s:%d", inet_ntoa(server.sin_addr), PORT);

    if (respuesta == SOCKET_ERROR)
    {
        escribirLog(LOG_FILE, "Error al asociar el socket con la direccion del servidor. Cerrando conexión");
        closesocket(conn_socket);
        WSACleanup();
        return WSAGetLastError();
    }

    if (listen(conn_socket, 1) == SOCKET_ERROR)
    {
        escribirLog(LOG_FILE, "Error al habilitar conexiones entrantes. Cerrando conexión");
        closesocket(conn_socket);
        WSACleanup();
        return WSAGetLastError();
    }
}

int main(int argc, char *argv[])
{
    configurarSocket();

    // Aceptar conexiones
    escribirLog(LOG_FILE, "***** INICIANDO SERVER ******");

    char buffer[100];

    while (1)
    {
        escribirLog(LOG_FILE, "Esperando conexiones en el puerto %d...", PORT);
        stsize = sizeof(struct sockaddr);
        comm_socket = accept(conn_socket, (struct sockaddr *)&client, &stsize);

        if (comm_socket < 0)
        {
            escribirLog(LOG_FILE, "Error al aceptar la conexión. Cerrando conexión");
            closesocket(conn_socket);
            WSACleanup();
            return WSAGetLastError();
        }

        escribirLog(LOG_FILE, "Nuevo usuario conectado desde: %s", inet_ntoa(client.sin_addr));

        // Obtener credenciales de usuario
        recibirMensaje(comm_socket);
        char usuario[40];
        strncpy(usuario, recvBuff, sizeof(usuario) - 1);
        usuario[sizeof(usuario) - 1] = '\0'; // Agrega un caracter no nulo al final del buffer

        escribirLog(LOG_FILE, "Nombre de usuario recibido: %s", recvBuff);

        // Obtener la password de usuario
        recibirMensaje(comm_socket);
        char password[40];
        strncpy(password, recvBuff, sizeof(password) - 1);
        password[sizeof(password) - 1] = '\0';

        escribirLog(LOG_FILE, "Password recibida: %s", recvBuff);

        if (validarUsuario(usuario, password) == 1)
        {
            char *rol = obtenerRol(usuario);

            escribirLog(LOG_FILE, "Usuario logeado correctamente");
            escribirLog(LOG_FILE, "[%s] Usuario %s, direccion %s", mayusculas(rol), usuario, inet_ntoa(client.sin_addr));
            enviarMensajeACliente(comm_socket, "Usuario validado");

            // Ciclo de comunicacion con el cliente hasta que se cierre la conexion
            while (1)
            {
                int bytesRecibidos = recibirMensaje(comm_socket);
                if (bytesRecibidos > 0)
                {
                    escribirLog(LOG_FILE, "[%s] %s: %s", mayusculas(rol), usuario, recvBuff);
                    enviarMensajeACliente(comm_socket, "Mensaje recibido");
                }
                else
                {
                    escribirLog(LOG_FILE, "Cliente desconectado");
                    break;
                }
            }
        }
        else
        {
            enviarMensajeACliente(comm_socket, "Usuario no validado.");
            escribirLog(LOG_FILE, "Usuario no validado.");
        }
    }

    return 0;
}