#include <stdio.h>
#include <math.h>

#include "minus.h"

bool isNumber(char c); //if this could be the start of a number
                       //includes '-' and '.'
bool isDigit(char c);
bool isLetter(char c);

int lastNamespacePosition;
Namespace searchNamespace;

// the current state of the interpreter
enum { READ, BRACKET_SEARCH, BRACKET_SKIP, BRACKET_READ } state;
// READ: read/interpret commands normally
// BRACKET_SEARCH: search for bracketed code to read
// BRACKET_SKIP: skipping code inside brackets
// BRACKET_READ: at start of namespace, running code inside brackets

void commandInit()
{
  state = READ;
}


void command()
{
  if(wordLength == 0)
    return;

  if(state == BRACKET_SKIP) {
    if(FIRST_WORD_CHAR == ']')
      state = READ;
    return;
  }

  if(state == BRACKET_SEARCH) {
    switch(FIRST_WORD_CHAR) {
    case '{':
      searchNamespace++;
      return;
    case '}':
      if(searchNamespace == 0) {
	position = lastNamespacePosition;
	state = READ;
	return;
      } else {
	searchNamespace--;
	return;
      }
    case '[':
      if(searchNamespace == 0)
	state = BRACKET_READ;
      return;
    default:
      return;
    }
  }
  
  if(isNumber(FIRST_WORD_CHAR) && isDigit(LAST_WORD_CHAR)) {
    Number n = strtod(word, NULL);
    stackPush(n);
    return;
  }
  if(isLetter(FIRST_WORD_CHAR)) {
    Variable * var = findVar(word);
    if(var == NULL) {
      programError("Variable doesn't exist!\n");
      return;
    }
    stackPush((*var).value);
    return;
  }
  

  Number a, b, c;
  int i;
  
  switch(FIRST_WORD_CHAR) {
  
  case ';':
    stackPopTest();
    return;

  case '@':
    stackPush(position);
    return;

  case '!': //TODO: test
    a = stackPopTest();
    if(a > processedProgramSize || a < 0)
      programError("Invalid jump location!\n");
    position = a;
    bool result = nextWord();
    if(!result)
      halt();
    return;

  case ':': // set LOCATION to VALUE
    a = stackPopTest();
    b = stackPeekTest();
    if(a <= 0) {
      return;
    } else if(a == 1) {
      putchar(b);
      return;
    } else if(a == 2) {
      putc(b, stderr);
      return;
    } else {
      * numberToPointer(a) = b;
      return;
    }

  case '^':
    a = stackPopTest();
    if(a == 1) {
      stackPush(getchar());
      return;
    } else {
      stackPush( * numberToPointer(a) );
      return;
    }

  case '[':
    state = BRACKET_SKIP;
    return;

  case ']':
    if(state == BRACKET_READ)
      state = BRACKET_SEARCH;
    return;

  case '.':
    a = stackPopTest();
    stackPush(pointerToNumber( malloc(a * sizeof(Number)) ));
    return;

  case ',':
    if(wordLength == 1) {
      a = stackPeekTest();
      free( numberToPointer(a) );
      return;
    }

  case '{':
    incNamespace();
    searchNamespace = 0;
    lastNamespacePosition = position;
    state = BRACKET_SEARCH;
    return;
  case '}':
    decNamespace();
    return;

  case '+':
    a = stackPeekTest();
    makeVar(word + 1, a);
    return;
  case '=':
    a = stackPeekTest();
    Variable * v = findVar(word + 1);
    if(v == NULL) {
      programError("Variable doesn't exist!\n");
      return;
    }
    (*v).value = a;
    return;
  
  case '-':
    a = stackPopTest();
    b = stackPopTest();
    stackPush(a - b);
    return;
  case '/':
    a = stackPopTest();
    b = stackPopTest();
    stackPush(a / b);
    return;
  case '%':
    a = stackPopTest();
    b = stackPopTest();
    stackPush(fmod(a, b)); //TODO: use remainder() instead?
    return;
  case '>':
    a = stackPopTest();
    b = stackPopTest();
    stackPush(a > b);
    return;
  case '?':
    a = stackPopTest();
    b = stackPopTest();
    c = stackPopTest();
    stackPush(a ? b : c);
    return;
  }
  
  programError("Unknown word!\n");
  
}

bool isNumber(char c)
{
  return isDigit(c) || c == '-' || c == '.';
}

bool isDigit(char c)
{
  return (c >= '0') && (c <= '9');
}

bool isLetter(char c)
{
  return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'));
}

