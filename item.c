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
   Item transaction = {0};

   transaction.id = i_db->curr_id;
   i_db->curr_id += 1;

   printf("Digite o nome para o item: \n");
   scanf(" %[^\n]", transaction.name);
   
   int opt = 0;
   printf("Escolha o tipo para o item: \n");
   print_item_types();
   scanf("%d", &opt);
   if(opt > 10 || opt < 0) return INVALID_ENTRY;
   transaction.type_item = (TypeItem) opt;
   opt = 0;
   
   printf("Digite o espaco ocupado pelo item: \n");
   scanf("%d", &opt);
   if(opt > 50 || opt < 0) return INVALID_ENTRY;
   transaction.occupied_space = opt;
   opt = 0;

   printf("Digite o bonus de ataque do item: \n");
   scanf("%d", &opt); // O scanf vem PRIMEIRO
   if(opt < 0) return INVALID_ENTRY; // Depois valida
   transaction.attack_bonus = opt;

   printf("Digite o bonus de defesa do item: \n");
   scanf("%d", &opt);
   if(opt < 0) return INVALID_ENTRY;
   transaction.defence_bonus = opt;

   printf("Digite o bonus de vida do item: \n");
   scanf("%d", &opt);
   if(opt < 0) return INVALID_ENTRY;
   transaction.life_bonus = opt;

   printf("Digite o bonus de iniciativa do item: \n");
   scanf("%d", &opt);
   if(opt < 0) return INVALID_ENTRY;
   transaction.iniciative_bonus = opt;

   printf("Digite o bonus de poder do item: \n");
   scanf("%d", &opt);
   if(opt < 0) return INVALID_ENTRY;
   transaction.power_bonus = opt;

   memcpy(it, &transaction, sizeof(Item));

   i_db->itens[it->id] = *it;
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