#include "party.h"
#include "adventure.h"


int party_total_power(Party p, CharacterDB c_db){
    int total_power = 0;
    for (int i = 0; i < p.quantity; ++i) {
        Character *c =
            (Character*)select_character_by_id(p.ids_adventures[i], &c_db);

        if(c == NULL) return 0;

        total_power += c->power;
    }

    return total_power;
}

int create_party(int ids[MAX_PARTY_SIZE], int quantity, Party* p, CharacterDB c_db){
    for (int i = 0; i < quantity; ++i) {
        p->ids_adventures[i] = ids[i];
    }

    p->quantity = quantity;

    // gerar poder total;
    // pegar os bonecos no banco de dados, e somar o poder total de cada um
    p->total_power = party_total_power(*p, c_db);

    return 1;
}