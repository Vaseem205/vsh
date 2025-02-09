
// #define USE_STD_GETLINE 1
#define TOK_DELIM " \t\r\n\a"
#define TOK_SIZE 64

/*
  Function Declarations for builtin shell commands:
 */
int vsh_cd(char **args);
int vsh_help(char **args);
int vsh_exit(char **args);