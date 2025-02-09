# vsh - Shell Implementation

## Overview
vsh is a simple command-line shell implemented in C. It reads user commands, parses them into arguments, and executes them. The shell supports built-in commands (`cd`, `help`, `exit`) and can also execute external programs using the `execvp` system call.

## Features
- Supports built-in commands:
  - `cd <directory>`: Change directory
  - `help`: Display available built-in commands
  - `exit`: Exit the shell
- Reads input using `getline()` or manual buffer allocation.
- Parses user input into arguments.
- Executes external commands using `fork()` and `execvp()`.
- Implements a basic command loop (READ -> PARSE -> EXECUTE).

## Files and Structure
The project consists of three files:

### 1. `vsh.c`
This is the entry point of the shell. It calls the `shell()` function in a loop until the user decides to exit.
```c
#include <stdio.h>
#include "main.c"

int main(void) {
    int status;
    do {
        status = shell();
    } while (status);
    return 0;
}
```

### 2. `vsh.h`
This header file declares built-in function prototypes and defines constants for token parsing.
```c
#define TOK_DELIM " \t\r\n\a"
#define TOK_SIZE 64

int vsh_cd(char **args);
int vsh_help(char **args);
int vsh_exit(char **args);
```

### 3. `main.c`
This file contains the main logic for:
- Built-in command execution (`cd`, `help`, `exit`)
- Reading user input (`getline()` or manual input handling)
- Tokenizing input into arguments
- Forking child processes and executing commands
- Main shell loop

#### Core Functions:
- `readLine()`: Reads user input from standard input.
- `readArgs()`: Tokenizes the input into arguments.
- `launch()`: Forks a child process and executes a command.
- `execute()`: Checks if the command is built-in; otherwise, launches it as an external command.
- `shell()`: The main loop that runs the shell.

## Compilation and Execution
### Compile:
```bash
gcc vsh.c -o vsh
```

### Run the Shell:
```bash
./vsh
```

## Example Usage
```bash
> ls
> mkdir test_dir
> cd test_dir
> pwd
> help
> exit
```

## Notes
- If `USE_STD_GETLINE` is defined in `vsh.h`, `getline()` will be used for input.
- If `USE_STD_GETLINE` is not defined, manual input handling will be used.

## License
This project is open-source and can be modified and distributed freely.


