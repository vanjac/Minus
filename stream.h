typedef struct {
  int (*getchar)(void * data);
  void * data;
} InStream;

typedef struct {
  void (*putchar)(int c, void * data);
  void * data;
} OutStream;

void sgetc(InStream stream);
void sputc(int c, OutStream out);
