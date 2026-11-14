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
   cmd_append(
      &cmd, "gcc", "-Wall", "-Wextra",
      "-Iincludes" , "-o", OUTPUT_FOLDER"main",
      "main.c", "./utilitaries/error.c", "adventure.c", "item.c", "interface.c",
      "beast.c", "./utilitaries/parse_file.c", "./utilitaries/read_txt.c" /*Mais arquivos aqui*/
   );
   if (!cmd_run(&cmd)) return 1;

   return 0;
}