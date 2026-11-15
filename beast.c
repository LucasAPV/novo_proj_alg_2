#include "beasts.h"
#include "parse_file.h"
#include "read_txt.h"
#include <stdio.h>

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
   if (b_db == NULL) {
       printf("[DEBUG] TAVA VAZIO O B_DB!!\n");
       return -1;
   }

   FILE *f = open_file(FILE_PATH, "r");
   if (f == NULL) {
       printf("[DEBUG] NAO CONSEGUIU ABRIR O ARQUIVO DE BESTAS!!\n");
       return -1;
   }

   char **lines = get_file_text(f, NULL);
   fclose(f);
   if (lines == NULL) {
       printf("[DEBUG] NAO CONSEGUIU PEGAR AS LINHAS!!\n");
       return -1;
   }

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

static const char *freq_str(Freq f) {
    switch (f) {
        case FREQ_MUITO_RARO: return "MUITO RARO";
        case FREQ_RARO:       return "RARO";
        case FREQ_INCOMUM:    return "INCOMUM";
        case FREQ_COMUM:      return "COMUM";
    }
    return "DESCONHECIDA";
}

static const char *active_str(Active a) {
    switch (a) {
        case QUALQUER_HORARIO: return "QUALQUER HORARIO";
        case DIURNO:           return "DIURNO";
        case NOTURNO:          return "NOTURNO";
    }
    return "DESCONHECIDA";
}

static const char *dieta_str(Dieta d) {
    switch (d) {
        case ONIVORO:         return "ONIVORO";
        case CARNIVORO:       return "CARNIVORO";
        case HERBIVORO:       return "HERBIVORO";
        case PARASITA:        return "PARASITA";
        case NAO_SE_ALIMENTA: return "NAO SE ALIMENTA";
    }
    return "DESCONHECIDA";
}

static const char *ordem_str(Ordem o) {
    switch (o) {
        case CAOTICO:      return "CAOTICO";
        case LEAL:         return "LEAL";
        case NEUTRO_ORDEM: return "NEUTRO";
    }
    return "DESCONHECIDA";
}

static const char *moral_str(Moral m) {
    switch (m) {
        case BOM:          return "BOM";
        case MAU:          return "MAU";
        case NEUTRO_MORAL: return "NEUTRO";
    }
    return "DESCONHECIDA";
}

/* ---------- Helpers: structs ---------- */

/* Imprime "XdY+Z" (omite o bonus se for 0). Sem quebra de linha. */
static void print_dice(Dice d) {
    printf("%dd%d", d.dice_quantity, d.faces_of_dices);
    if (d.bonus != 0) {
        printf("%+d", d.bonus);
    }
}

static void print_attack_list(const AttackList *list) {
    if (list->quantidade == 0) {
        printf("NENHUM \n");
        return;
    }
    printf("\n");
    for (int i = 0; i < list->quantidade; i++) {
        printf("  - %s: ", list->itens[i].nome);
        print_dice(list->itens[i].dano);
        printf("\n");
    }
}

static void print_defence_list(const DefenceList *list) {
    if (list->quantidade == 0) {
        printf("NENHUMA \n");
        return;
    }
    printf("\n");
    for (int i = 0; i < list->quantidade; i++) {
        printf("  - %s (%d): %s \n",
               list->itens[i].nome,
               list->itens[i].valor,
               list->itens[i].descricao);
    }
}

/* ---------- Função principal ---------- */

void print_beast(Beast b) {
    int i;

    printf("ID: %d \n", b.id);
    printf("NOME: %s \n", b.nome);
    printf("DESCRICAO: %s \n", b.descricao);
    printf("FREQUENCIA: %s \n", freq_str(b.frequencia));

    printf("NUMERO ENCONTRADO: ");
    print_dice(b.numeroEncontrado);
    printf(" \n");

    /* Covil (vetor de inteiros de tamanho GRID) */
    printf("COVIL: ");
    for (i = 0; i < GRID; i++) {
        printf("%d%s", b.covil[i], (i < GRID - 1) ? ", " : "");
    }
    printf(" \n");

    /* Climas (imprime até a primeira string vazia) */
    printf("CLIMAS: ");
    for (i = 0; i < MAX_CLIMAS && b.climas[i][0] != '\0'; i++) {
        printf("%s%s", (i > 0) ? ", " : "", b.climas[i]);
    }
    printf(" \n");

    /* Terrenos (imprime até a primeira string vazia) */
    printf("TERRENOS: ");
    for (i = 0; i < MAX_TERRENOS && b.terrenos[i][0] != '\0'; i++) {
        printf("%s%s", (i > 0) ? ", " : "", b.terrenos[i]);
    }
    printf(" \n");

    printf("ORGANIZACAO: %s \n", b.organizacao);
    printf("ATIVIDADE: %s \n", active_str(b.atividade));
    printf("DIETA: %s \n", dieta_str(b.dieta));
    printf("INTELIGENCIA: %d \n", b.inteligencia);

    printf("TESOURO: pessoal=%d, covil=%d, masmorra=%d \n",
           b.tesouro.personal_treasure,
           b.tesouro.covil_treasure,
           b.tesouro.dungeon_treasure);

    printf("ALINHAMENTO: %s %s \n", ordem_str(b.ordem), moral_str(b.moralidade));
    printf("CA: %d \n", b.ca);

    /* Movimentos */
    printf("MOVIMENTOS: ");
    if (b.movimentos.quantidade == 0) {
        printf("NENHUM");
    }
    for (i = 0; i < b.movimentos.quantidade; i++) {
        printf("%s%s %d",
               (i > 0) ? ", " : "",
               b.movimentos.terrenos[i],
               b.movimentos.modificadores[i]);

        /* Se for voo, mostra a classe de manobrabilidade */
        if (strcmp(b.movimentos.terrenos[i], "VOO") == 0 &&
            b.movimentos.voo_acobracia != '\0') {
            printf(" (classe %c)", b.movimentos.voo_acobracia);
        }
    }
    printf(" \n");

    printf("DV: ");
    print_dice(b.dv);
    printf(" \n");

    printf("THAC0: %d \n", b.thac0);

    printf("ATAQUES: ");
    print_attack_list(&b.ataques);

    printf("ATAQUES ESPECIAIS: ");
    print_attack_list(&b.ataquesEspeciais);

    printf("DEFESAS ESPECIAIS: ");
    print_defence_list(&b.defesasEspeciais);

    printf("RESISTENCIA A MAGIA: %d%% \n", b.resistenciaMagia);
    printf("TAMANHO: %s \n", b.tamanho);

    printf("ALTURA: ");
    for (i = 0; i < VARIETY; i++) {
        printf("%.2f%s", b.altura[i], (i < VARIETY - 1) ? ", " : "");
    }
    printf(" \n");

    printf("MORAL: %d \n", b.moral);
    printf("CLASSIFICACAO MORAL: %s \n", b.classificacaoMoral);
    printf("XP: %d \n", b.xp);

    printf("PODER BASE: %d \n", b.poderBase);
    printf("MOD DIA: %d \n", b.modDia);
    printf("MOD NOITE: %d \n", b.modNoite);

    printf("MOD TERRENO: ");
    if (b.modTerreno.quantidade == 0) {
        printf("NENHUM");
    }
    for (i = 0; i < b.modTerreno.quantidade; i++) {
        printf("%s%s %d",
               (i > 0) ? ", " : "",
               b.modTerreno.itens[i].nome,
               b.modTerreno.itens[i].valor);
    }
    printf(" \n");

    printf("BONUS POR ADICIONAL: %d \n", b.bonusPorAdicional);
    printf("LIMITE GRUPO GRANDE: %d \n", b.limiteGrupoGrande);
    printf("BONUS GRUPO GRANDE: %d \n", b.bonusGrupoGrande);

    printf("DESCRICAO DE COMBATE: %s \n", b.descricaoCombate);
    printf("HABITAT E SOCIEDADE: %s \n", b.habitatSociedade);
    printf("ECOLOGIA: %s \n", b.descricaoEcologia);
}