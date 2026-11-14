#ifndef __INTERFACE__
#define __INTERFACE__

#include "basic_imports.h"
#include "definitions.h"

int create_initial_menu();
int character_area();
<<<<<<< HEAD
void create_options(char options[][TEXT_SIZE], int num_options);
=======
int item_area();
void create_options(char options[][TEXT_LENGTH], int num_options);
>>>>>>> eafc00424384ca135115fbe1d2a8d87499e2eae6
void create_title (char* title);
#endif