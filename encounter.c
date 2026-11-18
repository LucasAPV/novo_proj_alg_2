// Como forma de simplificar o processo de encontro,
// usarei um algoritmo mais simples do que o apresentado
#include "encounter.h"
#include "beasts.h"
#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>

static int roll_dice_qtd(Dice d){
    int amount = 0;
    for (int i = 0; i < d.dice_quantity; ++i) {
        amount += (rand() % d.faces_of_dices) + 1;
    }

    amount += d.bonus;

    return amount;
}

static EncounteredBeast choose_beast_and_quantity(BeastsCandidates b_cad){
    int choosen_id_beast = rand() % b_cad.quantity_found;
    Beast choosen_beast = b_cad.b[choosen_id_beast];
    int quantity_found = roll_dice_qtd(choosen_beast.numeroEncontrado);

    EncounteredBeast eb = {.quantity = quantity_found, .b = choosen_beast};

    return eb;
}

static void total_power_encounter(PossibleEncounters *enc){
    int amount = 0;

    // Para simplicidade, evitaremos os bonus adicionais
    for (int i = 0; i < MAX_ENCOUNTERS; ++i) {
        amount += (enc->encounter_beasts[i].b.poderBase);
    }

    enc->total_power = amount;
}

void generate_encounter(BeastsCandidates b_cad, PossibleEncounters* encounter){
    for (int i = 0; i < MAX_ENCOUNTERS; ++i) {
        encounter->encounter_beasts[i] = choose_beast_and_quantity(b_cad);
    }
    total_power_encounter(encounter);
}

void print_encounter(PossibleEncounters enc){
    printf("Poder Total: %d\n", enc.total_power);

    printf("Monstros: \n");

    for (int i = 0; i < MAX_ENCOUNTERS; ++i) {
        printf("\n Enconstro numero %d\n", i);
        lista_monstro_terreno(&enc.encounter_beasts[i].b, 1);
        printf("Numero encontrado: %d \n", enc.encounter_beasts[i].quantity);
    }
}