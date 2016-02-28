#include "minus.h"

void stackInit()
{
  stackMemorySize = STACK_BLOCK;
  stack = (Number *) malloc(stackMemorySize * sizeof(Number));
  if(stack == NULL)
    error("Couldn't allocate stack!\n");
  stackSize = 0;
}

void stackPush(Number n)
{
  if(stackSize >= MAX_STACK_SIZE)
    error("Max stack size reached!\n");

  if(stackSize >= stackMemorySize) {
    stackMemorySize += STACK_BLOCK;
    stack = (Number *) realloc(stack, stackMemorySize * sizeof(Number));
    if(stack == NULL)
      error("Couldn't allocate stack!\n");
  }

  stack[stackSize++] = n;
}

Number stackPop()
{
  if(stackIsEmpty())
    return 0.0;
  return stack[--stackSize];
}

Number stackPopTest()
{
  if(stackIsEmpty()) {
    programError("Cannot remove from empty stack!\n");
    return 0.0;
  }
  return stackPop();
}

Number stackPeek()
{
  return stack[stackSize - 1];
}

Number stackPeekTest()
{
  if(stackIsEmpty()) {
    programError("Stack is empty!\n");
    return 0.0;
  }
  return stackPeek();
}

bool stackIsEmpty()
{
  return (stackSize == 0);
}
