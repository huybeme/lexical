#include <stdio.h>
#include <stdlib.h>

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

// see if next token is int type
int matchtype(int type){
    // if int type, return 0, else return 1
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

void accept(Token token, int tempptr, enum type type1, char* line){
    int len = tempptr - ptr;
    token.type = type1;
    token.length = len;
    printf("%d-%d:",ptr, tempptr);
    for (int i = 0; i <= len; i++){
        token.str[i] = line[ptr +i];
    }
    token.str[len+1] = '\0';
    printf("%s\n", token.str);
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
//        printf("%c", read);
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
    int tempptr = ptr;     // used for looking ahead
    int done=0;             // set when token is identified or error is found

    Token token;

    while(ptr <= input_length){

        // check for operator
        if (matchoperator(input_string[ptr])){
            ptr--;          // adv past op
            tempptr = ptr+1;
            accept(token, tempptr, TYPE_OPERATOR, input_string);
            ptr++;
        } else if (input_string[ptr] == '\''){
            tempptr = ptr + 2;  // move to end of char (closing ')
            accept(token, tempptr, TYPE_CHAR, input_string);
            ptr = tempptr+1;
        } else if (input_string[ptr] == '"'){
            tempptr = ptr +1;  // token after "
            while (input_string[tempptr] != '"'){
                tempptr++;
            }
            accept(token, tempptr, TYPE_STRING, input_string);
            ptr = tempptr +1;
        }
        else {
            token.type = TYPE_INVALID;

        }
        ptr++;
    }
    printf("\n%d\n", token.type);




    return 0;
}