#include "parse_file.h"
#include "definitions.h"
#include "read_txt.h"
#include "beasts.h"
#include <stdlib.h>

char** next_beast_definition(int start_pos) {
   FILE *beasts_file = open_file(FILE_PATH, "r");
   if (beasts_file == NULL) return NULL;

   const int range[2] = {start_pos, start_pos + QTD_COLUMNS};
   char **beast_instance = get_file_text(beasts_file, range);

   fclose(beasts_file);
   return beast_instance;
}

void parse_beast(char** beast_def, , Beast *b){
   ColValue cv = {};
   for(int i = 0; i < QTD_COLUMNS; ++i) {
      cv = parse_line(beast_def[i]);

      switch (cv.col_name) {

         /* Tratando o que for inteiro */
         case ID:
               b->id = atoi(cv.value);
               break;
         case INTELIGENCIA:
               b->inteligencia = atoi(cv.value);
               break;
         case CA:
               b->ca = atoi(cv.value);
               break;
         case THAC_0:
               b->thac0 = atoi(cv.value);
               break;
         case RESISTENCIA_MAGIA:
               b->resistenciaMagia = atoi(cv.value);
               break;
         case MORAL:
               b->moral = atoi(cv.value);
               break;
         case XP:
               b->xp = atoi(cv.value);
               break;
         case PODER_BASE:
               b->poderBase = atoi(cv.value);
               break;
         case MOD_DIA:
               b->modDia = atoi(cv.value);
               break;
         case MOD_NOITE:
               b->modNoite = atoi(cv.value);
               break;
         case BONUS_POR_ADICIONAL:
               b->bonusPorAdicional = atoi(cv.value);
               break;
         case BONUS_GRUPO_GRANDE:
               b->bonusGrupoGrande = atoi(cv.value);
               break;
         case LIMITE_GRUPO_GRANDE:
               b->limiteGrupoGrande = atoi(cv.value);
               break;

         case TESOURO:
            b->tesouro = string_to_treasure(cv.value);
         break;
         /*Tratando o que for objetos compostos*/
         /*Criar funcao para parsear esses objetos especificos*/

         /*Tratando o que for vetores*/

         case NOME:
            strcpy(b->name, cv.value);
         break;

         case DESCRICAO:
            strcpy(b->descricao, cv.value);
         break;

         case COVIL:
            char *arr[GRID];
            int covil[GRID];
            
            string_to_array(cv.value, arr);

            covil[0] = atoi(arr[0]);
            covil[1] = atoi(arr[1]);

            b->covil = covil;
         break;

         case CLIMAS:
            char *climas[MAX_CLIMAS];
            string_to_array(cv.value, climas);

            strcpy(b->climas[0], climas[0]);
            strcpy(b->climas[1], climas[1]);
         break;

         case TERRENOS:
            char *terrenos[MAX_TERRENOS];

            string_to_array(cv.value, terrenos);

            for(int terr = 0; terr < MAX_TERRENOS; terr++){
               strcpy(b->terrenos[terr], terrenos[terr]);
            }
         break;

         case ORGANIZACAO:
            strcpy(b->organizacao, cv.value);
         break;

         case ATIVIDADE:
            Active *act;
            string_to_activity(cv.value, act);
            b->atividade = *act;
         break;
         /*Criar funcao para parsear os vetores especificos*/
      }
   }
}

// como forma de simplificar, irei utilizar a funcao atoi pra poder decidir o valor de cada coisa.
// ao inves de usar a tabela de tesouros do d&d.

Treasure string_to_treasure(char* str){
   Treasure t = {0};
   if(strcmp(str, "NENHUM") == 0) {return t}

   int type = 0;
   for(int i = 0; i < TREASURE_SIZE; ++i){
      if(type == 3) break;

      if((strcmp(str[i], '\0') == 0) || (strcmp(str[i], '\n'))){
         break;
      }

      if((strcmp(str[i], ';') == 0)){
         continue;
      }

      if(type == 0) t.personal_treasure = atoi(str[i]);
      if(type == 1) t.covil_treasure = atoi(str[i]);
      if(type == 2) t.dungeon_treasure = atoi(str[i]);
      type++;
   }

   return t;
}

void string_to_activity(char *str, Active *a){
   if(strcmp(str, "QUALQUER_HORARIO") == 0){
      *a = (Active) QUALQUER_HORARIO;
   } else if (strcmp(str, "DIURNO") == 0) {
      *a = (Active) DIURNO;
   } else {
      *a = (Active) NOTURNO;
   }
}

//# Listas usam virgula; 
void string_to_array(char* str, char** str_array) {
   int i = 0;
   
   char* token = strtok(str, ",");
   
   while (token != NULL) {
      str_array[i] = token;
      i++;
      token = strtok(NULL, ",");
   }

   str_array[i] = NULL;
}



//# Subestruturas usam dois-pontos; 

//# Elementos compostos usam barra vertical;