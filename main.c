#include "basic_imports.h"
#include "definitions.h"
#include "adventure.h"
#include "item.h"
#include "error.h"
#include "interface.h"

void actions_for_characters(CharacterDB *c_db) {
   while(1){
      int opt = character_area();
      switch(opt){
         case 0:
            return;

         case 1:
            create_character(c_db);
            break;

         case 2: {
            int id = 0;
            printf("Digite o ID do personagem que deseja ver: \n");
            scanf("%d", &id);
            printf("\n");
            if (print_character(id, *c_db) == NULL) {
               printf("Personagem nao encontrado!\n");
            }
            break;
         }

         case 3: {
            int id = 0;
            printf("Digite o ID do personagem que deseja alterar: \n");
            scanf("%d", &id);
            update_character(id, c_db);
            break;
         }

         case 4: {
            int id = 0;
            printf("Digite o ID do personagem que deseja remover: \n");
            scanf("%d", &id);
            remove_character(id, c_db);
            break;
         }

         case 5:
            print_all_characters(*c_db);
            break;

         case 6: { // Administrar inventário (submenu)
            int id_char = 0, sub_opt = 0;
            printf("Digite o ID do personagem: \n");
            scanf("%d", &id_char);
            Character *c = (Character*) select_character_by_id(id_char, c_db);
            if (c != NULL) {
               printf("\n--- ADMINISTRAR INVENTARIO (%s) ---\n", c->name);
               printf("1 - Listar itens da mochila\n");
               printf("2 - Descartar item da mochila\n");
               printf("Escolha uma opcao: ");
               scanf("%d", &sub_opt);

               if (sub_opt == 1) {
                  print_inventory(id_char, *c_db);
               } else if (sub_opt == 2) {
                  print_inventory(id_char, *c_db);
                  if (c->inventory.free_index > 0) {
                     int index_item = 0;
                     printf("Digite o indice do item a descartar da mochila: \n");
                     scanf("%d", &index_item);
                     drop_item_from_inventory(c, index_item);
                  }
               } else {
                  printf("Opcao invalida!\n");
               }
            } else {
               printf("Personagem invalido!\n");
            }
            break;
         }

         case 7: {
            int id = 0;
            printf("Digite o ID do personagem: \n");
            scanf("%d", &id);
            Character *c = (Character*) select_character_by_id(id, c_db);
            if (c != NULL) {
               print_equipped_items(*c);
            } else {
               printf("Personagem invalido!\n");
            }
            break;
         }

         case 8: { // Equipar item (Troca automática integrada na equip_item)
            int id_char = 0, id_item = 0, hand = 0;
            printf("Digite o ID do personagem: \n");
            scanf("%d", &id_char);
            
            printf("Digite o indice do item no inventario: \n");
            scanf("%d", &id_item);
            
            printf("Se for arma/escudo, qual mao? (0-Esquerda, 1-Direita, 2-Nenhuma): \n");
            scanf("%d", &hand);

            Character *c = (Character*) select_character_by_id(id_char, c_db);
            
            if (c != NULL) {
               if (equip_item(id_item, c, hand) == OKAY) {
                  printf("Item equipado (ou trocado) com sucesso!\n");
               } else {
                  printf("Falha ao equipar o item.\n");
               }
            } else {
               printf("Personagem invalido ou nao encontrado!\n");
            }
            break;
         }

         case 9: {
            int id_char = 0, type_item = 0, hand = 0;
            printf("Digite o ID do personagem: \n");
            scanf("%d", &id_char);
            
            printf("Escolha o tipo do item a desequipar:\n");
            print_item_types();
            scanf("%d", &type_item);
            
            printf("Se for Arma 1 Mao ou Escudo, qual mao? (0-Esquerda, 1-Direita, 2-Nenhuma): \n");
            scanf("%d", &hand);

            Character *c = (Character*) select_character_by_id(id_char, c_db);
            if (c != NULL) {
               if (unequip_item(c, type_item, hand) == OKAY) {
                  printf("Item desequipado e retornado ao inventario!\n");
               } else {
                  printf("Falha ao desequipar (Slot vazio ou inventario cheio).\n");
               }
            } else {
               printf("Personagem invalido!\n");
            }
            break;
         }

         case 10: {
            int id = 0;
            printf("Digite o ID do personagem: \n");
            scanf("%d", &id);
            Character *c = (Character*) select_character_by_id(id, c_db);
            if (c != NULL) {
               calculate_total_attributes(*c);
            } else {
               printf("Personagem invalido!\n");
            }
            break;
         }

         default:
            printf("Opcao invalida!\n");
            break;
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

         case 4:
            print_all_items(*i_db);
            break;

         case 5: {
            int id = 0;
            printf("Digite o ID do item que deseja alterar: \n");
            scanf("%d", &id);
            update_item(id, i_db);
            break;
         }

         case 6: {
            int id = 0;
            printf("Digite o ID do item que deseja remover do banco: \n");
            scanf("%d", &id);
            remove_item_from_db(id, i_db);
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