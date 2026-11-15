#include "actions.h"
#include "adventure.h"
#include "error.h"
#include "interface.h"
#include "item.h"
#include "beasts.h"
#include <stdio.h>

int main(void) {
  CharacterDB c_db = {0};
  ItemDB i_db = {0};

  /* Carregando as beastas de uma vez */
  BeastDB b_db = {0};
  beast_db_init(&b_db);
  if (load_beasts(&b_db) == -1) {log_error(UNABLE_TO_CREATE_BEAST_DB);}

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

    default:
      printf("Opcao invalida!\n");
      break;
    }
  }
  return 0;
}