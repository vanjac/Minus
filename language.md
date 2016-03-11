#Minus
Minus is designed to be as simple to implement as possible, while still being mostly comfortable to program in. It has 20 commands, and 4 constructs interpreted by the preprocessor.

##Minus Concepts
There is a single datatype in Minus - the Number (defined as a `double` in the C interpreter). All Minus commands operate on the Stack - a stack of Numbers.

Minus programs are sequences of "words," separated by whitespace. These words are read by the interpreter in an unusual way - lines are read in order, top to bottom (with exceptions), but words on the line are read right to left. This allows more readability for commands that operate on the stack - instead of being written as postfix commands, like `5 6 -`, they can be written as prefix commands, like `- 5 6`. (Or `(- 5 6)` or `-(5 6)`. Parenthesis are ignored).

##Preprocessor Rules
Before being run, a Minus program is preprocessed. The main purpose of the Minus preprocessor is to remove unnecessary whitespace - spaces, tabs, newlines, and parenthesis are all considered whitespace (parenthesis have no effect on program flow, they are only for organization). The preprocessor also looks for some other patterns:
- Lines that begin with a `#` are ignored (commented)
- Lines that begin with a `$` and follow the pattern `$name value $` are treated as keyword definitions. If `name` (without the leading `$`) is found as a separate word (surrounded by whitespace) anywhere in the program past that point, it will be replaced with `value`. `value` can contain whitespace and even span multiple lines.
- Lines that begin with a `` ` `` and follow the pattern `` `path/to/file `` are replaced with the contents of the file found at `path/to/file`. File paths follow Unix file path conventions.
- Strings surrounded by quotes `"like this"` are replaced with Minus code that adds each character to the stack. Escape characters can be used in strings with `\00`, where `00` is a sequence of 2 hex digits representing the ASCII character with that value.

##Minus Commands
Once a Minus program has been preprocessed, execution begins starting with the first line of the program. These are the words understood by the interpreter.
- `123` adds the number 123 to the stack. Numbers can optionally begin with a negative sign, and can have an optional decimal point in them.
- `;` removes the top number on the stack.
- `+varName` creates a variable named `varName` in the current namespace, and sets its value to the top number on the stack, without removing it. Two variables of the same name cannot exist in the same namespace, but can exist in different namespaces - in this case, the variable in the highest-numbered namespace is used.
- `=varName` sets the value of an already-existing variable to the top number on the stack, without removing it.
- `varName` adds the value of the variable to the stack.
- `{` increments the namespace counter by 1. Before execution continues, bracketed lines are searched for in the current namespace (but not subnamespaces) and executed - see below.
- `}` decrements the namespace counter by 1, and deletes all variables associated with the namespace.
- `[` and `]` characters mark the beginning and end of code that should be run before anything else in the namespace. Once execution actually reaches this point, these sections are skipped. There can be any number of these sections in a namespace.
- `@` adds the program counter position to the stack.
- `!` removes the top number on the stack and sets the program counter to that position. The first word there is skipped before execution continues.
- `-` removes two stack items and subtracts the second from the first. The result is then added to the stack.
- `/` removes two stack items and divides the first by the second. The result is then added to the stack.
- `%` removes two stack items and finds the remainder when the first is divided by the second. The result is then added to the stack.
- `>` removes two stack items and adds a non-zero value if the first is greater than the second - otherwise it adds 0.
- `?` removes three stack items. If the first is non-zero, it adds the second back onto the stack - otherwise it adds the third.
- `.` removes the top stack item, allocates that many Numbers' worth of memory, and adds a pointer to the stack. All pointers are in increments of Number-sizes.
- `,` frees the memory the top number in the stack points to. Nothing is removed from the stack.
- `^` removes a pointer from the stack and adds the number value at that memory location to the stack. 1 is a special location - `^ 1` returns the next character on the input stream. `^ 0` causes an error.
- `:` removes a pointer and a number from the stack and stores the number at that memory location. The number is then added back onto the stack. 1 and 2 are special locations - `: 1 value` sends a character to the output stream, and `: 2 value` sends a character to the error stream. `: 0 value` causes an error.
