typedef struct {
  int (*getchar)(void * data);
  void * data;
} InStream;

typedef struct {
  void (*putchar)(int c, void * data);
  void * data;
} OutStream;

int sgetc(InStream in);
void sputc(int c, OutStream out);
