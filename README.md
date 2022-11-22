# Project_Linux_K-SEA_Shell

# Objectives
* Create a shell that reads in commands a user types that executes in an infinite loop.
* Implement the shell to execute child processes from the shell (the shell is the parent process) based on the command.

# Details
* The shell has a prompt "k-sea-shell>" in front of each command that is typed out.
* The maximum input size(i.e. BUFFER_SIZE) on a given line will be 80 characters for the shell.
* When a new child process is launched from the shell, the child process runs in the foreground by default until it is completed.
* The shell has some built-in functions. 
1. 'exit' to terminate your shell
2. 'cd' to change directories
3. 'help' explaining all the built-in commands available in the shell
4. 'game' to play a guessing game
* If a command is not found, the shell prints out an error message, and resume execution.
