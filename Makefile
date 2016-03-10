all:
	cc -o minus minus.h stream.h minus.c process.c run.c commands.c -lm stack.c namespace.c
