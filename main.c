#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void check_if_exist(FILE *file, char* name)
{
    if (file == NULL)
    {
        printf("'%s' not found.\n", name);
        exit(0);
    }
    else
    {
        printf("'%s' opened.\n", name);
    }
}


int ptr = 0;
char* input;
int input_length = 0;
enum type{TYPE_INTEGER, TYPE_FLOAT, TYPE_STRING, TYPE_CHAR, TYPE_COMMENT,
    TYPE_TYPE, TYPE_RESERVED, TYPE_OPERATOR, TYPE_IDENTIFIER, TYPE_INVALID};
FILE *input_file, *printout;

typedef struct{
    int type;       // type of token (there are 9)
    char str[100];  // value of token
    int length;     // length of token in number of bytes
}Token;

Token token;

const char* getType(enum type t){
    switch (t)
    {
        case TYPE_INTEGER:
            return "TYPE_INTEGER";
        case TYPE_FLOAT:
            return "TYPE_FLOAT";
        case TYPE_STRING:
            return "TYPE_STRING";
        case TYPE_CHAR:
            return "TYPE_CHAR";
        case TYPE_COMMENT:
            return "TYPE_COMMENT";
        case TYPE_TYPE:
            return "TYPE_TYPE";
        case TYPE_RESERVED:
            return "TYPE_RESERVED";
        case TYPE_OPERATOR: return "TYPE_OPERATOR";
        case TYPE_IDENTIFIER: return "TYPE_IDENTIFIER";
        case TYPE_INVALID: return "TYPE_INVALID";
    }
}

// advances nexttoken
void next(){
    ptr++;
}

// reached end of input
int doneWithInput(){
    //moves ptr past white spaces
    return 0;
}

// fill empty token with next token
int identifyNextToken(Token *token){
    // if not valid token, return 0, else 1
    return 0;
}


// match operators of equal values, >= <= and not > or <
int match(char* string, int type){
    // if its an operator, return 0, else 1
    return 0;
}

// note strcmp(equal,equal) returns 0 which is true
int matchtype(char* id){
    if (strcmp(id, "void") == 0 || strcmp(id, "char") == 0 || strcmp(id, "int") == 0 || strcmp(id, "float") == 0){
        return 1;
    }
    return 0;
}

int matchreserved(char id[]){
    if (strcmp(id, "sizeof") == 0 || strcmp(id, "enum") == 0 || strcmp(id, "case") == 0 || strcmp(id, "default") == 0 ||
            strcmp(id, "if") == 0 || strcmp(id, "else") == 0 || strcmp(id, "switch") == 0 || strcmp(id, "while") == 0 ||
            strcmp(id, "do") == 0 || strcmp(id, "for") == 0 || strcmp(id, "goto") == 0 || strcmp(id, "continue") == 0 ||
            strcmp(id, "break") == 0 || strcmp(id, "return") == 0){
        return 1;
    }
    return 0;
}

int matchoperator(char op){
    if (op == '+'  || op == '-'|| op == '*' || op == '/' || op == '<' || op == '>' ||
            op == '='  || op == '!'|| op == '&' || op == '?' || op == '[' || op == ']' ||
            op == '{' || op == '}' || op == '|' || op == '(' || op == ')' || op == '%' ||
            op == '^' || op == ',' || op == ':' || op == ';' || op == '#' || op == '~' ||
            op == '\\'){
        return 1;
    }

    return 0;
}

int foundwhitespace(char space){
    if (space == ' ' || space == '\t' || space == '\n')
        return 1;
    else
        return 0;
}

void accept(int tptr, enum type type1, char* line){
    int len = tptr - ptr;
    token.type = type1;
    token.length = len;

    for (int i = 0; i <= len; i++){
        token.str[i] = line[ptr +i];
    }
    token.str[len+1] = '\0';

    fprintf(printout, "pointers: %d-%d\t",ptr, tptr);
    ptr = tptr + 1; // move ptr to start of next token

}

void printcolor(){

    if (strcmp(getType(token.type), "TYPE_TYPE") == 0){     //blue
        printf("\x1b[34m%s\x1b[0m", token.str);
    } else if (strcmp(getType(token.type), "TYPE_STRING") == 0 || strcmp(getType(token.type), "TYPE_CHAR") == 0){       //green
        printf("\x1b[32m%s\x1b[0m", token.str);
    } else if (strcmp(getType(token.type), "TYPE_INTEGER") == 0 || strcmp(getType(token.type), "TYPE_FLOAT") == 0){     //cyan
        printf("\x1b[36m%s\x1b[0m", token.str);
    } else if (strcmp(getType(token.type), "TYPE_RESERVED") == 0){      //purple
        printf("\x1b[35m%s\x1b[0m", token.str);
    } else if (strcmp(getType(token.type), "TYPE_COMMENT") == 0){       // hite (shaded)
        printf("\x1b[37m%s\x1b[0m", token.str);
    } else if (strcmp(getType(token.type), "TYPE_OPERATOR") == 0){      //yellow
        printf("\x1b[33m%s\x1b[0m", token.str);
    } else if (strcmp(getType(token.type), "TYPE_INVALID") == 0){       //bright red
        printf("\x1b[91m%s\x1b[0m", token.str);
    }
    else{   //regular
        printf("%s", token.str);
    }
}

int main(int argc, char* argv[]) {

    // take in an input for a file location
    if (argv[1] != '\0')
        input = argv[1];
    else
        input = "main.c";

    //get input file - turn this into an input eventually
    input_file = fopen(input, "r");
    check_if_exist(input_file, input);

    //create output
    char* output_path = "lex_data.txt";
    printout = fopen(output_path, "w+");

    char read;
    read = fgetc(input_file);
    // get length of file for string building
    while (read != EOF) {
        input_length++;
        fputc(read, printout);
        read = fgetc(input_file);
    }
    printf("\n\n");

    // put the file into a string
    char* input_string;
    input_string=(char*)malloc(input_length);
    rewind(input_file);
    for(int i=0; i<input_length; i++){
        input_string[i]=fgetc(input_file);
    }
    input_string[input_length] = 0;
//    printf("%s\n\n",input_string);
    fprintf(printout, "\n\t**end of method printout**"
                      "\ntotal num of chars in file: %d\n\n\n", input_length);
    fclose(input_file);

    int state = 0;
    int tptr = ptr;     // used for looking ahead
    int done=0;             // set when token is identified or error is found

    while(ptr <= input_length){

        // print and move past white spaces - handle last white space at the end
        if (input_string[ptr] == ' ' || input_string[ptr] == '\t' || input_string[ptr] == '\n' || input_string[ptr] == '\0') {
            if (input_string[ptr] == '\0') {
                ptr++;
                continue;
            }
            printf("%c", input_string[ptr]);
            ptr++;
            continue;
        }
        // check for operator (state 1) - keep ptrs at one spot, may need to update for operators with len > 1
        else if (matchoperator(input_string[ptr])){
            tptr = ptr;
            // check for comments (state 2)
            if (input_string[ptr] == '/' && input_string[tptr+1] == '/'){
                while (input_string[tptr] != '\n'){
                    tptr++;
                }
                tptr--;
                accept(tptr, TYPE_COMMENT, input_string);
            } else if (input_string[tptr] == '/' && input_string[tptr+1] == '*'){
                tptr += 2;
                while (input_string[tptr] != '*' && input_string[tptr+1] != '/'){
                    tptr++;
                }
                tptr++; // capture the second operator for closing comment
                accept(tptr, TYPE_COMMENT, input_string);
            } else if (input_string[ptr] == '#' && input_string[ptr+1] == 'i') {    //handle library inclusion (state 2)
                int count = 0;
                char word[10];

                while(input_string[tptr] != ' '){
                    tptr++;
                    word[count] = input_string[tptr];
                    count++;
                }
                word[count-1] = '\0';
                while(input_string[tptr] != '\n'){
                    tptr++;
                }

                if(strcmp(word, "include") == 0 || strcmp(word, "define") == 0)    // handle include libraries - type invalid for now
                    accept(tptr, TYPE_INVALID, input_string);
                else
                    accept(tptr, TYPE_INVALID, input_string);
            }
            else
                accept(tptr, TYPE_OPERATOR, input_string);
        }
        // check for char (state 1)
        else if (input_string[ptr] == '\''){
            if (input_string[ptr+1] == '\\')    // handle backslash chars
                tptr = ptr+3;
            else
                tptr = ptr + 2;  // move to end of char (closing ')
            if (input_string[tptr] == '\'')
                accept(tptr, TYPE_CHAR, input_string);
            else {      //syntax error, char has too many inputs
                while (input_string[ptr] != '\'')
                {
                    ptr++;
                    if (input_string[ptr] == '\0')
                        break;
                }
                tptr = ptr;
                accept(tptr, TYPE_INVALID, input_string);
            }
        }
        // check for string (state 1)
        else if (input_string[ptr] == '"'){
            tptr = ptr +1;  // token after "
            while (input_string[tptr] != '"'){
                tptr++;
                if (input_string[tptr] == '\0') // no ending quotes, syntax error
                    break;
            }
            if (input_string[ptr] == input_string[tptr])
                accept(tptr, TYPE_STRING, input_string);
            else
                accept(tptr, TYPE_INVALID, input_string);
        }
        // check for integer (state 1)
        else if (input_string[ptr] >= '0' && input_string[ptr] <= '9'){
            tptr = ptr;
            state = 1;
            while (input_string[tptr] >= '0' && input_string[tptr] <= '9'){
                tptr++;
                // check for float (state 2)
                if (input_string[tptr] == '.'){
                    tptr++; //move past decimal
                    while (input_string[tptr] >= '0' && input_string[tptr] <= '9') {
                        tptr++;
                    }
                    state = 2;
                }
            }
            if (matchoperator(input_string[tptr]) == 1 || foundwhitespace(input_string[tptr]) == 1) {               // lexical standpoint, this is okay, may run into syntax errors
                tptr--; // need to decrement due to while loop
                if (state == 1)
                    accept(tptr, TYPE_INTEGER, input_string);        //handle float types**
                else
                    accept(tptr, TYPE_FLOAT, input_string);
            }
            else
                accept(tptr, TYPE_INVALID, input_string);
        }
        // check for identifiers (state 1)
        else if ((input_string[ptr] >= 65 && input_string[ptr] <= 90) || (input_string[ptr] >= 97 && input_string[ptr] <= 122)){    //A-Z a-z
            tptr = ptr;
            state = 1;
            while ((input_string[tptr] >= 65 && input_string[tptr] <= 90) || (input_string[tptr] >= 97 && input_string[tptr] <= 122) ||
                    (input_string[tptr] >= 48 && input_string[tptr] <= 57) || input_string[tptr] == '_' || input_string[tptr] == '.' ||
                    input_string[tptr] == '\n' || input_string[tptr] == '\t'){
                tptr++;
            }
            // state 2 - type and reserved
            if (input_string[tptr] == '=' || input_string[tptr] == '(' || input_string[tptr] == '[' || input_string[tptr] == ' ' ||
                    matchoperator(input_string[tptr]) == 1) {
                if (input_string[tptr] != '&')  // if this is not a pointer to address
                    tptr--;
                accept(tptr, TYPE_IDENTIFIER, input_string);

                if (matchtype(token.str) == 1)
                    token.type = TYPE_TYPE;
                else if (matchreserved(token.str) == 1)
                    token.type = TYPE_RESERVED;
            }
            else
                accept(tptr, TYPE_INVALID, input_string);
        }
        else{
            printf("\n\nFATAL ERROR\n");
            break;
        }

        //printf("\t%s: %s\n", getType(token.type), token.str);
        fprintf(printout,"\t%s: %s\n", getType(token.type), token.str);
        printcolor();
    }


    return 0;
}