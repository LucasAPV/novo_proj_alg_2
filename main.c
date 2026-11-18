#include "actions.h"
#include "adventure.h"
#include "context.h"
#include "definitions.h"
#include "encounter.h"
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
  BeastsCandidates b_candidates = {0};
 // PARTY
  Party party = {0};

  //  ENCONTROS E CONTEXTO
  ContextEncounter context = {0};
  PossibleEncounters encontro = {0};
  int choosed_encounter = 0;

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
        actions_for_party(&party, c_db);
    break;

    case 5:
        actions_for_context(&context, party);
    break;

    case 6:
        actions_for_candidates(context, &b_candidates, b_db);
    break;

    case 7:
        actions_for_encounter(b_candidates, &encontro, &choosed_encounter);
    break;

    default:
      printf("Opcao invalida!\n");
      break;
    }
  }
  return 0;
}