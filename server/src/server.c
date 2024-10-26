// SERVER
#include <stdio.h>
#include <WinSock2.h>

#include "../include/Rutas.h"
#include "../include/Usuario.h"
#include "../include/Figurita.h"
#include "../include/PeticionIntercambio.h"

#include "../../common/include/ArchivosHandler.h"
#include "../../common/include/CmdStyle.h"
#include "../../common/include/Requests.h"

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

void insertarFigurita(SOCKET comm_socket, char *usuario)
{
    char *rol = obtenerRol(usuario);
    char *accion = recvBuff;

    enviarMensajeACliente(comm_socket, "Introduce el nombre de la figurita: ");
    recibirMensaje(comm_socket);
    char nombre[50];
    strncpy(nombre, recvBuff, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
    escribirLog(LOG_FILE, "Nombre de la figurita: %s", nombre);

    enviarMensajeACliente(comm_socket, "Introduce el pais de la figurita: ");
    recibirMensaje(comm_socket);
    char pais[50];
    strncpy(pais, recvBuff, sizeof(pais) - 1);
    pais[sizeof(pais) - 1] = '\0';
    escribirLog(LOG_FILE, "Pais de la figurita: %s", pais);

    enviarMensajeACliente(comm_socket, "Introduce si la figurita esta disponible (1: si, 0: no): ");
    recibirMensaje(comm_socket);
    int disponible = atoi(recvBuff);
    escribirLog(LOG_FILE, "Disponible: %d", disponible);

    // Figurita
    Figurita figurita;
    figurita.id = obtenerUltimaFiguritaId() + 1;
    strcpy(figurita.usuario, usuario);
    strcpy(figurita.pais, pais);
    strcpy(figurita.jugador, nombre);
    figurita.disponible = disponible;

    if (agregarFigurita(figurita) == 0)
    {
        enviarMensajeACliente(comm_socket, "Figurita insertada correctamente");
        escribirLog(LOG_FILE, "Figurita insertada correctamente");
    }
    else
    {
        enviarMensajeACliente(comm_socket, "Error al insertar la figurita");
        escribirLog(LOG_FILE, "Error al insertar la figurita");
    }
}

void verFiguritas(SOCKET comm_socket, char *usuario)
{
    char *rol = obtenerRol(usuario);
    char *accion = recvBuff;

    int figuritasCount = obtenerFiguritasCount();
    char *lista = figuritasToStringByUsuarioByDisponible(obtenerFiguritas(), figuritasCount, usuario, 1);
    sprintf(lista, "%s\n", lista);

    enviarMensajeACliente(comm_socket, lista);
    escribirLog(LOG_FILE, "Figuritas enviadas al cliente");
}

int insertarUsuario(SOCKET comm_socket, char *usuario)
{
    // VALIDAR QUE EL USUARIO TENGA ROL DE ADMINISTRADOR
    char *rol = obtenerRol(usuario);
    enviarMensajeACliente(comm_socket, rol);
    recibirMensaje(comm_socket);

    if (strcmp(rol, ADMIN_USER) != 0)
    {
        enviarMensajeACliente(comm_socket, "No tienes permisos para insertar un usuario");
        escribirLog(LOG_FILE, "Usuario %s no tiene permisos para insertar un usuario", usuario);
        return -1;
    }

    Usuario usuarioTemp;
    char rolTemp[40] = "coleccionista";

    // Obtener credenciales de usuario
    enviarMensajeACliente(comm_socket, "Introduce el nombre de usuario: ");
    recibirMensaje(comm_socket);
    char nombre[40];
    strncpy(nombre, recvBuff, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0'; // Agrega un caracter no nulo al final del buffer
    escribirLog(LOG_FILE, "Nombre de usuario recibido: %s", recvBuff);

    enviarMensajeACliente(comm_socket, "Introduce la password: ");
    recibirMensaje(comm_socket);
    char password[40];
    strncpy(password, recvBuff, sizeof(password) - 1);
    password[sizeof(password) - 1] = '\0';

    // Validar que la password no sea un string vacío
    if (strlen(password) == 0)
    {
        enviarMensajeACliente(comm_socket, "Error al dar de alta el nuevo usuario: datos incompletos");
        escribirLog(LOG_FILE, "Error al dar de alta el nuevo usuario: datos incompletos");
        return -1;
    }

    escribirLog(LOG_FILE, "Password recibida");

    // Usuario
    strcpy(usuarioTemp.nombre, nombre);
    strcpy(usuarioTemp.password, password);
    strcpy(usuarioTemp.rol, rolTemp);
    usuarioTemp.activo = true;

    if (existeUsuario(usuarioTemp.nombre) == 1)
    {
        enviarMensajeACliente(comm_socket, "El usuario que se intenta agregar ya existe");
        escribirLog(LOG_FILE, "El usuario que se intenta agregar ya existe, intente de nuevo");
        return -1;
    }

    if (agregarUsuario(usuarioTemp) == 0)
    {
        char buffer[512];
        sprintf(buffer, "Usuario %s insertado correctamente", usuarioTemp.nombre);
        enviarMensajeACliente(comm_socket, buffer);
        escribirLog(LOG_FILE, buffer);
    }
    else
    {
        enviarMensajeACliente(comm_socket, "Error al insertar el usuario");
        escribirLog(LOG_FILE, "Error al insertar el usuario");
    }
}
int actualizarPeticionIntercambio(PeticionIntercambio peticion)
{
    FILE *archivo = fopen(PETICIONES_INTERCAMBIO_FILE, "r+");
    if (archivo == NULL)
    {
        return -1; // Error al abrir el archivo
    }

    // Crear un archivo temporal para escribir las actualizaciones
    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL)
    {
        fclose(archivo);
        return -1; // Error al crear el archivo temporal
    }

    char linea[256];
    int actualizada = 0;

    // Leer línea por línea del archivo original
    while (fgets(linea, sizeof(linea), archivo))
    {
        char usuario[50], paisO[50], jugadorO[50], paisR[50], jugadorR[50], estado[50];
        sscanf(linea, "%49[^|]|%49[^|]|%49[^|]|%49[^|]|%49[^|]|%49[^\n]", usuario, paisO, jugadorO, paisR, jugadorR, estado);

        // Verificar si la línea corresponde a la petición que se quiere actualizar
        if (strcmp(usuario, peticion.usuarioCreador) == 0 && strcmp(paisO, peticion.paisOf) == 0 && strcmp(jugadorO, peticion.jugadorOf) == 0)
        {
            // Escribir la línea actualizada en el archivo temporal
            fprintf(temp, "%s|%s|%s|%s|%s|%s\n", peticion.usuarioCreador, peticion.paisOf, peticion.jugadorOf, peticion.paisReq, peticion.jugadorReq, peticion.estado);
            actualizada = 1; // Marcar que se ha actualizado
        }
        else
        {
            // Escribir la línea original en el archivo temporal
            fputs(linea, temp);
        }
    }

    fclose(archivo); // Cerrar el archivo original
    fclose(temp);    // Cerrar el archivo temporal

    // Solo renombrar si se ha realizado una actualización
    if (actualizada)
    {
        // Eliminar el archivo original
        remove(PETICIONES_INTERCAMBIO_FILE);
        
        rename("temp.txt", PETICIONES_INTERCAMBIO_FILE);
    }
    else
    {
        // Si no se actualizó, eliminar el archivo temporal
        remove("temp.txt");
    }

    return actualizada ? 0 : 1; // Retornar el estado
}
int emparejarPeticionesIntercambio()
{
    PeticionIntercambio *peticiones = obtenerPeticionesIntercambio();
    int peticionesCount = obtenerPeticionesIntercambioCount();

    for (int i = 0; i < peticionesCount; i++)
    {
        if (strcmp(peticiones[i].estado, "PENDIENTE") == 0)
        {
            for (int j = i + 1; j < peticionesCount; j++)
            {
                if (strcmp(peticiones[j].estado, "PENDIENTE") == 0 &&
                    strcmp(peticiones[i].paisReq, peticiones[j].paisOf) == 0 &&
                    strcmp(peticiones[i].jugadorReq, peticiones[j].jugadorOf) == 0 &&
                    strcmp(peticiones[i].paisOf, peticiones[j].paisReq) == 0 &&
                    strcmp(peticiones[i].jugadorOf, peticiones[j].jugadorReq) == 0)
                {
                    // Marcar peticiones como REALIZADA
                    strcpy(peticiones[i].estado, "REALIZADA");
                    strcpy(peticiones[j].estado, "REALIZADA");

                    // Actualizar peticiones en el archivo
                    int result1 = actualizarPeticionIntercambio(peticiones[i]);
                    int result2 = actualizarPeticionIntercambio(peticiones[j]);

                    if (result1 == 0 && result2 == 0)
                    {
                        escribirLog(LOG_FILE, "Intercambio realizado entre %s y %s", peticiones[i].usuarioCreador, peticiones[j].usuarioCreador);
                    }
                    else
                    {
                        // Manejar el error de actualización
                        escribirLog(LOG_FILE, "Error al actualizar el intercambio entre %s y %s", peticiones[i].usuarioCreador, peticiones[j].usuarioCreador);
                    }
                }
            }
        }
    }

    return 1; // Retornar éxito
}
int insertarPeticionIntercambio(SOCKET comm_socket, char *usuario)
{
    char nombreOf[50];
    char paisOf[50];
    char nombreReq[50];
    char paisReq[50];
    int opcion;
    Figurita figurita;

    enviarMensajeACliente(comm_socket, OK);
    recibirMensaje(comm_socket);

    if (strcmp(recvBuff, VER_FIGURITAS) == 0)
    {

        // Envia figuritas disponibles del usuario al cliente
        verFiguritas(comm_socket, usuario);
        enviarMensajeACliente(comm_socket, OK);
    }

    recibirMensaje(comm_socket);
    if (strcmp(recvBuff, OK) == 0)
    {
        enviarMensajeACliente(comm_socket, "Introduce el id de la figurita que quieres intercambiar: ");
        recibirMensaje(comm_socket);
        opcion = atoi(recvBuff);
        escribirLog(LOG_FILE, "Id de la figurita a intercambiar: %d", opcion);

        enviarMensajeACliente(comm_socket, "Introduce el nombre de la figurita que quieres recibir: ");
        recibirMensaje(comm_socket);
        strncpy(nombreReq, recvBuff, sizeof(nombreReq) - 1);
        nombreReq[sizeof(nombreReq) - 1] = '\0';
        escribirLog(LOG_FILE, "Nombre de la figurita a recibir: %s", nombreReq);

        enviarMensajeACliente(comm_socket, "Introduce el pais de la figurita que quieres recibir: ");
        recibirMensaje(comm_socket);
        strncpy(paisReq, recvBuff, sizeof(paisReq) - 1);
        paisReq[sizeof(paisReq) - 1] = '\0';
        escribirLog(LOG_FILE, "Pais de la figurita a recibir: %s", paisReq);
    }

    figurita = obtenerFiguritaPorId(opcion);

    strcpy(nombreOf, figurita.jugador);
    strcpy(paisOf, figurita.pais);

    // SE GUARDAN LOS DATOS EN EL ARCHIVO DE PETICIONES DE INTERCAMBIO
    // CON EL FORMATO usuarioCreador|paisOf|jugadorOf|paisReq|jugadorReq|estado
    PeticionIntercambio peticion;
    strcpy(peticion.usuarioCreador, usuario);
    strcpy(peticion.paisOf, figurita.pais);
    strcpy(peticion.jugadorOf, figurita.jugador);
    strcpy(peticion.paisReq, paisReq);
    strcpy(peticion.jugadorReq, nombreReq);
    strcpy(peticion.estado, "PENDIENTE");

    int validacionIntercambio = agregarPeticionIntercambio(peticion);

    if (validacionIntercambio == 1)
    {
        enviarMensajeACliente(comm_socket, "Peticion de intercambio insertada correctamente");
        escribirLog(LOG_FILE, "Peticion de intercambio insertada correctamente");
    }
    else if (validacionIntercambio == 2)
    {
        enviarMensajeACliente(comm_socket, "La peticion de intercambio ya existe, buscando intercambio...");
        escribirLog(LOG_FILE, "La peticion de intercambio ya existe, buscando intercambio...");
        int emparejamiento = emparejarPeticionesIntercambio();
        if (emparejamiento == 1)
        {
            enviarMensajeACliente(comm_socket, "Intecambio completado correctamente");
        }
        else
        {
            enviarMensajeACliente(comm_socket, "Error al intercambiar la peticion");
        }
    }
    else
    {
        enviarMensajeACliente(comm_socket, "Error al insertar la peticion de intercambio");
        escribirLog(LOG_FILE, "Error al insertar la peticion de intercambio");
    }
}

void bajaUsuario(SOCKET comm_socket, char *usuario)
{
    recibirMensaje(comm_socket);

    char *rol = obtenerRol(usuario);
    if (strcmp(recvBuff, SOLICITAR_ROL) == 0)
    {
        enviarMensajeACliente(comm_socket, rol);
    }

    if (strcmp(rol, ADMIN_USER) != 0)
    {
        enviarMensajeACliente(comm_socket, "No tienes permisos para dar de baja un usuario");
        escribirLog(LOG_FILE, "Usuario %s no tiene permisos para dar de baja un usuario", usuario);
        return;
    }
    int usuariosActivos = obtenerUsuariosActivosCount();
    char *lista = usuariosToString(obtenerUsuariosActivos(), usuariosActivos);
    sprintf(lista, "\n%s\nSeleccione el nombre del usuario a eliminar:", lista);

    enviarMensajeACliente(comm_socket, lista);

    recibirMensaje(comm_socket);
    char usuarioAEliminar[40];
    strncpy(usuarioAEliminar, recvBuff, sizeof(usuarioAEliminar) - 1);
    usuarioAEliminar[sizeof(usuarioAEliminar) - 1] = '\0'; // Agrega un caracter no nulo al final del buffer

    char buffer[100];
    int resultadoEliminarUsuario = eliminarUsuario(usuarioAEliminar);
    int resultadoEstadoPeticionUsuario = modificarEstadoPeticionUsuario(usuarioAEliminar);

    if (resultadoEliminarUsuario == 1)
    {

        sprintf(buffer, "Usuario %s eliminado correctamente", usuarioAEliminar);
        enviarMensajeACliente(comm_socket, buffer);
        escribirLog(LOG_FILE, buffer);
    }
    else
    {

        sprintf(buffer, "Error al eliminar el usuario %s, no existe", usuarioAEliminar);
        enviarMensajeACliente(comm_socket, buffer);
        escribirLog(LOG_FILE, buffer);
    }

    if (resultadoEstadoPeticionUsuario == 1)
    {
        char buffer[100];
        sprintf(buffer, "Peticiones de intercambio del usuario %s canceladas correctamente", usuarioAEliminar);
        escribirLog(LOG_FILE, buffer);
    }
    else
    {
        escribirLog(LOG_FILE, "Error al cancelar las peticiones de intercambio del usuario, no existe el usuario");
    }
}
void verRegistroActividades(SOCKET comm_socket, char *usuario)
{
    // VALIDA QUE EL USUARIO TENGA ROL DE ADMINISTRADOR
    char *rol = obtenerRol(usuario);
    enviarMensajeACliente(comm_socket, rol);
    if (strcmp(rol, ADMIN_USER) != 0)
    {
        enviarMensajeACliente(comm_socket, "No tienes permisos para ver el registro de actividades");
        escribirLog(LOG_FILE, "Usuario %s no tiene permisos para ver el registro de actividades", usuario);
        return;
    }
    FILE *file = fopen(LOG_FILE, "r");
    if (file == NULL)
    {
        perror("Error al abrir el archivo LOG");
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        enviarMensajeACliente(comm_socket, buffer);
    }
    enviarMensajeACliente(comm_socket, FIN);
    fclose(file);
    escribirLog(LOG_FILE, "Registro de actividades enviado al cliente");
}

void manejarSolicitudes(SOCKET comm_socket, char *recvBuff, char *usuario)
{

    // Insertar figurita
    if (strcmp(recvBuff, INSERTAR_FIGURITA) == 0)
    {
        insertarFigurita(comm_socket, usuario);
    }

    if (strcmp(recvBuff, VER_FIGURITAS) == 0)
    {
        verFiguritas(comm_socket, usuario);
    }

    if (strcmp(recvBuff, BAJA_USUARIO) == 0)
    {

        bajaUsuario(comm_socket, usuario);
    }
    if (strcmp(recvBuff, INSERTAR_USUARIO) == 0)
    {
        insertarUsuario(comm_socket, usuario);
    }

    if (strcmp(recvBuff, INSERTAR_PETICION_INTERCAMBIO) == 0)
    {
        insertarPeticionIntercambio(comm_socket, usuario);
    }

    if (strcmp(recvBuff, VER_REGISTRO_ACTIVIDADES) == 0)
    {
        verRegistroActividades(comm_socket, usuario);
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
                    manejarSolicitudes(comm_socket, recvBuff, usuario);
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