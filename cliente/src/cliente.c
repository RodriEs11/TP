//CLIENTE
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>


char sendBuff[512],recvBuff[512];

int main(int argc, char *argv[])  
{  

    WSADATA wsaData;
    SOCKET conn_socket;
    struct sockaddr_in server;
    struct hostent *host;
    int respuesta;

    // Inicializar Winsock
    respuesta = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (respuesta != 0) {
        printf("Error al inicializar Winsock");
        getchar();        
        return -1;
    }
    

    // Configurar ip
    host = (struct hostent *)gethostbyname("localhost");
    if (host == NULL) {
        printf("No se pudo resolver host\n");
        getchar();
        WSACleanup();
        return WSAGetLastError();
    }

    // Crear socket
    conn_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (conn_socket == INVALID_SOCKET) {
        printf("No se pudo crear socket\n");
        getchar();
        WSACleanup();
        return WSAGetLastError();
    }

    // Configurar servidor
    memset(&server, 0, sizeof(server));
    memcpy(&server.sin_addr.s_addr, host->h_addr, host->h_length);

    server.sin_family = AF_INET;
    server.sin_port = htons(6000);


    // Conexion al servidor
    if (connect(conn_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Error al conectar al servidor\n");
        closesocket(conn_socket);
        WSACleanup();
        return WSAGetLastError();
    }

    printf("***** CLIENTE ******\n");
    printf("Conexion establecida con %s\n", inet_ntoa(server.sin_addr));

    // Enviar mensaje
    strcpy(sendBuff, "Hola querido servidor, soy el cliente");
    send(conn_socket, sendBuff, sizeof(sendBuff), 0);
    printf("Datos enviados: %s\n", sendBuff);

    printf("Recibiendo mensaje del servidor\n");
    recv(conn_socket, recvBuff, sizeof(recvBuff), 0);
    printf("Datos recibidos: %s\n", recvBuff);

    getchar();

    // Cerrar socket y liberar Winsock
    closesocket(conn_socket);
    WSACleanup();

    
    return 0;  
}