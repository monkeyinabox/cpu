/*
  cpu.h
  bho1 16.8.2015
  GPL
*/

#define ADDR_WIDTH 16

struct _6510_cpu {
  char pcl[9];
  char pch[9];
  char regx[9];
  char regy[9];
  char rega[9];
  char flags[9];
  char sp[9];

  char abrl[9];
  char abrh[9];
  char dbr[9];
  char ir[9];
  char rw;
  int interrupt;
  int cycles;
};

void access_memory( struct _6510_cpu* cpu, char memory[][9]);

int pc2int(struct _6510_cpu*cpu);
void inc_pc(struct _6510_cpu* cpu);
void dec_pc(struct _6510_cpu* cpu);
int fetch( struct _6510_cpu* cpu, char memory[][9] ) ;
int exec( struct _6510_cpu* cpu, char memory[][9]  ) ;
void fetch_exec_step( struct _6510_cpu* cpu, char memory[][9] ) ;
void fetch_exec_cycle( struct _6510_cpu* cpu, char memory[][9]  ) ;

int imp_adr_mode(struct _6510_cpu* cpu, char memory[][9] );
int imm_adr_mode(struct _6510_cpu* cpu, char memory[][9] ); 
int rel_adr_mode(struct _6510_cpu* cpu, char memory[][9] ); 
int izx_adr_mode(struct _6510_cpu* cpu, char memory[][9] ); 
int izy_adr_mode(struct _6510_cpu* cpu, char memory[][9] ); 
int zp_adr_mode(struct _6510_cpu* cpu, char memory[][9] ); 
int zpx_adr_mode(struct _6510_cpu* cpu, char memory[][9] ); 
int zpy_adr_mode(struct _6510_cpu* cpu, char memory[][9] ); 
int abs_adr_mode(struct _6510_cpu* cpu, char memory[][9] ); 
int abx_adr_mode(struct _6510_cpu* cpu, char memory[][9] ); 
int aby_adr_mode(struct _6510_cpu* cpu, char memory[][9] ); 
int ind_adr_mode(struct _6510_cpu* cpu, char memory[][9] ); 
void cpu_6502_illegal(struct _6510_cpu* cpu, char memory[][9] ); 
void reset_cpu(struct _6510_cpu* cpu);



