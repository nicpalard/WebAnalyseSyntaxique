#include "web.h"
#include <stdlib.h>
#include <stdio.h>

int yyparse(void);


int main(void)
{
  yyparse();
  return 0;
}

