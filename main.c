#include "basic_imports.h"
#include "definitions.h"
#include "adventure.h"
#include "item.h"
#include "error.h"
#include "interface.h"

void actions_for_characters(CharacterDB *c_db) { //Removi o parametro ItemDB *i_db, pois ativava o -Wall e -Wextra ao compilar, já que o parametro não era utilizado
   while(1){
      int opt = character_area();
      switch(opt){
         case 0:
            return; // Para sair do menu interno
         break;

         case 1:
            create_character(c_db);
         break;

         case 2: {
            int id = 0;
            printf("Digite o id do personagem que deseja ver: \n");
            scanf("%d", &id);
            printf("\n");
            print_character(id, *c_db);
            break;
         }

         case 3: {
            int id = 0;
            printf("Digite o ID do personagem para ver o inventario: \n");
            scanf("%d", &id);
            printf("\n");
            print_inventory(id, *c_db);
            break;
         }

            case 4:
            case 5: {
               int id_char = 0, id_item = 0, hand = 0;
               printf("Digite o ID do personagem: \n");
               scanf("%d", &id_char);
               
               printf("Digite o ID (indice) do item no inventario: \n");
               scanf("%d", &id_item);
               
               printf("Se for arma/escudo, qual mao? (0-Esquerda, 1-Direita, 2-Nenhuma): \n");
               scanf("%d", &hand);

               Character *c = (Character*) select_character_by_id(id_char, c_db);
               
               if (c != NULL) {
               if (opt == 4) {
                   if (equip_item(id_item, c, hand) == OKAY) {
                       printf("Item equipado com sucesso!\n");
                   } else {
                       printf("Falha ao equipar o item.\n");
                   }
               } else {
                   if (switch_items(id_item, c, hand) == OKAY) {
                       printf("Item trocado com sucesso!\n");
                   } else {
                       printf("Falha ao trocar o item.\n");
                   }
               }
            } else {
               printf("Personagem invalido ou nao encontrado!\n");
            }
            break;
         }
      }
   }
}

void actions_for_items(CharacterDB *c_db, ItemDB *i_db){
   while(1){
      int opt = item_area();
      switch (opt){
         case 0:
            return;
            
         case 1: {
            Item novo_item;
            create_item(&novo_item, i_db);
            break;
         }
            
         case 2: {
            int id = 0;
            printf("Digite o ID do item que deseja ver: \n");
            scanf("%d", &id);
            printf("\n");
            
            if (id >= 0 && id < i_db->curr_id) {
                print_item(i_db->itens[id]);
            } else {
                printf("ID de item invalido!\n");
            }
            break;
         }
            
         case 3: {
            int id_char = 0, id_item = 0;
            printf("Digite o ID do personagem: \n");
            scanf("%d", &id_char);
            
            printf("Digite o ID do item criado no banco de itens: \n");
            scanf("%d", &id_item);
            
            Character *c = (Character*) select_character_by_id(id_char, c_db);
            
            if (c != NULL && id_item >= 0 && id_item < i_db->curr_id) {
                Item item_para_dar = i_db->itens[id_item];
                if (put_in_bag(c, item_para_dar) == OKAY) {
                    printf("Item adicionado ao inventario com sucesso!\n");
                } else {
                    printf("Falha: Inventario cheio ou sem espaco.\n");
                }
            } else {
                printf("Personagem ou Item nao encontrado.\n");
            }
            break;
         }
            
         default:
            printf("Opcao invalida!\n");
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
            actions_for_characters(&c_db); //&i_db removido para não ativar o -Wall e -Wextra na compilação por falta de uso
         break;

         case 2:
            actions_for_items(&c_db, &i_db);
         break;
         
         default:
            printf("Opcao invalida!\n");
         break;
      }
   }
   return 0;
}