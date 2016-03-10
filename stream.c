#include "stream.h"

int sgetc(InStream in)
{
  return (*(in.getchar))(in.data);
}
void sputc(int c, OutStream out)
{
  (*(out.putchar))(c, out.data);
}
