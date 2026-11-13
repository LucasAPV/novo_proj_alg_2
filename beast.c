#include "beast.h"
#include "parse_file.h"

void* create_beast(BeastDB *b_db, Beast *b){
   char** beast_def = next_beast_definition(int start_pos);
   parse_beast(beast_def, b);
   *b_db->beasts[*b.id]; 
}