#include "basic_imports.h"
#include "definitions.h"
#include "adventure.h"
#include "item.h"
#include "error.h"
#include "interface.h"

void actions_for_characters(CharacterDB *c_db, ItemDB *i_db) {
   while(1){
      int opt = character_area();
      switch(opt) {
         case 0:
            return; // Para sair do menu interno
         break;
         case 1:
            create_character(c_db);
         break;

         case 2:
            int id = 0;
            printf("Digite o id do personagem que deseja ver: \n");
            scanf("%d", &id);
            printf("\n");
            print_character(id, *c_db);
         break;
   }
}
}

int main(void) {
   CharacterDB c_db = {0};
   ItemDB i_db = {0};

   while (1) {
      int option = create_initial_menu();

      switch(option) {
         case 0:
            printf("SAINDO...\n");
            exit(0);
         break;

         case 1:
            actions_for_characters(&c_db, &i_db);
         break;
      }
   }
   return 0;
}