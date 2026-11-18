#include "context.h"
#include "beasts.h"
#include "definitions.h"
#include <stdio.h>
int generate_context(char terrain[TEXT_SIZE], Active period,
    Dificulty dificulty, ContextEncounter *c, int party_power) {
    c->period = period;
    c->dificulty = dificulty;
    strcpy(c->terrain, terrain);

    switch (dificulty) {
        case EASY:
            c->minimum = (float)(party_power * EASY_MIN + 99) / 100;
            c->maximum = (float)(party_power * EASY_MAX) / 100;
        break;

        case MEDIUM:
            c->minimum = (float)(party_power * MEDIUM_MIN + 99) / 100;
            c->maximum = (float)(party_power * MEDIUM_MAX) / 100;
        break;

        case HARD:
            c->minimum = (float)(party_power * HARD_MIN + 99) / 100;
            c->maximum = (float)(party_power * HARD_MAX) / 100;
        break;

        default:
            printf("DIFICULDADE %d DESCONHECIDA", dificulty);
            return 0;
    }
    return 1;
}

void print_context(ContextEncounter context) {
    printf("\nTerreno: %s\n", context.terrain);
    switch ((int)context.period) {
        case (int)DIURNO:
            printf("Periodo: DIURNO\n");
        break;

        case (int)NOTURNO:
            printf("Periodo: NOTURNO\n");
        break;

        case (int)QUALQUER_HORARIO:
            printf("Periodo: QUALQUER_HORARIO\n");
        break;
    }

    switch (context.dificulty) {
        case HARD:
            printf("Dificuldade: HARD\n");
        break;

        case EASY:
            printf("Dificuldade: EASY\n");
        break;

        case MEDIUM:
            printf("Dificuldade: MEDIUM\n");
        break;
    }
}