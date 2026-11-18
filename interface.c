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
        "Acessar area dos Monstros",
        "Acessar area da Party",
        "Acessar area de Criacao de um Contexto",
        "Acessar area da Criacao dos Monstros Candidatos",
        "Acessar area de Possiveis encontros"
   };

   create_options(options, 8);
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

int candidates_area(){
    int opt = 0;
    char opcoes[][TEXT_SIZE] = {
       "Sair",
       "Gerar",
       "Conferir Candidatos"
    };

    create_title("AREA DOS CANDIDATOS");
    create_options(opcoes, 3);

    scanf("%d", &opt);

    return opt;
}

int party_area(){
    int opt = 0;
    char opcoes[][TEXT_SIZE] = {
       "Sair",
       "Gerar PARTY",
       "Conferir PARTY"
    };

    create_title("AREA DA PARTY");
    create_options(opcoes, 3);

    scanf("%d", &opt);

    return opt;
}

int context_area(){
    int opt = 0;
    char opcoes[][TEXT_SIZE] = {
       "Sair",
       "Gerar Contexto",
       "Conferir Contexto"
    };

    create_title("AREA DO CONTEXTO");
    create_options(opcoes, 3);

    scanf("%d", &opt);

    return opt;
}

int encounter_area(){
    int opt = 0;
    char opcoes[][TEXT_SIZE] = {
       "Sair",
       "Gerar Encontro",
       "Conferir e Escolher um Encontro"
    };

    create_title("AREA DO Encontro");
    create_options(opcoes, 3);

    scanf("%d", &opt);

    return opt;
}