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
- Strings surrounded by quotes `"like this"` are replaced with Minus code that allocates memory for the string, copies the characters to memory, and returns a pointer. This memory must be manually freed with `,` - see below.

##Minus Commands
Once a Minus program has been preprocessed, execution begins starting with the first line of the program. These are the words understood by the interpreter.
- `123` adds the number 123 to the stack. Numbers can have negative signs and decimal points in them.
- `;` removes the top number on the stack.
- `+varName` creates a variable named `varName` in the current namespace, and sets its value to the top number on the stack, without removing it.
- `=varName` sets the value of an already-existing variable in any namespace to the top number on the stack, without removing it.
- `varName` adds the value of the variable, found in any namespace, to the stack.
- `{` increments the namespace counter by 1. Before execution continues, `|` lines are searched for in the current namespace (but not subnamespaces) - see below.
- `}` decrements the namespace counter by one, and deletes all variables associated with the namespace.
- `|` indicates that from here to the beginning of the line should be the first code run in this namespace. There can be multiple of these lines.
- `@` adds the program counter position to the stack.
- `!` removes the top number on the stack and sets the program counter to that position. The first word there is skipped before execution continues.
- `-` removes two stack items and subtracts the second from the first.
- `/` removes two stack items and divides the first by the second.
- `%` removes two stack items and finds the remainder when the first is divided by the second.
- `>` removes two stack items and adds a non-zero value if the first is greater than the second - otherwise it adds 0.
- `?` removes three stack items. If the first is non-zero, it adds the second back onto the stack - otherwise it adds the third.
- `.` removes the top stack item, allocates that many Numbers' worth of memory, and adds a pointer to the stack. All pointers are in increments of Number-sizes.
- `,` removes a pointer from the stack and frees the memory it points to.
- `..` removes a pointer and a number to the stack, and `realloc`'s the memory at that pointer to be the specified size. It adds a new pointer or the same pointer back onto the stack.
- `^` removes a pointer from the stack and gets the number value at that memory location. 1 is a special location - `^ 1` returns the next character on the input stream. `^ 0` causes an error.
- `:` removes a pointer and a number from the stack and puts the number at that memory location. The number is then added back onto the stack. 1 and 2 are special locations - `: 1 value` sends a character to the output stream, and `: 2 value` sends a character to the error stream. `: 0 value` causes an error.
