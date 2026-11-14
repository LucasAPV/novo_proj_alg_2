#include "interface.h"

void create_title (char* title) {
   printf("\n\n ====== %s ====== \n\n", title);
}


void create_options(char options[][TEXT_SIZE], int num_options) {
   for(int i = 0; i < num_options; ++i){
      printf("%d - %s\n", i, options[i]);
   }
}

int create_initial_menu() {
   printf("Escolha uma das opcoes:\n");
   printf("1- Acessar area dos Personagens\n");
   printf("2- Acessar area dos Itens\n");
   printf("0- Sair do programa\n");

   int opt = 0;
   scanf("%d", &opt);

   return opt;
}

int character_area() {
   int opt = 0;
   char opcoes[][TEXT_SIZE] = {
      "Sair\0",
      "Criar um personagem\0",
      "Listar um persoangem\0",
      "Listar inventario de um personagem\0",
      "Equipar item do inventário\0",
      "Trocar item equipado por um do inventario\0"
   };

   create_title("AREA DOS PERSONAGENS");
   create_options(opcoes, 6);

   scanf("%d", &opt);

   return opt;
}