#include<stdio.h>
#include<string.h>
#include<stdbool.h>

main(int argc, char *argv[]) {
  int i = 1; 
  bool omit = false; 

  if(argc > 1) {
    if(strcmp(argv[1], "-n") == 0) {
      i = 2; 
      omit = true; 
    }
    for( i; i < argc; i++) {
      if( (argc - i) == 1) {
	printf("%s", argv[i]); 
      } else {
	printf("%s ", argv[i]);
      } 
    }
  }
  if(!omit) {
    printf("\n"); 
  }


}
