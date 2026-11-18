#include "actions.h"
#include "adventure.h"
#include "beasts.h"
#include "context.h"
#include "definitions.h"
#include "encounter.h"
#include "error.h"
#include "filter.h"
#include "interface.h"
#include "party.h"
#include <stdio.h>

void actions_for_characters(CharacterDB *c_db) {
  while (1) {
    int opt = character_area();
    switch (opt) {
    case 0:
      return;

    case 1:
      create_character(c_db);
      break;

    case 2: {
      int id = 0;
      printf("Digite o ID do personagem que deseja ver: \n");
      scanf("%d", &id);
      printf("\n");
      if (print_character(id, *c_db) == NULL) {
        printf("Personagem nao encontrado!\n");
      }
      break;
    }

    case 3: {
      int id = 0;
      printf("Digite o ID do personagem que deseja alterar: \n");
      scanf("%d", &id);
      update_character(id, c_db);
      break;
    }

    case 4: {
      int id = 0;
      printf("Digite o ID do personagem que deseja remover: \n");
      scanf("%d", &id);
      remove_character(id, c_db);
      break;
    }

    case 5:
      print_all_characters(*c_db);
      break;

    case 6: { // Administrar inventário (submenu)
      int id_char = 0, sub_opt = 0;
      printf("Digite o ID do personagem: \n");
      scanf("%d", &id_char);
      Character *c = (Character *)select_character_by_id(id_char, c_db);
      if (c != NULL) {
        printf("\n--- ADMINISTRAR INVENTARIO (%s) ---\n", c->name);
        printf("1 - Listar itens da mochila\n");
        printf("2 - Descartar item da mochila\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &sub_opt);

        if (sub_opt == 1) {
          print_inventory(id_char, *c_db);
        } else if (sub_opt == 2) {
          print_inventory(id_char, *c_db);
          if (c->inventory.free_index > 0) {
            int index_item = 0;
            printf("Digite o indice do item a descartar da mochila: \n");
            scanf("%d", &index_item);
            drop_item_from_inventory(c, index_item);
          }
        } else {
          printf("Opcao invalida!\n");
        }
      } else {
        printf("Personagem invalido!\n");
      }
      break;
    }

    case 7: {
      int id = 0;
      printf("Digite o ID do personagem: \n");
      scanf("%d", &id);
      Character *c = (Character *)select_character_by_id(id, c_db);
      if (c != NULL) {
        print_equipped_items(*c);
      } else {
        printf("Personagem invalido!\n");
      }
      break;
    }

    case 8: { // Equipar item (Troca automática integrada na equip_item)
      int id_char = 0, id_item = 0, hand = 0;
      printf("Digite o ID do personagem: \n");
      scanf("%d", &id_char);

      printf("Digite o indice do item no inventario: \n");
      scanf("%d", &id_item);

      printf("Se for arma/escudo, qual mao? (0-Esquerda, 1-Direita, "
             "2-Nenhuma): \n");
      scanf("%d", &hand);

      Character *c = (Character *)select_character_by_id(id_char, c_db);

      if (c != NULL) {
        if (equip_item(id_item, c, hand) == OKAY) {
          printf("Item equipado (ou trocado) com sucesso!\n");
        } else {
          printf("Falha ao equipar o item.\n");
        }
      } else {
        printf("Personagem invalido ou nao encontrado!\n");
      }
      break;
    }

    case 9: {
      int id_char = 0, type_item = 0, hand = 0;
      printf("Digite o ID do personagem: \n");
      scanf("%d", &id_char);

      printf("Escolha o tipo do item a desequipar:\n");
      print_item_types();
      scanf("%d", &type_item);

      printf("Se for Arma 1 Mao ou Escudo, qual mao? (0-Esquerda, 1-Direita, "
             "2-Nenhuma): \n");
      scanf("%d", &hand);

      Character *c = (Character *)select_character_by_id(id_char, c_db);
      if (c != NULL) {
        if (unequip_item(c, type_item, hand) == OKAY) {
          printf("Item desequipado e retornado ao inventario!\n");
        } else {
          printf("Falha ao desequipar (Slot vazio ou inventario cheio).\n");
        }
      } else {
        printf("Personagem invalido!\n");
      }
      break;
    }

    case 10: {
      int id = 0;
      printf("Digite o ID do personagem: \n");
      scanf("%d", &id);
      Character *c = (Character *)select_character_by_id(id, c_db);
      if (c != NULL) {
        calculate_total_attributes(*c);
      } else {
        printf("Personagem invalido!\n");
      }
      break;
    }

    default:
      printf("Opcao invalida!\n");
      break;
    }
  }
}

void actions_for_items(CharacterDB *c_db, ItemDB *i_db) {
  while (1) {
    int opt = item_area();
    switch (opt) {
    case 0:
      return;

    case 1: {
      Item novo_item;
      create_item(&novo_item, i_db);
      break;
    }

    case 2: {
      int id = 0;
      printf("Digite o ID do item que deseja ver: \n");
      scanf("%d", &id);
      printf("\n");

      if (id >= 0 && id < i_db->curr_id) {
        print_item(i_db->itens[id]);
      } else {
        printf("ID de item invalido!\n");
      }
      break;
    }

    case 3: {
      int id_char = 0, id_item = 0;
      printf("Digite o ID do personagem: \n");
      scanf("%d", &id_char);

      printf("Digite o ID do item criado no banco de itens: \n");
      scanf("%d", &id_item);

      Character *c = (Character *)select_character_by_id(id_char, c_db);

      if (c != NULL && id_item >= 0 && id_item < i_db->curr_id) {
        Item item_para_dar = i_db->itens[id_item];
        if (put_in_bag(c, item_para_dar) == OKAY) {
          printf("Item adicionado ao inventario com sucesso!\n");
        } else {
          printf("Falha: Inventario cheio ou sem espaco.\n");
        }
      } else {
        printf("Personagem ou Item nao encontrado.\n");
      }
      break;
    }

    case 4:
      print_all_items(*i_db);
      break;

    case 5: {
      int id = 0;
      printf("Digite o ID do item que deseja alterar: \n");
      scanf("%d", &id);
      update_item(id, i_db);
      break;
    }

    case 6: {
      int id = 0;
      printf("Digite o ID do item que deseja remover do banco: \n");
      scanf("%d", &id);
      remove_item_from_db(id, i_db);
      break;
    }

    default:
      printf("Opcao invalida!\n");
      break;
    }
  }
}

void actions_for_monsters(BeastDB *b_db) {
    while (1) {
        int opt = monster_area();

        switch (opt) {
            case 0:
                return;
            break;

            case 1:
                int id = 0;
                printf("Digite o ID desejado:\n");
                scanf("%d", &id);

                Beast *b_id = get_beast_by_id(b_db, id);
                print_beast(*b_id);
            break;

            case 2:
                char name[100]; // Aloca 100 bytes de memória modificável
                printf("Digite o nome da besta a ser buscada:\n");

                // %99s limita a leitura a 99 caracteres para proteger a memória
                if (scanf("%99s", name) == 1) {
                    printf("Passou pelo o scan\n");

                    Beast *b_nome = buscaMonstroPorNome(b_db, name);
                    printf("Passou pela busca\n");

                    if (b_nome != NULL) {
                        print_beast(*b_nome);
                    } else {
                        printf("Besta não encontrada.\n");
                    }
                }
            break;

            case 3:
                printf("Digite o id do mostro a ser deletado: \n");
                int id_deletado;
                scanf("%d", &id_deletado);
                if(removeMonstro(b_db, id_deletado) == 0) log_error(ERROR_DELETE_ON_MONSTER);
            break;

            case 4:
                listaMonstros(b_db);
            break;

            case 5:
                int periodo;
                printf("Escolha o periodo desejado:\n");
                printf("1 - QUALQUER_HORARIO\n");
                printf("2 - DIURNO\n");
                printf("3 - NOTURNO\n");
                scanf("%d", &periodo);

                if(periodo - 1 < 0 || periodo - 1 > 2) {
                    log_error(INVALID_ENTRY);
                    return;
                }
                ativoNoPeriodo(b_db, (Active)(periodo - 1));
            break;

            case 6:
                printf("Digite o terreno desejado:\n");
                char terreno[100];
                if (scanf("%99s", terreno) == 1) {
                    Beast *b_terreno[MAX_BEASTS] = {0};
                    int n = listaMonstrosPorTerreno(b_db, terreno, b_terreno, MAX_BEASTS);

                    printf("\n\n MONSTROS NO TERRENO %s \n\n", terreno);
                    if (n == 0) {
                        printf("Nenhum monstro encontrado nesse terreno. \n");
                    }
                    for (int i = 0; i < n; i++) {
                        lista_monstro_terreno(b_terreno[i], 1);
                    }
                }
            break;
        }
    }
}

void actions_for_candidates(ContextEncounter c, BeastsCandidates *b, BeastDB b_db){
    while (1) {
        int opt = candidates_area();
        switch (opt) {
            case 0:
                return;
            break;

            case 1:
                if(filter_candidates(b_db, b, c) == 0) {
                    printf("DEU ERRADO!!\n");
                    return;
                }
                printf("CANDIDATOS GERADOS!!\n");
            break;

            case 2:
                lista_monstro_terreno(b->b, 50);
            break;
        }
    }
}

void actions_for_party(Party *party, CharacterDB c_db){
    while(1){
        int opt = party_area();
        switch (opt) {
            case 0:
                return;
            break;

            case 1:
                int qtd = 0;
                printf("Selecione a quantidade de personagens para a PARTY:\n");
                scanf("%d", &qtd);

                int ids[MAX_PARTY_SIZE] = {0};

                for(int i = 0; i < qtd; ++i){
                    printf("Digite o id para o %d personagem: \n", i + 1);
                    scanf("%d", &ids[i]);
                }

                printf("ENTROU DA FUNCAO DE CRIAR PARTY!!!\n");
                create_party(ids, qtd, party, c_db);
            break;

            case 2:
                print_party(party);
            break;

        }
    }

}

void actions_for_context(ContextEncounter *context, Party party){
    while (1) {
        int opt = context_area();

        switch (opt) {
            case 0:
                return;
            break;

            case 1:
                Active periodo;
                int p_opt = 0;
                printf("Digite o Periodo desejado:\n");
                printf("1 - QUALQUER HORARIO\n 2 - DIURNO \n 3 - NOTURNO \n");
                scanf("%d", &p_opt);
                periodo = (Active) (p_opt--);
                Dificulty dificulty;
                int df_opt = 0;
                printf("Digite a Dificuldade desejado: \n");
                printf("1 - FACIL\n 2 - MEDIO \n 3 - DIFICIL \n");
                scanf("%d", &df_opt);
                dificulty = (Dificulty) (df_opt--);

                printf("Digite o terreno desejado:\n");
                char terreno[TEXT_SIZE];
                if (scanf("%99s", terreno) == 1) {
                    generate_context(terreno, periodo,
                    dificulty, context, party.total_power);
                }

                print_context(*context);
            break;

            case 2:
                print_context(*context);
            break;
        }
    }
}

void actions_for_encounter(BeastsCandidates b_cad, PossibleEncounters *enc, int *ch_enc){
    while(1){
        int opt = encounter_area();

        switch (opt) {
            case 0:
                return;
            break;

            case 1:
                generate_encounter(b_cad, enc);
                printf("Encontro Gerado!!\n");
            break;

            case 2:
                int ch = 0;
                print_encounter(*enc);
                printf("\n\n Escolha o enconstro desejado: \n");
                scanf("%d", &ch);
                if (ch > 2 || ch < 0) {
                    printf("opcao invalida!!!");
                    return;
                }

                *ch_enc = ch;
                printf("\nEncontro %d foi escolhido!!!\n", ch);
            break;
        }
    }
}