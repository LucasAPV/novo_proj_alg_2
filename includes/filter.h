#ifndef __FILTER__
#define __FILTER__

#include "beasts.h"
#include "context.h"

typedef struct {
    Beast b[MAX_BEASTS];
    int quantity_found;
} BeastsCandidates ;

int filter_candidates(BeastDB b_db, BeastsCandidates *b, ContextEncounter c);

#endif