#include <stdlib.h>

//maximum characters in a word
#define WORD_SIZE 1024

//size of memory allocated at once
#define PROCESSED_PROGRAM_BLOCK 1024
#define STACK_BLOCK 256

//max items in the stack -- could limit recursion
#define MAX_STACK_SIZE 2048
//max namespaces -- could limit recursion
#define MAX_NAMESPACES 256
//max variables in a namespace
#define MAX_VARS 256

#define FIRST_WORD_CHAR (word[0])
#define LAST_WORD_CHAR (word[wordLength-1])

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

char * originalProgram;
unsigned int originalProgramSize;
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


unsigned long stringHash(unsigned char * string, int length);

void closeAll();
void error();
void programError();
void halt();

void process();
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
