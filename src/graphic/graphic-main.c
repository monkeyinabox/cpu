/*

  graphic-main.c
  bho1 - init 23.8.2015 

  Compile with  `sdl-config --cflags --libs`
  

  
*/
#include <string.h>
#include <stdio.h>
#include <signal.h>

#include "register.h"
#include "print-cpu.h"
//#include "cpu.h"
#include "memory.h"



extern char *read_line();
extern int parse_line(struct _6510_cpu* cpu, char memory[][9], char* string);

static int do_parse = 1;

#include <stdlib.h>

#include "graphic.h"

#include "SDL.h"
#include "SDL_mouse.h"

#include <SDL_events.h>

struct _vic* vic;

struct _6510_cpu cpu;
char memory[0x10000][9];

char *randomptr = 0;

// u are here because u got hit by a C-c
// go back to parsing
void intHandler(int dummy) {
  do_parse = 1;
  dummy++;
  dummy--;
}

//
// write random into mem[0xfe]
// used in www.6502asm.com simulation
//
void rnd_update(){
  if(randomptr){
    int2register(rand() % 0x100,randomptr);
  }
}
  
/*
  main:
  options:

  --memory <memory-dump>
  Read .mem file <memory-dump> and copy into memory
  --binary <binary file> <address>
  load binary file into memory at adress <address>
  --source <disassembler source>
  Load file <disassembler source> into source array
  --register <filename>
  copy content in <filename>  into corresponding register
*/
int main(int argc, char *argv[])
{
  
  signal(SIGINT, intHandler);
  
  init_memory(memory);
  
  reset_cpu(&cpu);
  struct _vic vicstruct;
  vic = &vicstruct;
  init_display(vic);
  
  // handling parameter --memory <dumpfile>
  // ./sipiu --memory sipiu-codebase/disco/disco.mem
  if(argc==3){
    if(!strcmp(argv[1], "--memory")){
      printf("Loading %s into memory\n", argv[2]);
      randomptr =  memory[0x00fe];
      read_memorydump(argv[2], memory);
    }
  }
  // handling parameter --binary <dumpfile>  adr
  // ./sipiu --binary test/cl-6502/test.bin  000a
  if(argc==4){
    if(!strcmp(argv[1], "--binary")){
      int adr =0;
      sscanf(argv[3], "%4x ", &adr);
    
      printf("Loading %s into memory at 0x%04x\n", argv[2], adr);
      // testvector
      read_binary_memorydump(argv[2], memory, adr);
      // cl-6502
      //      read_binary_memorydump(argv[2], memory, 0x0000);
    }
  }

  vic->videobase = 0x200;
  printf("Video memory base at %04x\n", vic->videobase);

  draw_bitmap_memory(&cpu, vic, memory); // draw_memory(memory+videobase);

  print_cpu_dis(&cpu,memory);
  char *line = 0;
  
  while(1){
    // read input
    // parse input
    // do the stuff
    while(do_parse){
      line = read_line();
      do_parse = parse_line(&cpu, memory, line);
    }
    fetch(&cpu, memory);
    exec(&cpu, memory);

    print_cpu_dis(&cpu,memory);
    draw_bitmap_memory(&cpu, vic, memory);   
  }
  
  printf("exit sipiu ..\n");
  SDL_Quit();
  exit(0);
  return 0;
}
