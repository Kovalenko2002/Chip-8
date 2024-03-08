#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ROM_SIZE 4069
#define READ_AREA 0x200

void load_rom(char* rom_name);
void translate_opcode(int* hexa);
void hex2asm();
