#include "read_txt.h"
#include "definitions.h"
#include <string.h>

FILE* open_file(char* file_name, char* mode){
   FILE *f;
   f = fopen(file_name, mode);
   return f;
}

/*
   Estou passando a responsabilidade de free do arquivo para o caller desta funcao,
   ou seja, quem chama tem que liberar o arquivo depois.

   Alem do mais, acredito que se for passar o intervalo [a,a] ele pega a linha a
*/

char** get_file_text(FILE *f, const int intervalo[2]) {
    if (f == NULL) return NULL;

    char *buffer = NULL;
    long size_to_read = 0;

    if (intervalo != NULL) {
        int start = intervalo[0];
        int end = intervalo[1];

        if (end < start) return NULL;
        fseek(f, start, SEEK_SET);
        size_to_read = end - start;

        buffer = (char *)malloc(size_to_read + 1);
        if (buffer == NULL) return NULL;

        size_t been_read = fread(buffer, 1, size_to_read, f);
        buffer[been_read] = '\0';
    } else {
        fseek(f, 0, SEEK_END);
        long tamanho_total = ftell(f);
        rewind(f);

        buffer = (char *)malloc(tamanho_total + 1);
        if (buffer == NULL) return NULL;

        size_t lidos = fread(buffer, 1, tamanho_total, f);
        buffer[lidos] = '\0';
    }

    // 1. Conta o número de linhas no buffer
    int num_lines = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '\n') {
            num_lines++;
        }
    }
    // Se o buffer não estiver vazio e o último caractere não for '\n', conta a última linha
    if (buffer[0] != '\0' && buffer[strlen(buffer) - 1] != '\n') {
        num_lines++;
    }

    // Se o arquivo estiver vazio
    if (num_lines == 0) {
        free(buffer);
        // Retorna um vetor contendo apenas NULL
        char **empty_lines = (char **)malloc(sizeof(char *));
        if (empty_lines) empty_lines[0] = NULL;
        return empty_lines;
    }

    // 2. Aloca o vetor de ponteiros para as strings (+1 para o ponteiro NULL no final)
    char **lines = (char **)malloc((num_lines + 1) * sizeof(char *));
    if (lines == NULL) {
        free(buffer);
        return NULL;
    }

    // 3. Separa o buffer em linhas individuais
    int line_idx = 0;
    char *cursor = buffer;
    char *next_newline = NULL;

    while ((next_newline = strchr(cursor, '\n')) != NULL) {
        size_t line_len = next_newline - cursor;

        lines[line_idx] = (char *)malloc(line_len + 1);
        if (lines[line_idx] == NULL) {
            // Em caso de falha, limpa tudo o que já foi alocado para evitar vazamento de memória
            for (int j = 0; j < line_idx; j++) free(lines[j]);
            free(lines);
            free(buffer);
            return NULL;
        }

        memcpy(lines[line_idx], cursor, line_len);
        
        // Remove o '\r' caso o arquivo venha de sistemas Windows (CRLF)
        if (line_len > 0 && lines[line_idx][line_len - 1] == '\r') {
            lines[line_idx][line_len - 1] = '\0';
        } else {
            lines[line_idx][line_len] = '\0';
        }

        line_idx++;
        cursor = next_newline + 1;
    }

    // Trata a última linha caso o arquivo não termine com '\n'
    if (*cursor != '\0') {
        size_t line_len = strlen(cursor);
        lines[line_idx] = (char *)malloc(line_len + 1);
        if (lines[line_idx] != NULL) {
            strcpy(lines[line_idx], cursor);
            // Remove '\r' se houver
            size_t len = strlen(lines[line_idx]);
            if (len > 0 && lines[line_idx][len - 1] == '\r') {
                lines[line_idx][len - 1] = '\0';
            }
            line_idx++;
        }
    }

    // Marca o fim do vetor de strings com NULL
    lines[line_idx] = NULL;

    // O buffer temporário não é mais necessário, pois copiamos os dados para as linhas
    free(buffer);

    return lines;
}


ColValue line_to_struct(char* col_buffer, char* value_buffer) {
   ColValue cv = {0};
   cv.value = value_buffer;
   cv.col_name = (ColumnBeast)-1; // Caso nao ache

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

ColValue parse_line(char* line){
ColValue cv = {0};
char* col_buffer = (char*)malloc(sizeof(char)*TEXT_SIZE);
char* value_buffer = (char*)malloc(sizeof(char)*TEXT_SIZE);
int found_separator = 0;
int counter = 0;

for(int i = 0; i < TEXT_SIZE; ++i){
    if(strcmp(&line[i], "\0") == 0) break;

    if(strcmp(&line[i], "=") == 0) found_separator = 1;

    if(found_separator == 0) {
        col_buffer[i] = line[i];
    } else {
        value_buffer[counter] = line[i];
        counter++;
    }
}
cv = line_to_struct(col_buffer,value_buffer);

return cv;
}
