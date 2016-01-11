/*
  register.c
  init: 16.8.2015
  
  GPL
*/
#include <ctype.h>
#include <stdio.h>

#include "register.h"

extern  void two_complement(char*);

void ldhex2regnibble(char hexcode, char* regnibble){
	
	switch (tolower(hexcode)){
	case '0':
		*regnibble++ = '0';
		*regnibble++ = '0';
		*regnibble++ = '0';
		*regnibble = '0';
		break;
	case '1':
		*regnibble =   '0';
		*(regnibble+1) = '0';
		*(regnibble+2) = '0';
		*(regnibble+3) = '1';
		break;
	case '2':
		*regnibble =   '0';
		*(regnibble+1) = '0';
		*(regnibble+2) = '1';
		*(regnibble+3) = '0';
		break;
	case '3':
		*regnibble =   '0';
		*(regnibble+1) = '0';
		*(regnibble+2) = '1';
		*(regnibble+3) = '1';
		break;
	case '4':
		*regnibble =   '0';
		*(regnibble+1) = '1';
		*(regnibble+2) = '0';
		*(regnibble+3) = '0';
		break;
	case '5':
		*regnibble =   '0';
		*(regnibble+1) = '1';
		*(regnibble+2) = '0';
		*(regnibble+3) = '1';
		break;
	case '6':
		*regnibble =   '0';
		*(regnibble+1) = '1';
		*(regnibble+2) = '1';
		*(regnibble+3) = '0';
		break;
	case '7':
		*regnibble =   '0';
		*(regnibble+1) = '1';
		*(regnibble+2) = '1';
		*(regnibble+3) = '1';
		break;
	case '8':
		*regnibble =   '1';
		*(regnibble+1) = '0';
		*(regnibble+2) = '0';
		*(regnibble+3) = '0';
		break;
	case '9':
		*regnibble =   '1';
		*(regnibble+1) = '0';
		*(regnibble+2) = '0';
		*(regnibble+3) = '1';
		break;
	case 'a' :
		*regnibble =   '1';
		*(regnibble+1) = '0';
		*(regnibble+2) = '1';
		*(regnibble+3) = '0';
		break;
	case 'b' :
		*regnibble =   '1';
		*(regnibble+1) = '0';
		*(regnibble+2) = '1';
		*(regnibble+3) = '1';
		break;
	case 'c' :
		*regnibble =   '1';
		*(regnibble+1) = '1';
		*(regnibble+2) = '0';
		*(regnibble+3) = '0';
		break;
	case 'd' :
		*regnibble =   '1';
		*(regnibble+1) = '1';
		*(regnibble+2) = '0';
		*(regnibble+3) = '1';
		break;
	case 'e' :
		*regnibble =   '1';
		*(regnibble+1) = '1';
		*(regnibble+2) = '1';
		*(regnibble+3) = '0';
		break;
	case 'f' :
		*regnibble =   '1';
		*(regnibble+1) = '1';
		*(regnibble+2) = '1';
		*(regnibble+3) = '1';
		break;
	}
}

/*
  convert bitstring <bitstr> from bit <start> to bit <end> into integer
  101000 ->
   ^  ^
   |  |
 start|
     end
  convert "010" into int 2
*/
int conv_bitstr2int(char *bitstr, unsigned int start, unsigned int end){
  unsigned int i=0;
  int sum=0;
  for(i=start; i<=end;i++)
    if(bitstr[i] == '1'){
      sum = (sum << 1) | 1;
    }else
      sum = (sum << 1);
  return sum;
}


int word2int(char* a, char* b){
  char word[17] = "0000000000000000";
  cp_register(a, word);
  cp_register(b, &word[8]);
  return conv_bitstr2int(word, 0, 15);
}


/*
  s2int(char*reg)
  signed register value to C int representation
*/
int s2int(char* reg){
  char dummy[8];
  int signeda;
  if(reg[0] == '1'){
    cp_register(reg, dummy);
    two_complement(dummy);
    signeda = - conv_bitstr2int(dummy, 0, 7);
  }else{
    signeda = conv_bitstr2int(reg, 0, 7);
  }
  return signeda;
}


void ldhex2register(char* hexcode, char* reg){
	switch(REG_WIDTH){
		case 8:
			ldhex2regnibble(*hexcode, reg);
			ldhex2regnibble(*(hexcode+1), reg+4);
			break;
		case 16:
			ldhex2regnibble(*hexcode, reg);
			ldhex2regnibble(*(hexcode+1), reg+4);
			ldhex2regnibble(*(hexcode+2), reg+8);
			ldhex2regnibble(*(hexcode+3), reg+12);
			break;
	}
}

/*
  some helper function transforming int
  to ASCII "10101010" format
*/
void int2register(int number, char* reg){
  reg[8]=0x0;
  int i;
  for(i=0; i<8;i++){
    if(number & 1<<i)
      reg[7-i]='1';
    else
      reg[7-i]='0';
  }
}


void cp_register(char *r1, char *r2){
  int i;
  for(i=0; i<REG_WIDTH; i++)
    r2[i] = r1[i];
  r2[i] = 0;
}

void cp_byte(char *r1, char *r2){
  int i;
  for(i=0; i<8; i++)
    r2[i] = r1[i]; //    *(r2+i) = *(r1+i);
}

