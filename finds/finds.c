/*
 * Program written by Pablo Gutierrez at Boston University 
 * Finds: a simple program that allows you to search files in specified path for specified input string
 * 
 * How to run: finds -p pathname [-f c|h|S] [-l] -s s
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>

#define FLAGS "p:f:ls:"
#define ENABLED 0
#define DISABLED 1
#define READ "r"

int search_file(char *path, char *filetype, char *filename, char *input_str) {               
  char *line     = NULL;  //Pointer for current line in file              
  char printpath = 0;     //Used to organize print statements to stdout
  size_t len     = 0;     //Pointer used to store length of line
  ssize_t read;           //Pointer to return value of getline
  FILE *fp;               //File pointer
  char *ext;              //Pointer to extension of file
  
  //Searches all regular files
  if (filetype == NULL) { 
    fp = fopen(path, READ); 
    if(fp == NULL) 
      fprintf(stderr, "Invalid path or file: %s\n", path); 
    while ((read = getline(&line, &len, fp)) != -1) {
      //strstr - method for finding substring in a string
      if (strstr(line, input_str) != NULL) {
  	fprintf(stdout, "%s", line); 
	printpath = 1; 
      }
    }
    if (printpath == 1) 
      fprintf(stdout, "%s\n", path); 
    
  //Searches only files with extension that is specified
  } else { 
    //strrchr - method for moving pointer of string to value specified 
    ext = strrchr(filename, '.'); 
    if (ext != NULL) {
      if (strcmp((ext+1), filetype) == 0) {
	fp = fopen(path, READ); 
       	if(fp == NULL) 
	  fprintf(stderr, "Invalid path or file: %s\n", path); 
	while ((read = getline(&line, &len, fp)) != -1) {
	  if (strstr(line, input_str) != NULL) {
	    fprintf(stdout, "%s", line); 
	    printpath = 1; 
	  }
	}
	if (printpath == 1) 
	  fprintf(stdout, "%s\n", path);
      }
    }
  }
  return 0; 
}

int search_path(char *path, char *filetype, int sl, char *input_str) {
  char *fullpath;             //Path specified by command-line
  char *newpath;              //New path created for each item (directory, file or symbolic link)
  int len = PATH_MAX + 1;     //Max path length for linux machine +1
  DIR *dp;                    //Directory pointer
  struct dirent *current_item;//Struct with info for each item
  unsigned char dtype;        //Used to get info from struct
  char *dname;                //Used to get info from struct 
  
  
  fullpath        = (char *)malloc(len); 
  newpath         = (char *)malloc(len); 
  strcpy(fullpath, path);                 //create controlled buffer: path --> fullpath
  fullpath[len-1] = 0;                    //Null-terminated
  fullpath        = strcat(fullpath, "/");
  strcpy(newpath, fullpath);              //initialize controlled buffer: fullpath --> newpath
  
  //Opens directory if this is possible
  if ((dp = opendir(fullpath)) == NULL) {
    fprintf (stderr, "finds: '%s': No such file or directory", path);
    return -1; 
  }

  while ((current_item = readdir(dp)) != NULL) { 
    if (strcmp(current_item->d_name, ".") == 0 || strcmp(current_item->d_name, "..") == 0)
      continue; 
    // If item is directory call method again on that directory
    if ((dtype = current_item ->d_type) == DT_DIR) {
      dname = current_item -> d_name;
      strcat(newpath, dname);        
      search_path(newpath, filetype, sl, input_str);
      strcpy(newpath, fullpath);                    //reset newpath variable
    } else if (dtype == DT_REG) {
      dname = current_item -> d_name;
      strcat(newpath, dname);    
      search_file(newpath, filetype, dname, input_str); 
      strcpy(newpath, fullpath);                   //reset newpath variable
    } else if (sl == ENABLED && dtype == DT_LNK) {
      dname = current_item -> d_name;
      strcat(newpath, dname);       
      search_path(newpath, filetype, sl, input_str);
      strcpy(newpath, fullpath);                   //reset newpath variable
    } else {
      continue; 
    }
  }
  free(fullpath); 
  free(newpath); 
  return 0; 
}

int main(int argc, char **argv) {
  int c;
  char *path             = NULL; 
  char *filetype         = NULL; 
  int  sl                = DISABLED;   //Symbolic links
  char *input_str        = NULL;

  // Value initialized for getopt()
  opterr                 = 0; 

  //uses getopt to organize command-line arguments
  while ((c = getopt(argc, argv, FLAGS)) != -1) 
    switch (c) 
      {
      case 'p':
	path = optarg;
	break;
      case 'f':
	filetype = optarg; 
	break; 
      case 'l':
	sl = ENABLED; 
	break; 
      case 's':
	input_str = optarg; 
	break; 
      case '?':
	if (optopt == 'p')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
	else if (optopt == 'f')
	  fprintf (stderr, "Option -%c requires an argument.\n", optopt); 
	else if (optopt == 's')
	  fprintf (stderr, "Option -%c requires an argument.\n", optopt); 
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
	return 1; 
      default:
	abort();
      }
 
  // Begin searching specified path for string 
  search_path(path, filetype, sl, input_str); 
  return 0; 
}


//eof 
