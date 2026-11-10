#ifndef __ITEM__
#define __ITEM__

#include "definitions.h"

typedef enum {
   HELMET,
   CHESTPLATE,
   GLOVES,
   LEGS,
   GREAVES,
   RING,
   NECKLACE,
   BELT,
   ONE_HAND_SWORD,
   TWO_HAND_SWORD,
   SHIELD
} TypeItem;

typedef struct 
{
   int id;
   char name[NAME_LENGHT];
   TypeItem type_item;
   int occupied_space;
   int attack_bonus;
   int defence_bonus;
   int life_bonus;
   int iniciative_bonus;
   int power_bonus;
}Item;

typedef struct {
   Item itens[MAX_CAPACITY];
   int curr_id;
} ItemDB;

void* create_item(Item *it, ItemDB *i_db);
void print_item_types();
void print_item(Item it);
#endif