#Finds 

finds: a program that searches for a string inside all 
files of directory and displays the lines with that string 
and the path it is contained in; written in C. 

Similar to UNIX command: "find -type f pathname -name '*' -exec grep 's' {} \; -print"

Example: ./finds -p <pathname> [-f c|h|S] -s <string> 

Flag options: 
	-f c : only search for .c files
	-f h : only search for .h files
	-f S : only search for .S files

Substitute <pathname> for the desired path and <string> for the string that is being 
searched. 

Example input:  "./finds -p ~/Desktop/github/miscellaneous-code -f c -s echo"

Example output: 

"my_echo: an emulation of the echo program in linux: it will print 

my_echo hello

/Users/PGP/Desktop/github/miscellaneous-code/my_echo/my_echo.c"

Author: Pablo Gutierrez