#ifndef __PARTY__
#define __PARTY__

#include "adventure.h"
#include "definitions.h"
#include "basic_imports.h"

typedef struct {
    int ids_adventures[MAX_PARTY_SIZE];
    int quantity;
    int total_power; // Soma dos itens equipados, nao dos da mochila
} Party ;


//Parte 3, GERACAO DE ENCONTROS.
// Preciso de uma estrutura que represente a party, dificuldade, terreno, dia ou noite,
// Calcular poder do grupo; filtrar candidatos;
// sortear composições; aplicar modificadores; validar poder e diversidade



int create_party(int ids[MAX_PARTY_SIZE], int quantity, Party* p, CharacterDB c_db);


#endif