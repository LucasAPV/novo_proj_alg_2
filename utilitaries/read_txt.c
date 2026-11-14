#include "read_txt.h"
#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* open_file(char* file_name, char* mode){
   FILE *f;
   f = fopen(file_name, mode);
   return f;
}

/*
   Le o arquivo POR LINHAS e devolve um vetor de strings terminado em NULL.

   intervalo = {linha_inicial, linha_final}, ambos INCLUSIVOS e contados a partir de 0
   (entao [a,a] devolve so a linha a). Se intervalo == NULL, devolve o arquivo todo.

   (A versao anterior tratava o intervalo como BYTES no fseek/fread, mas o
   next_beast_definition passava numeros de linhas - por isso nao funcionava.)

   Responsabilidade de liberar: quem chama deve usar free_file_text() no retorno
   e fclose() no arquivo.

   Obs.: linhas maiores que LINE_BUFFER_SIZE-1 caracteres seriam quebradas em duas.
*/
char** get_file_text(FILE *f, const int intervalo[2]) {
    if (f == NULL) return NULL;

    int start = 0;
    int end = -1;               /* -1 = ate o fim do arquivo */

    if (intervalo != NULL) {
        start = intervalo[0];
        end = intervalo[1];
        if (start < 0 || end < start) return NULL;
    }

    rewind(f);

    int capacity = 64;
    int count = 0;
    char **lines = (char **)malloc((capacity + 1) * sizeof(char *));
    if (lines == NULL) return NULL;

    char buffer[LINE_BUFFER_SIZE];
    int line_no = 0;

    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        if (end >= 0 && line_no > end) break;

        if (line_no >= start) {
            /* Remove '\n' e '\r' (arquivos CRLF do Windows) */
            buffer[strcspn(buffer, "\r\n")] = '\0';

            if (count == capacity) {
                capacity *= 2;
                char **tmp = (char **)realloc(lines, (capacity + 1) * sizeof(char *));
                if (tmp == NULL) {
                    lines[count] = NULL;
                    free_file_text(lines);
                    return NULL;
                }
                lines = tmp;
            }

            size_t len = strlen(buffer);
            char *copy = (char *)malloc(len + 1);
            if (copy == NULL) {
                lines[count] = NULL;
                free_file_text(lines);
                return NULL;
            }
            memcpy(copy, buffer, len + 1);
            lines[count++] = copy;
        }
        line_no++;
    }

    lines[count] = NULL;
    return lines;
}

void free_file_text(char **lines) {
    if (lines == NULL) return;
    for (int i = 0; lines[i] != NULL; i++) free(lines[i]);
    free(lines);
}


ColValue line_to_struct(char* col_buffer, char* value_buffer) {
   ColValue cv;
   memset(&cv, 0, sizeof(cv));
   cv.value = value_buffer;
   cv.col_name = COLUMN_UNKNOWN; // Caso nao ache

   // Calcula size da tabela
   size_t num_maps = sizeof(COLUMN_MAPS) / sizeof(COLUMN_MAPS[0]);

   // Percorre a tabela buscando a correspondencia
   for (size_t i = 0; i < num_maps; i++) {
      if (strcmp(col_buffer, COLUMN_MAPS[i].str) == 0) {
         cv.col_name = COLUMN_MAPS[i].beast;
         break;
      }
   }

   return cv;
}

/*
   "nome=Gargula" => col_name = NOME, value = "Gargula"

   O value aponta para DENTRO da propria linha (logo depois do primeiro '='),
   entao nao ha malloc nem free. Ele so vale enquanto a linha existir
   (isto e, ate o free_file_text).
   Linhas sem '=' (ex.: "[MONSTRO]") voltam com COLUMN_UNKNOWN.
*/
ColValue parse_line(char* line){
    char col_buffer[TEXT_SIZE];
    ColValue cv;
    memset(&cv, 0, sizeof(cv));
    cv.col_name = COLUMN_UNKNOWN;
    cv.value = (char*)"";

    char* eq = strchr(line, '=');
    if (eq == NULL) return cv;

    size_t col_len = (size_t)(eq - line);
    if (col_len >= TEXT_SIZE) return cv;

    memcpy(col_buffer, line, col_len);
    col_buffer[col_len] = '\0';

    return line_to_struct(col_buffer, eq + 1);
}