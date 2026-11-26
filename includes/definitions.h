#ifndef __DEFINITIONS__
#define __DEFINITIONS__

/*
    * Esse macro foi gerado para fazer prints de debug mais facilmente @LucasAPV
*/
#define debug(fmt, ...) printf("\n[DEBUG] " fmt "\n", ##__VA_ARGS__)


#define OKAY (void*)1
#define ERROR (void*) 0

#define EASY_MIN 0.6
#define EASY_MAX 0.8
#define MEDIUM_MIN 0.9
#define MEDIUM_MAX 1.1
#define HARD_MIN 1.2
#define HARD_MAX 1.5


#define NAME_LENGHT 49
#define MAX_CHARACTERS 20
#define MAX_CAPACITY 50
#define TEXT_SIZE 256            /* era 100 */
#define MAX_PARTY_SIZE 6
#define MAX_ENCOUNTERS 3
#define TREASURE_SIZE 10
#define MAX_TEXT_LENGTH_MOB 256  /* era 50: as descricoes do exemplo tem ~80 bytes */
#define GRID 2
#define VARIETY 5
#define QTD_COLUMNS 38           /* era 37: o enum ColumnBeast tem 38 colunas */
#define MAX_CLIMAS 2
#define MAX_TERRENOS 5
#define MAX_SEPARETORS 6
#define LINE_BUFFER_SIZE 1024    /* tamanho maximo de uma linha do arquivo */
#endif