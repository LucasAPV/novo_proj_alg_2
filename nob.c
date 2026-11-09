#define NOB_IMPLEMENTATION
#define OUTPUT_FOLDER "./output/"
#include "nob.h"

int main(int argc, char **argv)
{
   GO_REBUILD_URSELF(argc, argv);
   Cmd cmd = {0};

   cmd_append(&cmd, "mkdir", "-p", OUTPUT_FOLDER);
   if (!cmd_run(&cmd)) return 1;


   /* Para colocar mais arquivos, basta coloca os nomes deles em cada string */
   cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-o", OUTPUT_FOLDER"main", "main.c" /*Mais arquivos aqui*/);
   if (!cmd_run(&cmd)) return 1;

   return 0;
}