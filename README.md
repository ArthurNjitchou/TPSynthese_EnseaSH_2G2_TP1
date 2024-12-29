# TPSynthese_EnseaSH_2G2_TP1
## Authors: NJITCHOU Arthur & HAZAN Justine

## Description

ENSEA Shell is a tiny command interpreter developed as part of the 2nd year Computer Science practical work at ENSEA. It allows users to execute commands, display exit codes, and measure execution times of launched programs.

## Objectives

The project aims to create a minimal shell with the following features:

1. Display a welcome message and a simple prompt.
2. Execute user-entered commands in a REPL (Read-Eval-Print Loop).
3. Manage shell output with the command `exit` or `<ctrl>+d`.
4. Display the return code (or signal) of the previous command in the prompt.
5. Measure command execution time using `clock_gettime`.
6. Execute complex commands with arguments.
7. Manage input and output redirection with `>` and `<`.
8. Manage pipe redirection with `|`.
9. Execute programs in the background with `&`.

## Installation (Important! Each code is for the specific question)
For all function, we need to : 
1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd <repository-directory>
   ```
We will compile each code regarding of what we need to do.
2. Compile the code:
   ```bash
   gcc -o enseash enseash.c
   ```
If it is for the second function: 
```bash
   gcc -o q2enseash q2enseash.c
   ```
... 

If it is for the last function : 
```bash
   gcc -o q7enseash q7enseash.c
   ```
## Usage

To launch the shell, run the following command:

```bash
./enseash
```

### Basic Commands

- Type a simple command, such as `fortune` or `date`, to execute the program.
- Type `exit` to quit the shell.

### Examples of Usage

```bash
$ ./enseash
Welcome to ENSEA Tiny Shell.
Type 'exit' to quit.
enseash % fortune
Today is what happened to yesterday.
enseash % date
Sun Dec 13 13:19:40 CET 2020
enseash %
```


### Background Execution

To run programs in the background, use the `&` symbol.

```bash
enseash % sleep 10 &
[1] 3656
enseash [1&] %
[1]+ Ended: sleep 10 &
enseash [exit: 0|10s] %
```
