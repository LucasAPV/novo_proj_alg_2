#ifndef __READ_TXT__
#define __READ_TXT__

#include "beasts.h"
#include "basic_imports.h"
typedef struct ColValue {
   ColumnBeast col_name;
   char* value;
} ColValue;

FILE* open_file(char* file_name, char* mode);
char** get_file_text(FILE *f, const int intervalo[2]);
ColValue parse_line(char* line);
void free_file_text(char **lines);
#endif

