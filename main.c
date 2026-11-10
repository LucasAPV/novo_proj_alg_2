#include "basic_imports.h"
#include "definitions.h"
#include "adventure.h"
#include "item.h"
#include "error.h"


int main(void) {
   
   CharacterDB c_db = {0};
   Character c = {0};

   ItemDB i_db = {0};
   Item it1 = {0}; 
   Item it2 = {0};   

   if(create_item(&it1, &i_db) != OKAY) log_error(INVALID_ENTRY);
   if(create_item(&it2, &i_db) != OKAY) log_error(INVALID_ENTRY);
   
   if(create_character(&c, &c_db) != OKAY) log_error(INVALID_ENTRY);

   if(put_in_bag(&c, it2) != OKAY)  log_error(INVALID_ENTRY);
   if(put_in_bag(&c, it2) != OKAY)  log_error(NOT_ENOUGH_SPACE);

   if(equip_item(0, &c, 0) != OKAY) log_error(INVALID_ARRAY_POSITION);
   if(equip_item(1, &c, 0) != OKAY) log_error(INVALID_ARRAY_POSITION);
   if (print_inventory(c) != OKAY)  log_error(INVALID_ARRAY_POSITION);
   return 0;
}