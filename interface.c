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
<<<<<<< HEAD
   char opcoes[][TEXT_SIZE] = {
      "Sair\0",
      "Criar um personagem\0",
      "Listar um persoangem\0",
      "Listar inventario de um personagem\0",
      "Equipar item do inventário\0",
      "Trocar item equipado por um do inventario\0"
=======
   char opcoes[][TEXT_LENGTH] = {
      "Encerrar",                     
      "Cadastrar personagem",        
      "Consultar personagem por ID", 
      "Alterar personagem",          
      "Remover personagem",          
      "Listar personagens",          
      "Administrar inventario",      
      "Consultar equipamentos",      
      "Equipar item",                
      "Desequipar item",             
      "Exibir atributos totais"      
>>>>>>> eafc00424384ca135115fbe1d2a8d87499e2eae6
   };

   create_title("AREA DOS PERSONAGENS");
   create_options(opcoes, 11);

   scanf("%d", &opt);

   return opt;
}

int item_area() {
   int opt = 0;
   char opcoes[][TEXT_LENGTH] = {
      "Sair",
      "Criar um item",
      "Consultar um item por ID",
      "Adicionar item ao inventario de um personagem",
      "Listar todos os itens cadastrados",
      "Alterar um item cadastrado",
      "Remover um item do banco de itens"
   };

   create_title("AREA DOS ITENS");
   create_options(opcoes, 7);

   scanf("%d", &opt);

   return opt;
}