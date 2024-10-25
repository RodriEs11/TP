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
    int count = 0;
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

char *figuritasToString(Figurita *figuritas, int numFiguritas)
{
    if (figuritas == NULL || numFiguritas == 0)
    {
        char *noFiguritas = malloc(25);
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