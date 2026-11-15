#include "interface.h"
#include "includes/definitions.h"

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
   char options[][TEXT_SIZE] = {
        "Sair do programa",
        "Acessar area dos Personagens",
        "Acessar area dos Itens",
        "Acessar area dos Monstros"
   };

   create_options(options, 4);
   int opt = 0;
   scanf("%d", &opt);

   return opt;
}

int character_area() {
   int opt = 0;
   char opcoes[][TEXT_SIZE] = {
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
   };

   create_title("AREA DOS PERSONAGENS");
   create_options(opcoes, 11);

   scanf("%d", &opt);

   return opt;
}

int item_area() {
   int opt = 0;
   char opcoes[][TEXT_SIZE] = {
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

int monster_area(){
    int opt = 0;
    char opcoes[][TEXT_SIZE] = {
       "Sair",
       "Buscar monstro por ID",
       "Buscar monstro por NOME",
       "Remover monstro",
       "Listar TODOS os mostros",
       "Listar monstros que estao ativo no periodo",
       "Listar monstro por terreno"
    };

    create_title("AREA DOS MONSTROS");
    create_options(opcoes, 7);

    scanf("%d", &opt);

    return opt;
}