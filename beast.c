#include "beasts.h"
#include "parse_file.h"
#include "read_txt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void beast_db_init(BeastDB *b_db){
   if (b_db == NULL) return;
   memset(b_db, 0, sizeof(*b_db));
}

/* Procura pelo id (nao pelo indice do vetor, entao os ids nao precisam ser 0..49). */
Beast* get_beast_by_id(BeastDB *b_db, int id){
   if (b_db == NULL) return NULL;
   for (int i = 0; i < b_db->count; ++i) {
      if (b_db->beasts[i].id == id) return &b_db->beasts[i];
   }
   return NULL;
}

/*
   Le a besta cujo "[MONSTRO]" esta na linha start_pos, faz o parse
   direto dentro do proximo slot livre do banco e registra.
   Retorna OKAY em caso de sucesso, ERROR se: o banco esta cheio, o arquivo
   nao abriu, a linha start_pos nao e "[MONSTRO]" ou o id ja existe.
*/
void* create_beast(BeastDB *b_db, int start_pos){
   if (b_db == NULL || b_db->count >= MAX_BEASTS) return ERROR;

   char** beast_def = next_beast_definition(start_pos);
   if (beast_def == NULL) return ERROR;

   if (beast_def[0] == NULL || strcmp(beast_def[0], "[MONSTRO]") != 0) {
      free_file_text(beast_def);
      return ERROR;
   }

   Beast *slot = &b_db->beasts[b_db->count];
   parse_beast(beast_def, slot);
   free_file_text(beast_def);   /* tudo ja foi copiado para dentro do Beast */

   /* slot ainda nao conta em count, entao o get so enxerga as bestas anteriores */
   if (get_beast_by_id(b_db, slot->id) != NULL) {
      memset(slot, 0, sizeof(*slot));
      return ERROR;   /* id duplicado */
   }

   b_db->count++;
   return OKAY;
}

/*
   Carrega todas as bestas do arquivo (ate MAX_BEASTS). Primeiro varre o
   arquivo procurando as linhas "[MONSTRO]", assim linhas em branco entre
   as bestas nao quebram nada. Retorna quantas foram carregadas (ou -1 se
   o arquivo nao abriu).
*/
int load_beasts(BeastDB *b_db){
   if (b_db == NULL) return -1;

   FILE *f = open_file(FILE_PATH, "r");
   if (f == NULL) return -1;

   char **lines = get_file_text(f, NULL);
   fclose(f);
   if (lines == NULL) return -1;

   int starts[MAX_BEASTS];
   int n = 0;
   for (int i = 0; lines[i] != NULL && n < MAX_BEASTS; ++i) {
      if (strcmp(lines[i], "[MONSTRO]") == 0) starts[n++] = i;
   }
   free_file_text(lines);

   for (int k = 0; k < n; ++k) {
      if (create_beast(b_db, starts[k]) == ERROR) {
         fprintf(stderr, "Aviso: besta na linha %d nao foi carregada (id duplicado ou bloco invalido)\n",
                 starts[k]);
      }
   }

   return b_db->count;
}