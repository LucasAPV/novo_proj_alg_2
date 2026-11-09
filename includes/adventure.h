#ifndef __ADVENTURES__
#define __ADVENTURES__

#include "item.h"

typedef enum {
   HUMAN,
   ELF, 
   DWARF, 
   HALFLING
} Race;


typedef enum {
   WARRIOR,
   ROUGUE, 
   MAGE, 
   CLERIC
} Class;

typedef struct {
   Item head;
   Item cheast;
   Item legs;
   Item gloves;
   Item greaves;
   Item ring;
   Item necklace;
   Item belt;
   Item right_hand;
   Item left_hand;
}BodyPartItem;

typedef struct {
   int id;
   char name[NAME_LENGHT];
   Race race;
   Class class;
   int level; // 1 -- 20
   int max_life_points; // 1 -- 999
   int current_life_points; // 0 -- max_life_points
   int attack; // 0 -- 30
   int defence; // 1 -- 30
   int iniciative; // -5 -- 20
   int power; // 1 -- 100
   
   union{
      Item itens[MAX_CAPACITY];
      int curr_ocupied_space;
   } inventory;

   BodyPartItem items_in_body;
} Character;

typedef struct {
   Character chars[20];
   int curr_id;
   int size;
} CharacterDB;

void* create_character(Character *c, CharacterDB *c_db);
void* select_character_by_id(int id, CharacterDB *c_db);
void* print_character(Character c);
void* put_item_in_bag(Item i);
void* equip_item(int id, Character *c, int hand);
void* switch_items(int id_item_to_equip, Character *c, int hand);
Item* get_equipment_slot(Character *c, int type_item, int hand);
void print_races();
void print_classes();
#endif