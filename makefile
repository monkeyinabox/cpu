IDIR =src/include
CC=gcc
CFLAGS=-Wall -g -I$(IDIR)
LIBS=-lreadline 


_OBJ = alu/alu.o alu/flags.o memory/memory.o cpu/parser.o cpu/cpu.o graphic/graphic-main.o cpu/mu-code.o print/print-cpu.o cpu/disassemble.o graphic/graphic.o cpu/opcode_tbl.o cpu/register.o
_OBJ = src/alu/alu.o src/alu/flags.o src/memory/memory.o src/cpu/parser.o src/cpu/cpu.o src/graphic/graphic-main.o src/cpu/mu-code.o src/printers/print-cpu.o src/cpu/disassemble.o src/graphic/graphic.o src/cpu/opcode_tbl.o src/alu/register.o

OBJ = $(_OBJ)

VPATH = alu cpu graphic print memory include

src/alu/alu.o: src/alu/alu.c src/include/alu.h
		$(CC) -c -o $@ $< $(CFLAGS)

src/alu/flags.o: src/alu/flags.c src/include/flags.h
		$(CC) -c -o $@ $< $(CFLAGS)

src/memory/memory.o: src/memory/memory.c src/include/memory.h
		$(CC) -c -o $@ $< $(CFLAGS)

src/cpu/parser.o: src/cpu/parser.c 
		$(CC) `sdl2-config --cflags --libs` -c -o $@ $< $(CFLAGS)

src/cpu/cpu.o: src/cpu/cpu.c src/include/cpu.h
		$(CC) -c -o $@ $< $(CFLAGS)

src/graphic/graphic-main.o: src/graphic/graphic-main.c #src/include/graphic-main.h
		$(CC) `sdl2-config --cflags --libs` -c -o $@ $< $(CFLAGS)

src/cpu/mu-code.o: src/cpu/mu-code.c src/include/mu-code.h
		$(CC) -c -o $@ $< $(CFLAGS)

src/printers/print-cpu.o: src/printers/print-cpu.c src/include/print-cpu.h
		$(CC) -c -o $@ $< $(CFLAGS)

src/cpu/disassemble.o: src/cpu/disassemble.c src/include/disassemble.h
		$(CC) -c -o $@ $< $(CFLAGS)

src/graphic/graphic.o: src/graphic/graphic.c src/include/graphic.h
		$(CC) `sdl2-config --cflags --libs` -c -o $@ $< $(CFLAGS)

src/cpu/opcode_tbl.o: src/cpu/opcode_tbl.c src/include/opcode_tbl.h
		$(CC) -c -o $@ $< $(CFLAGS)

src/alu/register.o: src/alu/register.c src/include/register.h
		$(CC) -c -o $@ $< $(CFLAGS)

sipiu: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS) `sdl2-config --cflags --libs`

.PHONY: clean

clean:
	find . -name "*.o" -exec rm {} \; -print && rm sipiu


.PHONY: tags

tags:
	rm -f TAGS
	find . -name "*.[ch]" -print | xargs etags -a

.PHONY: test

# test should run until end and produce endless loop at address $3c37 with 
# jmp $3c37
# jmp $3c37
test: sipiu
	echo -e "Wait for 3c37  4c 37 3c  JMP $3c37\n"
	echo -e "go 1000\nstep 40000000\nquit\n" | ./sipiu --binary test/cl-6502/test.bin  000a | tail

