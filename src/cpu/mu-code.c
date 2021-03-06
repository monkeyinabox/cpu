/***

student mucode.c

***/

#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "alu.h"
#include "flags.h"
#include "register.h"

int sp2int(struct _6510_cpu*cpu){
  int sp = 0x100 | conv_bitstr2int(cpu->sp,0,7);
  return sp;
}
void eadr2adr(int eadr, char*adr){
  int bits = 0x1;
  int i;
  for(i=0;i<16;i++){
    if(eadr>>i & bits){
      adr[15-i]='1';
    }
  }
}

// dummy, not used
int imp_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){
  return -1;
}

// immediate
int imm_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){

  int pc = pc2int(cpu);
  inc_pc(cpu);
  return ( conv_bitstr2int(memory[pc+1],0,7) << 8) | conv_bitstr2int(memory[pc],0,7) ;
}

// relative
int rel_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){
/**
  cp_byte(cpu->pcl, cpu->abrl);
  cp_byte(cpu->pch, cpu->abrh);

  cpu->rw='1';
  access_memory(cpu, memory);
 
  alu(ALU_OP_ADD, cpu->dbr, cpu->pcl, cpu->dbr, 0); 
  
  //strcpy(dummy, cpu->pcl);
  //strcat(dummy, cpu->pch);
  
  return conv_bitstr2int(cpu->dbr,0,15);
*/
    int pc = pc2int(cpu);
    int pcData =  conv_bitstr2int(memory[pc],0,7) ;
    int iadr;
    if(pcData > 127){
      pcData = pcData - 256; 
      return pc + pcData + 1; 
    }
    else{
      return pc + pcData + 1; 
    }
}

// indirect X-indexed zero page
int izx_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){
 /** 
  char dummy[] = "00000000";
  char zero[] = "00000000";

  cp_byte(cpu->pcl, cpu->abrl);
  cp_byte(cpu->pch, cpu->abrh);

  cpu->rw='1';
  access_memory(cpu, memory);

  cp_byte(cpu->pcl, cpu->abrl);
  cp_byte(zero, cpu->abrh);

  cpu->rw='1';
  access_memory(cpu, memory);

  cp_byte(cpu->dbr, dummy);
  alu(ALU_OP_ADD, "00000001", cpu->dbr, cpu->dbr, 0); 

  cp_byte(cpu->dbr, cpu->abrl);
  cp_byte(zero, cpu->abrh);

  cpu->rw='1';
  access_memory(cpu, memory);

  alu(ALU_OP_ADD, cpu->dbr, cpu->regy, cpu->dbr, 0);

  cp_byte(cpu->dbr, cpu->abrl);
  cp_byte(dummy, cpu->abrh);

  return conv_bitstr2int(cpu->dbr,0,7);
*/
    int pc = pc2int(cpu);
    inc_pc(cpu);
    int pcData =  conv_bitstr2int(memory[pc],0,7) ;
    return ( conv_bitstr2int(memory[pcData + 1],0,7) << 8) | (conv_bitstr2int(memory[pcData],0,7) + conv_bitstr2int(cpu->regx,0,7)) ;

}

// indirect Y-indexed zero page
int izy_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){
/**  char dummy[] = "00000000";
  char zero[] = "00000000";

  cp_byte(cpu->pcl, cpu->abrl);
  cp_byte(cpu->pch, cpu->abrh);

  cpu->rw='1';
  access_memory(cpu, memory);

  cp_byte(cpu->pcl, cpu->abrl);
  cp_byte(zero, cpu->abrh);

  cpu->rw='1';
  access_memory(cpu, memory);

  cp_byte(cpu->dbr, dummy);
  alu(ALU_OP_ADD, "00000001", cpu->dbr, cpu->dbr, 0); 

  cp_byte(cpu->dbr, cpu->abrl);
  cp_byte(zero, cpu->abrh);

  cpu->rw='1';
  access_memory(cpu, memory);


  alu(ALU_OP_ADD, cpu->dbr, cpu->regy, cpu->dbr, 0);

  cp_byte(cpu->dbr, cpu->abrl);
  cp_byte(dummy, cpu->abrh);

  return conv_bitstr2int(cpu->dbr,0,7);
*/
  int pc = pc2int(cpu);
  inc_pc(cpu);
  int pcData =  conv_bitstr2int(memory[pc],0,7) ;
  return ( conv_bitstr2int(memory[pcData + 1],0,7) << 8) | ( conv_bitstr2int(memory[pcData],0,7) + conv_bitstr2int(cpu->regy,0,7)) ;
}

// zero page 
int zp_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){
  char zero[9]="00000000";
 
  cp_byte(cpu->pcl, cpu->abrl);
  cp_byte(cpu->pch, cpu->abrh);
  
  cpu->rw='1';
  access_memory(cpu, memory);

  cp_byte(cpu->dbr,cpu->abrl);
  cp_byte(zero,cpu->abrh);

  cpu->rw='1';
  access_memory(cpu, memory);
  
  inc_pc(cpu);
  return conv_bitstr2int(cpu->dbr,0,7);
}

// zero page with index register X
int zpx_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){
 
  char zero[9]="00000000";
 
  cp_byte(cpu->pcl, cpu->abrl);
  cp_byte(cpu->pch, cpu->abrh);
  
  cpu->rw='1';
  access_memory(cpu, memory);

  alu(ALU_OP_ADD, cpu->dbr, cpu->regx, cpu->dbr,"00000000");
  cp_byte(cpu->dbr,cpu->abrl);
  cp_byte(zero,cpu->abrh);

  cpu->rw='1';
  access_memory(cpu, memory);
  
  inc_pc(cpu);
  return conv_bitstr2int(cpu->dbr,0,7);
}

// zero page with index register Y
int zpy_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){
  char zero[]="00000000";
 
  cp_byte(cpu->pcl, cpu->abrl);
  cp_byte(cpu->pch, cpu->abrh);
  
  cpu->rw='1';
  access_memory(cpu, memory);

  alu(ALU_OP_ADD, cpu->dbr, cpu->regy, cpu->dbr, 0);
  cp_byte(cpu->dbr,cpu->abrl);
  cp_byte(zero,cpu->abrh);

  cpu->rw='1';
  access_memory(cpu, memory);
  
  inc_pc(cpu);
  return conv_bitstr2int(cpu->dbr,0,7);
}

// absolute 
int abs_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){
  int pc = pc2int(cpu);
  inc_pc(cpu);
  inc_pc(cpu);
  return ( conv_bitstr2int(memory[pc + 1],0,7) << 8) | ( conv_bitstr2int(memory[pc],0,7) + conv_bitstr2int(cpu->regy,0,7)) ;
}

// absolute with index x
int abx_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){
/*  char dummy[] = "00000000";

  cp_byte(cpu->pcl, cpu->abrl);
  cp_byte(cpu->pch, cpu->abrh);

  cpu->rw='1';
  access_memory(cpu, memory);

  cp_byte(cpu->dbr, dummy);
  
  inc_pc(cpu);

  cp_byte(cpu->pcl, cpu->abrl);
  cp_byte(cpu->pch, cpu->abrh);

  access_memory(cpu, memory);
  inc_pc(cpu);
  
  cp_byte(cpu->dbr, cpu->abrl);
  cp_byte(dummy, cpu->abrh);
  
  alu(ALU_OP_ADD, cpu->abrl, cpu->regx, cpu->abrl, 0);
  cpu->rw='1';
  access_memory(cpu, memory);
// Need to return 16Bit
return conv_bitstr2int(cpu->dbr,0,7);
*/

    int pc = pc2int(cpu);
    inc_pc(cpu);
    inc_pc(cpu);
    return ( conv_bitstr2int(memory[pc + 1],0,7) << 8) | (conv_bitstr2int(memory[pc],0,7) + conv_bitstr2int(cpu->regx,0,7)) ;

}

// absolute with index y
int aby_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){
/**  char dummy[] = "00000000";

  cp_byte(cpu->pcl, cpu->abrl);
  cp_byte(cpu->pch, cpu->abrh);

  cpu->rw='1';
  access_memory(cpu, memory);

  cp_byte(cpu->dbr, dummy);

  inc_pc(cpu);

  cp_byte(cpu->pcl, cpu->abrl);
  cp_byte(cpu->pch, cpu->abrh);

  access_memory(cpu, memory);
  inc_pc(cpu);

  cp_byte(cpu->dbr, cpu->abrl);
  cp_byte(dummy, cpu->abrh);

  alu(ALU_OP_ADD, cpu->abrl, cpu->regy, cpu->abrl, 0);
  cpu->rw='1';
  access_memory(cpu, memory);

// Need to return 16Bit
return conv_bitstr2int(cpu->dbr,0,7);
*/
    int pc = pc2int(cpu);
    inc_pc(cpu);
    inc_pc(cpu);
    return ( conv_bitstr2int(memory[pc + 1],0,7) << 8) | (conv_bitstr2int(memory[pc],0,7) + conv_bitstr2int(cpu->regy,0,7)) ;


} 

// dummy, not used
int ind_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){
  return -1;
} 

void zsflag(struct _6510_cpu* cpu, char* mem){
  int i;
  cpu->flags[SFLAG] = mem[0];
  cpu->flags[ZFLAG] = '1';
  for(i=0;i<8;i++){
    if(mem[i]=='1'){
      cpu->flags[ZFLAG] = '0';
      break;
    }
  }
}


void cpu_6502_illegal(struct _6510_cpu* cpu, char memory[][9] ){}
 
void do_ADC(struct _6510_cpu* cpu, char *mem){
  alu(ALU_OP_ADC, cpu->rega, mem, cpu->rega, cpu->flags);
}
void do_AND(struct _6510_cpu* cpu, char *mem){
  alu(ALU_OP_AND, cpu->rega, mem, cpu->rega, cpu->flags);
}
void do_ASL(struct _6510_cpu* cpu, char *mem){
  alu(ALU_OP_ASL, mem, mem, mem, cpu->flags);
}

//http://www.dwheeler.com/6502/oneelkruns/asm1step.html
void do_BIT(struct _6510_cpu* cpu, char *mem){
  int i;
  cpu->flags[NFLAG] = mem[0];
  cpu->flags[VFLAG] = mem[1];
  for(i=0;i<8;i++){
    if(cpu->rega[i]=='1' &&  mem[i]=='1'){
      cpu->flags[ZFLAG] = '0';
      return;
    }
  }
  cpu->flags[ZFLAG] = '1'; 
}
void do_CMP(struct _6510_cpu* cpu, char *mem){
  char dummy[] = "00000000";
  alu(ALU_OP_CMP, cpu->rega, mem,  dummy, cpu->flags);
}
void do_CPX(struct _6510_cpu* cpu, char *mem){
  char dummy[] = "00000000";
  alu(ALU_OP_CMP, cpu->regx, mem,  dummy, cpu->flags);
}
void do_CPY(struct _6510_cpu* cpu, char *mem){
  char dummy[] = "00000000";
  alu(ALU_OP_CMP, cpu->regy, mem,  dummy, cpu->flags);
}
void do_DEC(struct _6510_cpu* cpu, char *mem){
  char one[] = "00000001";
  alu(ALU_OP_SUB, mem, one, mem, 0);
  zsflag(cpu, mem);
}
void do_EOR(struct _6510_cpu* cpu, char *mem){
  alu(ALU_OP_XOR, cpu->rega, mem, cpu->rega, 0);
  zsflag(cpu, cpu->rega);
}
void do_INC(struct _6510_cpu* cpu, char *mem){
  alu(ALU_OP_ADD, "00000001", mem, mem, 0); 
  zsflag(cpu, mem);
}
void do_LSR(struct _6510_cpu* cpu, char *mem){
  alu(ALU_OP_LSR, mem, mem, mem, cpu->flags); 
}
void do_ORA(struct _6510_cpu* cpu, char *mem){
  alu(ALU_OP_OR, cpu->rega, mem, cpu->rega, cpu->flags); 
}
void do_ROL(struct _6510_cpu* cpu, char *mem){
  alu(ALU_OP_ROL, mem, mem, mem, cpu->flags);
}
void do_ROR(struct _6510_cpu* cpu, char *mem){
  alu(ALU_OP_ROR, mem, mem, mem, cpu->flags);
}
void do_SBC(struct _6510_cpu* cpu, char *mem){
  alu(ALU_OP_SBC, cpu->rega, mem, cpu->rega, cpu->flags);
}

/*
  opname:  BRK
  opcode:  0x00 
  ex:      BRK 
  eadr:    implizit
  flags:   NVBDIZC :--1-1--
  bytes:   1 
  desc:    interrupt 
  form1:   (S)-=:PC,P PC:=($FFFE) 
  form2:   Stack <- PC, PC <- ($fffe) 
  form3:   *       PC = PC + 1 
           *       bPoke(SP,PC.h) 
           *       SP = SP - 1 
           *       bPoke(SP,PC.l) 
           *       SP = SP - 1 
           *       bPoke(SP, (P|$10) ) 
           *       SP = SP - 1 
           *       l = bPeek($FFFE) 
           *       h = bPeek($FFFF)<<8 
           *       PC = h|l              
           *    
          
*/

void cpu_6502_BRK_imp(struct _6510_cpu* cpu, char mem[][9]){
  // Stack <- PC, PC <- ($fffe)
  int sp = sp2int(cpu);
  cp_register(cpu->pcl,mem[sp--]);
  cp_register(cpu->pch,mem[sp]);
  alu(ALU_OP_SUB, cpu->sp, "00000010", cpu->sp, 0);
  cp_register(mem[0xfffe],cpu->pcl);
  cp_register(mem[0xffff],cpu->pch);  
}


/*
  opname:  ORA
  opcode:  0x01 
  ex:      ORA ($A5,X)
  eadr:    indexed with indirection
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    or with accumulator 
  form1:   A:=A or {adr} 
  form2:   A <- (A) V M               
  form3:   *       A = A | M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_ORA_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);
}


/*
  opname:  ORA
  opcode:  0x05 
  ex:      ORA $AB
  eadr:    zero page
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    or with accumulator 
  form1:   A:=A or {adr} 
  form2:   A <- (A) V M               
  form3:   *       A = A | M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_ORA_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);
}


/*
  opname:  ASL
  opcode:  0x06 
  ex:      ASL $AB
  eadr:    zero page
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    arithmetic shift left 
  form1:   {adr}:={adr}*2 
  form2:   C <- A7, A <- (A) << 1     
  form3:   *       P.C = B.7 
           *       B = (B << 1) & $FE 
           *       P.N = B.7 
           *       P.Z = (B==0) ? 1:0  
           *    
          
*/

void cpu_6502_ASL_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_ASL(cpu, mem[eadr]);
}


/*
  opname:  PHP
  opcode:  0x08 
  ex:      PHP 
  eadr:    implizit
  flags:   NVBDIZC :-------
  bytes:   1 
  desc:    push processor status (SR) 
  form1:   (S)-:=P 
  form2:   Stack <- (P)               
  form3:   *       bPoke(SP,P) 
           *       SP = SP - 1  
           *    
          
*/

void cpu_6502_PHP_imp(struct _6510_cpu* cpu, char mem[][9]){
/**
    char dummy[]= "00000000";
    dummy[7] = cpu->flags[NFLAG];
    dummy[6] = cpu->flags[VFLAG];
    dummy[5] = cpu->flags[XFLAG];
	dummy[4] = cpu->flags[BFLAG];
	dummy[3] = cpu->flags[DFLAG];
	dummy[2] = cpu->flags[IFLAG];
	dummy[1] = cpu->flags[ZFLAG];
	dummy[0] = cpu->flags[CFLAG];
	cp_byte(cpu->flags, cpu->sp); 
*/
	cp_byte(cpu->flags, cpu->sp); 
	do_DEC(cpu, cpu->sp);
	inc_pc(cpu);
}


/*
  opname:  ORA
  opcode:  0x09 
  ex:      ORA #$AB    
  eadr:    immediate
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    or with accumulator 
  form1:   A:=A or {adr} 
  form2:   A <- (A) V M               
  form3:   *       A = A | M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_ORA_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);
}


/*
  opname:  ASL
  opcode:  0x0A 
  ex:      ASL 
  eadr:    implizit
  flags:   NVBDIZC :*----**
  bytes:   1 
  desc:    arithmetic shift left 
  form1:   {adr}:={adr}*2 
  form2:   C <- A7, A <- (A) << 1     
  form3:   *       P.C = B.7 
           *       B = (B << 1) & $FE 
           *       P.N = B.7 
           *       P.Z = (B==0) ? 1:0  
           *    
          
*/

void cpu_6502_ASL_imp(struct _6510_cpu* cpu, char mem[][9]){
  do_ASL(cpu, cpu->rega);
  //inc_pc(cpu);
}


/*
  opname:  ORA
  opcode:  0x0D 
  example: ORA $ABCD
  adr:  absolute 16 bit address; data = mem[abs_high|abs_low]
  flags:    NVBDIZC :*----*-
  bytes: 3 
  
*/

void cpu_6502_ORA_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);
}


/*
  opname:  ASL
  opcode:  0x0E 
  example: ASL $ABCD
  adr:  absolute 16 bit address; data = mem[abs_high|abs_low]
  flags:    NVBDIZC :*----**
  bytes: 3 
  
*/

void cpu_6502_ASL_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_ASL(cpu, mem[eadr]);
}


/*
  opname:  BPL
  opcode:  0x10 
  ex:      BPL $AB
  eadr:    relativ
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    branch on plus (negative clear) 
  form1:   branch on N=0 
  form2:   if N=0, PC = PC + offset   
  form3:   *       if (P.N == 0) GOTO (PC+M)  
           *    
          
*/

void cpu_6502_BPL_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  char adr[] = "0000000000000000";
  eadr2adr(eadr,adr);

  if(cpu->flags[NFLAG]=='0'){
    cp_byte(adr, cpu->pch);
    cp_byte(&(adr[8]), cpu->pcl);
  } else {
    inc_pc(cpu);
  } 
}


/*
  opname:  ORA
  opcode:  0x11 
  ex:      ORA ($A5),Y
  eadr:    indirection with index
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    or with accumulator 
  form1:   A:=A or {adr} 
  form2:   A <- (A) V M               
  form3:   *       A = A | M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_ORA_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);
}


/*
  opname:  ORA
  opcode:  0x15 
  ex:      ORA $AB,X
  eadr:    zero page with index X
  /flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    or with accumulator 
  form1:   A:=A or {adr} 
  form2:   A <- (A) V M               
  form3:   *       A = A | M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_ORA_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);
}


/*
  opname:  ASL
  opcode:  0x16 
  ex:      ASL $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    arithmetic shift left 
  form1:   {adr}:={adr}*2 
  form2:   C <- A7, A <- (A) << 1     
  form3:   *       P.C = B.7 
           *       B = (B << 1) & $FE 
           *       P.N = B.7 
           *       P.Z = (B==0) ? 1:0  
           *    
          
*/

void cpu_6502_ASL_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_ASL(cpu, mem[eadr]);
}


/*
  opname:  CLC
  opcode:  0x18 
  ex:      CLC 
  eadr:    implizit
  flags:   NVBDIZC :------0
  bytes:   1 
  desc:    clear carry 
  form1:   C:=0 
  form2:   C <- 0                     
  form3:   *       P.C = 0  
           *    
          
*/

void cpu_6502_CLC_imp(struct _6510_cpu* cpu, char mem[][9]){
    cpu->flags[CFLAG] = '0';
}


/*
  opname:  ORA
  opcode:  0x19 
  ex:      ORA $ABCD,Y
  eadr:    absolut with index Y
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    or with accumulator 
  form1:   A:=A or {adr} 
  form2:   A <- (A) V M               
  form3:   *       A = A | M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_ORA_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);
}


/*
  opname:  ORA
  opcode:  0x1D 
  ex:      ORA $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    or with accumulator 
  form1:   A:=A or {adr} 
  form2:   A <- (A) V M               
  form3:   *       A = A | M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_ORA_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_ORA(cpu, mem[eadr]);
}


/*
  opname:  ASL
  opcode:  0x1E 
  ex:      ASL $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    arithmetic shift left 
  form1:   {adr}:={adr}*2 
  form2:   C <- A7, A <- (A) << 1     
  form3:   *       P.C = B.7 
           *       B = (B << 1) & $FE 
           *       P.N = B.7 
           *       P.Z = (B==0) ? 1:0  
           *    
          
*/

void cpu_6502_ASL_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_ASL(cpu, mem[eadr]);
}


/*
  opname:  JSR
  opcode:  0x20 
  ex:      JSR $ABCD
  eadr:    absolute
  flags:   NVBDIZC :-------
  bytes:   3 
  desc:    jump subroutine 
  form1:   (S)-:=PC PC:={adr} 
  form2:   Stack <- PC, PC <- Address 
  form3:   *       t = PC - 1 
           *       bPoke(SP,t.h) 
           *       SP = SP - 1 
           *       bPoke(SP,t.l) 
           *       SP = SP - 1 
           *       PC = $A5B6     
           *    
          
*/

void cpu_6502_JSR_abs(struct _6510_cpu* cpu, char mem[][9]){
/**
  int sp = sp2int(cpu);
  //int eadr = abs_adr_mode(cpu, mem);

  char dummy[] = "00000000";

  cp_register(cpu->pcl, cpu->abrl);
  cp_register(cpu->pch, cpu->abrh);

  cpu->rw='1';
  access_memory(cpu, mem);

  cp_register(cpu->dbr, dummy);

  cp_register(cpu->pcl, mem[sp--]);
  cp_register(cpu->pch, mem[sp]);

  alu(ALU_OP_SUB, cpu->sp, "00000010", cpu->sp, 0);

  cp_register(cpu->pcl, cpu->abrl);
  cp_register(cpu->pch, cpu->abrh);

  cpu->rw='1';
  access_memory(cpu, mem);

  cp_register(cpu->dbr, cpu->pcl);
  cp_register(dummy, cpu->pch);
  */
 int eadr = abs_adr_mode(cpu,mem);
 int sp = sp2int(cpu); 
 char adr[] = "0000000000000000";
 eadr2adr(eadr,adr);

 cp_register(cpu->pch, mem[sp--]);
 cp_register(cpu->pcl, mem[sp]);
 alu(ALU_OP_SUB, cpu->sp, "00000010", cpu->sp, 0);

 cp_byte(adr, cpu->pch);
 cp_byte(&(adr[8]), cpu->pcl);
}


/*
  opname:  AND
  opcode:  0x21 
  ex:      AND ($A5,X)
  eadr:    indexed with indirection
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    and (with accumulator) 
  form1:   A:=A\&{adr} 
  form2:   A <- (A) /\ M              
  form3:   *       A = A & M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_AND_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);
}


/*
  opname:  BIT
  opcode:  0x24 
  ex:      BIT $AB
  eadr:    zero page
  flags:   NVBDIZC :**---*-
  bytes:   2 
  desc:    bit test 
  form1:   N:=b7 V:=b6 Z:=A\&{adr} 
  form2:   Z <- ~(A /\ M) N<-M7 V<-M6 
  form3:   *       t = A & M 
           *       P.N = t.7 
           *       P.V = t.6 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_BIT_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_BIT(cpu, mem[eadr]);
}


/*
  opname:  AND
  opcode:  0x25 
  ex:      AND $AB
  eadr:    zero page
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    and (with accumulator) 
  form1:   A:=A\&{adr} 
  form2:   A <- (A) /\ M              
  form3:   *       A = A & M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_AND_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);
}


/*
  opname:  ROL
  opcode:  0x26 
  ex:      ROL $AB
  eadr:    zero page
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    rotate left 
  form1:   {adr}:={adr}*2+C 
  form2:   C <- A7 & A <- A << 1 + C  
  form3:   *       t = B.7 
           *       B = (B << 1) & $FE 
           *       B = B | P.C 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7           
           *    
          
*/

void cpu_6502_ROL_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_ROL(cpu, mem[eadr]);
}


/*
  opname:  PLP
  opcode:  0x28 
  ex:      PLP 
  eadr:    implizit
  flags:   NVBDIZC :**-****
  bytes:   1 
  desc:    pull processor status (SR) 
  form1:   P:=+(S) 
  form2:   A <- (Stack)               
  form3:   *       SP = SP + 1 
           *       P = bPeek(SP)   
           *    
          
*/

void cpu_6502_PLP_imp(struct _6510_cpu* cpu, char mem[][9]){
  // P <- (Stack)
  int sp = sp2int(cpu);
  sp = ((sp + 1) & 0xff ) | 0x100 ;
  cp_register(mem[sp], cpu->flags);
  alu(ALU_OP_ADD, "00000001", cpu->sp, cpu->sp, 0);
  cpu->flags[XFLAG] = '1';
  cpu->flags[BFLAG] = '1';
}


/*
  opname:  AND
  opcode:  0x29 
  ex:      AND #$AB    
  eadr:    immediate
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    and (with accumulator) 
  form1:   A:=A\&{adr} 
  form2:   A <- (A) /\ M              
  form3:   *       A = A & M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_AND_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);
}


/*
  opname:  ROL
  opcode:  0x2A 
  ex:      ROL 
  eadr:    implizit
  flags:   NVBDIZC :*----**
  bytes:   1 
  desc:    rotate left 
  form1:   {adr}:={adr}*2+C 
  form2:   C <- A7 & A <- A << 1 + C  
  form3:   *       t = B.7 
           *       B = (B << 1) & $FE 
           *       B = B | P.C 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7           
           *    
          
*/

void cpu_6502_ROL_imp(struct _6510_cpu* cpu, char mem[][9]){
	int eadr = imp_adr_mode(cpu,mem);
	do_ROL(cpu, mem[eadr]);
}


/*
  opname:  BIT
  opcode:  0x2C 
  ex:      BIT $ABCD
  eadr:    absolute
  flags:   NVBDIZC :**---*-
  bytes:   3 
  desc:    bit test 
  form1:   N:=b7 V:=b6 Z:=A\&{adr} 
  form2:   Z <- ~(A /\ M) N<-M7 V<-M6 
  form3:   *       t = A & M 
           *       P.N = t.7 
           *       P.V = t.6 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_BIT_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_BIT(cpu, mem[eadr]);
}


/*
  opname:  AND
  opcode:  0x2D 
  ex:      AND $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    and (with accumulator) 
  form1:   A:=A\&{adr} 
  form2:   A <- (A) /\ M              
  form3:   *       A = A & M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_AND_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);
}


/*
  opname:  ROL
  opcode:  0x2E 
  ex:      ROL $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    rotate left 
  form1:   {adr}:={adr}*2+C 
  form2:   C <- A7 & A <- A << 1 + C  
  form3:   *       t = B.7 
           *       B = (B << 1) & $FE 
           *       B = B | P.C 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7           
           *    
          
*/

void cpu_6502_ROL_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_ROL(cpu, mem[eadr]);
}


/*
  opname:  BMI
  opcode:  0x30 
  ex:      BMI $AB
  eadr:    relativ
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    branch on minus (negative set) 
  form1:   branch on N=1 
  form2:   if N=1, PC = PC + offset   
  form3:   *       if (P.N == 1) GOTO (PC+M)  
           *    
          
*/

void cpu_6502_BMI_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  char adr[] = "0000000000000000";
  eadr2adr(eadr,adr);

  if(cpu->flags[NFLAG]=='1'){
    cp_byte(adr, cpu->pch);
    cp_byte(&(adr[8]), cpu->pcl);
  } else {
    inc_pc(cpu);
  }
}


/*
  opname:  AND
  opcode:  0x31 
  ex:      AND ($A5),Y
  eadr:    indirection with index
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    and (with accumulator) 
  form1:   A:=A\&{adr} 
  form2:   A <- (A) /\ M              
  form3:   *       A = A & M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_AND_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);
}


/*
  opname:  AND
  opcode:  0x35 
  ex:      AND $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    and (with accumulator) 
  form1:   A:=A\&{adr} 
  form2:   A <- (A) /\ M              
  form3:   *       A = A & M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_AND_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);
}


/*
  opname:  ROL
  opcode:  0x36 
  ex:      ROL $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    rotate left 
  form1:   {adr}:={adr}*2+C 
  form2:   C <- A7 & A <- A << 1 + C  
  form3:   *       t = B.7 
           *       B = (B << 1) & $FE 
           *       B = B | P.C 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7           
           *    
          
*/

void cpu_6502_ROL_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_ROL(cpu, mem[eadr]);
}


/*
  opname:  SEC
  opcode:  0x38 
  ex:      SEC 
  eadr:    implizit
  flags:   NVBDIZC :------1
  bytes:   1 
  desc:    set carry 
  form1:   C:=1 
  form2:   C <- 1                     
  form3:   *       P.C = 1  
           *    
          
*/

void cpu_6502_SEC_imp(struct _6510_cpu* cpu, char mem[][9]){
  cpu->flags[CFLAG] = '1';
}


/*
  opname:  AND
  opcode:  0x39 
  ex:      AND $ABCD,Y
  eadr:    absolut with index Y
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    and (with accumulator) 
  form1:   A:=A\&{adr} 
  form2:   A <- (A) /\ M              
  form3:   *       A = A & M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_AND_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);
}


/*
  opname:  AND
  opcode:  0x3D 
  ex:      AND $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    and (with accumulator) 
  form1:   A:=A\&{adr} 
  form2:   A <- (A) /\ M              
  form3:   *       A = A & M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_AND_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_AND(cpu, mem[eadr]);
}


/*
  opname:  ROL
  opcode:  0x3E 
  ex:      ROL $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    rotate left 
  form1:   {adr}:={adr}*2+C 
  form2:   C <- A7 & A <- A << 1 + C  
  form3:   *       t = B.7 
           *       B = (B << 1) & $FE 
           *       B = B | P.C 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7           
           *    
          
*/

void cpu_6502_ROL_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_ROL(cpu, mem[eadr]);
}


/*
  opname:  RTI
  opcode:  0x40 
  ex:      RTI 
  eadr:    implizit
  flags:   NVBDIZC :**-****
  bytes:   1 
  desc:    return from interrupt 
  form1:   P,PC:=+(S) 
  form2:   P <- (Stack), PC <-(Stack) 
  form3:   *       SP = SP - 1 
           *       P = bPeek(SP) 
           *       SP = SP - 1 
           *       l = bPeek(SP) 
           *       SP = SP - 1 
           *       h = bPeek(SP)<<8 
           *       PC = h|l          
           *    
          
*/

void cpu_6502_RTI_imp(struct _6510_cpu* cpu, char mem[][9]){
// P <- (Stack), PC <-(Stack)
  int sp = sp2int(cpu);
  cp_register(mem[sp+1], cpu->flags);
  cp_register(mem[sp+2], cpu->pcl);
  cp_register(mem[sp+3], cpu->pch);
  alu(ALU_OP_ADD, cpu->sp, "00000011", cpu->sp, 0);
}


/*
  opname:  EOR
  opcode:  0x41 
  ex:      EOR ($A5,X)
  eadr:    indexed with indirection
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    exclusive or (with accumulator) 
  form1:   A:=A exor {adr} 
  form2:   A <- (A) \-/ M             
  form3:   *       A = A ^ M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_EOR_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);
}


/*
  opname:  EOR
  opcode:  0x45 
  ex:      EOR $AB
  eadr:    zero page
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    exclusive or (with accumulator) 
  form1:   A:=A exor {adr} 
  form2:   A <- (A) \-/ M             
  form3:   *       A = A ^ M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_EOR_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);
}


/*
  opname:  LSR
  opcode:  0x46 
  ex:      LSR $AB
  eadr:    zero page
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    logical shift right 
  form1:   {adr}:={adr}/2 
  form2:   C <- A0, A <- (A) >> 1     
  form3:   *       P.N = 0 
           *       P.C = B.0 
           *       B = (B >> 1) & $7F 
           *       P.Z = (B==0) ? 1:0  
           *    
          
*/

void cpu_6502_LSR_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_LSR(cpu, mem[eadr]);
}


/*
  opname:  PHA
  opcode:  0x48 
  ex:      PHA 
  eadr:    implizit
  flags:   NVBDIZC :-------
  bytes:   1 
  desc:    push accumulator 
  form1:   (S)-:=A 
  form2:   Stack <- (A)               
  form3:   *       bPoke(SP,A) 
           *       SP = SP - 1  
           *    
          
*/

void cpu_6502_PHA_imp(struct _6510_cpu* cpu, char mem[][9]){
  cp_byte(cpu->rega, cpu->sp);
  do_DEC(cpu,cpu->sp);
  //inc_pc(cpu);
}


/*
  opname:  EOR
  opcode:  0x49 
  ex:      EOR #$AB    
  eadr:    immediate
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    exclusive or (with accumulator) 
  form1:   A:=A exor {adr} 
  form2:   A <- (A) \-/ M             
  form3:   *       A = A ^ M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_EOR_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);
}


/*
  opname:  LSR
  opcode:  0x4A 
  ex:      LSR 
  eadr:    implizit
  flags:   NVBDIZC :*----**
  bytes:   1 
  desc:    logical shift right 
  form1:   {adr}:={adr}/2 
  form2:   C <- A0, A <- (A) >> 1     
  form3:   *       P.N = 0 
           *       P.C = B.0 
           *       B = (B >> 1) & $7F 
           *       P.Z = (B==0) ? 1:0  
           *    
          
*/

void cpu_6502_LSR_imp(struct _6510_cpu* cpu, char mem[][9]){
	int eadr = imp_adr_mode(cpu, mem);
	do_LSR(cpu, mem[eadr]);
}


/*
  opname:  JMP
  opcode:  0x4C 
  ex:      JMP $ABCD
  eadr:    absolute
  flags:   NVBDIZC :-------
  bytes:   3 
  desc:    jump 
  form1:   PC:={adr} 
  form2:   PC <- Address              
  form3:   *       PC = M  
           *    
          
*/

void cpu_6502_JMP_abs(struct _6510_cpu* cpu, char mem[][9]){
 // int eadr = abs_adr_mode(cpu, mem);
 // cp_byte(mem[eadr], cpu->pcl);

  char _tmp[] = "00000000";
  
  cp_byte(cpu->pcl, cpu->abrl);
  cp_byte(cpu->pch, cpu->abrh);

  cpu->rw='1';
  access_memory(cpu, mem);
  cp_byte(cpu->dbr, _tmp);
  
  inc_pc(cpu);
  
  cp_byte(cpu->pcl, cpu->abrl);
  cp_byte(cpu->pch, cpu->abrh);

  cpu->rw='1';
  access_memory(cpu, mem);

  cp_register(_tmp, cpu->pcl);
  cp_register(cpu->dbr, cpu->pch);
  inc_pc(cpu); 
}


/*
  opname:  EOR
  opcode:  0x4D 
  ex:      EOR $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    exclusive or (with accumulator) 
  form1:   A:=A exor {adr} 
  form2:   A <- (A) \-/ M             
  form3:   *       A = A ^ M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_EOR_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);
}


/*
  opname:  LSR
  opcode:  0x4E 
  ex:      LSR $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    logical shift right 
  form1:   {adr}:={adr}/2 
  form2:   C <- A0, A <- (A) >> 1     
  form3:   *       P.N = 0 
           *       P.C = B.0 
           *       B = (B >> 1) & $7F 
           *       P.Z = (B==0) ? 1:0  
           *    
          
*/

void cpu_6502_LSR_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_LSR(cpu, mem[eadr]);
}


/*
  opname:  BVC
  opcode:  0x50 
  ex:      BVC $AB
  eadr:    relativ
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    branch on overflow clear 
  form1:   branch on V=0 
  form2:   if V=0, PC = PC + offset   
  form3:   *       if (P.V == 0) GOTO (PC+M)  
           *    
          
*/

void cpu_6502_BVC_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  char adr[] = "0000000000000000";
  eadr2adr(eadr,adr);

  if(cpu->flags[VFLAG]=='0'){
    cp_byte(adr, cpu->pch);
    cp_byte(&(adr[8]), cpu->pcl);
  } else {
    inc_pc(cpu);
  }
}


/*
  opname:  EOR
  opcode:  0x51 
  ex:      EOR ($A5),Y
  eadr:    indirection with index
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    exclusive or (with accumulator) 
  form1:   A:=A exor {adr} 
  form2:   A <- (A) \-/ M             
  form3:   *       A = A ^ M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_EOR_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);
}


/*
  opname:  EOR
  opcode:  0x55 
  ex:      EOR $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    exclusive or (with accumulator) 
  form1:   A:=A exor {adr} 
  form2:   A <- (A) \-/ M             
  form3:   *       A = A ^ M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_EOR_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);
}


/*
  opname:  LSR
  opcode:  0x56 
  ex:      LSR $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    logical shift right 
  form1:   {adr}:={adr}/2 
  form2:   C <- A0, A <- (A) >> 1     
  form3:   *       P.N = 0 
           *       P.C = B.0 
           *       B = (B >> 1) & $7F 
           *       P.Z = (B==0) ? 1:0  
           *    
          
*/

void cpu_6502_LSR_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_LSR(cpu, mem[eadr]);
}


/*
  opname:  CLI
  opcode:  0x58 
  ex:      CLI 
  eadr:    implizit
  flags:   NVBDIZC :----0--
  bytes:   1 
  desc:    clear interrupt disable 
  form1:   I:=0 
  form2:   I <- 0                     
  form3:   *       P.I = 0  
           *    
          
*/

void cpu_6502_CLI_imp(struct _6510_cpu* cpu, char mem[][9]){
    cpu->flags[IFLAG] = '0';
}

/*
  opname:  EOR
  opcode:  0x59 
  ex:      EOR $ABCD,Y
  eadr:    absolut with index Y
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    exclusive or (with accumulator) 
  form1:   A:=A exor {adr} 
  form2:   A <- (A) \-/ M             
  form3:   *       A = A ^ M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_EOR_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);
}


/*
  opname:  EOR
  opcode:  0x5D 
  ex:      EOR $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    exclusive or (with accumulator) 
  form1:   A:=A exor {adr} 
  form2:   A <- (A) \-/ M             
  form3:   *       A = A ^ M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_EOR_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_EOR(cpu, mem[eadr]);
}


/*
  opname:  LSR
  opcode:  0x5E 
  ex:      LSR $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    logical shift right 
  form1:   {adr}:={adr}/2 
  form2:   
  form3:   *       P.N = 0 
           *       P.C = B.0 
           *       B = (B >> 1) & $7F 
           *       P.Z = (B==0) ? 1:0  
           *    
          
*/

void cpu_6502_LSR_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_LSR(cpu, mem[eadr]);
}


/*
  opname:  RTS
  opcode:  0x60 
  ex:      RTS 
  eadr:    implizit
  flags:   NVBDIZC :-------
  bytes:   1 
  desc:    return from subroutine 
  form1:   PC:=+(S) 
  form2:   PC <- (Stack)              
  form3:   *       SP = SP + 1 
           *       l = bPeek(SP) 
           *       SP = SP + 1 
           *       h = bPeek(SP)<<8 
           *       PC = (h|l) +1     
           *    
          
*/

void cpu_6502_RTS_imp(struct _6510_cpu* cpu, char mem[][9]){
	/*alu(ALU_OP_ADD, cpu->sp, "000000001", cpu->pcl, 0);
	inc_pc(cpu);*/

  int sp = sp2int(cpu);
  cp_register(mem[sp], cpu->pcl);
  cp_register(mem[sp], cpu->pch);
  alu(ALU_OP_SUB, cpu->sp, "00000010", cpu->sp, 0);
  //inc_pc(cpu);

}


/*
  opname:  ADC
  opcode:  0x61 
  ex:      ADC ($A5,X)
  eadr:    indexed with indirection
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    add with carry 
  form1:   A:=A+{adr} 
  form2:   A <- (A) + M + C           
  form3:   *       t = A + M + P.C 
           *       P.V = (A.7!=t.7) ? 1:0 
           *       P.N = A.7 
           *       P.Z = (t==0) ? 1:0 
           *       IF (P.D) 
           *         t = bcd(A) + bcd(M) + P.C 
           *         P.C = (t>99) ? 1:0 
           *       ELSE 
           *         P.C = (t>255) ? 1:0 
           *       A = t & 0xFF                 
           *    
          
*/

void cpu_6502_ADC_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);
}


/*
  opname:  ADC
  opcode:  0x65 
  ex:      ADC $AB
  eadr:    zero page
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    add with carry 
  form1:   A:=A+{adr} 
  form2:   A <- (A) + M + C           
  form3:   *       t = A + M + P.C 
           *       P.V = (A.7!=t.7) ? 1:0 
           *       P.N = A.7 
           *       P.Z = (t==0) ? 1:0 
           *       IF (P.D) 
           *         t = bcd(A) + bcd(M) + P.C 
           *         P.C = (t>99) ? 1:0 
           *       ELSE 
           *         P.C = (t>255) ? 1:0 
           *       A = t & 0xFF   
           *    
          
*/

void cpu_6502_ADC_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);
}


/*
  opname:  ROR
  opcode:  0x66 
  ex:      ROR $AB
  eadr:    zero page
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    rotate right 
  form1:   {adr}:={adr}/2+C*128 
  form2:   C<-A0 & A<- (A7=C + A>>1)  
  form3:   *       t = B.0 
           *       B = (B >> 1) & $7F 
           *       B = B | ((P.C) ? $80:$00) 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7                  
           *    
          
*/

void cpu_6502_ROR_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_ROR(cpu, mem[eadr]);
}


/*
  opname:  PLA
  opcode:  0x68 
  ex:      PLA 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    pull accumulator 
  form1:   A:=+(S) 
  form2:   A <- (Stack)               
  form3:   *       SP = SP + 1 
           *       A = bPeek(SP) 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_PLA_imp(struct _6510_cpu* cpu, char mem[][9]){
  int sp = sp2int(cpu);
  cp_register(mem[++sp], cpu->rega);
  alu(ALU_OP_ADC, cpu->sp, "00000001", cpu->sp, cpu->flags);
  //inc_pc(cpu);
}


/*
  opname:  ADC
  opcode:  0x69 
  ex:      ADC #$AB    
  eadr:    immediate
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    add with carry 
  form1:   A:=A+{adr} 
  form2:   A <- (A) + M + C           
  form3:   *       t = A + M + P.C 
           *       P.V = (A.7!=t.7) ? 1:0 
           *       P.N = A.7 
           *       P.Z = (t==0) ? 1:0 
           *       IF (P.D) 
           *         t = bcd(A) + bcd(M) + P.C 
           *         P.C = (t>99) ? 1:0 
           *       ELSE 
           *         P.C = (t>255) ? 1:0 
           *       A = t & 0xFF                 
           *    
          
*/

void cpu_6502_ADC_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);
}


/*
  opname:  ROR
  opcode:  0x6A 
  ex:      ROR 
  eadr:    implizit
  flags:   NVBDIZC :*----**
  bytes:   1 
  desc:    rotate right 
  form1:   {adr}:={adr}/2+C*128 
  form2:   C<-A0 & A<- (A7=C + A>>1)  
  form3:   *       t = B.0 
           *       B = (B >> 1) & $7F 
           *       B = B | ((P.C) ? $80:$00) 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7                  
           *    
          
*/

void cpu_6502_ROR_imp(struct _6510_cpu* cpu, char mem[][9]){
  do_ROR(cpu, cpu->rega);
}


/*
  opname:  JMP
  opcode:  0x6C 
  ex:      JMP ($ABCD)
  eadr:    absolute indirect
  flags:   NVBDIZC :-------
  bytes:   3 
  desc:    jump 
  form1:   PC:={adr} 
  form2:   PC <- Address              
  form3:   *       PC = M  
           *    
          
*/

void cpu_6502_JMP_ind(struct _6510_cpu* cpu, char mem[][9]){
  int pc=pc2int(cpu);
  int iadr = ( conv_bitstr2int(mem[pc+1],0,7) << 8) | conv_bitstr2int(mem[pc],0,7) ;
  cp_register(mem[iadr] ,cpu->pcl) ;
  cp_register(mem[iadr+1],cpu->pch) ;
}


/*
  opname:  ADC
  opcode:  0x6D 
  ex:      ADC $ABCD
  eadr:    absolute
  flags:   NVBDIZC :**---**
  bytes:   3 
  desc:    add with carry 
  form1:   A:=A+{adr} 
  form2:   A <- (A) + M + C           
  form3:   *       t = A + M + P.C 
           *       P.V = (A.7!=t.7) ? 1:0 
           *       P.N = A.7 
           *       P.Z = (t==0) ? 1:0 
           *       IF (P.D) 
           *         t = bcd(A) + bcd(M) + P.C 
           *         P.C = (t>99) ? 1:0 
           *       ELSE 
           *         P.C = (t>255) ? 1:0 
           *       A = t & 0xFF                 
           *    
          
*/

void cpu_6502_ADC_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);
}


/*
  opname:  ROR
  opcode:  0x6E 
  ex:      ROR $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    rotate right 
  form1:   {adr}:={adr}/2+C*128 
  form2:   C<-A0 & A<- (A7=C + A>>1)  
  form3:   *       t = B.0 
           *       B = (B >> 1) & $7F 
           *       B = B | ((P.C) ? $80:$00) 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7                  
           *    
          
*/

void cpu_6502_ROR_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_ROR(cpu, mem[eadr]);
}


/*
  opname:  BVS
  opcode:  0x70 
  ex:      BVS $AB
  eadr:    relativ
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    branch on overflow set 
  form1:   branch on V=1 
  form2:   if V=1, PC = PC + offset   
  form3:   *       if (P.V == 1) GOTO (PC+M)  
           *    
          
*/

void cpu_6502_BVS_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  char adr[] = "0000000000000000";
  eadr2adr(eadr,adr);

  if(cpu->flags[VFLAG]=='1'){
    cp_byte(adr, cpu->pch);
    cp_byte(&(adr[8]), cpu->pcl);
  } else {
    inc_pc(cpu);
  }
}


/*
  opname:  ADC
  opcode:  0x71 
  ex:      ADC ($A5),Y
  eadr:    indirection with index
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    add with carry 
  form1:   A:=A+{adr} 
  form2:   A <- (A) + M + C           
  form3:   *       t = A + M + P.C 
           *       P.V = (A.7!=t.7) ? 1:0 
           *       P.N = A.7 
           *       P.Z = (t==0) ? 1:0 
           *       IF (P.D) 
           *         t = bcd(A) + bcd(M) + P.C 
           *         P.C = (t>99) ? 1:0 
           *       ELSE 
           *         P.C = (t>255) ? 1:0 
           *       A = t & 0xFF                 
           *    
          
*/

void cpu_6502_ADC_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);
}


/*
  opname:  ADC
  opcode:  0x75 
  ex:      ADC $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    add with carry 
  form1:   A:=A+{adr} 
  form2:   A <- (A) + M + C           
  form3:   *       t = A + M + P.C 
           *       P.V = (A.7!=t.7) ? 1:0 
           *       P.N = A.7 
           *       P.Z = (t==0) ? 1:0 
           *       IF (P.D) 
           *         t = bcd(A) + bcd(M) + P.C 
           *         P.C = (t>99) ? 1:0 
           *       ELSE 
           *         P.C = (t>255) ? 1:0 
           *       A = t & 0xFF                 
           *    
          
*/

void cpu_6502_ADC_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);
}


/*
  opname:  ROR
  opcode:  0x76 
  ex:      ROR $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    rotate right 
  form1:   {adr}:={adr}/2+C*128 
  form2:   C<-A0 & A<- (A7=C + A>>1)  
  form3:   *       t = B.0 
           *       B = (B >> 1) & $7F 
           *       B = B | ((P.C) ? $80:$00) 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7                  
           *    
          
*/

void cpu_6502_ROR_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_ROR(cpu, mem[eadr]);
}


/*
  opname:  SEI
  opcode:  0x78 
  ex:      SEI 
  eadr:    implizit
  flags:   NVBDIZC :----1--
  bytes:   1 
  desc:    set interrupt disable 
  form1:   I:=1 
  form2:   I <- 1                     
  form3:   *       P.I = 1  
           *    
          
*/

void cpu_6502_SEI_imp(struct _6510_cpu* cpu, char mem[][9]){
  cpu->flags[IFLAG] = '1';
}


/*
  opname:  ADC
  opcode:  0x79 
  ex:      ADC $ABCD,Y
  eadr:    absolut with index Y
  flags:   NVBDIZC :**---**
  bytes:   3 
  desc:    add with carry 
  form1:   A:=A+{adr} 
  form2:   A <- (A) + M + C           
  form3:   *       t = A + M + P.C 
           *       P.V = (A.7!=t.7) ? 1:0 
           *       P.N = A.7 
           *       P.Z = (t==0) ? 1:0 
           *       IF (P.D) 
           *         t = bcd(A) + bcd(M) + P.C 
           *         P.C = (t>99) ? 1:0 
           *       ELSE 
           *         P.C = (t>255) ? 1:0 
           *       A = t & 0xFF                 
           *    
          
*/

void cpu_6502_ADC_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);
}


/*
  opname:  ADC
  opcode:  0x7D 
  ex:      ADC $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :**---**
  bytes:   3 
  desc:    add with carry 
  form1:   A:=A+{adr} 
  form2:   A <- (A) + M + C           
  form3:   *       t = A + M + P.C 
           *       P.V = (A.7!=t.7) ? 1:0 
           *       P.N = A.7 
           *       P.Z = (t==0) ? 1:0 
           *       IF (P.D) 
           *         t = bcd(A) + bcd(M) + P.C 
           *         P.C = (t>99) ? 1:0 
           *       ELSE 
           *         P.C = (t>255) ? 1:0 
           *       A = t & 0xFF                 
           *    
          
*/

void cpu_6502_ADC_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_ADC(cpu, mem[eadr]);
}


/*
  opname:  ROR
  opcode:  0x7E 
  ex:      ROR $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    rotate right 
  form1:   {adr}:={adr}/2+C*128 
  form2:   C<-A0 & A<- (A7=C + A>>1)  
  form3:   *       t = B.0 
           *       B = (B >> 1) & $7F 
           *       B = B | ((P.C) ? $80:$00) 
           *       P.C = t 
           *       P.Z = (B==0) ? 1:0 
           *       P.N = B.7                  
           *    
          
*/

void cpu_6502_ROR_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_ROR(cpu, mem[eadr]);
}


/*
  opname:  STA
  opcode:  0x81 
  ex:      STA ($A5,X)
  eadr:    indexed with indirection
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    store accumulator 
  form1:   {adr}:=A 
  form2:   M <- (A)                   
  form3:   *       M = A  
           *    
          
*/

void cpu_6502_STA_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  cp_byte(cpu->rega, mem[eadr]);	
}


/*
  opname:  STY
  opcode:  0x84 
  ex:      STY $AB
  eadr:    zero page
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    store Y 
  form1:   {adr}:=Y 
  form2:   M <- (Y)                   
  form3:   *       M = Y   
           *    
          
*/

void cpu_6502_STY_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  cp_byte(cpu->regy, mem[eadr]);	
}


/*
  opname:  STA
  opcode:  0x85 
  ex:      STA $AB
  eadr:    zero page
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    store accumulator 
  form1:   {adr}:=A 
  form2:   M <- (A)                   
  form3:   *       M = A  
           *    
          
*/

void cpu_6502_STA_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  cp_byte(cpu->rega, mem[eadr]);	
}


/*
  opname:  STX
  opcode:  0x86 
  ex:      STX $AB
  eadr:    zero page
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    store X 
  form1:   {adr}:=X 
  form2:   M <- (X)                   
  form3:   *       M = X   
           *    
          
*/

void cpu_6502_STX_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  cp_byte(cpu->regx, mem[eadr]);	
}


/*
  opname:  DEY
  opcode:  0x88 
  ex:      DEY 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    decrement Y 
  form1:   Y:=Y-1 
  form2:   Y <- (Y) - 1               
  form3:   *       Y = Y - 1 
           *       P.Z = (Y==0) ? 1:0 
           *       P.N = Y.7           
           *    
          
*/

void cpu_6502_DEY_imp(struct _6510_cpu* cpu, char mem[][9]){
  do_DEC(cpu, cpu->regy);
}


/*
  opname:  TXA
  opcode:  0x8A 
  ex:      TXA 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    transfer X to accumulator 
  form1:   A:=X 
  form2:   A <- (X)                   
  form3:   *       A = X 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_TXA_imp(struct _6510_cpu* cpu, char mem[][9]){  
  cp_register(cpu->regx, cpu->rega);
}


/*
  opname:  STY
  opcode:  0x8C 
  ex:      STY $ABCD
  eadr:    absolute
  flags:   NVBDIZC :-------
  bytes:   3 
  desc:    store Y 
  form1:   {adr}:=Y 
  form2:   M <- (Y)                   
  form3:   *       M = Y   
           *    
          
*/

void cpu_6502_STY_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  cp_register(cpu->regy, mem[eadr]);	
}


/*
  opname:  STA
  opcode:  0x8D 
  ex:      STA $ABCD
  eadr:    absolute
  flags:   NVBDIZC :-------
  bytes:   3 
  desc:    store accumulator 
  form1:   {adr}:=A 
  form2:   M <- (A)                   
  form3:   *       M = A  
           *    
          
*/

void cpu_6502_STA_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  cp_register(cpu->rega, mem[eadr]);	
}


/*
  opname:  STX
  opcode:  0x8E 
  ex:      STX $ABCD
  eadr:    absolute
  flags:   NVBDIZC :-------
  bytes:   3 
  desc:    store X 
  form1:   {adr}:=X 
  form2:   M <- (X)                   
  form3:   *       M = X   
           *    
          
*/

void cpu_6502_STX_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  cp_register(cpu->regx, mem[eadr]);	
}


/*
  opname:  2.sbbv6.ch)CC
  opcode:  0x90 
  ex:      BCC $AB
  eadr:    relativ
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    branch on carry clear 
  form1:   branch on C=0 
  form2:   if C=0, PC = PC + offset   
  form3:   *       if (P.C == 0) GOTO (PC+M)  
           *    
          
*/

void cpu_6502_BCC_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  char adr[] = "0000000000000000";
  eadr2adr(eadr,adr);

  if(cpu->flags[CFLAG]=='0'){
    cp_byte(adr, cpu->pch);
    cp_byte(&(adr[8]), cpu->pcl);
  } else {
    inc_pc(cpu);
  }
}
/*
  opname:  STA
  opcode:  0x91 
  ex:      STA ($A5),Y
  eadr:    indirection with index
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    store accumulator 
  form1:   {adr}:=A 
  form2:   M <- (A)                   
  form3:   *       M = A  
           *    
          
*/

void cpu_6502_STA_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  cp_register(cpu->rega, mem[eadr]);
}


/*
  opname:  STY
  opcode:  0x94 
  ex:      STY $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    store Y 
  form1:   {adr}:=Y 
  form2:   M <- (Y)                   
  form3:   *       M = Y   
           *    
          
*/

void cpu_6502_STY_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  cp_register(cpu->regy, mem[eadr]);
}


/*
  opname:  STA
  opcode:  0x95 
  ex:      STA $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    store accumulator 
  form1:   {adr}:=A 
  form2:   M <- (A)                   
  form3:   *       M = A  
           *    
          
*/

void cpu_6502_STA_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  cp_register(cpu->rega, mem[eadr]);
}


/*
  opname:  STX
  opcode:  0x96 
  ex:      STX $AB,X
  eadr:    zero page with index Y
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    store X 
  form1:   {adr}:=X 
  form2:   M <- (X)                   
  form3:   *       M = X   
           *    
          
*/

void cpu_6502_STX_zpy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpy_adr_mode(cpu, mem);
  cp_register(cpu->regy, mem[eadr]);
}


/*
  opname:  TYA
  opcode:  0x98 
  ex:      TYA 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    transfer Y to accumulator 
  form1:   A:=Y 
  form2:   A <- (Y)                   
  form3:  
*/

void cpu_6502_TYA_imp(struct _6510_cpu* cpu, char mem[][9]){
  cp_register(cpu->regy, cpu->rega);
  //inc_pc(cpu);
}


/*
  opname:  STA
  opcode:  0x99 
  ex:      STA $ABCD,Y
  eadr:    absolut with index Y
  flags:   NVBDIZC :-------
  bytes:   3 
  desc:    store accumulator 
  form1:   {adr}:=A 
  form2:   M <- (A)                   
  form3:   *       M = A  
           *    
          
*/

void cpu_6502_STA_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  cp_register(cpu->rega, mem[eadr]);
}


/*
  opname:  TXS
  opcode:  0x9A 
  ex:      TXS 
  eadr:    implizit
  flags:   NVBDIZC :-------
  bytes:   1 
  desc:    transfer X to stack pointer 
  form1:   S:=X 
  form2:   S <- (X)                   
  form3:   *       SP = X  
           *    
          
*/

void cpu_6502_TXS_imp(struct _6510_cpu* cpu, char mem[][9]){
  cp_register(cpu->regx, cpu->sp);
  //inc_pc(cpu);
}


/*
  opname:  STA
  opcode:  0x9D 
  ex:      STA $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :-------
  bytes:   3 
  desc:    store accumulator 
  form1:   {adr}:=A 
  form2:   M <- (A)                   
  form3:   *       M = A  
           *    
          
*/

void cpu_6502_STA_abx(struct _6510_cpu* cpu, char mem[][9]){
 int eadr = abx_adr_mode(cpu, mem);
 cp_byte(cpu->rega, mem[eadr]);

}


/*
  opname:  LDY
  opcode:  0xA0 
  ex:      LDY #$AB    
  eadr:    immediate
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    load Y 
  form1:   Y:={adr} 
  form2:   Y <- M                     
  form3:   *       Y = M 
           *       P.N = Y.7 
           *       P.Z = (Y==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDY_imm(struct _6510_cpu* cpu, char mem[][9]){
int eadr = imm_adr_mode(cpu, mem);
cp_register(mem[eadr], cpu->regy);
zsflagging(cpu->flags, cpu->regy);

}


/*
  opname:  LDA
  opcode:  0xA1 
  ex:      LDA ($A5,X)
  eadr:    indexed with indirection
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    load accumulator 
  form1:   A:={adr} 
  form2:   A <- M                     
  form3:   *       A = M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_LDA_izx(struct _6510_cpu* cpu, char mem[][9]){
int eadr = izx_adr_mode(cpu, mem);
cp_register(mem[eadr], cpu->rega);
zsflagging(cpu->flags, cpu->rega);
}


/*
  opname:  LDX
  opcode:  0xA2 
  ex:      LDX #$AB    
  eadr:    immediate
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:     
  form1:   X:={adr} 
  form2:   X <- M                     
  form3:   *       X = M 
           *       P.N = X.7 
           *       P.Z = (X==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDX_imm(struct _6510_cpu* cpu, char mem[][9]){
 int eadr = imm_adr_mode(cpu, mem);
cp_register(mem[eadr], cpu->regx);
zsflagging(cpu->flags, cpu->regx);
}


/*
  opname:  LDY
  opcode:  0xA4 
  ex:      LDY $AB
  eadr:    zero page
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    load Y 
  form1:   Y:={adr} 
  form2:   Y <- M                     
  form3:   *       Y = M 
           *       P.N = Y.7 
           *       P.Z = (Y==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDY_zp(struct _6510_cpu* cpu, char mem[][9]){
int eadr = zp_adr_mode(cpu, mem);
cp_register(mem[eadr], cpu->regy);
zsflagging(cpu->flags, cpu->regy);
}


/*
  opname:  LDA
  opcode:  0xA5 
  ex:      LDA $AB
  eadr:    zero page
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    load accumulator 
  form1:   A:={adr} 
  form2:   A <- M                     
  form3:   *       A = M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_LDA_zp(struct _6510_cpu* cpu, char mem[][9]){
 int eadr = zp_adr_mode(cpu, mem);
cp_register(mem[eadr], cpu->rega);
zsflagging(cpu->flags, cpu->rega);
}


/*
  opname:  LDX
  opcode:  0xA6 
  ex:      LDX $AB
  eadr:    zero page
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:     
  form1:   X:={adr} 
  form2:   X <- M                     
  form3:   *       X = M 
           *       P.N = X.7 
           *       P.Z = (X==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDX_zp(struct _6510_cpu* cpu, char mem[][9]){
 int eadr = zp_adr_mode(cpu, mem);
cp_register(mem[eadr], cpu->regx);
zsflagging(cpu->flags, cpu->regx);
}


/*
  opname:  TAY
  opcode:  0xA8 
  ex:      TAY 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    transfer accumulator to Y 
  form1:   Y:=A 
  form2:   Y <- (A)                   
  form3:   *       Y = A 
           *       P.N = Y.7 
           *       P.Z = (Y==0) ? 1:0  
           *    
          
*/

void cpu_6502_TAY_imp(struct _6510_cpu* cpu, char mem[][9]){
  cp_register(cpu->rega, cpu->regy);
  zsflagging(cpu->flags, cpu->regy);
}


/*
  opname:  LDA
  opcode:  0xA9 
  ex:      LDA #$AB    
  eadr:    immediate
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    load accumulator 
  form1:   A:={adr} 
  form2:   A <- M                     
  form3:   *       A = M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_LDA_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  cp_register(mem[eadr], cpu->rega);
  zsflagging(cpu->flags, cpu->rega);
}


/*
  opname:  TAX
  opcode:  0xAA 
  ex:      TAX 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    transfer accumulator to X 
  form1:   X:=A 
  form2:   X <- (A)                   
  form3:   *       X = A 
           *       P.N = X.7 
           *       P.Z = (X==0) ? 1:0  
           *    
          
*/

void cpu_6502_TAX_imp(struct _6510_cpu* cpu, char mem[][9]){
  cp_register(cpu->rega, cpu->regx);
  zsflagging(cpu->flags, cpu->regx);
}


/*
  opname:  LDY
  opcode:  0xAC 
  ex:      LDY $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    load Y 
  form1:   Y:={adr} 
  form2:   Y <- M                     
  form3:   *       Y = M 
           *       P.N = Y.7 
           *       P.Z = (Y==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDY_abs(struct _6510_cpu* cpu, char mem[][9]){
 int eadr = abs_adr_mode(cpu, mem);
cp_byte(mem[eadr], cpu->regy);
zsflagging(cpu->flags, cpu->regy);
}


/*
  opname:  LDA
  opcode:  0xAD 
  ex:      LDA $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    load accumulator 
  form1:   A:={adr} 
  form2:   A <- M                     
  form3:   *       A = M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_LDA_abs(struct _6510_cpu* cpu, char mem[][9]){
 int eadr = abs_adr_mode(cpu, mem);
cp_byte(mem[eadr], cpu->rega);
zsflagging(cpu->flags, cpu->rega);

}


/*
  opname:  LDX
  opcode:  0xAE 
  ex:      LDX $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:     
  form1:   X:={adr} 
  form2:   X <- M                     
  form3:   *       X = M 
           *       P.N = X.7 
           *       P.Z = (X==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDX_abs(struct _6510_cpu* cpu, char mem[][9]){
 int eadr = abs_adr_mode(cpu, mem);
cp_byte(mem[eadr], cpu->regx);
zsflagging(cpu->flags, cpu->regx);

}


/*
  opname:  BCS
  opcode:  0xB0 
  ex:      BCS $AB
  eadr:    relativ
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    branch on carry set 
  form1:   branch on C=1 
  form2:   if C=1, PC = PC + offset   
  form3:   *       if (P.C == 1) GOTO (PC+M)  
           *    
          
*/

void cpu_6502_BCS_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  char adr[] = "0000000000000000";
  eadr2adr(eadr,adr);

  if(cpu->flags[CFLAG]=='1'){
    cp_byte(adr, cpu->pch);
    cp_byte(&(adr[8]), cpu->pcl);
  } else {
    inc_pc(cpu);
  }
}


/*
  opname:  LDA
  opcode:  0xB1 
  ex:      LDA ($A5),Y
  eadr:    indirection with index
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    load accumulator 
  form1:   A:={adr} 
  form2:   A <- M                     
  form3:   *       A = M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_LDA_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
cp_byte(mem[eadr], cpu->rega);
zsflagging(cpu->flags, cpu->rega);

}


/*
  opname:  LDY
  opcode:  0xB4 
  ex:      LDY $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    load Y 
  form1:   Y:={adr} 
  form2:   Y <- M                     
  form3:   *       Y = M 
           *       P.N = Y.7 
           *       P.Z = (Y==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDY_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
cp_byte(mem[eadr], cpu->regy);
zsflagging(cpu->flags, cpu->regy);

}


/*
  opname:  LDA
  opcode:  0xB5 
  ex:      LDA $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    load accumulator 
  form1:   A:={adr} 
  form2:   A <- M                     
  form3:   *       A = M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_LDA_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
cp_byte(mem[eadr], cpu->rega);
zsflagging(cpu->flags, cpu->rega);

}


/*
  opname:  LDX
  opcode:  0xB6 
  ex:      LDX $AB,X
  eadr:    zero page with index Y
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:     
  form1:   X:={adr} 
  form2:   X <- M                     
  form3:   *       X = M 
           *       P.N = X.7 
           *       P.Z = (X==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDX_zpy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpy_adr_mode(cpu, mem);
cp_byte(mem[eadr], cpu->regx);
zsflagging(cpu->flags, cpu->regx);

}


/*
  opname:  CLV
  opcode:  0xB8 
  ex:      CLV 
  eadr:    implizit
  flags:   NVBDIZC :-0-----
  bytes:   1 
  desc:    clear overflow 
  form1:   V:=0 
  form2:   V <- 0                     
  form3:   *       P.V = 0  
           *    
          
*/

void cpu_6502_CLV_imp(struct _6510_cpu* cpu, char mem[][9]){
  cpu->flags[VFLAG] = '0';
}


/*
  opname:  LDA
  opcode:  0xB9 
  ex:      LDA $ABCD,Y
  eadr:    absolut with index Y
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    load accumulator 
  form1:   A:={adr} 
  form2:   A <- M                     
  form3:   *       A = M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_LDA_aby(struct _6510_cpu* cpu, char mem[][9]){
  	int eadr = aby_adr_mode(cpu, mem);
	cp_byte(mem[eadr], cpu->rega);
	zsflagging(cpu->flags, cpu->rega);
}


/*
  opname:  TSX
  opcode:  0xBA 
  ex:      TSX 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    transfer stack pointer to X 
  form1:   X:=S 
  form2:   X <- (S)                   
  form3:   *       X = SP 
           *       P.N = X.7 
           *       P.Z = (X==0) ? 1:0  
           *    
          
*/

void cpu_6502_TSX_imp(struct _6510_cpu* cpu, char mem[][9]){
  cp_byte(cpu->sp,cpu->regx);
  inc_pc(cpu);
}


/*
  opname:  LDY
  opcode:  0xBC 
  ex:      LDY $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    load Y 
  form1:   Y:={adr} 
  form2:   Y <- M                     
  form3:   *       Y = M 
           *       P.N = Y.7 
           *       P.Z = (Y==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDY_abx(struct _6510_cpu* cpu, char mem[][9]){
  	int eadr = abx_adr_mode(cpu, mem);
	cp_byte(mem[eadr], cpu->regy);
	zsflagging(cpu->flags, cpu->regy);
}


/*
  opname:  LDA
  opcode:  0xBD 
  ex:      LDA $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    load accumulator 
  form1:   A:={adr} 
  form2:   A <- M                     
  form3:   *       A = M 
           *       P.N = A.7 
           *       P.Z = (A==0) ? 1:0  
           *    
          
*/

void cpu_6502_LDA_abx(struct _6510_cpu* cpu, char mem[][9]){
  	int eadr = abx_adr_mode(cpu, mem);
	cp_byte(mem[eadr], cpu->rega);
	zsflagging(cpu->flags, cpu->rega);
}


/*
  opname:  LDX
  opcode:  0xBE 
  ex:      LDX $ABCD,Y
  eadr:    absolut with index Y
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:     
  form1:   X:={adr} 
  form2:   X <- M                     
  form3:   *       X = M 
           *       P.N = X.7 
           *       P.Z = (X==0) ? 1:0 
           *    
          
*/

void cpu_6502_LDX_aby(struct _6510_cpu* cpu, char mem[][9]){
  	int eadr = aby_adr_mode(cpu, mem);
	cp_byte(mem[eadr], cpu->regx);
	zsflagging(cpu->flags, cpu->rega);
}


/*
  opname:  CPY
  opcode:  0xC0 
  ex:      CPY #$AB    
  eadr:    immediate
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    compare with Y 
  form1:   Y-{adr} 
  form2:   (Y - M) -> NZC             
  form3:   *       t = Y - M 
           *       P.N = t.7 
           *       P.C = (Y>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CPY_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_CPY(cpu, mem[eadr]);
}


/*
  opname:  CMP
  opcode:  0xC1 
  ex:      CMP ($A5,X)
  eadr:    indexed with indirection
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    compare (with accumulator) 
  form1:   A-{adr} 
  form2:   (A - M) -> NZC             
  form3:   *       t = A - M 
           *       P.N = t.7 
           *       P.C = (A>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CMP_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);
}


/*
  opname:  CPY
  opcode:  0xC4 
  ex:      CPY $AB
  eadr:    zero page
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    compare with Y 
  form1:   Y-{adr} 
  form2:   (Y - M) -> NZC             
  form3:   *       t = Y - M 
           *       P.N = t.7 
           *       P.C = (Y>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CPY_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_CPY(cpu, mem[eadr]);
}


/*
  opname:  CMP
  opcode:  0xC5 
  ex:      CMP $AB
  eadr:    zero page
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    compare (with accumulator) 
  form1:   A-{adr} 
  form2:   (A - M) -> NZC             
  form3:   *       t = A - M 
           *       P.N = t.7 
           *       P.C = (A>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CMP_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);
}


/*
  opname:  DEC
  opcode:  0xC6 
  ex:      DEC $AB
  eadr:    zero page
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    decrement 
  form1:   {adr}:={adr}-1 
  form2:   M <- (M) - 1               
  form3:   *       M = (M - 1) & $FF 
           *       P.N = M.7 
           *       P.Z = (M==0) ? 1:0  
           *    
          
*/

void cpu_6502_DEC_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_DEC(cpu, mem[eadr]);
}


/*
  opname:  INY
  opcode:  0xC8 
  ex:      INY 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    increment Y 
  form1:   Y:=Y+1 
  form2:   Y <- (Y) + 1               
  form3:   *       Y = Y + 1 
           *       P.Z = (Y==0) ? 1:0 
           *       P.N = Y.7           
           *    
          
*/

void cpu_6502_INY_imp(struct _6510_cpu* cpu, char mem[][9]){
  do_INC(cpu, cpu->regy);
}


/*
  opname:  CMP
  opcode:  0xC9 
  ex:      CMP #$AB    
  eadr:    immediate
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    compare (with accumulator) 
  form1:   A-{adr} 
  form2:   (A - M) -> NZC             
  form3:   *       t = A - M 
           *       P.N = t.7 
           *       P.C = (A>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CMP_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);
}


/*
  opname:  DEX
  opcode:  0xCA 
  ex:      DEX 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    decrement X 
  form1:   X:=X-1 
  form2:   X <- (X) - 1               
  form3:   *       X = X - 1 
           *       P.Z = (X==0) ? 1:0 
           *       P.N = X.7           
           *    
          
*/

void cpu_6502_DEX_imp(struct _6510_cpu* cpu, char mem[][9]){
  do_DEC(cpu, cpu->regx);
}


/*
  opname:  CPY
  opcode:  0xCC 
  ex:      CPY $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    compare with Y 
  form1:   Y-{adr} 
  form2:   (Y - M) -> NZC             
  form3:   *       t = Y - M 
           *       P.N = t.7 
           *       P.C = (Y>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CPY_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_CPY(cpu, mem[eadr]);
}


/*
  opname:  CMP
  opcode:  0xCD 
  ex:      CMP $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    compare (with accumulator) 
  form1:   A-{adr} 
  form2:   (A - M) -> NZC             
  form3:   *       t = A - M 
           *       P.N = t.7 
           *       P.C = (A>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CMP_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);
}


/*
  opname:  DEC
  opcode:  0xCE 
  ex:      DEC $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    decrement 
  form1:   {adr}:={adr}-1 
  form2:   M <- (M) - 1               
  form3:   *       M = (M - 1) & $FF 
           *       P.N = M.7 
           *       P.Z = (M==0) ? 1:0  
           *    
          
*/

void cpu_6502_DEC_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_DEC(cpu, mem[eadr]);
}


/*
  opname:  BNE
  opcode:  0xD0 
  ex:      BNE $AB
  eadr:    relativ
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    branch on not equal (zero clear) 
  form1:   branch on Z=0 
  form2:   if Z=0, PC = PC + offset   
  form3:   *       if (P.Z == 0) GOTO (PC+M)  
           *    
          
*/

void cpu_6502_BNE_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  char adr[] = "0000000000000000";
  eadr2adr(eadr,adr);

  if(cpu->flags[ZFLAG]=='0'){
    cp_byte(adr, cpu->pch);
    cp_byte(&(adr[8]), cpu->pcl);
  } else {
    inc_pc(cpu);
  }
}


/*
  opname:  CMP
  opcode:  0xD1 
  ex:      CMP ($A5),Y
  eadr:    indirection with index
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    compare (with accumulator) 
  form1:   A-{adr} 
  form2:   (A - M) -> NZC             
  form3:   *       t = A - M 
           *       P.N = t.7 
           *       P.C = (A>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CMP_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);
}


/*
  opname:  CMP
  opcode:  0xD5 
  ex:      CMP $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    compare (with accumulator) 
  form1:   A-{adr} 
  form2:   (A - M) -> NZC             
  form3:   *       t = A - M 
           *       P.N = t.7 
           *       P.C = (A>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CMP_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);
}


/*
  opname:  DEC
  opcode:  0xD6 
  ex:      DEC $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    decrement 
  form1:   {adr}:={adr}-1 
  form2:   M <- (M) - 1               
  form3:   *       M = (M - 1) & $FF 
           *       P.N = M.7 
           *       P.Z = (M==0) ? 1:0  
           *    
          
*/

void cpu_6502_DEC_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_DEC(cpu, mem[eadr]);
}


/*
  opname:  CLD
  opcode:  0xD8 
  ex:      CLD 
  eadr:    implizit
  flags:   NVBDIZC :---0---
  bytes:   1 
  desc:    clear decimal 
  form1:   D:=0 
  form2:   D <- 0                     
  form3:   *       P.D = 0  
           *    
          
*/

void cpu_6502_CLD_imp(struct _6510_cpu* cpu, char mem[][9]){
    cpu->flags[DFLAG] = '0';
}


/*
  opname:  CMP
  opcode:  0xD9 
  ex:      CMP $ABCD,Y
  eadr:    absolut with index Y
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    compare (with accumulator) 
  form1:   A-{adr} 
  form2:   (A - M) -> NZC             
  form3:   *       t = A - M 
           *       P.N = t.7 
           *       P.C = (A>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CMP_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);
}


/*
  opname:  CMP
  opcode:  0xDD 
  ex:      CMP $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    compare (with accumulator) 
  form1:   A-{adr} 
  form2:   (A - M) -> NZC             
  form3:   *       t = A - M 
           *       P.N = t.7 
           *       P.C = (A>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CMP_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_CMP(cpu, mem[eadr]);
}


/*
  opname:  DEC
  opcode:  0xDE 
  ex:      DEC $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    decrement 
  form1:   {adr}:={adr}-1 
  form2:   M <- (M) - 1               
  form3:   *       M = (M - 1) & $FF 
           *       P.N = M.7 
           *       P.Z = (M==0) ? 1:0  
           *    
          
*/

void cpu_6502_DEC_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_DEC(cpu, mem[eadr]);
}


/*
  opname:  CPX
  opcode:  0xE0 
  ex:      CPX #$AB    
  eadr:    immediate
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    compare with X 
  form1:   X-{adr} 
  form2:   (X - M) -> NZC             
  form3:   *       t = X - M 
           *       P.N = t.7 
           *       P.C = (X>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CPX_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_CPX(cpu, mem[eadr]);
}


/*
  opname:  SBC
  opcode:  0xE1 
  ex:      SBC ($A5,X)
  eadr:    indexed with indirection
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    subtract with carry 
  form1:   A:=A-{adr} 
  form2:   A <- (A) - M - ~C          
  form3:   *       IF (P.D) 
           *         t = bcd(A) - bcd(M) - !P.C 
           *         P.V = (t>99 OR t<0) ? 1:0 
           *       ELSE 
           *         t = A - M - !P.C 
           *         P.V = (t>127 OR t<-128) ? 1:0 
           *       P.C = (t>=0) ? 1:0 
           *       P.N = t.7 
           *       P.Z = (t==0) ? 1:0 
           *       A = t & 0xFF                     
           *    
          
*/

void cpu_6502_SBC_izx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izx_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);
}


/*
  opname:  CPX
  opcode:  0xE4 
  ex:      CPX $AB
  eadr:    zero page
  flags:   NVBDIZC :*----**
  bytes:   2 
  desc:    compare with X 
  form1:   X-{adr} 
  form2:   (X - M) -> NZC             
  form3:   *       t = X - M 
           *       P.N = t.7 
           *       P.C = (X>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CPX_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_CPX(cpu, mem[eadr]);
}


/*
  opname:  SBC
  opcode:  0xE5 
  ex:      SBC $AB
  eadr:    zero page
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    subtract with carry 
  form1:   A:=A-{adr} 
  form2:   A <- (A) - M - ~C          
  form3:   *       IF (P.D) 
           *         t = bcd(A) - bcd(M) - !P.C 
           *         P.V = (t>99 OR t<0) ? 1:0 
           *       ELSE 
           *         t = A - M - !P.C 
           *         P.V = (t>127 OR t<-128) ? 1:0 
           *       P.C = (t>=0) ? 1:0 
           *       P.N = t.7 
           *       P.Z = (t==0) ? 1:0 
           *       A = t & 0xFF                     
           *    
          
*/

void cpu_6502_SBC_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);
}


/*
  opname:  INC
  opcode:  0xE6 
  ex:      INC $AB
  eadr:    zero page
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    increment 
  form1:   {adr}:={adr}+1 
  form2:   M <- (M) + 1               
  form3:   *       M = (M + 1) & $FF 
           *       P.N = M.7 
           *       P.Z = (M==0) ? 1:0  
           *    
          
*/

void cpu_6502_INC_zp(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zp_adr_mode(cpu, mem);
  do_INC(cpu, mem[eadr]);
}


/*
  opname:  INX
  opcode:  0xE8 
  ex:      INX 
  eadr:    implizit
  flags:   NVBDIZC :*----*-
  bytes:   1 
  desc:    increment X 
  form1:   X:=X+1 
  form2:   X <- (X) +1                
  form3:   *       X = X + 1 
           *       P.Z = (X==0) ? 1:0 
           *       P.N = X.7           
           *    
          
*/

void cpu_6502_INX_imp(struct _6510_cpu* cpu, char mem[][9]){
  do_INC(cpu, cpu->regx);
}


/*
  opname:  SBC
  opcode:  0xE9 
  ex:      SBC #$AB    
  eadr:    immediate
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    subtract with carry 
  form1:   A:=A-{adr} 
  form2:   A <- (A) - M - ~C          
  form3:   *       IF (P.D) 
           *         t = bcd(A) - bcd(M) - !P.C 
           *         P.V = (t>99 OR t<0) ? 1:0 
           *       ELSE 
           *         t = A - M - !P.C 
           *         P.V = (t>127 OR t<-128) ? 1:0 
           *       P.C = (t>=0) ? 1:0 
           *       P.N = t.7 
           *       P.Z = (t==0) ? 1:0 
           *       A = t & 0xFF                     
           *    
          
*/

void cpu_6502_SBC_imm(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = imm_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);
}


/*
  opname:  NOP
  opcode:  0xEA 
  ex:      NOP 
  eadr:    implizit
  flags:   NVBDIZC :-------
  bytes:   1 
  desc:    no operation 
  form1:     
  form2:   [no operation]             
  form3:   *       ~none~  
           *    
          
*/

void cpu_6502_NOP_imp(struct _6510_cpu* cpu, char mem[][9]){
  inc_pc(cpu);
  return;
}


/*
  opname:  CPX
  opcode:  0xEC 
  ex:      CPX $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----**
  bytes:   3 
  desc:    compare with X 
  form1:   X-{adr} 
  form2:   (X - M) -> NZC             
  form3:   *       t = X - M 
           *       P.N = t.7 
           *       P.C = (X>=M) ? 1:0 
           *       P.Z = (t==0) ? 1:0  
           *    
          
*/

void cpu_6502_CPX_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_CPX(cpu, mem[eadr]);
}


/*
  opname:  SBC
  opcode:  0xED 
  ex:      SBC $ABCD
  eadr:    absolute
  flags:   NVBDIZC :**---**
  bytes:   3 
  desc:    subtract with carry 
  form1:   A:=A-{adr} 
  form2:   A <- (A) - M - ~C          
  form3:   *       IF (P.D) 
           *         t = bcd(A) - bcd(M) - !P.C 
           *         P.V = (t>99 OR t<0) ? 1:0 
           *       ELSE 
           *         t = A - M - !P.C 
           *         P.V = (t>127 OR t<-128) ? 1:0 
           *       P.C = (t>=0) ? 1:0 
           *       P.N = t.7 
           *       P.Z = (t==0) ? 1:0 
           *       A = t & 0xFF                     
           *    
          
*/

void cpu_6502_SBC_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);
}


/*
  opname:  INC
  opcode:  0xEE 
  ex:      INC $ABCD
  eadr:    absolute
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    increment 
  form1:   {adr}:={adr}+1 
  form2:   M <- (M) + 1               
  form3:   *       M = (M + 1) & $FF 
           *       P.N = M.7 
           *       P.Z = (M==0) ? 1:0  
           *    
          
*/

void cpu_6502_INC_abs(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abs_adr_mode(cpu, mem);
  do_INC(cpu, mem[eadr]);
}


/*
  opname:  BEQ
  opcode:  0xF0 
  ex:      BEQ $AB
  eadr:    relativ
  flags:   NVBDIZC :-------
  bytes:   2 
  desc:    branch on equal (zero set) 
  form1:   branch on Z=1 
  form2:   if Z=1, PC = PC + offset   
  form3:   *       if (P.Z == 1) GOTO (PC+M)  
           *    
          
*/

void cpu_6502_BEQ_rel(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = rel_adr_mode(cpu, mem);
  char adr[] = "0000000000000000";
  eadr2adr(eadr,adr);

  if(cpu->flags[ZFLAG]=='1'){
    cp_byte(adr, cpu->pch);
    cp_byte(&(adr[8]), cpu->pcl);
  } else {
    inc_pc(cpu);
  }
}


/*
  opname:  SBC
  opcode:  0xF1 
  ex:      SBC ($A5),Y
  eadr:    indirection with index
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    subtract with carry 
  form1:   A:=A-{adr} 
  form2:   A <- (A) - M - ~C          
  form3:   *       IF (P.D) 
           *         t = bcd(A) - bcd(M) - !P.C 
           *         P.V = (t>99 OR t<0) ? 1:0 
           *       ELSE 
           *         t = A - M - !P.C 
           *         P.V = (t>127 OR t<-128) ? 1:0 
           *       P.C = (t>=0) ? 1:0 
           *       P.N = t.7 
           *       P.Z = (t==0) ? 1:0 
           *       A = t & 0xFF                     
           *    
          
*/

void cpu_6502_SBC_izy(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = izy_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);
}


/*
  opname:  SBC
  opcode:  0xF5 
  ex:      SBC $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :**---**
  bytes:   2 
  desc:    subtract with carry 
  form1:   A:=A-{adr} 
  form2:   A <- (A) - M - ~C          
  form3:   *       IF (P.D) 
           *         t = bcd(A) - bcd(M) - !P.C 
           *         P.V = (t>99 OR t<0) ? 1:0 
           *       ELSE 
           *         t = A - M - !P.C 
           *         P.V = (t>127 OR t<-128) ? 1:0 
           *       P.C = (t>=0) ? 1:0 
           *       P.N = t.7 
           *       P.Z = (t==0) ? 1:0 
           *       A = t & 0xFF                     
           *    
          
*/

void cpu_6502_SBC_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);
}


/*
  opname:  INC
  opcode:  0xF6 
  ex:      INC $AB,X
  eadr:    zero page with index X
  flags:   NVBDIZC :*----*-
  bytes:   2 
  desc:    increment 
  form1:   {adr}:={adr}+1 
  form2:   M <- (M) + 1               
  form3:   *       M = (M + 1) & $FF 
           *       P.N = M.7 
           *       P.Z = (M==0) ? 1:0  
           *    
          
*/

void cpu_6502_INC_zpx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = zpx_adr_mode(cpu, mem);
  do_INC(cpu, mem[eadr]);
}


/*
  opname:  SED
  opcode:  0xF8 
  ex:      SED 
  eadr:    implizit
  flags:   NVBDIZC :---1---
  bytes:   1 
  desc:    set decimal 
  form1:   D:=1 
  form2:   D <- 1                     
  form3:   *       P.D = 1  
           *    
          
*/

void cpu_6502_SED_imp(struct _6510_cpu* cpu, char mem[][9]){
  cpu->flags[DFLAG] = '1';
}


/*
  opname:  SBC
  opcode:  0xF9 
  ex:      SBC $ABCD,Y
  eadr:    absolut with index Y
  flags:   NVBDIZC :**---**
  bytes:   3 
  desc:    subtract with carry 
  form1:   A:=A-{adr} 
  form2:   A <- (A) - M - ~C          
  form3:   *       IF (P.D) 
           *         t = bcd(A) - bcd(M) - !P.C 
           *         P.V = (t>99 OR t<0) ? 1:0 
           *       ELSE 
           *         t = A - M - !P.C 
           *         P.V = (t>127 OR t<-128) ? 1:0 
           *       P.C = (t>=0) ? 1:0 
           *       P.N = t.7 
           *       P.Z = (t==0) ? 1:0 
           *       A = t & 0xFF                     
           *    
          
*/

void cpu_6502_SBC_aby(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = aby_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);
}


/*
  opname:  SBC
  opcode:  0xFD 
  ex:      SBC $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :**---**
  bytes:   3 
  desc:    subtract with carry 
  form1:   A:=A-{adr} 
  form2:   A <- (A) - M - ~C          
  form3:   *       IF (P.D) 
           *         t = bcd(A) - bcd(M) - !P.C 
           *         P.V = (t>99 OR t<0) ? 1:0 
           *       ELSE 
           *         t = A - M - !P.C 
           *         P.V = (t>127 OR t<-128) ? 1:0 
           *       P.C = (t>=0) ? 1:0 
           *       P.N = t.7 
           *       P.Z = (t==0) ? 1:0 
           *       A = t & 0xFF                     
           *    
          
*/

void cpu_6502_SBC_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_SBC(cpu, mem[eadr]);
}


/*
  opname:  INC
  opcode:  0xFE 
  ex:      INC $ABCD,X
  eadr:    absolut with index X
  flags:   NVBDIZC :*----*-
  bytes:   3 
  desc:    increment 
  form1:   {adr}:={adr}+1 
  form2:   M <- (M) + 1               
  form3:   *       M = (M + 1) & $FF 
           *       P.N = M.7 
           *       P.Z = (M==0) ? 1:0  
           *    
          
*/

void cpu_6502_INC_abx(struct _6510_cpu* cpu, char mem[][9]){
  int eadr = abx_adr_mode(cpu, mem);
  do_INC(cpu, mem[eadr]);
}



