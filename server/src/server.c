// SERVER
#include <stdio.h>
#include <WinSock2.h>


char sendBuff[512],recvBuff[512];

int main(int argc, char *argv[])
{
    WSADATA wsaData;
    WORD wVersionRequerida = MAKEWORD(2, 2);

    SOCKET conn_socket, comm_socket;
    SOCKET comunicacion;

    struct sockaddr_in server;
    struct sockaddr_in client;
    struct hostent *host;
    int respuesta, stsize;

    respuesta = WSAStartup(wVersionRequerida, &wsaData);

    if (respuesta)
    {
        printf("No se pudo iniciar Winsock\n");
        getchar();
        return respuesta;
    }

    // Ip del servidor
    host = (struct hostent *)gethostbyname("localhost");

    if (host == NULL)
    {
        printf("No se pudo resolver el host\n");
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
        getchar();
        WSACleanup();
        return WSAGetLastError();
    }

    memset(&server, 0, sizeof(server));
    memcpy(&server.sin_addr.s_addr, host->h_addr, host->h_length);

    server.sin_family = AF_INET; // TCP / UDP
    server.sin_port = htons(6000); // Puerto



    // Se asocia el socket con la direccion del servidor
    respuesta = bind(conn_socket, (struct sockaddr *)&server, sizeof(server));
    if( respuesta == SOCKET_ERROR){
        printf("Error al asociar el socket con la direccion del servidor\n");
        closesocket(conn_socket);
        WSACleanup();
        return WSAGetLastError();
    }

    if(listen(conn_socket, 1) == SOCKET_ERROR){
        printf("Error al habilitar conexiones entrantes\n");
        closesocket(conn_socket);
        WSACleanup();
        getchar();
        return WSAGetLastError();
    }

    // Aceptar conexiones
    printf("***** SERVER ******\n");
    printf("Esperando conexiones...\n");
    stsize = sizeof(struct sockaddr);
    comm_socket = accept(conn_socket, (struct sockaddr *)&client, &stsize);

    if (comm_socket == INVALID_SOCKET)
    {
        printf("Error al aceptar la conexion\n");
        closesocket(conn_socket);
        WSACleanup();
        getchar();
        return WSAGetLastError();
    }

    printf("Conexion entrante desde: %s\n", inet_ntoa(client.sin_addr));

    // Cerrar socket de escucha
    closesocket(conn_socket);


    // Recibir mensaje
    recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
    printf("Datos recibidos: %s\n", recvBuff);

    // Enviar mensaje
    strcpy(sendBuff, "Hola cliente, tu mensaje fue recibido");
    send(comm_socket, sendBuff, sizeof(sendBuff), 0);

    // Cerramos el socket de comunicacion
    closesocket(comm_socket);

    // Cerramos la libreria winsock
    WSACleanup();

    return 0;
}