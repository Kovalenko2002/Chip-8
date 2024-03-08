#ifndef CPU_H
#define CPU_H

/* Includes */

#include <stdint.h>
#include <SDL2/SDL.h>

/* Macros */

#define MEMORY_SIZE 4069
#define READ_AREA 0x200
#define REGISTER_NUMBER 16
#define STACK_SIZE 16
#define CPU_SPEED 4
#define FPS 16 // ms
#define TIME_FREQUENCY 60 // Hz
#define DIGIT_PATH "./digit"
#define NB_KEYS 16
#define KEY_PRESSED 1
#define KEY_UNPRESSED 0

/* Structs */

/**
 * @brief Structure containing the element to simulate the CPU. 
 * @param ram An array of 1 bytes of size 4069.
 * @param V An array containing the 16 registers of 1 byte size.
 * @param I A 2 bytes value.
 * @param PC A 2 bytes program counter.
 * @param delay A 1 byte delay timer register.
 * @param sound_timer A 1 byte sound timer register.
 * @param stack The stack of the CPU, its size is 16.
 * @param stack_pointer The pointer of last occupied value in stack. 
 * @param keyboard a table indicating if key were pressed. */
typedef struct {
    uint8_t ram[MEMORY_SIZE];
    uint8_t V[REGISTER_NUMBER];
    uint16_t I;
    uint16_t PC;
    uint8_t delay, sound_timer;
    uint16_t stack[STACK_SIZE];
    uint8_t stack_pointer;
    uint8_t keyboard[NB_KEYS];
} cpu;

/* Globals */

extern cpu CPU;

/* Functions */

void initialize();
void time_count();
uint16_t get_opcode();
uint8_t interpret_opcode(uint16_t opcode);
void load_digit(char* digit_binary);
void load_game(char* rom_name);
void draw_sprite(uint8_t x, uint8_t y, uint8_t height);
uint8_t wait_for_key(Uint8 b);

#endif /* CPU_H */
