#ifndef __ERRORS__
#define __ERRORS__

typedef enum {
   INVALID_ENTRY,
   INVALID_ARRAY_POSITION,
   INEXISTENT_OBJECT,
   NOT_ENOUGH_SPACE,
   UNABLE_TO_SWITCH_ITENS
} Error;

void log_error(int err);

#endif