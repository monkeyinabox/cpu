#include "cpu.h"
void print_stack(struct _6510_cpu* cpu, char memory[][9]);
void print_memory_page0(char memory[][9], int adr);
void print_dis_memory(char memory[][9], int adr);
void printm(struct _6510_cpu* cpu, char memory[][9]);
void print_cpu(struct _6510_cpu* cpu);
void print_cpu_dis(struct _6510_cpu* cpu, char memory[][9] ) ;
