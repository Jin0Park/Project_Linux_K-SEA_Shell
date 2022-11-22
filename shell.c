#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <math.h>
#define MAXARGS 5
#define MAXLINE 8192
#define MAX_INPUT_SIZE 80
#define NUM_OF_COMMANDS 4
char* PROMPT = "k-sea-shell> ";
char* ERROR_MESSAGE = "Command not found--Did you mean something else?";
char *argv[MAXARGS] = {NULL, NULL, NULL, NULL, NULL};

char* commands[NUM_OF_COMMANDS] = {"cd", "help", "exit", "game"};

void* cd() {
    int ch = chdir(argv[1]);
    if (ch < 0) {
        printf("cd was unsuccessful\n");
    }
}

void* help() {
    printf("cd - change the current working directory\n");
    printf("help - show all of the built-in commands\n");
    printf("exit - terminate the shell\n");
    printf("game - play a guessing game\n");
}

void* game() {
    int n = 10;
    int total_guess = 5;
    int num_chosen = rand() % n;
    int guess;
    int gameSet = 0;
    while (gameSet == 0) {
       if (total_guess == 0) {
           printf("The number was %d!\n", num_chosen);
           printf("You lost!\n");
           gameSet = 1;
        } else {
            printf("Guess a number between 1 and 10:  ");
            while ((guess = getchar()) != '\n' && guess != EOF) {
                if (guess != num_chosen + '0') {
                    total_guess--;;
                    printf("You now have %d guess left.\n", total_guess);
                } else {
                    printf("You win!\n");
                    gameSet = 1;
                }
            }
        }
    }
}


// have an array of string, first should be the command.
// second should be the input.
// third should be null
int parse(char* string, char* array[MAXARGS]) {
    if (string == NULL || array == NULL) {
        return -1;
    }

    char* token = strtok(string, " \n\t");
    array[0] = token;
    int i = 1;
    while (token != NULL) {
        token = strtok(NULL, " \n\t");
        array[i] = token;
        i++;
    }
    return 0;
}

void* command_functions[NUM_OF_COMMANDS] = {cd, help, exit, game};

struct Function {
    const char *name;
    void* (*function)(void);
};


struct Function functions[] =
{
    {"cd", cd},
    {"help", help},
    {"game", game},
};

enum {NUM_FUNCTIONS = sizeof(functions) / sizeof(functions[0])};

/***
 **  Wrapper of fork(). Checks for fork errors, quits on error. 
 **/
pid_t Fork(void) {
  pid_t pid;  
  if ((pid = fork()) < 0) {
    fprintf(stderr, "%s: %s\n", "fork error", strerror(errno));
    exit(0);
  }
  return pid;
}

/***
 **  Wrapper of fgets. Checks and quits on Unix error. 
 **/
char* Fgets(char *ptr, int n, FILE *stream) {
  char *rptr;
  if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream)) {
    fprintf(stderr, "%s\n", "fgets error");
    exit(0);
  }
  return rptr;
}

// Is the command one built into the shell?
// That is, that the *shell* has implemented?
// If so, execute it here
int builtin_command(char** argv) {
  if (strcmp(argv[0], "exit") == 0) {
    printf("exiting the shell...\n");
    exit(0);  
  }
  int i;
  for (i=0; i < NUM_FUNCTIONS; i++) {
    if (strcmp(argv[0], functions[i].name) == 0) {
        (*functions[i].function)();
        return 1;
    }
  }
  return 0;
}

// maybe use the map. key as command string, and value as the functionlity.
// you can call the methods in eval.
void eval(char *cmdline) {
  char buf[MAXLINE]; /* Holds modified command line */
  pid_t pid; /* Process id */
  strcpy(buf, cmdline);

  // Split buf into args
  parse(cmdline, argv); 

  if (argv[0] == NULL)
    return;

  if (builtin_command(argv) == 0) {
    // Check that the command/program exists in Unix (ie /bin/)
    // child
     if ((pid = fork()) == 0) {
        char bin[MAXLINE] = "/bin/";
        strcat(bin, argv[0]);
        argv[0] = bin;
        int res = execve(argv[0], argv, NULL);
        if (res < 0) {
            printf("Please put a valid command.\n");
        }
        exit(1);
    // parent
    } else {
        wait(NULL);
    }
  }
}

int main() {
  char cmdline[MAXLINE]; /* command line buffer */
  while (1) {
    // Print command prompt
    printf("k-sea-shell> ");
    // Read input from user
    Fgets(cmdline, MAXLINE, stdin);
    // If we get the eof, quit the program/shell
    if (feof(stdin))
      exit(0);
    // Otherwise, evaluate the input and execute. 
    eval(cmdline);
  }
}
