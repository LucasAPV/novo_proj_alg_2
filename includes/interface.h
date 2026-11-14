#ifndef __INTERFACE__
#define __INTERFACE__

#include "basic_imports.h"
#include "definitions.h"

int create_initial_menu();
int character_area();
void create_options(char options[][TEXT_SIZE], int num_options);
void create_title (char* title);
#endif