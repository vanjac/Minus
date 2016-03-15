#include <stdlib.h>
#include <stdio.h>

//maximum characters in a word
#define WORD_SIZE 4096

//size of memory allocated at once
#define PROCESSED_PROGRAM_BLOCK 2048
#define STACK_BLOCK 2048

//max items in the stack -- could limit recursion or string lengths
#define MAX_STACK_SIZE 131072
//max namespaces -- could limit recursion
#define MAX_NAMESPACES 256
//max variables in a namespace
#define MAX_VARS 256

#define KEYWORD_NAME_LEN 64
#define KEYWORD_BLOCK 64 //num keywords allocated at a time

typedef enum { FALSE = 0, TRUE } bool;
typedef double Number;
typedef unsigned char Namespace;

typedef struct {
  unsigned long nameHash;
  Number value;
} Variable;

typedef struct {
  Variable vars[MAX_VARS];
  unsigned int numVars;
} NamespaceState;

typedef struct {
  char name[KEYWORD_NAME_LEN];
  char * value; //null-terminated
  int valueSize; //size of allocated memory
} Keyword;

char * processedProgram;
unsigned int processedProgramSize;
unsigned int processedProgramMaxSize; //size of allocated memory

int position; //offset from processedProgram, should point to the end of a word
int wordIndex, wordLength;
char word[WORD_SIZE];
Namespace namespace;
NamespaceState namespaces[MAX_NAMESPACES];

Number * stack;
unsigned int stackSize; // in Numbers
unsigned int stackMemorySize;

Keyword * keywords;
int numKeywords;
int keywordsSize; //size of allocated memory


unsigned long stringHash(unsigned char * string, int length);

FILE * readFile(char * name);

void closeAll();
void error();
void programError();
void halt();

void process(FILE * file);
void runInit();
void runStep();

void commandInit();
void command();

bool nextLine(); //false if end of program
bool nextWord(); //false if end of program

Number pointerToNumber(Number * pointer);
Number * numberToPointer(Number n);

void stackInit();
void stackPush(Number n);
Number stackPop();
//before popping, check if the stack is empty -- if it is, fail
Number stackPopTest();
Number stackPeek();
Number stackPeekTest();
bool stackIsEmpty();

void namespaceInit();
void incNamespace();
void decNamespace();
//search for variable in all namespaces, starting with highest-numbered
//return NULL if not found
Variable * findVar(char * name);
bool makeVar(char * name, Number value); //TRUE if var already exists
