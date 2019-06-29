# Project1-Spring2017
C program that matches words in a set of input files to a given keyword
- Implemented using multiple processes created by main program.
- Children are created using fork() system call.
- A child is created for each input file.

# pmatch1.c
Information is passed from children to parent using intermadiate files.

# pmatch2.c
Information is passed from children to parent using unnamed pipes.

# Notes
- Duplicate input files are not checked
- If the keyword is located at the end of the file, program fails to recognize it as a match.
