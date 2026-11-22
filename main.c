#include "actions.h"
#include "adventure.h"
#include "beasts.h"
#include "context.h"
#include "definitions.h"
#include "encounter.h"
#include "error.h"
#include "filter.h"
#include "interface.h"
#include "item.h"
#include "party.h"
#include <stdio.h>


typedef struct {
    Party p;
    PossibleEncounters encounter;
    int choosed_encounter;
    
    int total_beast_power;
    int total_party_power;
} BattleSpecifier;

int battle_area(){
    char opcoes[][TEXT_SIZE] = [
        "Sair",
        "Lutar",
        "Fugir"
    ];
    create_title("BATALHA");
    create_options(opcoes, 3);
    
    int opt = 0;
    scanf("%d", &opt);
    
    return opt;
}

void show_party_members(Party party, CharacterDB c_db, int life[]){
    for(int i = 0; i < party.quantity; ++i){
        Character c = (Character)*select_character_by_id(party.ids_adventures[i], &c_db);    
        printf("\n| ID: [%d] | NAM: %20s | POW: %3d | HP: %3d |\n", 
            c.id, c.name, c.power, c.current_life_points);
        life[i] = c.current_life_points;
    }
}

void show_monster_members(EncounteredBeast enc_b, int lifes[]){
    for(int i = 0; i < enc_b.quantity; +i){
        printf("\n|ID: [%d] | NAM: %20s | HP: %3d | POW: %3d |\n", i, enc_b.b.nome, lifes[i], enc_b.b.poderBase);
    }
}

void gen_life(int gen_life[], int size){
    int min_life = 10;
    for(int i = 0; i < size - 1; ++i){
        gen_life[i] = (rand()%50) + min_life ;
    }
}

void attack_phase(bool turn, int char_atk, Beast beast_atk, int beast_life[], int char_life[], CharacterDB c_db){
    if(turn){
        // Adventure Turn
        int char_power = select_character_by_id(char_atk, &c_db).power;
        beast_life[beast_atk.id] -= char_power;
    } else {
        // Monster Turn
        int beast_power = beast_atk.poderBase;
        char_life[char_atk] -= beast_power;
    }
}

void actions_for_battle(int opt, BattleSpecifier *bs, CharacterDB c_db){
    // Estou criando um vetor de vidas porque nn tem vida na especificacao dos bixos. PQP
    int chosen_enc = bs->choosed_encounter;
    
    int party_number = bs->p.quantity;
    int number_of_beasts = bs->encounter[chosen_enc].quantity;
    
    int life_points_beasts[number_of_beasts] = {0};
    int life_points_adv[party_number] = {0};
    
    gen_life(life_points_beasts, number_of_beasts);
    int beast_atk = 0;
    bool turn = true; // true -> adv || false -> beast
    while (1) {
        if(turn == false){
            beast_atk = rand()%number_of_beasts;
            attack_phase(turn, char_attack, beast_atk, life_points_beasts, life_points_adv);
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
                int char_attack = 0;
                printf("Qual personagem deve atacar?:\n");
                show_party_members(bs->p, c_db, life_points_adv);
                scanf("%d", &char_attack);
            
                beast_atk = 0;
                printf("Qual mostro deve ser atacado?:\n");
                show_monster_members(bs->encounter[chosen_enc], life_points_beasts);
                scanf("%d", &beast_atk);
                break;
            
            case 3:
                return;
                break;
            
            default:
                printf("\n OPCAO INVALIDA!!\n");
                break;
        }
        attack_phase(turn, char_attack, beast_atk, life_points_beasts, life_points_adv);
        turn = !turn;
    }
}

int main(void) {
  /* PERSONAGENS */
  CharacterDB c_db = {0};
  ItemDB i_db = {0};

  /* BEASTAS */
  /* Carregando as beastas de uma vez */
  BeastDB b_db = {0};
  beast_db_init(&b_db);
  if (load_beasts(&b_db) == -1) {
    log_error(UNABLE_TO_CREATE_BEAST_DB);
  }
  BeastsCandidates b_candidates = {0};
  // PARTY
  Party party = {0};

  //  ENCONTROS E CONTEXTO
  ContextEncounter context = {0};
  PossibleEncounters encontro = {0};
  int choosed_encounter = 0;

  while (1) {
    int option = create_initial_menu();

    switch (option) {
    case 0:
      printf("SAINDO...\n");
      exit(0);
      break;

    case 1:
      actions_for_characters(&c_db); //&i_db removido para não ativar o -Wall e
                                     //-Wextra na compilação por falta de uso
      break;

    case 2:
      actions_for_items(&c_db, &i_db);
      break;

    case 3:
      actions_for_monsters(&b_db);
      break;

    case 4:
      actions_for_party(&party, c_db);
      break;

    case 5:
      actions_for_context(&context, party);
      break;

    case 6:
      actions_for_candidates(context, &b_candidates, b_db);
      break;

    case 7:
      actions_for_encounter(b_candidates, &encontro, &choosed_encounter);
      break;

    default:
      printf("Opcao invalida!\n");
      break;
    }
  }
  return 0;
}
