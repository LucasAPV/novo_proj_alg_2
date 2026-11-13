#include "basic_imports.h"
#include "adventure.h"
#include "error.h"
#include "definitions.h"

void print_races(){
   printf("%d - raça HUMAN\n", HUMAN);
   printf("%d - raça ELF\n", ELF);
   printf("%d - raça DWARF\n", DWARF );
   printf("%d - raça HALFLING\n", HALFLING);
}

void print_classes(){
   printf("%d - classe WARRIOR\n", WARRIOR);
   printf("%d - classe ROUGUE\n", ROUGUE);
   printf("%d - classe MAGE\n", MAGE);
   printf("%d - classe CLERIC\n", CLERIC);
}

void* create_character(CharacterDB *c_db) {
   Character transaction = {0};
   transaction.id = c_db->curr_id;
   
   printf("Digite o nome para o personagem: \n");
   scanf(" %[^\n]", transaction.name);
   
   printf("Selecione a raça do personagem: \n");
   int opt;
   print_races();
   scanf("%d", &opt);
   transaction.race = (Race) opt;

   opt = 0;
   printf("Selecione a classe do personagem: \n");
   print_classes();
   scanf("%d", &opt);
   transaction.class = (Class) opt;

   transaction.level = 1;

   printf("Digite o maximo de HP do personagem: \n");
   opt = 0;
   scanf("%d", &opt);

   if (opt < 1 || opt > 999) return NULL;
   
   transaction.max_life_points = opt;
   transaction.current_life_points = transaction.max_life_points;
   

   printf("Digite o ataque do personagem: \n");
   scanf("%d", &opt);
   if (opt < 1 || opt > 30) return NULL;
   transaction.attack = opt;
   
   printf("Digite a defesa do personagem: \n");
   scanf("%d", &opt);
   if (opt < 1 || opt > 30) return NULL;
   transaction.defence = opt;

   printf("Digite a iniciativa do personagem: \n");
   scanf("%d", &opt);
   if (opt < -5 || opt > 20) return NULL;
   transaction.iniciative = opt;

   printf("Digite o poder do personagem: \n");
   scanf("%d", &opt);
   if (opt < 1 || opt > 100) return NULL;
   transaction.power = opt;

   transaction.inventory.curr_ocupied_space = 0; 
   transaction.inventory.free_index = 0;
   c_db->chars[transaction.id] = transaction;
   c_db->curr_id += 1;

   return OKAY;
}

void* select_character_by_id(int id, CharacterDB *c_db){
   if (id < 0 || id >= 20 || c_db->chars[id].level == 0) {
      return NULL;
   }
   return &c_db->chars[id];
   
}

void* print_character(int id, CharacterDB c_db) {
   Character c = c_db.chars[id];
   if (c.id < 0 || c.id >= 20 || c.level == 0) {
      return NULL;
   }

   printf("ID: %d\n"              , c.id);
   printf("NAME: %s\n"            , c.name);
   printf("RACE: %d\n"            , c.race );
   printf("CLASS: %d\n"           , c.class);
   printf("LEVEL: %d\n"           , c.level);
   printf("MAX_LIFE_POINTS: %d\n" , c.max_life_points);
   printf("CURR_LIFE_POINTS: %d\n", c.current_life_points);
   printf("ATTACK: %d\n"          , c.attack);
   printf("DEFENCE: %d\n"         , c.defence);
   printf("INICIATIVE: %d\n"      , c.iniciative);
   printf("POWER: %d\n"           , c.power);

   return OKAY;
}

void* print_inventory(int id, CharacterDB c_db){
   Character c = c_db.chars[id];
   if (c.id < 0 || c.id >= 20 || c.level == 0) {
      return NULL;
   }

   for(int i = 0; i < MAX_CAPACITY; ++i){
      if(strcmp(c.inventory.itens[i].name, "") == 0) continue;
      print_item(c.inventory.itens[i]);
   }

   return OKAY;
}

/*
   [TODO]
   title="FUNCAO DE EQUIPAR E TROCAR"
   desc="TERMINAR A FUNCAO DE EQUIPAR OS ITENS. SE O ITEM JA ESTIVER EQUIPADO, TROCAR ELE"
   relevance=Importante
   [\TODO]
*/

/*
   [TODO]
   title="FUNCAO DE TROCAR"
   desc="TROCA O ITEM ATIVO PARA UM DA BAG"
   relevance=Importante
   [\TODO]
*/

void* put_in_bag(Character *c, Item it){
   if(c->inventory.curr_ocupied_space > MAX_CAPACITY && it.occupied_space + c->inventory.curr_ocupied_space > MAX_CAPACITY) return NULL;

   c->inventory.itens[c->inventory.free_index] = it;
   c->inventory.free_index += 1;
   
   return OKAY;
}

Item* get_equipment_slot(Character *c, int type_item, int hand) {
   switch (type_item) {
      case HELMET:     return &(c->items_in_body.head);
      case CHESTPLATE: return &(c->items_in_body.cheast);
      case GLOVES:     return &(c->items_in_body.gloves);
      case LEGS:       return &(c->items_in_body.legs);
      case GREAVES:    return &(c->items_in_body.greaves);
      case RING:       return &(c->items_in_body.ring);
      case NECKLACE:   return &(c->items_in_body.necklace);
      case BELT:       return &(c->items_in_body.belt);
      
      case ONE_HAND_SWORD:
      case SHIELD:
         return (hand == 0) ? &(c->items_in_body.left_hand)  : 
               ((hand == 1) ? &(c->items_in_body.right_hand) : NULL) ;
         
      case TWO_HAND_SWORD:
         return &(c->items_in_body.left_hand); // O slot principal para armas de duas mãos
         
      default: 
         return NULL;
   }
}

// Função para trocar o item do inventário com o item equipado
void* switch_items(int id_item_to_equip, Character *c, int hand) {
   if (id_item_to_equip < 0 || id_item_to_equip >= MAX_CAPACITY) {
      return ERROR;
   }

   Item item_to_equip = c->inventory.itens[id_item_to_equip];
   Item *target_slot = get_equipment_slot(c, item_to_equip.type_item, hand);

   if (!target_slot) {
      return ERROR; // Tipo de item inválido
   }

   // Caso especial para espada de duas mãos (ocupa as duas mãos)
   if (item_to_equip.type_item == TWO_HAND_SWORD) {
      // Aqui você pode decidir o que fazer com o item da mão direita se houver
      c->items_in_body.right_hand = (Item){0}; // ou armazenar de alguma forma
   }

   // Troca direta usando ponteiros (sem repetir código para cada parte do corpo)
   Item temp = *target_slot;
   *target_slot = item_to_equip;
   c->inventory.itens[id_item_to_equip] = temp;

   return OKAY;
}

// Função para equipar o item
void* equip_item(int id_item, Character *c, int hand) {
   if (id_item < 0 || id_item >= MAX_CAPACITY) {
      return NULL;
   }

   Item i = c->inventory.itens[id_item];
   print_item(i);
   Item *target_slot = get_equipment_slot(c, i.type_item, hand);

   if (!target_slot) {
      return NULL;
   }

    // Se o slot já estiver ocupado (assumindo type_item != 0 como ocupado), fazemos a troca
   if (target_slot->type_item != 0) {
      if (switch_items(id_item, c, hand) != OKAY) {
            log_error(UNABLE_TO_SWITCH_ITENS);
            return NULL;
      }
   } else {
        // Slot vazio: apenas movemos para o corpo e removemos do inventário
        *target_slot = i;
        c->inventory.itens[id_item] = (Item){0}; // Limpa o slot do inventário (ou marca como vazio)
   }

   return OKAY;
}