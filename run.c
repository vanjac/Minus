#include <string.h>
#include <math.h>

#include "minus.h"


void runInit()
{
  stackInit();
  namespaceInit();
  nextLine();
}

void runStep()
{
  bool result = nextWord();
  if(wordLength >= WORD_SIZE)
    error("Word too long!\n");
  memcpy(word, processedProgram + wordIndex, wordLength * sizeof(char));
  word[wordLength] = 0;
  command();
  if(!result)
    halt();
}

bool nextLine()
{
  char * pos = strchr(processedProgram + position, '\n');
  if(pos == NULL) { //end of file
    position = processedProgramSize - 1;
    return FALSE;
  }
  position = (int)(pos - processedProgram - 1);
  return TRUE;
}

bool nextWord()
{
  int originalPosition = position;
  char * spacePos = NULL;
  bool isNewline = FALSE;
  char * i;
  for(i = processedProgram + position; i >= processedProgram; i--) {
    if(*i == ' ') {
      spacePos = i;
      break;
    }
    if(*i == '\n') {
      spacePos = i;
      isNewline = TRUE;
      break;
    }
  }
  if(spacePos == NULL) {
    spacePos = processedProgram - 1;
    isNewline = TRUE;
  }
  position = (int)(spacePos - processedProgram - 1);
  wordIndex = position + 2;
  wordLength = originalPosition - position - 1;
  if(isNewline) {
    position += 2;
    if(!nextLine())
      return FALSE;
    position += 2;
    if(!nextLine())
      return FALSE;
  }
  return TRUE;
}

Number pointerToNumber(Number * pointer)
{
  return (Number)(long)pointer / (Number)sizeof(Number);
}

Number * numberToPointer(Number n)
{
  return (Number *)(long)(n * sizeof(Number));
}
