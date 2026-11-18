#ifndef __ENCOUNTER__
#define __ENCOUNTER__

#include "beasts.h"
#include "filter.h"

typedef struct {
    Beast b;
    int quantity;
} EncounteredBeast;

typedef struct {
    EncounteredBeast encounter_beasts[MAX_ENCOUNTERS];
    int total_power;
} PossibleEncounters;

void generate_encounter(BeastsCandidates b_cad, PossibleEncounters* encounter);
void print_encounter(PossibleEncounters enc);
#endif