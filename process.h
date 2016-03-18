#include "minus.h"

typedef struct {
  void (*putchar)(int c, void * data);
  void * data;
} OutStream;


void flushWord();

void sputc(int c, OutStream out);
void processAddChar(char c);
void processAddString(char * string, int maxLen);

void processFile(FILE * file);

bool isWhitespace(char c); //not including newlines
bool isHex(char c); //is hex code
char getHex(char c); //get hex value of character
void processAddChar(char c);
void processAddString(char * string, int maxLen);

//OutStreams
void processedProgramPutc(int c, void * data);
void processedProgramStream(OutStream * stream);

void keywordPutc(int c, void * data);
void keywordStream(Keyword * k, OutStream * stream);


OutStream currentOutStream;

//preprocessor state
bool whitespace;
bool lineIsEmpty;
