/* NAME: SHAIK SHABEENA
REGISTRATION NO: 25048_004

DESCRIPTION:
-> This project implements a Mini Shell using C programming in Linux. It accepts commands from the user, identifies whether they are built-in 
   or external commands, and executes them. The project uses Linux system calls to create and manage processes, handle signals, execute commands, 
   and establish communication between processes using pipes.

-> The shell supports basic built-in commands, external command execution, command piping, and basic job management. It uses a linked list to 
   store information about stopped processes and provides commands such as jobs, fg, and bg to manage stored jobs.

Below is the description of what each file and function does in the project.


                                                    // main.c

1. This file controls the starting point of the entire project.
2. It contains the main() function.
3. It clears the terminal using system("clear").
4. It calls scan_input() to start the mini shell.
5. It transfers control to the shell's main input-processing loop.




                                                // shell.h

1. This header file contains the common declarations used throughout the project.
2. It includes the required C and Linux header files.
3. It defines constants such as BUILTIN, EXTERNAL, and NO_COMMAND.
4. It defines ANSI color codes used to display a colored shell prompt.
5. It defines the struct node structure used to store process ID, command name, job status, job number, and the link to the next node.
6. It declares global variables and function prototypes shared between source files.
7. It acts as the common header file for the entire project.


 


                                                // scan_input.c

1. This file handles user input and controls the main execution flow of the shell.
2. The "scan_input()" function displays the prompt and reads commands entered by the user.
3. It loads the list of supported external commands by calling "extract_external_commands()".
4. It checks whether the user entered a "PS1=" command to change the shell prompt.
5. It calls "get_command()" to extract the first word from the input.
6. It calls "check_command_type()" to identify whether the command is built-in, external, or invalid.
7. Built-in commands are passed to "execute_internal_commands()".
8. For external commands, it uses "fork()" to create a child process.
9. The parent process uses "waitpid()" with "WUNTRACED" to detect when the foreground process stops.
10. If a process is stopped, it calls "insert_first()" to store the process details in the job linked list.
11. The `signal_handler()` function handles signals such as "SIGINT", "SIGTSTP', and "SIGCHLD".
12. This file connects user input, command execution, process management, and signal handling.







                                                    // commands.c

This file contains most of the command-processing functions, external command information, and job-management operations.

 1. extract_external_commands()

- Opens "external_commands.txt" using "open()".
- Reads the file character by character using "read()".
- Stores each external command name in an array.
- Terminates the array with "NULL".
- Makes the command list available for command identification.

 2. get_command()

- Extracts the first word from the user's input.
- For example, if the user enters ls -l, it extracts ls.
- Returns the extracted command name for identification.

 3. `check_command_type()`

- Compares the extracted command with the list of built-in commands.
- If a match is found, it returns "BUILTIN".
- Otherwise, it checks the external command list.
- If a match is found, it returns "EXTERNAL".
- If no match is found, it returns "NO_COMMAND".

 4. execute_internal_commands()

- Executes built-in commands directly within the shell process.
- Supports commands such as exit, cd, and pwd.
- Implements echo $$ to display the shell process ID.
- Implements echo $? to display the stored process status when available.
- Implements echo $SHELL to display the shell environment variable.
- Calls print_list() when the user enters jobs.
- Implements basic foreground and background job-control operations through fg and bg.
- This function handles built-in commands without creating a separate process for each command.

 5. execute_external_commands()

- Splits the input command into individual arguments using `strtok()`.
- Stores the arguments in an array suitable for `execvp()`.
- Checks whether the input contains the pipe (`|`) operator.
- If no pipe is present, executes the command using `execvp()`.
- If pipes are present, uses `pipe()` to create communication channels between commands.
- Uses `dup2()` to redirect standard input and standard output.
- Uses `fork()` to create child processes for pipeline commands.
- This function implements external command execution and basic pipeline processing.

 6. insert_first()

- Creates a new linked-list node using `malloc()`.
- Stores the stopped process's PID, command name, status, and job number.
- Inserts the new node at the beginning of the linked list.
- Maintains the information required for basic job management.

 7. delete_first()

- Removes the first node from the job linked list.
- Updates the head pointer to point to the next node.
- Releases the removed node's memory using `free()`.

8. print_list()

- Traverses the job linked list.
- Checks the status of each stored job.
- Displays the job number, command name, and stopped status for stored stopped processes.
- Allows the user to view the jobs maintained by the shell.

// external_commands.txt

1. This file stores the names of external commands recognized by the mini shell.
2. Each command is stored as a separate entry, usually on a new line.
3. The "extract_external_commands()" function reads the command names from this file.
4. The "check_command_type()" function compares the user's command against these entries.
5. If a command matches an entry, it is identified as an external command.
6. The command is then passed for execution, provided the executable is available to the system.
7. This file allows the shell to maintain a predefined list of supported external commands.

// About Built-in Commands

Built-in commands are handled directly by the shell rather than by launching a separate external program.

- exit — Terminates the mini shell.
- cd — Changes the current working directory.
- pwd — Displays the current working directory.
- echo $$ — Displays the shell process ID.
- echo $? — Displays the stored process status when available.
- echo $SHELL — Displays the value of the `SHELL` environment variable.
- jobs — Displays the jobs stored in the linked list.
- fg — Attempts to continue a stored stopped process in the foreground.
- bg — Attempts to continue a stored stopped process in the background.

 // About External Commands

1. External commands are programs that execute separately from the shell process.
2. Examples include commands such as ls, date, and sleep, provided they are included in external_commands.txt.
3. The shell creates a child process using fork().
4. The child process executes the requested program using execvp().
5. The parent process uses waitpid() to wait for the foreground child and detect whether it has stopped.
6. If the child stops, the shell stores its information in the job linked list.
7. This mechanism demonstrates process creation, execution, and synchronization in Linux.

 // About Piping

1. Piping allows the output of one command to become the input of another command.
2. The pipe operator (|) separates the commands.
3. The pipe() system call creates a communication channel between processes.
4. The fork() system call creates the child processes needed to execute the commands.
5. The dup2() system call redirects standard input and output to the appropriate pipe ends.
6. For example, ls | wc connects the output of ls to the input of wc.
7. This feature demonstrates inter-process communication in Linux.

 // About Signal Handling

1. Signal handling allows the shell to respond to certain events, such as keyboard interrupts and process-state changes.
2. "SIGINT" is associated with "Ctrl+C".
3. "SIGTSTP" is associated with "Ctrl+Z" when generated by the terminal.
4. "SIGCHLD" notifies the parent process when a child process changes state.
5. The "signal_handler()" function defines how the shell responds to the registered signals.
6. The child process resets "SIGINT" and "SIGTSTP" to their default behavior before executing an external command.
7. The shell uses "waitpid()" to detect stopped foreground processes and stores them for basic job management.




                                                // Overall Project Flow

1. The program starts from "main.c".
2. "scan_input()" initializes the shell and displays the prompt.
3. The user enters a command.
4. "get_command() extracts the command name.
5. "check_command_type()" identifies the command type.
6. Built-in commands are handled by "execute_internal_commands().
7. External commands are executed using "fork()" and "execvp()".
8. Pipeline commands use "pipe()" and "dup2()" to connect processes.
9. Signals and "waitpid()" help monitor process states.
10. Stopped processes are stored in a linked list using "insert_first()".
11. The "jobs", "fg", and "bg" commands operate on the stored job information.
12. After command processing, the shell returns to the prompt to accept the next command.

*/


#include<stdio.h>
#include<string.h>
#include "shell.h"


char input_string[20];
int main()
{
    system("clear");
    
    scan_input(prompt,input_string);


}