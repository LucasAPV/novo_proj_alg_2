#ifndef __PARSE_FILE__
#define __PARSE_FILE__
#define FILE_PATH "../beastiary/bestiario.txt"
#include "beasts.h"

char** next_beast_definition(int start_pos);
void parse_beast(char** beast_def, Beast *b);
int  string_to_array(char* str, char** str_array, int max);   // mudou
int  string_tok(char* str, char str_arr[MAX_SEPARETORS][TEXT_SIZE]); // mudou
void colon_separetor(const char* s, char* lhs, char* rhs);
void string_to_freq(char* str, Freq* f);
void string_to_dice(const char* str, Dice* d);
void string_to_attacks(char* str, AttackList* list);
void string_to_defences(char* str, DefenceList* list);
void string_to_terrain_mods(char* str, TerrainModList* list);


#endif