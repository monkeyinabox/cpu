#!/bin/bash
SOURCE=../opcode_table_bigABC
TARGET=../../src/cpu/mu-code.student.c

#opcode_table_bigABC
#0x01 @ 0x2 @ 0x6 @ ORA @ izx @ ADRM_IZX @ 1 @ ORA @ 0x01 @ cpu_6502_ora_izx @ izx @ or with accumulator @ ORA X,ind @ A:=A or {adr} @ *----*- @ ORA @    A = A | M @    P.N = A.7 @    P.Z = (A==0) ? 1:0  @ 
#0x61 @ 0x2 @ 0x6 @ ADC @ izx @ ADRM_IZX @ 1 @ ADC @ 0x61 @ cpu_6502_adc_izx @ izx @ add with carry @ ADC X,ind @ A:=A+{adr} @ **---** @ ADC @    t = A + M + P.C @    P.V = (A.7!=t.7) ? 1:0 @    P.N = A.7 @    P.Z = (t==0) ? 1:0 @    IF (P.D) @      t = bcd(A) + bcd(M) + P.C @      P.C = (t>99) ? 1:0 @    ELSE @      P.C = (t>255) ? 1:0 @    A = t & 0xFF                 @ 

awk -F"@" -e 'BEGIN { ops="ADC AND ASL BCC BCS BEQ BIT BMI BNE BPL BRK BVC BVS CLC CLD CLI CLV CMP CPX CPY DEC DEX DEY EOR INC INX INY JMP JSR LDA LDX LDY LSR NOP ORA PHA PHP PLA PLP ROL ROR RTI RTS SBC SEC SED SEI STA STX STY TAX TAY TSX TXA TXS TYA "; printf("/***\n Automatic generated by gen-mucode.c.bash\n***/\n\n\#include \"cpu.h\"\n\n");
split(ops,oparr," ");
for(t in oparr){
 printf("void do_%s(struct _6510_cpu* cpu, char *mem){\n}\n",oparr[t]);
}
}
END { printf("\nvoid imp_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){}\n void imm_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){} \n void rel_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){}\n void izx_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){}\n void izy_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){}\n void zp_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){}\n void zpx_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){}\n void zpy_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){}\n void abs_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){}\n void abx_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){}\n void aby_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){} \n void ind_adr_mode(struct _6510_cpu* cpu, char memory[][9] ){} \nvoid cpu_6502_illegal(struct _6510_cpu* cpu, char memory[][9] ){}\n "); }
$4!~/CRASH/ && $4!~/\*../{ opcode = $1; oplength=$2; opcycles=$3; opname=$4; opadrmode=$5; opadrmodeid=$6; cname=$10; shorttxt=$12; formaltxt=$14; flags=$15; a=0; for(i=17; i<=NF; i++){ logictxt[a++]=$i}
lines = i-17;
switch(opadrmode){
case " imm " : {
  adrmodetxt = "immediate -- data immediate after opcode; data = mem[PC+1]"
  exampletxt = "#$AB    "
  codetxt = "imm_adr_mode(cpu, mem);"
  break;
}
case " ind " : {
  adrmodetxt = "indirect; indirection via zp address; ea = mem[zpadr+1]|mem[zpadr]"
  exampletxt = "$ABCD"
  codetxt = "ind_adr_mode(cpu, mem);"
  break;
}
case " abs " : {
  adrmodetxt = "absolute 16 bit address; data = mem[abs_high|abs_low]"
  exampletxt = "$ABCD"
  codetxt = "abs_adr_mode(cpu, mem);"
  break;
}
case " abx " : {
  adrmodetxt = "absolute indexed addressing -- absolute 16 bit address with index X; ea = abs+X";
  exampletxt = "$ABCD,X";
  codetxt = "abx_adr_mode(cpu, mem);"
  break;
}
case " aby " : {
  adrmodetxt = "absolute 16 bit address with index Y; ea = abs+Y"
  exampletxt = "$ABCD,Y"
  codetxt = "aby_adr_mode(cpu, mem);"
  break;
}
case " izx " : {
  adrmodetxt = "indirect X-indexed zero page; ea = mem[zpadr+X|zpadr+X+1]"
  exampletxt = "($A5,X)"
  codetxt = "izx_adr_mode(cpu, mem);"
  break;
}
case " izy " : {
  adrmodetxt = "indirect Y-indexed zero page; ea = mem[zpadr|zpadr+1]+Y"
  exampletxt = "($A5),X"
  codetxt = "izy_adr_mode(cpu, mem);"
  break;
}
case " imp " : {
  adrmodetxt = "implizit; no additional info needed"
  exampletxt = "";
  codetxt = "";
  break;
}
case " rel " : {
  adrmodetxt = "relativ; address is relativ with 8-bit signed offset to PC; adr=pc+offset"
  exampletxt = "$AB";
  codetxt = "rel_adr_mode(cpu, mem);"
  break;
}
case " zp " : {
  adrmodetxt = "zero page ; ea = zpadr"
  exampletxt = "$AB"
  codetxt = "zp_adr_mode(cpu, mem);"
  break;
}
case " zpx " : {
  adrmodetxt = "zero page with index register X; ea = zpadr+X"
  exampletxt = "$AB,X"
  codetxt = "zpx_adr_mode(cpu, mem);"
  break;
}
case " zpy " : {
  adrmodetxt = "zero page with index register Y; ea = zpadr+Y"
  exampletxt = "$AB,X"
  codetxt = "zpy_adr_mode(cpu, mem);"
  break;
}
default: {
  adrmodetxt = "nix found"
  break;
}
}

#gsub("-","\\textendash",flags);
gsub(" ","",flags);
gsub(" ", "", opname)
gsub(" ", "", opadrmode)
gsub("\\&", "\\\\&", formaltxt)
flagstxt= "NVBDIZC :"flags;
printf("/*\n  opname:  %s\n  opcode:  %s\n  example: %s %s\n  adr:  %s\n  flags:    %s\n  bytes: %s\n  \n*/\n\nvoid cpu_6502_%s_%s(struct _6510_cpu* cpu, char mem[][9]){\n  int eadr = %s_adr_mode(cpu, mem);\n  do_%s(cpu, mem[eadr]);\n\n\n}\n\n\n",
opname, opcode, opname, exampletxt, adrmodetxt, flagstxt, substr(oplength,4), opname, opadrmode, opadrmode, opname, shorttxt, formaltxt)

}' < $SOURCE > $TARGET
