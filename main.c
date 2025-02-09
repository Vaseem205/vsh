#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "vsh.h"


/****************************Built-in Functions*************************************/

char *builtin_str[] = {"cd", "help", "exit"};

typedef int (*func_t)(char **);
func_t builtin_func[] = { vsh_cd, vsh_help, vsh_exit };



int lsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

int vsh_cd(char **args){
    if(args[1]==NULL){
        perror("No argument is provided for 'cd'\n");
    }else{
        if(chdir(args[1])==-1){
            perror("chdir error");
        }
    }
    return 1;
}

int vsh_help(char **args){

    printf("These are the available builtin fucntions: \n");
    int i = 0;
    for(int i=0; i<lsh_num_builtins(); i++){
        printf("%s\n", builtin_str[i]);
    }

    return 1;
}

int vsh_exit(char **args){
    return 0;
}

/********************************************************************************/

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
    char **tokens_backup;

    if(tokens == NULL){
        perror("Failed to allocated memory");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOK_DELIM);

    while(token!=NULL){
        tokens[position] = token;
        position++;

        if(position>=bufSize){
            bufSize += TOK_SIZE;            // increment the bufsize by 1kB
            tokens_backup = tokens;
            tokens = realloc(tokens, bufSize);
            if(tokens == NULL){
                free(tokens_backup);       // freering previous token-pointers as they had been reallocated
                perror("Failed to allocated memory");
                exit(EXIT_FAILURE);
            }
        }
        // strtok(NULL, TOK_DELIM) -> NULL tells strtok() to use the same line provided initially
        token = strtok(NULL, TOK_DELIM); //if there are no more strings left, it would return NULL.
    }
    tokens[position] = NULL;    
    return tokens;
}

int launch(char **args){

    pid_t pid, wpid;
    int status;

    pid = fork();

    if(pid==0){
        // child process
        if(execvp(args[0], args)==-1){                      // this will start the entirely diffrent program
            perror("couldn't execute the command");
        }
        exit(EXIT_FAILURE);                             
    }else if(pid<0){
        perror("Fork ERROR");
    }else{
        // parent process
        do{
            wpid = waitpid(pid, &status, WUNTRACED);        // 
        }while(!WIFEXITED(status) && !WIFSIGNALED(status)); // If the child has not exited or been killed, the loop continues.
    }

    return 1;
} 



int execute(char **args){
    
    if(args[0]==NULL){
        return 0;
    }

    for(int i=0; i<lsh_num_builtins(); i++){
        if(strcmp(args[0], builtin_str[i])==0){
            return builtin_func[i](args);
        }
    }

    return launch(args);
}



/*
READ: read the command from stanadard input
PARSE: separate the commands into program and arguments [mkdir folder1]is is what is being assigned to this and what is this y is this
EXECUTE: run the parsed command.
*/
int shell(){

    char *line;
    char **args;

    int status;

    do{
        printf("> ");
        line = readLine();
        // printf("%s\n", line);

        args = readArgs(line);

        // int i = 0;
        // while(args[i]!=NULL){
        //     printf("%s\n", args[i]);
        //     i++;
        // }

        // printf("%s\n", args[1]);
        status = execute(args);

        // printf("status: %d\n", status);

        free(line);
        free(args);

    }while(status);

    return status;

}