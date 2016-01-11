/*
  disassemble.c
  disassemble 6502 code 
  init 26.8.2015
*/

#include "opcode_tbl.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include "register.h"

// analyse content at memory mem and write ass code into buffer
int dis(struct _6510_cpu* cpu, char mem[][9], char* buffer){
  int opcode = conv_bitstr2int(mem[0],0,7);
  int m2 = conv_bitstr2int(mem[1],0,7);
  int m3 = conv_bitstr2int(mem[2],0,7);
  int pc = pc2int(cpu);
  if( opcode_tbl[opcode].op_fct == 0 ) {
    return -1;
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "imp")) {
    sprintf(buffer, "%04x  %02x        %s          ", pc, opcode, opcode_tbl[opcode].name);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "imm")) {
    sprintf(buffer, "%04x  %02x %02x     %s #$%02x     ", pc, opcode, m2, opcode_tbl[opcode].name, m2);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "zp")) {
    sprintf(buffer, "%04x  %02x %02x     %s $%02x      ", pc, opcode, m2, opcode_tbl[opcode].name, m2);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "zpx")) {
    sprintf(buffer, "%04x  %02x %02x     %s $%02x,X    ", pc, opcode, m2, opcode_tbl[opcode].name, m2);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "zpy")) {
    sprintf(buffer, "%04x  %02x %02x     %s $%02x,Y    ", pc, opcode, m2, opcode_tbl[opcode].name, m2);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "izx")) {
    sprintf(buffer, "%04x  %02x %02x     %s ($%02x,X)  ", pc, opcode, m2, opcode_tbl[opcode].name, m2);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "izy")) {
    sprintf(buffer, "%04x  %02x %02x     %s ($%02x),Y  ", pc, opcode, m2, opcode_tbl[opcode].name, m2);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "abs")) {
    sprintf(buffer, "%04x  %02x %02x %02x  %s $%02x%02x    ", pc, opcode, m2, m3, opcode_tbl[opcode].name, m3, m2);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "abx")) {
    sprintf(buffer, "%04x  %02x %02x %02x  %s $%02x%02x,X  ", pc, opcode, m2, m3, opcode_tbl[opcode].name, m3, m2);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "aby")) {
    sprintf(buffer, "%04x  %02x %02x %02x  %s $%02x%02x,Y  ", pc, opcode, m2, m3, opcode_tbl[opcode].name, m3, m2);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "rel")) {
    sprintf(buffer, "%04x  %02x %02x     %s %02x       ", pc, opcode, m2, opcode_tbl[opcode].name, m2);
  }
  return 1;
}

int dis_mem(char mem[][9], int adr, char* buffer){
  int opcode = conv_bitstr2int(mem[adr],0,7);
  int m2 = conv_bitstr2int(mem[adr+1],0,7);
  int m3 = conv_bitstr2int(mem[adr+2],0,7);

  if( opcode_tbl[opcode].illegal ) {
    sprintf(buffer, "%s  illegal  ", opcode_tbl[opcode].name);
    return opcode_tbl[opcode].bytes ? 1 : opcode_tbl[opcode].bytes;
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "imp")) {
    sprintf(buffer, "%s          ", opcode_tbl[opcode].name);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "imm")) {
    sprintf(buffer, "%s #$%02x     ", opcode_tbl[opcode].name, m2);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "zp")) {
    sprintf(buffer, "%s $%02x      ", opcode_tbl[opcode].name, m2);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "zpx")) {
    sprintf(buffer, "%s $%02x,X    ", opcode_tbl[opcode].name, m2);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "zpy")) {
    sprintf(buffer, "%s $%02x,Y    ", opcode_tbl[opcode].name, m2);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "izx")) {
    sprintf(buffer, "%s ($%02x,X)  ", opcode_tbl[opcode].name, m2);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "izy")) {
    sprintf(buffer, "%s ($%02x),Y  ", opcode_tbl[opcode].name, m2);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "abs")) {
    sprintf(buffer, "%s $%02x%02x    ", opcode_tbl[opcode].name, m3, m2);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "abx")) {
    sprintf(buffer, "%s $%02x%02x,X  ", opcode_tbl[opcode].name, m3, m2);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "aby")) {
    sprintf(buffer, "%s $%02x%02x,Y  ", opcode_tbl[opcode].name, m3, m2);
  }
  if(!strcmp( opcode_tbl[opcode].adrmode, "rel")) {
    sprintf(buffer, "%s %02x       ", opcode_tbl[opcode].name, m2);
  }
  return opcode_tbl[opcode].bytes;
}
