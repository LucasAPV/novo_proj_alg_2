#include "error.h"
#include "basic_imports.h"


void log_error(int err) {
   switch ((Error)err)
   {
   case INVALID_ARRAY_POSITION:
      fprintf(stderr, "\n\nERROR: INVALID ARRAY POSITION!!! FILE:%s == LINE:%d\n\n", __FILE__, __LINE__);
      break;

   case INVALID_ENTRY:
      fprintf(stderr, "\n\nERROR: INVALID ENTRY!!! FILE:%s == LINE:%d \n\n", __FILE__, __LINE__);
   break;

   case INEXISTENT_OBJECT:
      fprintf(stderr, "\n\nERROR: INVALID OBJECT!!! FILE:%s == LINE:%d \n\n", __FILE__, __LINE__);
   break;

   case NOT_ENOUGH_SPACE:
      fprintf(stderr, "\n\nERROR: NOT ENOUGH SPACE IN INVENTORY!!! FILE:%s == LINE:%d \n\n", __FILE__, __LINE__);
   break;

   case UNABLE_TO_CREATE_BEAST_DB:
       fprintf(stderr, "\n\nERROR: UNABLE TO CREATE BEAST DB!!! FILE:%s == LINE:%d \n\n", __FILE__, __LINE__);
   break;

   case ERROR_DELETE_ON_MONSTER:
       fprintf(stderr, "\n\nERROR: UNABLE TO DELETE BEAST!!! FILE:%s == LINE:%d \n\n", __FILE__, __LINE__);
   break;
   default:
      printf("UNKNOW ERROR!!! %s %d\n", __FILE__, __LINE__);
      break;
   }
}