#include "basic_imports.h"
#include "item.h"
#include "definitions.h"
#include "error.h"

void print_item_types() {
   printf("%d - HELMET \n"        , HELMET);
   printf("%d - CHESTPLATE \n"    , CHESTPLATE);
   printf("%d - GLOVES \n"        , GLOVES);
   printf("%d - LEGS \n"          , LEGS);
   printf("%d - GREAVES \n"       , GREAVES);
   printf("%d - RING \n"          , RING);
   printf("%d - NECKLACE \n"      , NECKLACE);
   printf("%d - BELT \n"          , BELT);
   printf("%d - ONE_HAND_SWORD \n", ONE_HAND_SWORD);
   printf("%d - TWO_HAND_SWORD \n", TWO_HAND_SWORD);
   printf("%d - SHIELD \n"        , SHIELD);
}

void* create_item(Item *it, ItemDB *i_db) {
   if (i_db->curr_id >= MAX_CAPACITY) {
      log_error(NOT_ENOUGH_SPACE);
      return ERROR;
   }

   Item transaction = {0};
   transaction.id = i_db->curr_id;

   printf("Digite o nome para o item: \n");
   scanf(" %48[^\n]", transaction.name);
   
   int opt = 0;
   printf("Escolha o tipo para o item: \n");
   print_item_types();
   scanf("%d", &opt);
   if(opt > 10 || opt < 0) {log_error(INVALID_ENTRY); return ERROR; };
   transaction.type_item = (TypeItem) opt;
   opt = 0;
   
   printf("Digite o espaco ocupado pelo item: \n");
   scanf("%d", &opt);
   if(opt > 50 || opt < 0) {log_error(INVALID_ENTRY); return ERROR; };
   transaction.occupied_space = opt;
   opt = 0;

   printf("Digite o bonus de ataque do item: \n");
   scanf("%d", &opt); // O scanf vem PRIMEIRO
   if(opt < 0) {log_error(INVALID_ENTRY); return ERROR; }; // Depois valida
   transaction.attack_bonus = opt;

   printf("Digite o bonus de defesa do item: \n");
   scanf("%d", &opt);
   if(opt < 0) {log_error(INVALID_ENTRY); return ERROR; };
   transaction.defence_bonus = opt;

   printf("Digite o bonus de vida do item: \n");
   scanf("%d", &opt);
   if(opt < 0) {log_error(INVALID_ENTRY); return ERROR; };
   transaction.life_bonus = opt;

   printf("Digite o bonus de iniciativa do item: \n");
   scanf("%d", &opt);
   if(opt < 0) {log_error(INVALID_ENTRY); return ERROR; };
   transaction.iniciative_bonus = opt;

   printf("Digite o bonus de poder do item: \n");
   scanf("%d", &opt);
   if(opt < 0) {log_error(INVALID_ENTRY); return ERROR; };
   transaction.power_bonus = opt;

   memcpy(it, &transaction, sizeof(Item));

   i_db->itens[it->id] = *it;
   i_db->curr_id += 1;
   return OKAY;
}

void print_item(Item it) {
   printf("ID: %d \n"              , it.id);
   printf("NAME: %s \n"            , it.name);
   printf("TYPE_ITEM: %d \n"       , it.type_item);
   printf("OCCUPIED_SPACE: %d \n"  , it.occupied_space);
   printf("ATTACK_BONUS: %d \n"    , it.attack_bonus);
   printf("DEFENCE_BONUS: %d \n"   , it.defence_bonus);
   printf("LIFE_BONUS: %d \n"      , it.life_bonus);
   printf("INICIATIVE_BONUS: %d \n", it.iniciative_bonus);
   printf("POWER_BONUS: %d \n"     , it.power_bonus);
}

// Listar todos os itens cadastrados no banco de itens
void print_all_items(ItemDB i_db) {
   if (i_db.curr_id == 0) {
      printf("\nNenhum item cadastrado no banco.\n");
      return;
   }

   printf("\n=== LISTA DE ITENS CADASTRADOS ===\n");
   for (int i = 0; i < i_db.curr_id; ++i) {
      if (strlen(i_db.itens[i].name) > 0) {
         printf("[ID: %d] Nome: %s | Tipo: %d | Espaco: %d | Atk: +%d | Def: +%d | HP: +%d | Ini: +%d | Pow: +%d\n",
                i_db.itens[i].id,
                i_db.itens[i].name,
                i_db.itens[i].type_item,
                i_db.itens[i].occupied_space,
                i_db.itens[i].attack_bonus,
                i_db.itens[i].defence_bonus,
                i_db.itens[i].life_bonus,
                i_db.itens[i].iniciative_bonus,
                i_db.itens[i].power_bonus);
      }
   }
}

// Alterar os dados de um item existente no banco
void* update_item(int id, ItemDB *i_db) {
   if (id < 0 || id >= i_db->curr_id || strlen(i_db->itens[id].name) == 0) {
      log_error(INEXISTENT_OBJECT);
      return NULL;
   }

   Item temp = i_db->itens[id];

   printf("\n=== ALTERAR ITEM ID %d ===\n", id);
   printf("Digite o novo nome para o item (Atual: %s): \n", temp.name);
   scanf(" %48[^\n]", temp.name);

   int opt = 0;
   printf("Escolha o novo tipo para o item: \n");
   print_item_types();
   scanf("%d", &opt);
   if (opt < 0 || opt > 10) {log_error(INVALID_ENTRY); return ERROR; };
   temp.type_item = (TypeItem) opt;

   printf("Digite o novo espaco ocupado pelo item (0-50): \n");
   scanf("%d", &opt);
   if (opt < 0 || opt > 50) {log_error(INVALID_ENTRY); return ERROR; };
   temp.occupied_space = opt;

   printf("Digite o novo bonus de ataque: \n");
   scanf("%d", &opt);
   if (opt < 0) {log_error(INVALID_ENTRY); return ERROR; };
   temp.attack_bonus = opt;

   printf("Digite o novo bonus de defesa: \n");
   scanf("%d", &opt);
   if (opt < 0) {log_error(INVALID_ENTRY); return ERROR; };
   temp.defence_bonus = opt;

   printf("Digite o novo bonus de vida: \n");
   scanf("%d", &opt);
   if (opt < 0) {log_error(INVALID_ENTRY); return ERROR; };
   temp.life_bonus = opt;

   printf("Digite o novo bonus de iniciativa: \n");
   scanf("%d", &opt);
   if (opt < 0) {log_error(INVALID_ENTRY); return ERROR; };
   temp.iniciative_bonus = opt;

   printf("Digite o novo bonus de poder: \n");
   scanf("%d", &opt);
   if (opt < 0) {log_error(INVALID_ENTRY); return ERROR; };
   temp.power_bonus = opt;

   i_db->itens[id] = temp;
   printf("Item atualizado com sucesso!\n");
   return OKAY;
}

// Remover um item do banco de itens mantendo o vetor contíguo
void* remove_item_from_db(int id, ItemDB *i_db) {
   if (id < 0 || id >= i_db->curr_id || strlen(i_db->itens[id].name) == 0) {
      log_error(INEXISTENT_OBJECT);
      return NULL;
   }

   for (int i = id; i < i_db->curr_id - 1; ++i) {
      i_db->itens[i] = i_db->itens[i + 1];
      i_db->itens[i].id = i;
   }

   i_db->itens[i_db->curr_id - 1] = (Item){0};
   i_db->curr_id -= 1;

   printf("Item removido do banco com sucesso!\n");
   return OKAY;
}