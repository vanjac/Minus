all:
	cc -o minus minus.h minus.c process.c run.c commands.c -lm stack.c namespace.c
