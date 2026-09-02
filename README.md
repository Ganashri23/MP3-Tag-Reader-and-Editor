# MP3 Tag Reader and Editor

## Description

This project is a **menu-driven MP3 Tag Reader and Editor developed in C**. It is used to read and modify **ID3v2.3 metadata** stored in an MP3 file.

The project provides options to view and edit information such as the **Title, Artist, Album, Year, Genre, and Comment** of an MP3 file.

It also supports **Command Line Arguments (CLA)** and provides a help option to guide the user through the available commands.

## Features

* Read MP3 ID3v2.3 tag information
* Display MP3 metadata
* Edit MP3 tag information
* Modify individual metadata fields
* Support for Command Line Arguments (CLA)
* Menu-driven interface
* Help option for available commands
* File handling for reading and updating MP3 files

## MP3 Metadata

The project works with commonly used ID3v2.3 metadata fields:

* Title
* Artist
* Album
* Year
* Genre
* Comment

## How It Works

1. The MP3 file is provided as input using Command Line Arguments.
2. The program validates the input and opens the MP3 file.
3. The ID3 tag information is read from the file.
4. The program displays a menu for viewing or editing the tags.
5. For editing, the selected metadata field is modified.
6. The updated tag information is written back to the MP3 file.

## Operations

### View

The View operation reads the MP3 metadata and displays the available tag information.

### Edit

The Edit operation allows the user to select a particular tag and update its value.

The modified information is then written back to the MP3 file.

### Help

The Help option displays information about the available commands and how to use the application.

## Command Line Arguments

The program uses Command Line Arguments to provide the MP3 file and select the required operation.

Example:

```bash
./mp3tag -v sample.mp3
```

The help option can be used to view the available commands:

```bash
./mp3tag --help
```

## Project Structure

```text
MP3-Tag-Reader-and-Editor/
│
├── main.c
├── view.c
├── edit.c
├── help.c
├── function.h
├── sample.mp3
├── .gitignore
└── .vscode/
```

## Concepts Used

* C Programming
* File Handling
* Structures
* Pointers
* Strings
* Functions
* Command Line Arguments
* Binary File Operations
* `fopen()`
* `fread()`
* `fwrite()`
* `fseek()`
* `memcpy()`
* Menu-driven programming

## Technologies Used

* **Language:** C
* **File Format:** MP3
* **Metadata Format:** ID3v2.3
* **Compiler:** GCC

## Compilation

```bash
gcc main.c view.c edit.c help.c -o mp3tag
```

## Execution

```bash
./mp3tag
```

## Learning Outcome

This project provided practical experience in **C file handling, binary file processing, Command Line Arguments, pointers, structures, string manipulation, and reading and modifying MP3 metadata**.

## Author

**Ganashri S**
