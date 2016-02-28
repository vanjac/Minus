#include <string.h>

#include "minus.h"


void setupNamespace();


void namespaceInit()
{
  namespace = 0;
  setupNamespace();
}

void incNamespace()
{
  if(namespace == MAX_NAMESPACES - 1) {
    programError("Max namespace reached!\n");
    return;
  }
  namespace++;
  setupNamespace();
}

void decNamespace()
{
  if(namespace == 0) {
    programError("Already at 0th namespace!\n");
    return;
  }
  namespace--;
}


void setupNamespace()
{
  namespaces[namespace].numVars = 0;
}


bool makeVar(char * name, Number value)
{
  NamespaceState * n = &namespaces[namespace];
  if((*n).numVars == MAX_VARS) {
    programError("Max variable limit reached!\n");
    return;
  }

  unsigned long hash = stringHash(name, strlen(name));
  
  //variable already exists in this namespace?
  unsigned int v;
  for(v = 0; v < (*n).numVars; v++) {
    if((*n).vars[v].nameHash == hash) {
      programError("Variable already exists!\n");
      return;
    }
  }

  Variable * var = &(*n).vars[(*n).numVars++];
  (*var).nameHash = hash;
  (*var).value = value;
}

Variable * findVar(char * name)
{
  unsigned long hash = stringHash(name, strlen(name));
  Namespace n;
  
  for(n = namespace; n != (Namespace)-1; n--) {
    NamespaceState state = namespaces[n];
    unsigned int v;
    for(v = 0; v < state.numVars; v++) {
      if(state.vars[v].nameHash == hash)
	return &namespaces[n].vars[v]; //pointer to Variable
    }
    
  }
  
  return NULL;
}
