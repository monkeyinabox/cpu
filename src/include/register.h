/*
  register.h
  bho1 2006
  bho1 2007
*/

#define REG_WIDTH 8

/*
  copy register source to register target:
  void cp_register(char* source, char* target);
*/
void cp_register(char* , char* );
void cp_byte(char* , char* );
void ldhex2register(char*, char*);
void int2register(int number, char* reg);
int conv_bitstr2int(char *bitstr, unsigned int start, unsigned int end);
int word2int(char* regh, char* regl);
int s2int(char* reg);


