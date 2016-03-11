typedef struct {
  void (*putchar)(int c, void * data);
  void * data;
} OutStream;

void sputc(int c, OutStream out);
