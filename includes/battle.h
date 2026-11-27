#ifndef __BATTLE__
#define __BATTLE__

#include "definitions.h"
#include "basic_imports.h" 
#include "party.h"
#include "beasts.h"
#include "adventure.h"
#include "filter.h"
#include "encounter.h"


typedef struct {
    Party p;
    PossibleEncounters encounter;
    int choosed_encounter;

    int total_beast_power;
    int total_party_power;
} BattleSpecifier;

void show_party_members(Party party, CharacterDB c_db, int life[]);
void show_monster_members(EncounteredBeast enc_b, int lifes[]);
void gen_life(int gen_life[], int size);
void attack_phase(bool turn, int char_atk, Beast beast_atk, int beast_life[], int life_beast_id, int char_life[], CharacterDB c_db);
bool is_everyone_dead(int lifes[], int size);
void show_battlefield(int char_life[], int beast_life[], Party party, EncounteredBeast enc_b, CharacterDB c_db);
void actions_for_battle(BattleSpecifier *bs, CharacterDB c_db);
#endif