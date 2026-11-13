#ifndef __PARSE_FILE__
#define __PARSE_FILE__
#define FILE_PATH "../beastiary/bestiario.txt"
#include "beasts.h"

char** next_beast_definition(int start_pos);

/*
   Essa funcao vai permitir transformar o texto na estrutura em C do monstro
*/
/*
[TODO]
   title="parse_beast"
   relevance=Importante
[/TODO]
*/
void parse_beast(char** beast_def, Beast *b);



#endif