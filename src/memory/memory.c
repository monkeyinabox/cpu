/*
  memory.c
  init: bo@1.2.2015
  implements memory: mem_mao, memory_access

*/
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <memory.h>

#include "register.h"


void init_memory(char memory[][9] ) {
  int i,j;
  for(i=0;i<MEMORY_SIZE;i++){
    for(j=0;j<8;j++){
      memory[i][j] = '0';
    }
    memory[i][j] = 0;
  }
}

char* ram(int adr, char memory[][9]){
  return memory[adr];
}
char* basic_ROM(int adr, char memory[][9]){
  return memory[adr];
}

char* kernel_ROM(int adr, char memory[][9]){
  return memory[adr];
}

char* char_ROM(int adr, char memory[][9]){
  return memory[adr];
}

char* IO_ram(int adr, char memory[][9]){
  return memory[adr];
}


void read_memorydump(char *fname, char memory[][9]){
  char buffer_reg[8+1];
  int adr, content;
  FILE *stream;
  
  buffer_reg[8] = '\0';
  if(!strcmp("-", fname)){
    stream = stdin;
  } else if ((stream = fopen( fname,"r")) == NULL){
    printf("Can't open %s\n", fname);
    exit(1);
  }
  while(!feof(stream)){
    // format string is:
    // <any space, including 0> <hex address> <any space, including 0> <hex content>
    //DELME    fscanf(stream, " %i %8c ", &adr, buffer);
    fscanf(stream, " %4x %2x ", &adr, &content);
    if(adr>=MEMORY_SIZE){
      printf("Illegal Address %i\nSkipping line ..\n", adr);
    } else {
      //	  printf("%i: %i->%02x\n", i++, adr, content);
      int2register(content, buffer_reg);
      cp_byte(buffer_reg, memory[adr]);
    }
  }
}

void read_binary_memorydump(char *fname, char memory[][9], int start){
  unsigned char c;
  int adr = start;
  FILE *stream;
  
  if(!strcmp("-", fname)){
    stream = stdin;
  } else if ((stream = fopen( fname,"r")) == NULL){
    printf("Can't open %s\n", fname);
    exit(1);
  }
  while(!feof(stream)){
    if( 1 != fscanf(stream, "%c", &c)){
      break;
    }
    if(adr<0 || adr>=MEMORY_SIZE-1){
      printf("Illegal Address %i\nSkipping line ..\n", adr);
    } else {
      int2register(c, memory[adr++]);
    }
  }
}
