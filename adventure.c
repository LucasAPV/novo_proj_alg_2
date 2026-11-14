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
   if(c_db->curr_id >= MAX_CHARACTERS){
      log_error(NOT_ENOUGH_SPACE);
      return NULL;
   }

   Character transaction = {0};
   transaction.id = c_db->curr_id;
   
   printf("Digite o nome para o personagem: \n");
   scanf(" %48[^\n]", transaction.name);
   
   printf("Selecione a raça do personagem: \n");
   int opt = 0;
   print_races();
   scanf("%d", &opt);

   if(opt < HUMAN || opt >= END_RACE){
      log_error(INVALID_ENTRY);
      return NULL;
   }

   transaction.race = (Race) opt;

   opt = 0;
   printf("Selecione a classe do personagem: \n");
   print_classes();
   scanf("%d", &opt);

   if(opt < WARRIOR || opt >= END_CLASS){
      log_error(INVALID_ENTRY);
      return NULL;
   }

   transaction.class = (Class) opt;

   transaction.level = 1;

   printf("Digite o maximo de HP do personagem: \n");
   opt = 0;
   scanf("%d", &opt);

   if (opt < 1 || opt > 999) { log_error(INVALID_ENTRY); return NULL; }
   
   transaction.max_life_points = opt;
   transaction.current_life_points = transaction.max_life_points;
   

   printf("Digite o ataque do personagem: \n");
   scanf("%d", &opt);
   if (opt < 1 || opt > 30) { log_error(INVALID_ENTRY); return NULL; }
   transaction.attack = opt;
   
   printf("Digite a defesa do personagem: \n");
   scanf("%d", &opt);
   if (opt < 1 || opt > 30) { log_error(INVALID_ENTRY); return NULL; }
   transaction.defence = opt;

   printf("Digite a iniciativa do personagem: \n");
   scanf("%d", &opt);
   if (opt < -5 || opt > 20) { log_error(INVALID_ENTRY); return NULL; }
   transaction.iniciative = opt;

   printf("Digite o poder do personagem: \n");
   scanf("%d", &opt);
   if (opt < 1 || opt > 100) { log_error(INVALID_ENTRY); return NULL; }
   transaction.power = opt;

   transaction.inventory.curr_ocupied_space = 0; 
   transaction.inventory.free_index = 0;
   c_db->chars[transaction.id] = transaction;
   c_db->curr_id += 1;

   return OKAY;
}

void* select_character_by_id(int id, CharacterDB *c_db){
   if (id < 0 || id >= MAX_CHARACTERS || c_db->chars[id].level == 0) {
      return NULL;
   }
   return &c_db->chars[id];
   
}

void* print_character(int id, CharacterDB c_db) {
   if (id < 0 || id >= MAX_CHARACTERS || c_db.chars[id].level == 0) {
      return NULL;
   }

   Character c = c_db.chars[id];

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
   if (id < 0 || id >= MAX_CHARACTERS || c_db.chars[id].level == 0) {
      return NULL;
   }

   Character c = c_db.chars[id];

   if (c.inventory.free_index == 0) {
      printf("\nInventario vazio.\n");
      return OKAY;
   }

   printf("\n=== INVENTARIO DE %s ===\n", c.name);
   for(int i = 0; i < c.inventory.free_index; ++i){
      printf("[Indice %d] ", i);
      print_item(c.inventory.itens[i]);
      printf("-------------------\n");
   }

   return OKAY;
}

int get_inventory_occupation(const Character *c){
   int space_used = 0;
   for(int i = 0; i < c->inventory.free_index; ++i){
      space_used += c->inventory.itens[i].occupied_space;
   }

   return space_used;
}

void* put_in_bag(Character *c, Item it) {
   int current_occupation = get_inventory_occupation(c);

   if (current_occupation + it.occupied_space > MAX_CAPACITY || c->inventory.free_index >= MAX_CAPACITY) {
      log_error(NOT_ENOUGH_SPACE);
      return ERROR;
   }

   c->inventory.itens[c->inventory.free_index] = it;
   c->inventory.free_index += 1;

   return OKAY;
}

void remove_item_from_inventory(Character *c, int index) {
   if (index < 0 || index >= c->inventory.free_index) return;

   for (int i = index; i < c->inventory.free_index - 1; ++i) {
      c->inventory.itens[i] = c->inventory.itens[i + 1];
   }

   c->inventory.itens[c->inventory.free_index - 1] = (Item){0};
   c->inventory.free_index -= 1;
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
   if (id_item_to_equip < 0 || id_item_to_equip >= c->inventory.free_index) {
      return ERROR;
   }

   Item item_to_equip = c->inventory.itens[id_item_to_equip];
   Item *target_slot = get_equipment_slot(c, item_to_equip.type_item, hand);

   if (!target_slot) {
      return ERROR;
   }

   // Caso especial para espada de duas mãos (trata a mão direita sem perder o item)
   if (item_to_equip.type_item == TWO_HAND_SWORD) {
      if (strlen(c->items_in_body.right_hand.name) > 0) {
         if (put_in_bag(c, c->items_in_body.right_hand) == ERROR) {
            printf("Sem espaco na mochila para desequipar o item da mao direita!\n");
            return ERROR;
         }
         c->items_in_body.right_hand = (Item){0};
      }
   }

   Item temp = *target_slot;
   *target_slot = item_to_equip;
   c->inventory.itens[id_item_to_equip] = temp;

   return OKAY;
}

// Função para equipar o item
void* equip_item(int id_item, Character *c, int hand) {
   if (id_item < 0 || id_item >= c->inventory.free_index) {
      return ERROR;
   }

   Item item_to_equip = c->inventory.itens[id_item];

   if (item_to_equip.type_item == TWO_HAND_SWORD) {
      if (strlen(c->items_in_body.right_hand.name) > 0) {
         if (put_in_bag(c, c->items_in_body.right_hand) == ERROR) {
            printf("Sem espaco na mochila para desequipar o item da mao direita!\n");
            return ERROR;
         }
         c->items_in_body.right_hand = (Item){0};
      }
   }

   if (hand == 1 && c->items_in_body.left_hand.type_item == TWO_HAND_SWORD) {
      if (put_in_bag(c, c->items_in_body.left_hand) == ERROR) {
         printf("Sem espaco na mochila para desequipar a arma de duas maos da mao esquerda!\n");
         return ERROR;
      }
      c->items_in_body.left_hand = (Item){0};
   }

   Item *target_slot = get_equipment_slot(c, item_to_equip.type_item, hand);
   if (!target_slot) return ERROR;

   if (strlen(target_slot->name) > 0) {
      return switch_items(id_item, c, hand);
   }

   *target_slot = item_to_equip;
   remove_item_from_inventory(c, id_item);

   return OKAY;
}

// Consultar equipamentos no corpo do personagem
void print_equipped_items(Character c) {
   printf("\n=== EQUIPAMENTOS DE %s ===\n", c.name);
   printf("Elmo: %s\n", strlen(c.items_in_body.head.name) > 0 ? c.items_in_body.head.name : "Vazio");
   printf("Peitoral: %s\n", strlen(c.items_in_body.cheast.name) > 0 ? c.items_in_body.cheast.name : "Vazio");
   printf("Manoplas: %s\n", strlen(c.items_in_body.gloves.name) > 0 ? c.items_in_body.gloves.name : "Vazio");
   printf("Calça: %s\n", strlen(c.items_in_body.legs.name) > 0 ? c.items_in_body.legs.name : "Vazio");
   printf("Grevas: %s\n", strlen(c.items_in_body.greaves.name) > 0 ? c.items_in_body.greaves.name : "Vazio");
   printf("Anel: %s\n", strlen(c.items_in_body.ring.name) > 0 ? c.items_in_body.ring.name : "Vazio");
   printf("Colar: %s\n", strlen(c.items_in_body.necklace.name) > 0 ? c.items_in_body.necklace.name : "Vazio");
   printf("Cinto: %s\n", strlen(c.items_in_body.belt.name) > 0 ? c.items_in_body.belt.name : "Vazio");
   printf("Mão Esquerda: %s\n", strlen(c.items_in_body.left_hand.name) > 0 ? c.items_in_body.left_hand.name : "Vazio");
   printf("Mão Direita: %s\n", strlen(c.items_in_body.right_hand.name) > 0 ? c.items_in_body.right_hand.name : "Vazio");
}

// Desequipar item do corpo e devolver à mochila
void* unequip_item(Character *c, int type_item, int hand) {
   Item *target_slot = get_equipment_slot(c, type_item, hand);

   if (target_slot == NULL || strlen(target_slot->name) == 0) {
      log_error(INEXISTENT_OBJECT);
      return ERROR;
   }

   if (put_in_bag(c, *target_slot) == OKAY) {
      *target_slot = (Item){0};
      return OKAY;
   }

   return ERROR;
}

// Calcular e exibir atributos totais (Base + Bónus dos itens equipados)
void calculate_total_attributes(Character c) {
   int total_atk = c.attack;
   int total_def = c.defence;
   int total_hp  = c.max_life_points;
   int total_ini = c.iniciative;
   int total_pow = c.power;

   Item slots[] = {
      c.items_in_body.head,
      c.items_in_body.cheast,
      c.items_in_body.gloves,
      c.items_in_body.legs,
      c.items_in_body.greaves,
      c.items_in_body.ring,
      c.items_in_body.necklace,
      c.items_in_body.belt,
      c.items_in_body.left_hand,
      c.items_in_body.right_hand
   };

   for (int i = 0; i < 10; ++i) {
      if (strlen(slots[i].name) > 0) {
         total_atk += slots[i].attack_bonus;
         total_def += slots[i].defence_bonus;
         total_hp  += slots[i].life_bonus;
         total_ini += slots[i].iniciative_bonus;
         total_pow += slots[i].power_bonus;
      }
   }

   printf("\n=== ATRIBUTOS TOTAIS DE %s ===\n", c.name);
   printf("Ataque Total: %d (Base: %d)\n", total_atk, c.attack);
   printf("Defesa Total: %d (Base: %d)\n", total_def, c.defence);
   printf("HP Máximo Total: %d (Base: %d)\n", total_hp, c.max_life_points);
   printf("Iniciativa Total: %d (Base: %d)\n", total_ini, c.iniciative);
   printf("Poder Total: %d (Base: %d)\n", total_pow, c.power);
}

// Listar todos os personagens cadastrados
void print_all_characters(CharacterDB c_db) {
   if (c_db.curr_id == 0) {
      printf("\nNenhum personagem cadastrado.\n");
      return;
   }

   printf("\n=== LISTA DE PERSONAGENS ===\n");
   for (int i = 0; i < c_db.curr_id; ++i) {
      if (c_db.chars[i].level > 0) {
         printf("ID: %d | Nome: %s | Raça: %d | Classe: %d | Nível: %d\n",
                c_db.chars[i].id,
                c_db.chars[i].name,
                c_db.chars[i].race,
                c_db.chars[i].class,
                c_db.chars[i].level);
      }
   }
}

// Alterar os dados de um personagem existente
void* update_character(int id, CharacterDB *c_db) {
   Character *c = (Character*) select_character_by_id(id, c_db);
   if (c == NULL) {
      log_error(INEXISTENT_OBJECT);
      return NULL;
   }

   Character temp = *c; // Cópia temporária para garantir atomicidade

   printf("\n=== ALTERAR PERSONAGEM ID %d ===\n", id);
   
   printf("Digite o novo nome para o personagem (Atual: %s): \n", temp.name);
   scanf(" %48[^\n]", temp.name);

   int opt = 0;
   printf("Selecione a nova raça do personagem: \n");
   print_races();
   scanf("%d", &opt);
   if (opt < HUMAN || opt >= END_RACE) {
      log_error(INVALID_ENTRY);
      return NULL;
   }
   temp.race = (Race) opt;

   printf("Selecione a nova classe do personagem: \n");
   print_classes();
   scanf("%d", &opt);
   if (opt < WARRIOR || opt >= END_CLASS) {
      log_error(INVALID_ENTRY);
      return NULL;
   }
   temp.class = (Class) opt;

   printf("Digite o novo HP Máximo (1-999): \n");
   scanf("%d", &opt);
   if (opt < 1 || opt > 999) { log_error(INVALID_ENTRY); return NULL; }
   temp.max_life_points = opt;
   temp.current_life_points = opt;

   printf("Digite o novo Ataque (1-30): \n");
   scanf("%d", &opt);
   if (opt < 1 || opt > 30) { log_error(INVALID_ENTRY); return NULL; }
   temp.attack = opt;

   printf("Digite a nova Defesa (1-30): \n");
   scanf("%d", &opt);
   if (opt < 1 || opt > 30) { log_error(INVALID_ENTRY); return NULL; }
   temp.defence = opt;

   printf("Digite a nova Iniciativa (-5 a 20): \n");
   scanf("%d", &opt);
   if (opt < -5 || opt > 20) { log_error(INVALID_ENTRY); return NULL; }
   temp.iniciative = opt;

   printf("Digite o novo Poder (1-100): \n");
   scanf("%d", &opt);
   if (opt < 1 || opt > 100) { log_error(INVALID_ENTRY); return NULL; }
   temp.power = opt;

   *c = temp; // Aplica as alterações apenas se tudo for válido

   printf("Personagem atualizado com sucesso!\n");
   return OKAY;
}

// Remover personagem do vetor e reorganizar os IDs
void* remove_character(int id, CharacterDB *c_db) {
   if (id < 0 || id >= c_db->curr_id || c_db->chars[id].level == 0) {
      log_error(INEXISTENT_OBJECT);
      return NULL;
   }

   for (int i = id; i < c_db->curr_id - 1; ++i) {
      c_db->chars[i] = c_db->chars[i + 1];
      c_db->chars[i].id = i; // Atualiza o ID para manter índice contíguo
   }

   c_db->chars[c_db->curr_id - 1] = (Character){0};
   c_db->curr_id -= 1;

   printf("Personagem removido com sucesso!\n");
   return OKAY;
}

// Descartar/destruir um item da mochila do personagem
void* drop_item_from_inventory(Character *c, int index) {
   if (c == NULL || index < 0 || index >= c->inventory.free_index) {
      log_error(INVALID_ARRAY_POSITION);
      return ERROR;
   }

   remove_item_from_inventory(c, index);
   printf("Item removido do inventario com sucesso!\n");
   return OKAY;
}