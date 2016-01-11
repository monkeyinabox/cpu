/*
  memory.h
*/


#define MEMORY_SIZE 65536 //Memory of 2^16 Memorycells
#define MEMORY_CELL_SIZE 8 //8 character bits per cell
#define READ '1'
#define WRITE '0'
#define ADDR_WIDTH 16

#define LORAM 0
#define HIRAM 1
#define CHAREN 2

char* ram(int adr, char memory[][9]);

char* basic_ROM(int adr, char memory[][9]);
char* kernel_ROM(int adr, char memory[][9]);
char* char_ROM(int adr, char memory[][9]);
char* IO_ram(int adr, char memory[][9]);

void init_memory(char memory[][9]);


void read_memorydump(char *argv, char memory[][9]);
void read_binary_memorydump(char *fname, char memory[][9], int start);

//void print_memory_page0(struct _6510_cpu* cpu, char memory[][9], int adr);


