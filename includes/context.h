#ifndef __CONTEXT__
#define __CONTEXT__

#include "beasts.h"
#include "basic_imports.h"
#include "definitions.h"

typedef enum {
    EASY,   // [60, 80]
    MEDIUM, // [90, 110]
    HARD    // [120, 150]
}  Dificulty;

typedef struct {
    char terrain[TEXT_SIZE];
    Active period;
    Dificulty dificulty;
    float minimum;
    float maximum;
} ContextEncounter;

int generate_context(char terrain[TEXT_SIZE], Active period, Dificulty dificulty, ContextEncounter *c, int party_power);
#endif