#include "stream.h"

void sputc(int c, OutStream out)
{
  (*(out.putchar))(c, out.data);
}
