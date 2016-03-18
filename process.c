#include <stdio.h>

#include "process.h"


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
  lineIsEmpty = TRUE;
  processedProgramStream(&currentOutStream);

  processFile(file);
  
  processAddChar(0);
}


void processFile(FILE * file)
{
  bool inKeyword = FALSE;
  int wordLen = 0;
  
  int c;
  while((c = fgetc(file)) != EOF) {
    
    if(isWhitespace(c)) {
      whitespace = TRUE;
    }
    
    else if(c == '\n') {
      if(!lineIsEmpty) {
	flushWord(wordLen);
	wordLen = 0;
	processAddChar('\n');
      }
      lineIsEmpty = TRUE;
    }
    
    else if(c == '#' && lineIsEmpty) {
      while(c != EOF && c != '\n')
	c = fgetc(file);
    }
    
    else if(c == '"') {
      flushWhitespace(&wordLen);
      
      c = fgetc(file);
      bool addWhitespace = FALSE;
      while(c != EOF && c != '"') {
	lineIsEmpty = FALSE;

	if(addWhitespace)
	  processAddChar(' ');
	
        char cString[4];
	sprintf(cString, "%d", c);
	processAddString(cString, sizeof(cString) / sizeof(char));
        addWhitespace = TRUE;
	
	c = fgetc(file);
      }
      whitespace = FALSE;
    }

    else if(c == '`' && lineIsEmpty) {
      char fileName[256];
      int i;
      for(i = 0; i < sizeof(fileName)/sizeof(char) - 1; i++) {
	c = fgetc(file);
	if(c == EOF || c == '\n') {
	  fileName[i] = 0;
	  break;
	}
	fileName[i] = c;
      }
      fileName[sizeof(fileName)/sizeof(char)-1] = 0;
      FILE * newFile = readFile(fileName);
      processFile(newFile);
    }

    else if(c == '$') {
      if(inKeyword) { //complete the current keyword
        flushWord(wordLen);
	wordLen = 0;
	
	Keyword * kPtr = (Keyword *)(currentOutStream.data);
	Keyword k = *kPtr;
	processedProgramStream(&currentOutStream);
	inKeyword = FALSE;
	lineIsEmpty = TRUE;
      } else { // not in keyword, start a new one
	//allocate memory for a new keyword...
	if(numKeywords >= MAX_KEYWORDS)
	  error("Max keywords reached!");
	if(numKeywords >= keywordsSize) {
	  keywordsSize += KEYWORD_BLOCK;
	  keywords = (Keyword *)realloc(keywords,
					keywordsSize * sizeof(Keyword));
	  if(keywords == NULL)
	    error("Couldn't allocate keyword memory!");
	}
	numKeywords++;

	Keyword * k = &keywords[numKeywords - 1];

	//get name
	char name[KEYWORD_NAME_LEN];
	int i = 0;
	char c;
	while((c = fgetc(file)) != EOF
	      && !isWhitespace(c)
	      && c != '\n') {
	  if(i >= KEYWORD_NAME_LEN)
	    error("Keyword name too long!");
	  name[i++] = c;
	}
	name[i] = 0;
	(*k).nameHash = stringHash(name, i);

        keywordStream(k, &currentOutStream);
	inKeyword = TRUE;

	whitespace = FALSE;
	lineIsEmpty = FALSE;
      }
    }
    
    else {
      flushWhitespace(&wordLen);
      whitespace = FALSE;
      lineIsEmpty = FALSE;

      if(wordLen >= WORD_SIZE)
	error("Maximum word size reached!");
      word[wordLen++] = c;
    }
	
  }

  if(!lineIsEmpty) {
    flushWord(wordLen);
    processAddChar('\n'); // end with a newline
    lineIsEmpty = TRUE;
  }
}


void flushWhitespace(int * wordLen)
{
  if(whitespace == TRUE && !lineIsEmpty) {
    flushWord(*wordLen);
    *wordLen = 0;
    processAddChar(' ');
  }
}

void flushWord(wordLen)
{
  word[wordLen] = 0;
  unsigned long hash = stringHash(word, wordLen);
  int i;
  bool matchFound = FALSE;
  Keyword match;
  for(i = 0; i < numKeywords; i++) {
    Keyword k = keywords[i];
    if(k.nameHash == hash) {
      matchFound = TRUE;
      match = k;
    }
  }

  if(matchFound)
    processAddString(match.value, match.valueSize);
  else
    processAddString(word, WORD_SIZE);
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

bool isHex(char c)
{
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
}

char getHex(char c)
{
  if( c >= '0' && c <= '9' )
    return c - '0';
  if( c >= 'a' && c <= 'f' )
    return c - 'a' + 10;
  return -1;
}



void sputc(int c, OutStream out)
{
  (*(out.putchar))(c, out.data);
}

void processAddChar(char c)
{
  sputc(c, currentOutStream);
}

void processAddString(char * string, int maxLen) {
  int i;
  for(i = 0; i < maxLen; i++) {
    char c = string[i];
    if(c == 0)
      return;
    processAddChar(c);
  }
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


void keywordPutc(int c, void * data)
{
  Keyword * k = (Keyword *)data;
  if((*k).valueSize >= (*k).valueMaxSize) {
    (*k).valueMaxSize += KEYWORD_VALUE_BLOCK;
    (*k).value = realloc((*k).value, (*k).valueMaxSize * sizeof(char));
    if((*k).value == NULL)
      error("Couldn't allocate keyword memory!\n");
  }

  (*k).value[(*k).valueSize] = c;
  (*k).valueSize++;
}

void keywordStream(Keyword * k, OutStream * stream)
{
  (*stream).putchar = &keywordPutc;
  (*stream).data = (void *)k;
}
