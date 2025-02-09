#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>




// #define USE_STD_GETLINE 1
#define TOK_DELIM " \t\r\n\a"
#define TOK_SIZE 64



char* readLine(){

    #ifdef USE_STD_GETLINE
        char *line = NULL;
        ssize_t bufSize = 0; //cuz, getline() accepts ssize_t datatype variable. And it is assigned to 0 becuase we gave authority to getline() to allocate a buffer for us

        /*
        How getline() Works with bufsize?

        ssize_t getline(char **lineptr, size_t *n, FILE *stream);

        - char **lineptr: Pointer to the pointer where the line will be stored.
        - size_t *n: Pointer to the buffer size (used for memory allocation/resizing).
        - FILE *stream: The input stream (e.g., stdin).

        Behavior of getline()
        - If *lineptr is NULL and *n is 0, getline() allocates a buffer automatically.
        - If *lineptr already points to a buffer, getline() resizes it if necessary.
        - The buffer size (*n) is updated to reflect the allocated memory.
        - getline() returns the number of characters read, or -1 on failure.
        */
        if(getline(&line, &bufSize, stdin)==-1){
            if(feof(stdin)){                        // we have reached EOF.
                exit(EXIT_SUCCESS);
            }else{
                perror("read failure");
                exit(EXIT_FAILURE);
            }
        }

        return line;

        
    #else
    #define BUFFER_SIZE 1024

    int bufsize = BUFFER_SIZE;
    char *buffer = (char *) malloc(bufsize * sizeof(char));
    int c;
    int position = 0;

    if(buffer == NULL){
        perror("Failed to allocated memory");
        exit(EXIT_FAILURE);
    }

    while(1){

        c = getchar();

        if(c==EOF){
            exit(EXIT_SUCCESS);
        }else if(c == '\n'){
            buffer[position] = '\0';
            return buffer;
        }else{
            buffer[position] = c;
        }

        position++;

        if(position>=BUFFER_SIZE){
            bufsize += BUFFER_SIZE;            // increment the bufsize by 1kB
            buffer = realloc(buffer, bufsize);

            if(buffer == NULL){
                perror("Failed to allocated memory");
                exit(EXIT_FAILURE);
            }
        }
    }

    #endif
}

char **readArgs(char *line){

    int bufSize = TOK_SIZE;

    char **tokens = malloc(bufSize*sizeof(char));
    char *token;
    int position = 0;

    if(tokens == NULL){
        perror("Failed to allocated memory");
        exit(EXIT_FAILURE);
    }



    token = strtok(line, TOK_DELIM);


    while(token!=NULL){
        tokens[position] = token;
        position++;

        if(position>=TOK_SIZE){
            bufSize += bufSize;            // increment the bufsize by 1kB
            tokens = realloc(tokens, bufSize);

            if(tokens == NULL){
                perror("Failed to allocated memory");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, TOK_DELIM);
    }

    tokens[position] = NULL;    

    return tokens;
}

/*
READ: read the command from stanadard input
PARSE: separate the commands into program and arguments [mkdir folder1]is is what is being assigned to this and what is this y is this

EXECUTE: run the parsed command.
*/
void shell(){

    char *line = readLine();
    // printf("%s\n", line);

    char **args = readArgs(line);

    int i = 0;
    while(args[i]!=NULL){
        printf("%s\n", args[i]);
        i++;
    }


    // printf("%s\n", args[1]);

    free(line);
    free(args);

}


int main(void){

    while(1){
        shell();
        break;
    }

    return 0;
}