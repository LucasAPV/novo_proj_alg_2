#include "actions.h"
#include "adventure.h"
#include "context.h"
#include "definitions.h"
#include "error.h"
#include "filter.h"
#include "interface.h"
#include "item.h"
#include "beasts.h"
#include "party.h"
#include <stdio.h>

int main(void) {
  /* PERSONAGENS */
  CharacterDB c_db = {0};
  ItemDB i_db = {0};

  /* BEASTAS */
  /* Carregando as beastas de uma vez */
  BeastDB b_db = {0};
  beast_db_init(&b_db);
  if (load_beasts(&b_db) == -1) {log_error(UNABLE_TO_CREATE_BEAST_DB);}

  /* PARTY */
  Party party = {0};

  /* ENCONTROS E CONTEXTO */
  ContextEncounter context = {0};
  BeastsCandidates b_candidates = {0};

  while (1) {
    int option = create_initial_menu();

    switch (option) {
    case 0:
      printf("SAINDO...\n");
      exit(0);
      break;

    case 1:
      actions_for_characters(&c_db); //&i_db removido para não ativar o -Wall e
                                     //-Wextra na compilação por falta de uso
      break;

    case 2:
      actions_for_items(&c_db, &i_db);
      break;

    case 3:
        actions_for_monsters(&b_db);
    break;

    case 4:
        /* Criacao da Party */
        int qtd = 0;
        printf("Selecione a quantidade de personagens para a PARTY:\n");
        scanf("%d", &qtd);

        int *ids = {0};

        for(int i = 0; i < qtd; ++i){
            printf("Digite o id para o %d personagem", i + 1);
            scanf("%d", &ids[i]);
        }

        create_party(ids, qtd, &party, c_db);
    break;
    case 5:
        /* Criacao de um Contexto */
        Active periodo;
        printf("Digite o Periodo desejado");
        printf("1 - QUALQUER HORARIO\n 2 - DIURNO \n 3 - NOTURNO \n");
        scanf("%d", &periodo);
        periodo--;

        Dificulty dificulty;
        printf("Digite a Dificuldade desejado");
        printf("1 - FACIL\n 2 - MEDIO \n 3 - DIFICIL \n");
        scanf("%d", &dificulty);
        dificulty--;

        printf("Digite o terreno desejado:\n");
        char terreno[TEXT_SIZE];
        if (scanf("%99s", terreno) == 1) {
            generate_context(terreno, periodo,
            dificulty, &context, party.total_power);
        }
    break;
    case 6:
        actions_for_candidates(context, &b_candidates, b_db);
    break;

    case 7:
        /* GERAR POSSIBILIDADES DE ENCONTROS COM TANTOS BIXOS */
    break;
    default:
      printf("Opcao invalida!\n");
      break;
    }
  }
  return 0;
}