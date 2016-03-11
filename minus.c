#include <stdio.h>

#include "minus.h"


int main(int argc, char * argv[])
{
  processedProgram = NULL;
  stack = NULL;

  if(argc != 2) {
    error("Minus takes a single argument: the program file to run.\n");
  }
  
  FILE * file;
  file = fopen(argv[1], "r");
  if(file == NULL)
    error("Couldn't open file!\n");
  process(file);
  fclose(file);

  //printf("%s\n\n", processedProgram);
  
  runInit();
  while(1)
    runStep();
  
}


void closeAll()
{
  if(processedProgram != NULL)
    free(processedProgram);
  if(stack != NULL)
    free(stack);
}

void error(char * message)
{
  fprintf(stderr, message);
  closeAll();
  exit(1);
}

void programError(char * message)
{
  fprintf(stderr, "Error at %d with word %s : ", position, word);
  error(message);
}

void halt()
{
  printf("Done.\n");
  int i;
  while(!stackIsEmpty())
    printf("%f\n", stackPop());
  closeAll();
  exit(0);
}

unsigned long stringHash(unsigned char * string, int length)
{
  // http://www.cse.yorku.ca/~oz/hash.html
  unsigned long hash = 5381;
  int i;

  for(i = 0; i < length; i++) {
    int c = *(string + i);
    hash = ((hash << 5) + hash) + c; // hash * 33 + c
  }

  return hash;
}
