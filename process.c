#include "minus.h"
#include "stream.h"

void processAddChar(char c);
bool isWhitespace(char c); //not including newlines

void process()
{
  processedProgramMaxSize =
    PROCESSED_PROGRAM_BLOCK;
  processedProgram =
    (char *) malloc(processedProgramMaxSize * sizeof(char));
  if(processedProgram == NULL)
    error("Couldn't allocate preprocess memory!\n");
  processedProgramSize = 0;

  bool whitespace = TRUE;
  bool newline = TRUE;
  bool lineIsEmpty = TRUE;
  bool inComment = FALSE;
  int i;
  for(i = 0; i < originalProgramSize; i++) {
    char c = originalProgram[i];
    
    if(inComment) {
      if(c == '\n')
	inComment = FALSE;
    }
    else if(isWhitespace(c)) {
      whitespace = TRUE;
    }
    else if(c == '\n') {
      if(!lineIsEmpty)
	processAddChar('\n');
      inComment = FALSE;
      newline = TRUE;
      lineIsEmpty = TRUE;
    }
    else if(c == '#' && lineIsEmpty) {
      inComment = TRUE;
    }
    else {
      if(whitespace == TRUE && !lineIsEmpty)
	processAddChar(' ');
      whitespace = FALSE;
      newline = FALSE;
      lineIsEmpty = FALSE;
      processAddChar(c);
    }
	
  }

  if(!lineIsEmpty)
    processAddChar('\n'); // end with a newline
  
  processAddChar(0);
}

void processAddChar(char c)
{
  if(processedProgramSize >= processedProgramMaxSize) {
    processedProgramMaxSize +=
      PROCESSED_PROGRAM_BLOCK;
    processedProgram =
      realloc(processedProgram, processedProgramMaxSize * sizeof(char));
    if(processedProgram == NULL)
      error("Couldn't allocate preprocess memory!\n");
  }

  processedProgram[processedProgramSize] = c;
  processedProgramSize++;
}

bool isWhitespace(char c) {
  switch(c) {
  case ' ':
    return TRUE;
  case '\t':
    return TRUE;
  case '(':
    return TRUE;
  case ')':
      return TRUE;
  default:
    return FALSE;
  }
}
