#include <stdio.h>

#include "minus.h"
#include "stream.h"

void processFile(FILE * file);

bool isWhitespace(char c); //not including newlines
void processAddChar(char c);

//OutStreams
void processedProgramPutc(int c, void * data);
void processedProgramStream(OutStream * stream);


OutStream currentOutStream;

//preprocessor state
bool whitespace;
bool newline;
bool lineIsEmpty;


void process(FILE * file)
{
  processedProgramMaxSize =
    PROCESSED_PROGRAM_BLOCK;
  processedProgram =
    (char *) malloc(processedProgramMaxSize * sizeof(char));
  if(processedProgram == NULL) {
    error("Couldn't allocate preprocess memory!\n");
    return;
  }
  processedProgramSize = 0;


  whitespace = TRUE;
  newline = TRUE;
  lineIsEmpty = TRUE;
  processedProgramStream(&currentOutStream);

  processFile(file);

  if(!lineIsEmpty)
    processAddChar('\n'); // end with a newline
  
  processAddChar(0);
}

void processFile(FILE * file)
{
  int c;
  while((c = fgetc(file)) != EOF) {
    if(isWhitespace(c)) {
      whitespace = TRUE;
    }
    else if(c == '\n') {
      if(!lineIsEmpty)
	processAddChar('\n');
      newline = TRUE;
      lineIsEmpty = TRUE;
    }
    else if(c == '#' && lineIsEmpty) {
      while(c != EOF && c != '\n')
	c = fgetc(file);
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
}


bool isWhitespace(char c)
{
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

void processAddChar(char c)
{
  sputc(c, currentOutStream);
}



void processedProgramPutc(int c, void * data)
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

void processedProgramStream(OutStream * stream)
{
  (*stream).putchar = &processedProgramPutc;
  (*stream).data = NULL;
}
