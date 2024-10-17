#ifndef MENU_H
#define MENU_H
#include <WinSock2.h>

#include "../../common/include/CmdStyle.h"
#include "../../common/include/Util.h"

void mostrarMenuPrincipal(struct sockaddr_in server, struct hostent *host);
void mostrarMenuConexion(struct sockaddr_in server);
void mostrarMenuFiguritas(struct sockaddr_in server);

void mostrarMenuInsertarFigurita(struct sockaddr_in server, char *nombre, char *pais, int disponible);

#endif // MENU_H