#include "filter.h"
#include "beasts.h"
#include "definitions.h"
#include <stdlib.h>
#include <string.h>

int filter_candidates(BeastDB b_db, BeastsCandidates *b, ContextEncounter c){
    char terreno[TEXT_SIZE];
    strcpy(terreno, c.terrain);
    Beast **possible_by_terrain = calloc(MAX_BEASTS, sizeof(Beast*));
    Beast *monster_by_period = malloc(MAX_BEASTS * sizeof(Beast));

    if(listaMonstrosPorTerreno(&b_db, terreno, possible_by_terrain, MAX_BEASTS) == 0){
        free(monster_by_period);
        free(possible_by_terrain);
        return 0;
    }

    int size = 0;

    for (int i = 0; i < MAX_BEASTS; ++i) {
        if(possible_by_terrain[i] == NULL || possible_by_terrain[i]->poderBase == 0) break;
        if(possible_by_terrain[i]->atividade == c.period){
            monster_by_period[size] = *possible_by_terrain[i];
            size++;
        }
    }

    if (size == 0) {
        free(monster_by_period);
        free(possible_by_terrain);
        return 0;
    }


    if(b->quantity_found >= 49) {
        free(monster_by_period);
        free(possible_by_terrain);
        return 0;
    }

    for (int i = 0; i < size; ++i) {
        b->b[i] = monster_by_period[i];
        b->quantity_found++;
    }


    free(monster_by_period);
    free(possible_by_terrain);

    return 1;
}