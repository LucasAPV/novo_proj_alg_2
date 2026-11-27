#include "battle.h"
#include "definitions.h"
#include "interface.h"
#include <stdlib.h>

void show_party_members(Party party, CharacterDB c_db, int life[]){
    for(int i = 0; i < party.quantity; ++i){
        Character* c = (Character*)select_character_by_id(party.ids_adventures[i], &c_db);
        printf("\n| ID: [%d] | NAM: %20s | POW: %3d | HP: %3d |\n",
            c->id, c->name, c->power, c->current_life_points);
        life[i] = c->current_life_points;
    }
}

void show_monster_members(EncounteredBeast enc_b, int lifes[]){
    for(int i = 0; i < enc_b.quantity; ++i){
        printf("\n|ID: [%d] | NAM: %20s | HP: %3d | POW: %3d |\n", i, enc_b.b.nome, lifes[i], enc_b.b.poderBase);
    }
}

void gen_life(int gen_life[], int size){
    int min_life = 10;
    for(int i = 0; i < size; ++i){
        gen_life[i] = (rand()%20) + min_life ;
    }
}

void attack_phase(bool turn, int char_atk, Beast beast_atk, int beast_life[], int life_beast_id, int char_life[], CharacterDB c_db){
    if(turn){
        // Adventure Turn
        Character *chara = (Character*)select_character_by_id(char_atk, &c_db);
        int char_power = chara->power;
        beast_life[life_beast_id] -= char_power;
    } else {
        // Monster Turn
        int beast_power = beast_atk.poderBase;
        char_life[char_atk] -= beast_power;
    }
}

bool is_everyone_dead(int lifes[], int size) {
    int m = 0;
    for(int i = 0; i < size; ++i){
        if(lifes[i] <= 0 || is_struct_empty(lifes[i])) m++;
    }

    return m == size;
}

void show_battlefield(int char_life[], int beast_life[], Party party, EncounteredBeast enc_b, CharacterDB c_db){
    create_title("CAMPO DE BATALHA");
    create_title("AVENTUREIROS");

    for(int i = 0; i < party.quantity; ++i) {
        Character *c = (Character*)select_character_by_id(party.ids_adventures[i], &c_db);
        int hp = char_life[i] > 0 ? char_life[i] : 0;
        printf("[%d] %-5s | HP: %3d %s\n", c->id, c->name, char_life[i], hp == 0 ? "CAIDO" : "");
    }

    create_title("BESTAS");

    for(int i = 0; i < enc_b.quantity; ++i) {
        int hp = beast_life[i] > 0 ? beast_life[i] : 0;
        printf("[%d] %-5s | HP: %3d %s\n", i, enc_b.b.nome, beast_life[i], hp == 0 ? "DERROTADO" : "");
    }
    printf("\n\n");
}


void actions_for_battle(BattleSpecifier *bs, CharacterDB c_db){
    // Estou criando um vetor de vidas porque nn tem vida na especificacao dos bixos. PQP
    int chosen_enc = bs->choosed_encounter;

    int party_number = 6;
    int number_of_beasts = bs->encounter.encounter_beasts->quantity;

    int *life_points_beasts = malloc(sizeof(int)*number_of_beasts);
    int life_points_adv[6] = {0};

    for(int i = 0; i < bs->p.quantity; ++i){
        Character *c = (Character*)select_character_by_id(bs->p.ids_adventures[i], &c_db);
        life_points_adv[i] = c->current_life_points;
    }

    gen_life(life_points_beasts, number_of_beasts);
    int beast_atk = 0;
    int char_atk  = 0;
    bool turn = true; // true -> adv || false -> beast
    Beast besta = {0};

    while (1) {
        //Campo de batalha
        show_battlefield(life_points_adv, life_points_beasts, bs->p, bs->encounter.encounter_beasts[chosen_enc], c_db);

        // Parada para aventureiros e bestas
        if(is_everyone_dead(life_points_adv, party_number)) break;
        if(is_everyone_dead(life_points_beasts, number_of_beasts)) break;

        if(turn == false){
            beast_atk = rand()%number_of_beasts;
            besta = bs->encounter.encounter_beasts[beast_atk].b;
            attack_phase(turn, char_atk, besta, life_points_beasts, beast_atk, life_points_adv, c_db);
            turn = !turn;
            printf("\nA BESTA %d REALIZAOU O ATAQUE, TOME CUIDADO!!\n", beast_atk);
            continue;
        }

        int opt = battle_area();
        switch(opt){
            case 0:
                return;
                break;

            case 1:
                printf("Qual personagem deve atacar?:\n");
                show_party_members(bs->p, c_db, life_points_adv);
                scanf("%d", &char_atk);

                if(life_points_adv[char_atk] <= 0){
                    printf("\n\nO PERSONAGEM JA ESTA MORTO!!! MORTO NAO ATACA\n\n");
                    continue;
                }

                beast_atk = 0;
                printf("Qual mostro deve ser atacado?:\n");
                show_monster_members(bs->encounter.encounter_beasts[chosen_enc], life_points_beasts);
                scanf("%d", &beast_atk);

                if(life_points_beasts[beast_atk] <= 0){
                    printf("\n\nO MOSTRO JA ESTA MORTO!!! NAO ADIANTA BATER EM CACHORRO MORTO!!\n\n");
                    continue;
                }

                break;

            case 3:
                return;
                break;

            default:
                printf("\n OPCAO INVALIDA!!\n");
                break;
        }
        attack_phase(turn, char_atk, besta, life_points_beasts, beast_atk, life_points_adv, c_db);
        turn = !turn;
    }

    //uptade_adventures();

    if(!turn) {
        printf("\n\n OS AVENTIREIROS GANHARAM!!! \n\n");
    } else {
        printf("\n\n OS AVENTIREIROS PERDERAM!!! \n\n");
    }
}