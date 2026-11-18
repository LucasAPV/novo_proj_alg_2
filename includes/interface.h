#ifndef __INTERFACE__
#define __INTERFACE__

#include "basic_imports.h"
#include "definitions.h"

int create_initial_menu();
int character_area();
int item_area();
int monster_area();
int candidates_area();
int party_area();
int context_area();
int encounter_area();

void create_options(char options[][TEXT_SIZE], int num_options);
void create_title (char* title);
#endif