# Minus Interpreter
An interpreter for Minus, a minimalist programming language (unrelated to [this](http://www.golfscript.com/minus/) Minus). See `language.md` for a description of the language and syntax.

This interpreter is still a work in progress. Not all Minus features have been implemented yet - specifically, the commands `[` and `]` do not fully work, and most of the preprocessor features are missing.

`make` compiles the interpreter into the same directory. To run Minus with a file, use `./minus path/to/file`.
