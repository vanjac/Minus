#include "minus.h"

typedef struct {
  void (*putchar)(int c, void * data);
  void * data;
} OutStream;


#define KEYWORD_NAME_LEN 64
#define KEYWORD_BLOCK 64 //num keywords allocated at a time
#define KEYWORD_VALUE_BLOCK 256 //characters in the value allocated at a time
#define MAX_KEYWORDS 4096

typedef struct {
  unsigned long nameHash;
  char * value; //null-terminated
  unsigned int valueSize; //length of string
  unsigned int valueMaxSize;
} Keyword;

Keyword * keywords;
int numKeywords;
int keywordsSize; //size of allocated memory


//flush word if it needs to be flushed after whitespace, clear wordLen
void flushWhitespace(int * wordLen); //pointer to wordlen variable

//flush the word in the `word` variable
//if the word matches a keyword, add that keyword instead
void flushWord(int wordLen);

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
