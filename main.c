#include <stdio.h>
#define NAME_LENGHT 49
#define MAX_CHARACTERS 20

typedef enum {
   INVALID_ENTRY
} Error;


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
} Character;

typedef struct {
   Character chars[20];
   int curr_id;
} CharacterDB;

void create_character(Character *c, CharacterDB *c_db);
void select_character_by_id(int id, CharacterDB c_db, Character *c);
void print_races();
void print_classes();

int main(void) {
   return 0;
}

void print_races(){
   printf("%d - raça HUMAN\n", HUMAN);
   printf("%d - raça ELF\n", ELF);
   printf("%d - raça DWARF\n", DWARF );
   printf("%d - raça HALFLING\n", HALFLING);
}

void print_classes(){
   printf("%d - classe WARRIOR\n", WARRIOR)
   printf("%d - classe ROUGUE\n", ROUGUE) 
   printf("%d - classe MAGE\n", MAGE)
   printf("%d - classe CLERIC\n", CLERIC)
}

void* create_character(Character *c, CharacterDB *c_db) {
   c->id = c_db->curr_id;
   c_db->curr_id += 1;

   printf("Digite o nome para o personagem: \n");
   fgets(c->name, sizeof(c->name), stdin);
   
   printf("Selecione a raça do personagem: \n");
   int opt;
   print_races();
   scanf("%d", &opt);
   c->race = (Race) opt;

   opt = 0;
   printf("Selecione a classe do personagem: \n");
   print_classes();
   scanf("%d", &opt);
   c->class = (Race) opt;

   c->level = 1;

   printf("Digite o maximo de HP do personagem: \n");
   opt = 0;
   scanf("%d", &opt);

   if (opt < 1 || opt > 999) return (void*) INVALID_ENTRY;
   
   c->max_life_points = opt;
   c->current_life_points = c->max_life_points;
   

   printf("Digite o ataque do personagem: \n");
   scanf("%d", &opt);
   if (opt < 1 || opt > 30) return (void*) INVALID_ENTRY;
   c->attack = opt;
   
   printf("Digite a defesa do personagem: \n");
   scanf("%d", &opt);
   if (opt < 1 || opt > 30) return (void*) INVALID_ENTRY;
   c->defence = opt;

   printf("Digite a iniciativa do personagem: \n");
   scanf("%d", &opt);
   if (opt < -5 || opt > 20) return (void*) INVALID_ENTRY;
   c->iniciative = opt;

   printf("Digite o poder do personagem: \n");
   scanf("%d", &opt);
   if (opt < 1 || opt > 100) return (void*) INVALID_ENTRY;
   c->power = opt;
}

