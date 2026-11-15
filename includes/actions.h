#ifndef __ACTIONS__
#define __ACTIONS__

#include <stdlib.h>
#include <stdio.h>
#include "interface.h"
#include "adventure.h"
#include "item.h"

void actions_for_items(CharacterDB *c_db, ItemDB *i_db);
void actions_for_characters(CharacterDB *c_db);


#endif