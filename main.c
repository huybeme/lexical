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
char* input = "test.c";
int input_length = 0;
enum type{TYPE_INTEGER, TYPE_FLOAT, TYPE_STRING, TYPE_CHAR, TYPE_COMMENT,
    TYPE_TYPE, TYPE_RESERVED, TYPE_OPERATOR, TYPE_IDENTIFIER, TYPE_INVALID};

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

int matchtype(char* id){
    if (strcmp(id, "void") != 0 || strcmp(id, "char") != 0 || strcmp(id, "int") != 0 || strcmp(id, "float") != 0){
        return 1;
    }
    return 0;
}

int matchreserved(char* id){
    if (strcmp(id, "sizeof") != 0 || strcmp(id, "enum") != 0 || strcmp(id, "case") != 0 || strcmp(id, "default") != 0 ||
            strcmp(id, "if") != 0 || strcmp(id, "else") != 0 || strcmp(id, "switch") != 0 || strcmp(id, "while") != 0 ||
            strcmp(id, "do") != 0 || strcmp(id, "for") != 0 || strcmp(id, "goto") != 0 || strcmp(id, "continue") != 0 ||
            strcmp(id, "break") != 0 || strcmp(id, "return") != 0){
        return 1;
    }
    return 0;
}

int matchoperator(char op){
    if (op == '+'  || op == '-'|| op == '*' || op == '/' || op == '<' || op == '>' ||
        op == '='  || op == '!'|| op == '&' || op == '?' || op == '[' || op == ']' ||
        op == '{' || op == '}' || op == '|' || op == '(' || op == ')' || op == '%' ||
        op == '^' || op == ',' || op == ':' || op == ';'){
        return 1;
    }
    return 0;
}

int matchchar(){

    return 0;
}

// match operators of equal values, >= <= and not > or <
int match(char* string, int type){
    // if its an operator, return 0, else 1
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

    printf("pointers: %d-%d   \t%s: ",ptr, tptr, getType(token.type));
    printf("%s\n", token.str);

    ptr = tptr + 1; // move ptr to start of next token

}

int main(int argc, char* argv[]) {

    //create a file
    FILE *input_file;

    //get input file - turn this into an input eventually
    input_file = fopen(input, "r");
    check_if_exist(input_file, input);

    char read;
    read = fgetc(input_file);
    // get length of file for string building
    while (read != EOF) {
        input_length++;
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
    printf("total num of chars in file: %d\n", input_length);
    fclose(input_file);

    int state = 0;
    int tptr = ptr;     // used for looking ahead
    int done=0;             // set when token is identified or error is found

//    Token token;

    while(ptr <= input_length){

        // check for operator (state 1) - keep ptrs at one spot, may need to update for operators with len > 1
        if (matchoperator(input_string[ptr])){
            tptr = ptr;
            accept(tptr, TYPE_OPERATOR, input_string);
            ptr--;  // due to accept ptr++; we need to move it back for cases where other types follow immediately
        }
        // check for char (state 1)
        else if (input_string[ptr] == '\''){
            tptr = ptr + 2;  // move to end of char (closing ')
            if (input_string[tptr] == '\'')
                accept(tptr, TYPE_CHAR, input_string);
            else {
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
//            printf("%d\n",state);
            if (input_string[tptr] == ';') { // need to handle for white spaces and operations***
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
            while ((input_string[tptr] >= 65 && input_string[tptr] <= 90) || (input_string[tptr] >= 97 && input_string[tptr] <= 122) || (input_string[tptr] >= 48 && input_string[tptr] <= 57)){
                tptr++;
            }

            printf("%d____\n", strcmp(token.str, "continue"));

            if (input_string[tptr] == '=' || input_string[tptr] == '(' || input_string[tptr] == '[' || input_string[tptr] == ' ') {  // handle white spaces!!!!!!!!!!!!!
                tptr--;
                if (matchtype(token.str) == 1)
                    accept(tptr, TYPE_TYPE, input_string);
                else if (matchreserved(token.str) == 1)
                    accept(tptr, TYPE_RESERVED, input_string);
                else
                    accept(tptr, TYPE_IDENTIFIER, input_string);
            }
            else
                accept(tptr, TYPE_INVALID, input_string);
        }



        // just else
        else {
            token.type = TYPE_INVALID;

        }
        ptr++;
    }


    return 0;
}