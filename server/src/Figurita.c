#include "../include/Figurita.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/Rutas.h"

#include "../../common/include/ArchivosHandler.h"

// Parsea un string con el formato "id;usuario;pais;jugador;disponible" y lo convierte en una Figurita
Figurita parsearFigurita(const char *texto)
{
    Figurita figurita;
    char buffer[256];
    strncpy(buffer, texto, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    char *token = strtok(buffer, ";");
    if (token != NULL)
    {
        figurita.id = atoi(token);
    }

    token = strtok(NULL, ";");
    if (token != NULL)
    {
        strncpy(figurita.usuario, token, sizeof(figurita.usuario));
        figurita.usuario[sizeof(figurita.usuario) - 1] = '\0';
    }

    token = strtok(NULL, ";");
    if (token != NULL)
    {
        strncpy(figurita.pais, token, sizeof(figurita.pais));
        figurita.pais[sizeof(figurita.pais) - 1] = '\0';
    }

    token = strtok(NULL, ";");
    if (token != NULL)
    {
        strncpy(figurita.jugador, token, sizeof(figurita.jugador));
        figurita.jugador[sizeof(figurita.jugador) - 1] = '\0';
    }

    token = strtok(NULL, ";");
    if (token != NULL)
    {
        figurita.disponible = atoi(token);
    }

    return figurita;
};

int obtenerUltimaFiguritaId()
{
    char **lineas = separarLineas(FIGURITAS_FILE);
    int maxId = 0;

    // Verificar si no hay figuritas
    if (strcmp(lineas[0], "") == 0)
    {
        return maxId; // Retornar 0 si no hay figuritas
    }

    for (int i = 0; lineas[i] != NULL; i++)
    {
        Figurita figurita = parsearFigurita(lineas[i]);
        if (figurita.id > maxId)
        {
            maxId = figurita.id;
        }
    }

    return maxId;
};

// Agrega una figurita al archivo de figuritas
// Devuelve 0 si tiene éxito, -1 si hay un error
int agregarFigurita(Figurita figurita)
{
    FILE *file = fopen(FIGURITAS_FILE, "a");
    if (file == NULL)
    {
        perror("Error al abrir el archivo");
        return -1;
    }
    fprintf(file, "%d;%s;%s;%s;%d\n", figurita.id, figurita.usuario, figurita.pais, figurita.jugador, figurita.disponible);
    fclose(file);
    return 0;
};

void actualizarFigurita(Figurita figurita)
{
    char **lineas = separarLineas(FIGURITAS_FILE);
    FILE *file = fopen(FIGURITAS_FILE, "w");
    if (file == NULL)
    {
        perror("Error al abrir el archivo");
        return;
    }
    for (int i = 0; lineas[i] != NULL; i++)
    {
        Figurita f = parsearFigurita(lineas[i]);
        if (f.id == figurita.id)
        {
            fprintf(file, "%d;%s;%s;%s;%d\n", figurita.id, figurita.usuario, figurita.pais, figurita.jugador, figurita.disponible);
        }
        else
        {
            fprintf(file, "%d;%s;%s;%s;%d\n", f.id, f.usuario, f.pais, f.jugador, f.disponible);
        }
    }
    fclose(file);
};

void eliminarFigurita(int id)
{
    char **lineas = separarLineas(FIGURITAS_FILE);
    FILE *file = fopen(FIGURITAS_FILE, "w");
    if (file == NULL)
    {
        perror("Error al abrir el archivo");
        return;
    }
    for (int i = 0; lineas[i] != NULL; i++)
    {
        Figurita f = parsearFigurita(lineas[i]);
        if (f.id != id)
        {
            fprintf(file, "%d;%s;%s;%s;%d\n", f.id, f.usuario, f.pais, f.jugador, f.disponible);
        }
    }
    fclose(file);
};

Figurita *obtenerFiguritas()
{
    char **lineas = separarLineas(FIGURITAS_FILE);
    Figurita *figuritas = malloc(sizeof(Figurita) * 100);
    for (int i = 0; lineas[i] != NULL; i++)
    {
        figuritas[i] = parsearFigurita(lineas[i]);
    }
    return figuritas;
};

int obtenerFiguritasCount()
{
    char **lineas = separarLineas(FIGURITAS_FILE);
    int count = -1; // Se resta 1 para no contar la última línea que es NULL

    if (lineas[0] == NULL || strcmp(lineas[0], "") == 0)
    {
        return 0;
    }

    for (int i = 0; lineas[i] != NULL; i++)
    {

        count++;
    }

    return count;
};

Figurita obtenerFiguritaPorId(int id)
{
    char **lineas = separarLineas(FIGURITAS_FILE);
    for (int i = 0; lineas[i] != NULL; i++)
    {
        Figurita figurita = parsearFigurita(lineas[i]);
        if (figurita.id == id)
        {
            return figurita;
        }
    }
};

Figurita *obtenerFiguritasPorUsuario(char *usuario)
{
    char **lineas = separarLineas(FIGURITAS_FILE);
    Figurita *figuritas = malloc(sizeof(Figurita) * 100);
    int count = 0;
    for (int i = 0; lineas[i] != NULL; i++)
    {
        Figurita figurita = parsearFigurita(lineas[i]);
        if (strcmp(figurita.usuario, usuario) == 0)
        {
            figuritas[count] = figurita;
            count++;
        }
    }
    return figuritas;
};

char *figuritasToStringByUsuario(Figurita *figuritas, int numFiguritas, const char *usuarioFiltro)
{
    if (figuritas == NULL || numFiguritas == 0)
    {
        char *noFiguritas = malloc(17);
        strcpy(noFiguritas, "No hay figuritas");
        return noFiguritas;
    }

    size_t bufferSize = 1024;
    char *buffer = malloc(bufferSize);
    if (buffer == NULL)
    {
        return NULL; // Manejo de errores de asignación de memoria.
    }
    buffer[0] = '\0'; // Inicializa el buffer como una cadena vacía.

    for (int i = 0; i < numFiguritas; i++)
    {
        // Si usuarioFiltro no es NULL, filtra por usuario
        if (usuarioFiltro != NULL && strcmp(figuritas[i].usuario, usuarioFiltro) != 0)
        {
            continue; // Salta esta figurita si el usuario no coincide
        }

        char temp[512];
        snprintf(temp, sizeof(temp), "ID: %d, Jugador: %s, Pais: %s, Disponible: %d\n",
                 figuritas[i].id, figuritas[i].jugador, figuritas[i].pais, figuritas[i].disponible);

        // Verifica si el buffer tiene suficiente espacio
        if (strlen(buffer) + strlen(temp) + 1 > bufferSize)
        {
            bufferSize *= 2;
            char *newBuffer = realloc(buffer, bufferSize);
            if (newBuffer == NULL)
            {
                free(buffer);
                return NULL; // Manejo de error de realloc.
            }
            buffer = newBuffer; // Actualiza el buffer con el nuevo tamaño.
        }

        strcat(buffer, temp); // Añade el nuevo registro al buffer.
    }

    // Si el buffer está vacío, significa que no hubo figuritas del usuario especificado.
    if (strlen(buffer) == 0)
    {
        free(buffer);
        char *noFiguritas = malloc(35);
        strcpy(noFiguritas, "No hay figuritas para este usuario");
        return noFiguritas;
    }

    return buffer;
}

char *figuritasToString(Figurita *figuritas, int numFiguritas)
{
    if (figuritas == NULL || numFiguritas == 0)
    {
        char *noFiguritas = malloc(17);
        strcpy(noFiguritas, "No hay figuritas");
        return noFiguritas;
    }

    size_t bufferSize = 1024;
    char *buffer = malloc(bufferSize);
    if (buffer == NULL)
    {
        return NULL; // Asegúrate de manejar errores de asignación de memoria.
    }
    buffer[0] = '\0'; // Inicializa el buffer como una cadena vacía.

    for (int i = 0; i < numFiguritas; i++)
    {
        char temp[512];
        snprintf(temp, sizeof(temp), "ID: %d, Jugador: %s, Pais: %s, Disponible: %d\n",
                 figuritas[i].id, figuritas[i].jugador, figuritas[i].pais, figuritas[i].disponible);

        // Verifica si el buffer tiene suficiente espacio
        if (strlen(buffer) + strlen(temp) + 1 > bufferSize)
        {
            bufferSize *= 2;
            char *newBuffer = realloc(buffer, bufferSize);
            if (newBuffer == NULL)
            {
                free(buffer);
                return NULL; // Maneja el error de realloc.
            }
            buffer = newBuffer; // Actualiza el buffer con el nuevo tamaño.
        }

        strcat(buffer, temp); // Añade el nuevo registro al buffer.
    }

    return buffer;
};

char *figuritasToStringByUsuarioByDisponible(Figurita *figuritas, int numFiguritas, const char *usuarioFiltro, int disponibleFiltro)
{
    if (figuritas == NULL || numFiguritas == 0)
    {
        char *noFiguritas = malloc(17);
        strcpy(noFiguritas, "No hay figuritas");
        return noFiguritas;
    }

    size_t bufferSize = 1024;
    char *buffer = malloc(bufferSize);
    if (buffer == NULL)
    {
        return NULL; // Manejo de errores de asignación de memoria.
    }
    buffer[0] = '\0'; // Inicializa el buffer como una cadena vacía.

    for (int i = 0; i < numFiguritas; i++)
    {
        // Si usuarioFiltro no es NULL, filtra por usuario
        if (usuarioFiltro != NULL && strcmp(figuritas[i].usuario, usuarioFiltro) != 0)
        {
            continue; // Salta esta figurita si el usuario no coincide
        }

        // Filtra por disponibilidad
        if (figuritas[i].disponible != disponibleFiltro)
        {
            continue; // Salta esta figurita si la disponibilidad no coincide
        }

        char temp[512];
        snprintf(temp, sizeof(temp), "ID: %d, Jugador: %s, Pais: %s, Disponible: %d\n",
                 figuritas[i].id, figuritas[i].jugador, figuritas[i].pais, figuritas[i].disponible);

        // Verifica si el buffer tiene suficiente espacio
        if (strlen(buffer) + strlen(temp) + 1 > bufferSize)
        {
            bufferSize *= 2;
            char *newBuffer = realloc(buffer, bufferSize);
            if (newBuffer == NULL)
            {
                free(buffer);
                return NULL; // Manejo de error de realloc.
            }
            buffer = newBuffer; // Actualiza el buffer con el nuevo tamaño.
        }

        strcat(buffer, temp); // Añade el nuevo registro al buffer.
    }

    // Si el buffer está vacío, significa que no hubo figuritas del usuario y disponibilidad especificados.
    if (strlen(buffer) == 0)
    {
        free(buffer);
        char *noFiguritas = malloc(35);
        strcpy(noFiguritas, "No hay figuritas para este usuario");
        return noFiguritas;
    }

    return buffer;
}