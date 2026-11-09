#include "basic_imports.h"
#include "definitions.h"
#include "adventure.h"
#include "item.h"
#include "error.h"


int main(void) {
   
   // CharacterDB c_db = {0};
   // Character c = {0};
   
   // if(create_character(&c, &c_db) != OKAY){
   //    log_error(INVALID_ENTRY);
   // }

   // if(print_character(c) != OKAY){
   //    log_error(INEXISTENT_OBJECT);
   // }

   Item it = {0};

   if(create_item(&it) != OKAY) { log_error(INVALID_ENTRY); }

   print_item(it);
   return 0;
}