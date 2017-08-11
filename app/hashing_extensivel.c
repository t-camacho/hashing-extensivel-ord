#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/hash.h"

int main(int argc, char *argv[]){
  FILE *file;
  char key[500];
  if(argc == 2){
    file = fopen(argv[1], "r");
    start();
    while(fscanf(file, "%s", key) != EOF){
      op_add(atoi(key));
      memset(key, '\0', 500);
    }
  }
  print_dir();
  return 0;
}
