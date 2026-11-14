#include "parse_file.h"
#include "definitions.h"
#include "read_txt.h"
#include "beasts.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/* ------------------------------------------------------------------ */
/* Helpers                                                             */
/* ------------------------------------------------------------------ */

/* strcpy seguro: nunca escreve alem de dst_size e sempre termina com '\0' */
static void copy_str(char* dst, size_t dst_size, const char* src){
   if (dst_size == 0) return;
   strncpy(dst, src, dst_size - 1);
   dst[dst_size - 1] = '\0';
}

/* Um item de tesouro pode ser numero ("3") ou letra ("C" => 3, A=1, B=2...) */
static int treasure_token_value(const char* tok){
   while (isspace((unsigned char)*tok)) tok++;
   if (isalpha((unsigned char)*tok)) return toupper((unsigned char)*tok) - 'A' + 1;
   return atoi(tok);
}

/* ------------------------------------------------------------------ */
/* Tipos simples                                                       */
/* ------------------------------------------------------------------ */

// como forma de simplificar, irei utilizar a funcao atoi pra poder decidir o valor de cada coisa.
// ao inves de usar a tabela de tesouros do d&d.
// Formato: "NENHUM" ou "pessoal;covil;dungeon". Um unico valor ("C") preenche so o pessoal.
Treasure string_to_treasure(char* str){
   Treasure t;
   memset(&t, 0, sizeof(t));
   if(strcmp(str, "NENHUM") == 0) return t;

   char tmp[TEXT_SIZE];
   copy_str(tmp, sizeof(tmp), str);   // strtok modifica a string, entao uso copia

   int values[3] = {0, 0, 0};
   int type = 0;

   char* token = strtok(tmp, ";");
   while(token != NULL && type < 3){
      values[type++] = treasure_token_value(token);
      token = strtok(NULL, ";");
   }

   t.personal_treasure = values[0];
   t.covil_treasure    = values[1];
   t.dungeon_treasure  = values[2];
   return t;
}

void string_to_freq(char* str, Freq* f){
   if(strcmp(str, "MUITO_RARO") == 0)      *f = FREQ_MUITO_RARO;
   else if(strcmp(str, "RARO") == 0)       *f = FREQ_RARO;
   else if(strcmp(str, "INCOMUM") == 0)    *f = FREQ_INCOMUM;
   else                                    *f = FREQ_COMUM;
}

void string_to_activity(char *str, Active *a){
   if(strcmp(str, "QUALQUER_HORARIO") == 0){
      *a = QUALQUER_HORARIO;
   } else if (strcmp(str, "DIURNO") == 0) {
      *a = DIURNO;
   } else {
      *a = NOTURNO;
   }
}

void string_to_diet(char* str_diet, Dieta* d) {
    if(strcmp(str_diet, "ONIVORO") == 0){
        *d = ONIVORO;
    } else if(strcmp(str_diet, "CARNIVORO") == 0){
        *d = CARNIVORO;
    } else if(strcmp(str_diet, "HERBIVORO") == 0){
        *d = HERBIVORO;
    } else if (strcmp(str_diet, "PARASITA") == 0){
        *d = PARASITA;
    } else {
        *d = NAO_SE_ALIMENTA;
    }
}

void string_to_ordem(char* str_ordem, Ordem* o){
    if(strcmp(str_ordem, "CAOTICO") == 0){
        *o = CAOTICO;
    } else if(strcmp(str_ordem, "LEAL") == 0){
        *o = LEAL;
    } else {
        *o = NEUTRO_ORDEM;
    }
}

void string_to_moral(char* str_moral, Moral* m){
    if(strcmp(str_moral, "BOM") == 0){
        *m = BOM;
    } else if(strcmp(str_moral, "MAU") == 0){
        *m = MAU;
    } else {
        *m = NEUTRO_MORAL;
    }
}

// "2d4" => {2,4,0}   "4d8+4" => {4,8,4}   "1d6-1" => {1,6,-1}
void string_to_dice(const char* str, Dice* d){
   d->dice_quantity = 0;
   d->faces_of_dices = 0;
   d->bonus = 0;
   (void)sscanf(str, "%dd%d%d", &d->dice_quantity, &d->faces_of_dices, &d->bonus);
}

/* ------------------------------------------------------------------ */
/* Separadores                                                         */
/* ------------------------------------------------------------------ */

//# Listas usam virgula;
// Retorna a quantidade de itens. str_array precisa ter espaco para max+1 ponteiros.
int string_to_array(char* str, char** str_array, int max) {
   int i = 0;

   char* token = strtok(str, ",");
   while (token != NULL && i < max) {
      str_array[i] = token;
      i++;
      token = strtok(NULL, ",");
   }

   str_array[i] = NULL;
   return i;
}

//# Subestruturas usam dois-pontos;
// Divide no PRIMEIRO ':'. O que sobra vai inteiro para rhs, entao
// "VOO:15:C" => lhs="VOO", rhs="15:C" (que da pra dividir de novo).
void colon_separetor(const char* str_with_colon, char* lhs, char* rhs){
    const char* colon = strchr(str_with_colon, ':');

    if (colon == NULL) {
        copy_str(lhs, TEXT_SIZE, str_with_colon);
        rhs[0] = '\0';
        return;
    }

    size_t lhs_len = (size_t)(colon - str_with_colon);
    if (lhs_len >= TEXT_SIZE) lhs_len = TEXT_SIZE - 1;
    memcpy(lhs, str_with_colon, lhs_len);
    lhs[lhs_len] = '\0';

    copy_str(rhs, TEXT_SIZE, colon + 1);
}

//# Elementos compostos usam barra vertical;
// Retorna quantos itens foram copiados (no maximo MAX_SEPARETORS).
int string_tok(char* str, char str_arr[MAX_SEPARETORS][TEXT_SIZE]){
    int i = 0;
    char *token = strtok(str, "|");
    while (token != NULL && i < MAX_SEPARETORS) {
        copy_str(str_arr[i], TEXT_SIZE, token);
        i++;
        token = strtok(NULL, "|");
    }
    return i;
}

/* ------------------------------------------------------------------ */
/* Estruturas compostas                                                */
/* ------------------------------------------------------------------ */

// "TERRESTRE:9|VOO:15:C"
void string_to_moveset(char* str_moveset, Moveset *mv){
    char str_arr[MAX_SEPARETORS][TEXT_SIZE];
    memset(mv, 0, sizeof(*mv));

    int total = string_tok(str_moveset, str_arr);

    for (int i = 0; i < total && mv->quantidade < MAX_TERRENOS; ++i) {
        char nome[TEXT_SIZE];
        char resto[TEXT_SIZE];
        colon_separetor(str_arr[i], nome, resto);

        int idx = mv->quantidade++;
        copy_str(mv->terrenos[idx], sizeof(mv->terrenos[idx]), nome);
        mv->modificadores[idx] = atoi(resto);   // atoi para no ':' => "15:C" vira 15

        if (strcmp(nome, "VOO") == 0) {
            const char* classe = strchr(resto, ':');
            if (classe != NULL && classe[1] != '\0') mv->voo_acobracia = classe[1];
        }
    }
}

// "GARRA:1d4|MORDIDA:1d6" ou "NENHUM"
void string_to_attacks(char* str, AttackList* list){
    char str_arr[MAX_SEPARETORS][TEXT_SIZE];
    memset(list, 0, sizeof(*list));
    if (strcmp(str, "NENHUM") == 0) return;

    int total = string_tok(str, str_arr);

    for (int i = 0; i < total && list->quantidade < MAX_SEPARETORS; ++i) {
        char nome[TEXT_SIZE];
        char dano[TEXT_SIZE];
        colon_separetor(str_arr[i], nome, dano);

        Attack* a = &list->itens[list->quantidade++];
        copy_str(a->nome, sizeof(a->nome), nome);
        string_to_dice(dano, &a->dano);
    }
}

// "PELE_PEDRA:1:Resiste a armas comuns|OUTRA:2:Texto" ou "NENHUM"
void string_to_defences(char* str, DefenceList* list){
    char str_arr[MAX_SEPARETORS][TEXT_SIZE];
    memset(list, 0, sizeof(*list));
    if (strcmp(str, "NENHUM") == 0) return;

    int total = string_tok(str, str_arr);

    for (int i = 0; i < total && list->quantidade < MAX_SEPARETORS; ++i) {
        char nome[TEXT_SIZE];
        char resto[TEXT_SIZE];
        char valor[TEXT_SIZE];
        char descricao[TEXT_SIZE];

        colon_separetor(str_arr[i], nome, resto);
        colon_separetor(resto, valor, descricao);

        Defence* d = &list->itens[list->quantidade++];
        copy_str(d->nome, sizeof(d->nome), nome);
        d->valor = atoi(valor);
        copy_str(d->descricao, sizeof(d->descricao), descricao);
    }
}

// "RUINAS:3,URBANO:1,MONTANHA:2" ou "NENHUM"
void string_to_terrain_mods(char* str, TerrainModList* list){
    char* arr[MAX_TERRENOS + 1];
    memset(list, 0, sizeof(*list));
    if (strcmp(str, "NENHUM") == 0) return;

    int total = string_to_array(str, arr, MAX_TERRENOS);

    for (int i = 0; i < total; ++i) {
        char nome[TEXT_SIZE];
        char valor[TEXT_SIZE];
        colon_separetor(arr[i], nome, valor);

        TerrainMod* m = &list->itens[list->quantidade++];
        copy_str(m->nome, sizeof(m->nome), nome);
        m->valor = atoi(valor);
    }
}

/* ------------------------------------------------------------------ */
/* Leitura da besta                                                    */
/* ------------------------------------------------------------------ */

/*
   start_pos = linha (a partir de 0) onde esta o "[MONSTRO]".
   Devolve o bloco inteiro ("[MONSTRO]" + QTD_COLUMNS campos + "[/MONSTRO]"),
   terminado em NULL. Quem chama deve liberar com free_file_text().
   Se nao houver blank lines entre as bestas, a besta N comeca na linha
   N * (QTD_COLUMNS + 2).
*/
char** next_beast_definition(int start_pos) {
   FILE *beasts_file = open_file(FILE_PATH, "r");
   if (beasts_file == NULL) return NULL;

   const int range[2] = {start_pos, start_pos + QTD_COLUMNS + 1};
   char **beast_instance = get_file_text(beasts_file, range);

   fclose(beasts_file);
   return beast_instance;
}

/*
   NAO chame free_file_text(beast_def) antes de terminar de usar o Beast:
   o parse copia tudo para dentro do Beast, entao depois de parse_beast
   voltar pode liberar sem problema.
*/
void parse_beast(char** beast_def, Beast *b){
   if (beast_def == NULL || b == NULL) return;
   memset(b, 0, sizeof(*b));

   for(int i = 0; beast_def[i] != NULL; ++i) {
      if (strcmp(beast_def[i], "[/MONSTRO]") == 0) break;

      ColValue cv = parse_line(beast_def[i]);
      char* v = cv.value;

      switch (cv.col_name) {

         /* Tratando o que for inteiro */
         case ID:                  b->id = atoi(v); break;
         case INTELIGENCIA:        b->inteligencia = atoi(v); break;
         case CA:                  b->ca = atoi(v); break;
         case THAC_0:              b->thac0 = atoi(v); break;
         case RESISTENCIA_MAGIA:   b->resistenciaMagia = atoi(v); break;
         case MORAL:               b->moral = atoi(v); break;
         case XP:                  b->xp = atoi(v); break;
         case PODER_BASE:          b->poderBase = atoi(v); break;
         case MOD_DIA:             b->modDia = atoi(v); break;
         case MOD_NOITE:           b->modNoite = atoi(v); break;
         case BONUS_POR_ADICIONAL: b->bonusPorAdicional = atoi(v); break;
         case BONUS_GRUPO_GRANDE:  b->bonusGrupoGrande = atoi(v); break;
         case LIMITE_GRUPO_GRANDE: b->limiteGrupoGrande = atoi(v); break;

         /* Tratando o que for texto */
         case NOME:
            copy_str(b->nome, sizeof(b->nome), v);
            break;
         case DESCRICAO:
            copy_str(b->descricao, sizeof(b->descricao), v);
            break;
         case ORGANIZACAO:
            copy_str(b->organizacao, sizeof(b->organizacao), v);
            break;
         case TAMANHO:
            copy_str(b->tamanho, sizeof(b->tamanho), v);
            break;
         case CLASSIFICACAO_MORAL:
            copy_str(b->classificacaoMoral, sizeof(b->classificacaoMoral), v);
            break;
         case DESCRICAO_COMBATE:
            copy_str(b->descricaoCombate, sizeof(b->descricaoCombate), v);
            break;
         case HABITAT_SOCIEDADE:
            copy_str(b->habitatSociedade, sizeof(b->habitatSociedade), v);
            break;
         case DESCRICAO_ECOLOGIA:
            copy_str(b->descricaoEcologia, sizeof(b->descricaoEcologia), v);
            break;

         /* Tratando enums */
         case FREQUENCIA:  string_to_freq(v, &b->frequencia); break;
         case ATIVIDADE:   string_to_activity(v, &b->atividade); break;
         case DIETA:       string_to_diet(v, &b->dieta); break;
         case ORDEM:       string_to_ordem(v, &b->ordem); break;
         case MORALIDADE:  string_to_moral(v, &b->moralidade); break;

         /* Tratando dados (XdY+Z) */
         case NUMERO_ENCONTRADO: string_to_dice(v, &b->numeroEncontrado); break;
         case DV:                string_to_dice(v, &b->dv); break;

         case TESOURO:
            b->tesouro = string_to_treasure(v);
            break;

         /* Tratando o que for vetores */
         case COVIL: {
            char* arr[GRID + 1];
            int n = string_to_array(v, arr, GRID);
            for (int k = 0; k < n; ++k) b->covil[k] = atoi(arr[k]);
            break;
         }

         case CLIMAS: {
            char* arr[MAX_CLIMAS + 1];
            int n = string_to_array(v, arr, MAX_CLIMAS);
            for (int k = 0; k < n; ++k)
               copy_str(b->climas[k], sizeof(b->climas[k]), arr[k]);
            break;
         }

         case TERRENOS: {
            char* arr[MAX_TERRENOS + 1];
            int n = string_to_array(v, arr, MAX_TERRENOS);
            for (int k = 0; k < n; ++k)
               copy_str(b->terrenos[k], sizeof(b->terrenos[k]), arr[k]);
            break;
         }

         case ALTURA: {
            char* arr[VARIETY + 1];
            int n = string_to_array(v, arr, VARIETY);
            for (int k = 0; k < n; ++k) b->altura[k] = (float)atof(arr[k]);
            break;
         }

         /* Tratando o que for objetos compostos */
         case MOVIMENTOS:         string_to_moveset(v, &b->movimentos); break;
         case ATAQUES:            string_to_attacks(v, &b->ataques); break;
         case ATAQUES_ESPECIAIS:  string_to_attacks(v, &b->ataquesEspeciais); break;
         case DEFESAS_ESPECIAIS:  string_to_defences(v, &b->defesasEspeciais); break;
         case MOD_TERRENO:        string_to_terrain_mods(v, &b->modTerreno); break;

         default:
            /* "[MONSTRO]", linhas em branco, colunas desconhecidas: ignora */
            break;
      }
   }
}