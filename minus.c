#include <stdio.h>

#include "minus.h"


void readFile(char * fileName);


int main(int argc, char * argv[])
{
  originalProgram = NULL;
  processedProgram = NULL;
  stack = NULL;
  
  readFile(argv[1]);
  process();
  runInit();
  while(1)
    runStep();
  
}

void readFile(char * fileName)
{
  FILE * file;
  file = fopen(fileName, "r");
  if(file == NULL)
    error("Couldn't open file!\n");
  
  fseek(file, 0L, SEEK_END);
  int size = (int)ftell(file);
  originalProgramSize = size;
  // return to original position
  fseek(file, 0L, SEEK_SET);

  
  originalProgram = (char *)malloc(size*sizeof(char));
  if(originalProgram == NULL) {
    fclose(file);
    error("Couldn't allocate memory!\n");
  }

  if( fread(originalProgram, sizeof(char), size, file) != size) {
    fclose(file);
    error("Error reading file!\n");
  }

  fclose(file);
}



void closeAll()
{
  if(originalProgram != NULL)
    free(originalProgram);
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
