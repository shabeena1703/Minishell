# 🐚 Mini Shell in C | Linux Command-Line Interpreter

A **Mini Shell developed using C programming in Linux** that executes commands, supports command piping, manages stopped processes, and demonstrates basic foreground and background job control.

This project provides practical experience with Linux system programming, process management, inter-process communication (IPC), and signal handling.

---

## 🎯 Project Overview

The Mini Shell is a simplified command-line interpreter that allows users to interact with the Linux operating system by entering commands.

It provides a custom shell prompt and supports built-in commands, external Linux commands, command pipelines, and basic job control.

The project demonstrates how a shell creates processes, executes programs, connects commands through pipes, and handles signals.

**Objective:** To understand the internal working of a Linux shell and gain hands-on experience with system calls, process management, and Linux system programming using C.

## ✨ Features

| Feature | Description |
|---|---|
| 🖥️ Custom Prompt | Displays a personalized, colored shell prompt. |
| ⚙️ Built-in Commands | Supports commands such as `cd`, `pwd`, `echo`, `jobs`, `fg`, `bg`, and `exit`. |
| 🚀 External Commands | Executes Linux commands such as `ls`, `grep`, `wc`, and `sleep`. |
| 🔗 Command Piping | Connects multiple commands using the `|` operator. |
| ⏸️ Job Control | Supports stopping foreground jobs using `Ctrl+Z`. |
| 📋 Job Listing | Displays tracked stopped jobs using `jobs`. |
| ▶️ Foreground Execution | Resumes tracked jobs using `fg`. |
| 🔄 Background Execution | Resumes stopped jobs in the background using `bg`. |
| 📡 Signal Handling | Demonstrates handling of signals such as `SIGINT`, `SIGTSTP`, and `SIGCHLD`. |
| 📂 External Command List | Loads recognized external command names from `external_commands.txt`. |
| 🧵 Process Management | Uses child processes to execute external programs. |

## 🛠️ Technologies Used

- **Programming Language:** C
- **Operating System:** Linux / Ubuntu (WSL)
- **Compiler:** GCC
- **System Programming:** Linux system calls
- **Inter-Process Communication:** Pipes
- **Signal Handling:** POSIX signals
- **Data Structure:** Singly linked list for job tracking

## 📂 Project Structure

```text
mini_shell/
├── main.c
├── scan_input.c
├── commands.c
├── shell.h
├── external_commands.txt
└── README.md
```

### File Descriptions

**`main.c`**
- Contains the entry point of the program.
- Starts the Mini Shell by calling the input-scanning function.

**`scan_input.c`**
- Displays the shell prompt and reads user input.
- Identifies the type of command entered.
- Controls the execution flow for built-in and external commands.
- Handles signals and waits for child processes.
- Tracks stopped jobs.

**`commands.c`**
- Maintains the supported built-in commands.
- Loads external command names from a file.
- Identifies and executes internal and external commands.
- Implements command piping.
- Implements basic foreground and background job-control operations.
- Maintains the linked list used for job tracking.

**`shell.h`**
- Contains header files, macros, structure definitions, global declarations, and function prototypes.
- Shares declarations between the source files.

**`external_commands.txt`**
- Contains the list of external commands recognized by the shell.
- Is loaded when the Mini Shell starts.

## 🔄 How the Mini Shell Works

The following flowchart illustrates the general command-execution process.

```text
                 +----------------------+
                 |  Start Mini Shell    |
                 +----------------------+
                            |
                            v
                 +----------------------+
                 |   Display Prompt     |
                 +----------------------+
                            |
                            v
                 +----------------------+
                 |    Read Command      |
                 +----------------------+
                            |
                            v
                 +----------------------+
                 | Identify Command Type|
                 +----------------------+
                            |
                 +----------+----------+
                 |                     |
                 v                     v
       +------------------+  +--------------------+
       | Built-in Command |  | External Command    |
       +------------------+  +--------------------+
                 |                     |
                 v                     v
       +------------------+  +--------------------+
       | Execute in Shell |  | Create Child Process|
       +------------------+  +--------------------+
                                       |
                                       v
                              +--------------------+
                              | Execute with execvp|
                              +--------------------+
                                       |
                                       v
                              +--------------------+
                              | Wait / Job Control |
                              +--------------------+
                                       |
                                       v
                              +--------------------+
                              | Display Prompt     |
                              +--------------------+
                                       |
                                       +------> Repeat
```

### Execution Flow

1. The shell displays its prompt.
2. The user enters a command.
3. The shell identifies whether the command is built-in or external.
4. Built-in commands are handled by the shell itself.
5. External commands are executed using child processes.
6. For pipelines, the shell connects the output of one command to the input of another.
7. The shell handles supported signals and basic job-control operations.

## 💻 Supported Commands

### Built-in Commands

| Command | Purpose |
|---|---|
| `cd` | Changes the current working directory. |
| `pwd` | Displays the current working directory. |
| `echo` | Displays text or supported shell values. |
| `jobs` | Lists tracked stopped jobs. |
| `fg` | Resumes a tracked job in the foreground. |
| `bg` | Resumes a stopped job in the background. |
| `exit` | Exits the shell. |
| `PS1=value` | Updates the stored prompt variable. |

*Note: The displayed prompt is formatted directly in the input-scanning code. Therefore, changing `PS1` may not change the visible prompt until the display code is updated.*

### External Commands

Examples include:

```bash
ls
ls -l
grep
wc
sleep
cat
date
```

The shell loads external command names from `external_commands.txt`.

## 🔗 Command Piping

Piping allows the output of one command to become the input of another command using the `|` operator.

### Example 1: Count Directory Listing Output

```bash
ls | wc
```

Here:
- `ls` displays directory entries.
- `|` passes the output to the next command.
- `wc` counts the lines, words, and bytes.

### Example 2: Filter and Count Output

```bash
ls -l | grep c | wc
```

Here:
1. `ls -l` displays detailed directory information.
2. `grep c` filters lines containing the character `c`.
3. `wc` counts the resulting lines, words, and bytes.

This demonstrates how multiple Linux commands can be connected in a pipeline.

## ⏯️ Process and Job Management

The Mini Shell implements basic job-control functionality to track stopped jobs and resume them.

### 1. Stop a Foreground Job

Press `Ctrl+Z` while a command such as `sleep` is running.

The shell tracks the stopped job and assigns a job number.

### 2. Display Jobs

The `jobs` command displays tracked stopped jobs, their job numbers, and their status.

### 3. Resume a Job in the Foreground

The `fg` command resumes a tracked job in the foreground. The shell waits for the job to finish or stop again.

### 4. Resume a Job in the Background

The `bg` command sends a continuation signal to a stopped job, allowing it to continue running in the background.

### 5. Background Job Completion

The shell can display notifications when tracked background jobs finish.

*Note: Job control is a basic implementation and is not fully equivalent to Bash. Job selection, child-process tracking, and completion notifications may have limitations.*

## 📡 Signal Handling

Signals notify a process about events that require attention.

| Signal | Purpose |
|---|---|
| `SIGINT` | Handles an interrupt, typically generated by `Ctrl+C`. |
| `SIGTSTP` | Handles a terminal stop request, typically generated by `Ctrl+Z`. |
| `SIGCHLD` | Notifies the parent process when a child process changes state. |
| `SIGCONT` | Resumes a stopped process. |

Signal handling works with process-management functions to provide basic job-control behavior.

## 🔑 Key System Calls

| Function | Purpose |
|---|---|
| `fork()` | Creates a child process. |
| `execvp()` | Executes a program in the child process. |
| `waitpid()` | Waits for a child process or checks its status. |
| `pipe()` | Creates a communication channel between processes. |
| `dup2()` | Redirects a file descriptor, such as standard input or output. |
| `kill()` | Sends a signal to a process. |
| `read()` | Reads data from a file descriptor. |
| `open()` | Opens a file. |
| `close()` | Closes a file descriptor. |
| `signal()` | Registers a signal handler. |

These functions provide the foundation for process creation, program execution, inter-process communication, and job control.

## ⚙️ Compilation and Execution

### Prerequisites

- Linux or Ubuntu (WSL is supported).
- GCC compiler.
- All source files and `external_commands.txt` in the project directory.

### Step 1: Navigate to the Project Directory

```bash
cd "mini_shell project"
```

If you are already in the project directory, skip this step.

### Step 2: Compile the Source Files

```bash
gcc main.c scan_input.c commands.c -o minishell
```

### Step 3: Run the Mini Shell

```bash
./minishell
```

### Step 4: Enter Commands

After the Mini Shell starts, enter commands at the prompt. The following examples show the outputs from your terminal session.

**Important:** Run the executable from the directory containing `external_commands.txt`, because the program loads that file using a relative path.

## 🖥️ Sample Outputs

### 1. Directory and File Listing

```text
shabeena/minishell$ pwd
/mnt/d/SHABEENA/Documents/EMERTXE/EMERTXE LINUX PROJECTS/mini_shell project

shabeena/minishell$ ls
a.out  commands.c  external_commands.txt  main.c  scan_input.c  shell.h

shabeena/minishell$ ls -l
total 60
-rwxrwxrwx 1 sksha sksha 22280 Oct 24 06:10 a.out
-rwxrwxrwx 1 sksha sksha  7891 Oct 15 15:18 commands.c
-rwxrwxrwx 1 sksha sksha  1289 Oct  9 04:10 external_commands.txt
-rwxrwxrwx 1 sksha sksha  9928 Oct 15 15:53 main.c
-rwxrwxrwx 1 sksha sksha  4297 Oct 15 15:11 scan_input.c
-rwxrwxrwx 1 sksha sksha  1437 Oct 11 15:46 shell.h
```

### 2. Stopping and Listing Jobs

```text
shabeena/minishell$ sleep 10
^Z[1]+  Stopped                    sleep 10

shabeena/minishell$ sleep 20
^Z[2]+  Stopped                    sleep 20

shabeena/minishell$ jobs
[2]+  Stopped                    sleep 20
[1]+  Stopped                    sleep 10
```

### 3. Foreground Job Execution

```text
shabeena/minishell$ fg
sleep 20
shabeena/minishell$ fg
sleep 10
shabeena/minishell$ jobs
shabeena/minishell$
```

After both jobs finish, the job list is empty.

### 4. Background Job Execution

```text
shabeena/minishell$ sleep 20
^Z[1]+  Stopped                    sleep 20

shabeena/minishell$ sleep 25
^Z[2]+  Stopped                    sleep 25

shabeena/minishell$ jobs
[2]+  Stopped                    sleep 25
[1]+  Stopped                    sleep 20

shabeena/minishell$ bg
[2]+ sleep 25 &

shabeena/minishell$ bg
[1]+ sleep 20 &
```

### 5. Background Job Completion

```text
[2]+ Done                    sleep 25

[1]+ Done                    sleep 20
```

### 6. Command Piping

**Example 1:**

```text
shabeena/minishell$ ls | wc
      6       6      67
```

**Example 2:**

```text
shabeena/minishell$ ls -l | grep c | wc
shabeena/minishell$       6      54     331
```

The second output reflects the prompt appearing before the pipeline output in your terminal session.

### 7. Background Job Already Running

```text
shabeena/minishell$ bg
bash: bg: job already in background

shabeena/minishell$ bg
bash: bg: job already in background
```

This demonstrates the message displayed when the shell is asked to resume a job that is already running in the background.

## ✅ Result

The Mini Shell was implemented in C and executed in a Linux environment.

The project demonstrates:

- Execution of built-in and external commands.
- Process creation and program execution using Linux system calls.
- Command piping between processes.
- Basic stopped-job tracking and foreground/background resumption.
- Signal handling and child-process management.

This project provides a practical foundation for understanding Linux shells and system-level programming in C.

## 📚 Learning Outcomes

Through this project, I gained practical experience in:

- Understanding how a shell accepts and processes commands.
- Differentiating built-in commands from external programs.
- Creating processes using `fork()`.
- Executing programs using `execvp()`.
- Monitoring child processes using `waitpid()`.
- Implementing pipelines using `pipe()` and `dup2()`.
- Handling signals in Linux applications.
- Implementing basic foreground and background job control.
- Using linked lists to track jobs.
- Organizing C programs using header files and multiple source files.

## 🚀 Future Enhancements

- Improve job tracking and job-number management.
- Make foreground and background process handling more reliable.
- Improve signal handling and child-process completion notifications.
- Add input/output redirection using `<`, `>`, and `>>`.
- Support quoted strings and more robust command parsing.
- Improve error handling and command validation.
- Add environment-variable support and command history.
- Enhance compatibility with standard Linux shell behavior.

## 👩‍💻 Author

**Shabeena**

📧 Email: skshabeena33@gmail.com

🔗 LinkedIn: https://www.linkedin.com/in/shaik-shabeena-36a7b9332/

