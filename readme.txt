Huy Le
comp525: construction of compilers
Project 2: Lexical Analyzer

This program will take in a .c file and lexically analyze read the contents of the file. The program will then print
out the contents sorted based on type shown by color code. The color code are as followed:
    TYPE_TYPE       purple
    TYPE_STRING     green
    TYPE_CHAR       green
    TYPE_INTEGER    cyan
    TYPE_FLOAT      cyan
    TYPE_COMMENT    white (shaded)
    TYPE_OPERATOR   yellow
    TYPE_INVALID    bright red
    TYPE_IDENTIFIER normal - no change

TYPE_INVALID is a type that is not handled yet by the program that may be outside the scope of the project outlined,
for example, hexadecimal identifiers will be flagged as TYPE_INVALID since the initial character starts as a 0.

To run the program, compile in terminal and pass a .c file as an argument. The file must be in the same directory. If no
argument is passed, the program will use itself as a file.