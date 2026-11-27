#include "actions.h"
#include "adventure.h"
#include "beasts.h"
#include "context.h"
#include "definitions.h"
#include "encounter.h"
#include "error.h"
#include "filter.h"
#include "interface.h"
#include "item.h"
#include "party.h"
#include "battle.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  /* PERSONAGENS */
  CharacterDB c_db = {0};
  ItemDB i_db = {0};

  /* BEASTAS */
  /* Carregando as beastas de uma vez */
  BeastDB b_db = {0};
  beast_db_init(&b_db);
  if (load_beasts(&b_db) == -1) {
    log_error(UNABLE_TO_CREATE_BEAST_DB);
  }
  BeastsCandidates b_candidates = {0};
  // PARTY
  Party party = {0};

  //  ENCONTROS E CONTEXTO
  ContextEncounter context = {0};
  PossibleEncounters encontro = {0};
  int choosed_encounter = -1;
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

    case 8:
        if(
            choosed_encounter == -1    ||
            is_struct_empty(party)     ||
            is_struct_empty(encontro)
            )
           {
               printf("\n\nUm dos dados esta faltando!! E necessario a escolha de qual encountro quer e uma party!! \n\n");
               break;
           }
        BattleSpecifier bs = {0};
        bs.p = party;
        bs.encounter = encontro;
        bs.choosed_encounter = choosed_encounter;

        /*
            @LucasAPV
            [TODO]
                title="Realizar ajustes finos na batalha"
                desc="Mostrar todas as bestas que estao na batalha, e mostrar tela de derrota ou vitoria, atualizar as vidas da party."
                relevance=Importante
            [\TODO]
        */

        actions_for_battle(&bs, c_db);
        break;
    default:
      printf("Opcao invalida!\n");
      break;
    }
  }
  return 0;
}
