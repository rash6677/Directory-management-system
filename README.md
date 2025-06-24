# Directory Management System

A simple, educational in-memory file and directory management system implemented in C++. This project simulates basic file system operations (create, read, delete, list) using a command-line interface, without interacting with the actual disk.

Features

Create virtual files with content
Read file contents
Delete files
List all files with metadata (size, blocks used)
Simple interactive CLI for user commands

Prerequisites
C++ compiler (g++)

Compilation
Open your terminal and run:

bash
g++ main.cpp -o filesystem
Replace main.cpp with your actual source file name if different.

Running
bash
./filesystem      # On Linux/macOS
filesystem.exe    # On Windows
Usage
On running, you’ll see a prompt like:

text
Simple File System CLI
Commands:
  create <path> <content>
  read <path>
  delete <path>
  list
  exit
>
Examples:

Create a file:
create /notes/todo.txt Buy groceries

List files:
list

Read a file:
read /notes/todo.txt

Delete a file:
delete /notes/todo.txt

Exit:
exit

How It Works
All files and their data exist only in memory while the program is running.
No real files are created or modified on your disk.
The system uses simple data structures (std::unordered_map, custom Inode, etc.) to simulate file system behavior.
