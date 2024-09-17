# Directory Copy Library in C

## Overview

This project is a C library designed to replicate the functionality of Python's `shutil.copytree`, allowing users to copy entire directory trees from a source to a destination directory. It handles various aspects of file copying, including symbolic links and file permissions, which can be controlled through user-specified flags.

## Project Description

### Objective

To develop a robust C library capable of recursively copying directories with options to handle symbolic links as links and to replicate file permissions accurately.

### Features

- **Recursive Directory Copying**: Copies all subdirectories and files from the source to the destination.
- **Symbolic Link Handling**: Optionally copies symbolic links as links rather than copying the files they point to.
- **Permission Copying**: Optionally replicates the file permissions from the source files to the destination files.
- **Robust Error Handling**: Implements comprehensive error handling to manage and report issues during the copy process.
- **Command-Line Interface**: Includes a command-line application to use the library functions, providing flags for optional behaviors.

### Implementation Details

#### Functions

- `copy_file`: Copies individual files, handling symbolic links and permissions based on flags.
- `copy_directory`: Recursively copies directories and their contents, leveraging `copy_file` for file copying.
- `create_directory`: Helper function to create directories as needed during the copying process.

#### Error Management

- Detailed error reporting with `perror` for system call failures.
- Cleanup routines ensure no resources are leaked even when errors occur.

### Usage

To use the library, compile the source code and link against the created library file. Here is how you can compile and run the accompanying command-line program:

```bash
gcc -c copytree.c -o copytree.o
ar rcs libcopytree.a copytree.o
gcc main.c -L. -lcopytree -o copy_directory_program
./copy_directory_program source_directory destination_directory
Command-Line Arguments
-l: Copy symbolic links as links.
-p: Copy file permissions.
Example command to use the library with all features:

bash
Copy code
./copy_directory_program -l -p source_directory destination_directory
Testing
Set up a test directory structure as described in the assignment to verify the functionality of your library. Use the command-line flags to test different configurations and ensure the output is as expected.

Provided Files
copytree.h: Header file with function declarations.
copytree.c: Implementation of the directory copy functionalities.
main.c: Main program to demonstrate and use the library.
Contributions
Contributions to enhance the functionality or improve the error handling are welcome. Please fork the repository, make your changes, and submit a pull request.

License
This project is released under the MIT License.

Contact
For more information, support, or to report issues, please contact me at [RanWurembrand@gmail.com].
