#ifndef __BEASTS__
#define __BEASTS__

#include "definitions.h"
#include <stdint.h>

typedef enum {
    FREQ_MUITO_RARO,
    FREQ_RARO,
    FREQ_INCOMUM,
    FREQ_COMUM
} Freq;

typedef enum {
    QUALQUER_HORARIO,
    DIURNO,
    NOTURNO
} Active;

typedef enum {
    ONIVORO,
    CARNIVORO,
    HERBIVORO,
    PARASITA,
    NAO_SE_ALIMENTA
} Dieta;

typedef enum {
    CAOTICO,
    LEAL,
    NEUTRO_ORDEM
} Ordem;

typedef enum {
    BOM,
    MAU,
    NEUTRO_MORAL
} Moral;

/* "4d8+4" => quantity=4, faces=8, bonus=4 */
typedef struct {
    int dice_quantity;
    int faces_of_dices;
    int bonus;
} Dice;

typedef struct {
    int personal_treasure;
    int covil_treasure;
    int dungeon_treasure;
} Treasure;

typedef struct {
    /* Utilizo o MAX_TERRENOS para poder definir a quantidade de modificadores */
    char terrenos[MAX_TERRENOS][TEXT_SIZE];
    int modificadores[MAX_TERRENOS];
    char voo_acobracia;   /* classe de manobrabilidade do voo ('C' em VOO:15:C) */
    int quantidade;       /* quantos movimentos foram lidos */
} Moveset;

/* "GARRA:1d4" */
typedef struct {
    char nome[MAX_TEXT_LENGTH_MOB];
    Dice dano;
} Attack;

typedef struct {
    Attack itens[MAX_SEPARETORS];
    int quantidade;
} AttackList;

/* "PELE_PEDRA:1:Resiste a armas comuns" */
typedef struct {
    char nome[MAX_TEXT_LENGTH_MOB];
    int valor;
    char descricao[MAX_TEXT_LENGTH_MOB];
} Defence;

typedef struct {
    Defence itens[MAX_SEPARETORS];
    int quantidade;
} DefenceList;

/* "RUINAS:3" */
typedef struct {
    char nome[MAX_TEXT_LENGTH_MOB];
    int valor;
} TerrainMod;

typedef struct {
    TerrainMod itens[MAX_TERRENOS];
    int quantidade;
} TerrainModList;


typedef struct {
    int id;
    char nome[MAX_TEXT_LENGTH_MOB];
    char descricao[MAX_TEXT_LENGTH_MOB];
    Freq frequencia;
    Dice numeroEncontrado;
    int covil[GRID];
    char climas[MAX_CLIMAS][MAX_TEXT_LENGTH_MOB];
    char terrenos[MAX_TERRENOS][MAX_TEXT_LENGTH_MOB];
    char organizacao[MAX_TEXT_LENGTH_MOB];
    Active atividade;
    Dieta dieta;
    int inteligencia;
    Treasure tesouro;
    Ordem ordem;
    Moral moralidade;
    int ca;
    Moveset movimentos;
    Dice dv;
    int thac0;
    AttackList ataques;
    AttackList ataquesEspeciais;
    DefenceList defesasEspeciais;
    int resistenciaMagia;
    char tamanho[MAX_TEXT_LENGTH_MOB];
    float altura[VARIETY];
    int moral;
    char classificacaoMoral[MAX_TEXT_LENGTH_MOB];
    int xp;
    int poderBase;
    int modDia;
    int modNoite;
    TerrainModList modTerreno;
    int bonusPorAdicional;
    int limiteGrupoGrande;
    int bonusGrupoGrande;
    char descricaoCombate[MAX_TEXT_LENGTH_MOB];
    char habitatSociedade[MAX_TEXT_LENGTH_MOB];
    char descricaoEcologia[MAX_TEXT_LENGTH_MOB];
} Beast;

#define MAX_BEASTS 50

/* ATENCAO: sizeof(BeastDB) fica na casa de ~600 KB. Declare como global/static
   ou use malloc; evite deixar na pilha. */
typedef struct {
    Beast beasts[MAX_BEASTS];   /* guardadas na ordem em que foram lidas */
    int count;                  /* quantas estao preenchidas */
} BeastDB;

typedef enum ColumnBeast {
    COLUMN_UNKNOWN = -1,   /* linha sem '=' ou coluna desconhecida ([MONSTRO], etc.) */
    ID,
    NOME,
    DESCRICAO,
    FREQUENCIA,
    NUMERO_ENCONTRADO,
    COVIL,
    CLIMAS,
    TERRENOS,
    ORGANIZACAO,
    ATIVIDADE,
    DIETA,
    INTELIGENCIA,
    TESOURO,
    ORDEM,
    MORALIDADE,
    CA,
    MOVIMENTOS,
    DV,
    THAC_0,
    ATAQUES,
    ATAQUES_ESPECIAIS,
    DEFESAS_ESPECIAIS,
    RESISTENCIA_MAGIA,
    TAMANHO,
    ALTURA,
    MORAL,
    CLASSIFICACAO_MORAL,
    XP,
    PODER_BASE,
    MOD_DIA,
    MOD_NOITE,
    MOD_TERRENO,
    BONUS_POR_ADICIONAL,
    LIMITE_GRUPO_GRANDE,
    BONUS_GRUPO_GRANDE,
    DESCRICAO_COMBATE,
    HABITAT_SOCIEDADE,
    DESCRICAO_ECOLOGIA
} ColumnBeast;

typedef struct {
    const char* str;
    ColumnBeast beast;
} ColumnMap;

static const ColumnMap COLUMN_MAPS[] = {
    {"id", ID},
    {"nome", NOME},
    {"descricao", DESCRICAO},
    {"frequencia", FREQUENCIA},
    {"numeroEncontrado", NUMERO_ENCONTRADO},
    {"covil", COVIL},
    {"climas", CLIMAS},
    {"terrenos", TERRENOS},
    {"organizacao", ORGANIZACAO},
    {"atividade", ATIVIDADE},
    {"dieta", DIETA},
    {"inteligencia", INTELIGENCIA},
    {"tesouro", TESOURO},
    {"ordem", ORDEM},
    {"moralidade", MORALIDADE},
    {"ca", CA},
    {"movimentos", MOVIMENTOS},
    {"dv", DV},
    {"thac0", THAC_0},
    {"ataques", ATAQUES},
    {"ataquesEspeciais", ATAQUES_ESPECIAIS},
    {"defesasEspeciais", DEFESAS_ESPECIAIS},
    {"resistenciaMagia", RESISTENCIA_MAGIA},
    {"tamanho", TAMANHO},
    {"altura", ALTURA},
    {"moral", MORAL},
    {"classificacaoMoral", CLASSIFICACAO_MORAL},
    {"xp", XP},
    {"poderBase", PODER_BASE},
    {"modDia", MOD_DIA},
    {"modNoite", MOD_NOITE},
    {"modTerreno", MOD_TERRENO},
    {"bonusPorAdicional", BONUS_POR_ADICIONAL},
    {"limiteGrupoGrande", LIMITE_GRUPO_GRANDE},
    {"bonusGrupoGrande", BONUS_GRUPO_GRANDE},
    {"descricaoCombate", DESCRICAO_COMBATE},
    {"habitatSociedade", HABITAT_SOCIEDADE},
    {"descricaoEcologia", DESCRICAO_ECOLOGIA}
};

void  beast_db_init(BeastDB *b_db);
void* create_beast(BeastDB *b_db, int start_pos);    /* OKAY ou ERROR */
int   load_beasts(BeastDB *b_db);                    /* devolve quantas carregou */
Beast* get_beast_by_id(BeastDB *b_db, int id);       /* NULL se nao existir */


#endif