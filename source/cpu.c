/**
 * @file cpu.c
 * @author Xavier Monard
 * @brief 
 * @version 0.1
 * @date 2023-06-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "include/cpu.h"
#include "include/display.h"

cpu CPU;

/**
 * @brief Initialize the CPU used by the emulator. It sets registers, the stack and keyboard state to 0.
 * 
 */
void initialize(){
    for (uint8_t k = 0; k < REGISTER_NUMBER; k++){
        CPU.V[k] = 0;
    }
    
    for (uint8_t j = 0; j < STACK_SIZE; j++){
        CPU.stack[j] = 0;
    }
    load_digit(DIGIT_PATH);
    CPU.I = 0;
    CPU.PC = READ_AREA;
    
    CPU.delay = 0;
    CPU.sound_timer = 0;

    for (uint8_t k = 0; k < NB_KEYS; k++){
        CPU.keyboard[k] = 0;
    }
}

/**
 * @brief Decount the ST and DT register if they are superior to 0.
 * 
 */
void time_count(){
    if (CPU.sound_timer > 0) {
        CPU.sound_timer--;
    }
    if (CPU.delay > 0){
        CPU.delay--;
    }
}

/**
 * @brief Load a 2 bytes opcode from memory
 * 
 * @return uint16_t A 2 bytes opcode
 */
uint16_t get_opcode(){
    return (CPU.ram[CPU.PC]<<8) + (CPU.ram[CPU.PC+1]);
}

/**
 * @brief Interpret a 2 bytes opcode and execute it.
 * 
 * @param opcode A 2 bytes opcode.
 */
uint8_t interpret_opcode(uint16_t opcode){
    uint8_t hexa[4];    
    uint16_t mask[4] = {0xF000, 0x0F00, 0x00F0, 0x000F};
    uint8_t keep_up = 1;

    for (uint8_t k = 0; k < 4; k++){
        hexa[k] = (opcode & mask[k]) >> (12 - 4 * k); 
    }

    /* Interpret leftmost 4 bits and developp if needed */
    switch (hexa[0]){
        case 0x00: // 0NNN and 00E0 and 00EE
            if (hexa[2] == 0xE && hexa[3] == 0x0){ // 00E0
                // Clear the display.
                clear_screen();                
            }
            else if (hexa[2] == 0xE && hexa[3] == 0xE){ // 00EE
                // Return from a subroutine.
                if (CPU.stack_pointer > 0){
                    CPU.stack_pointer--; 
                    CPU.PC = CPU.stack[CPU.stack_pointer];
                }
            } // ONNN (Nothing to do...)
            break;

        case 0x01: // 1nnn
            // Jump to location nnn.
            CPU.PC = (hexa[1]<<8) + (hexa[2]<<4) + hexa[3];
            CPU.PC-=2;
            break;

        case 0x02: //2nnn
            // Call subroutine at nnn.
            CPU.stack[CPU.stack_pointer] = CPU.PC;
            if (CPU.stack_pointer < 15 ){
                CPU.stack_pointer++;
            }
            
            CPU.PC = (hexa[1]<<8) + (hexa[2]<<4) + hexa[3];
            CPU.PC-=2;
            break;

        case 0x03: // 3xkk
            // Skip next instruction if Vx = kk.
            if (CPU.V[hexa[1]] == (hexa[2]<<4) + hexa[3]){
                CPU.PC+=2;
            }
            break;

        case 0x04: // 4xkk
            // Skip next instruction if Vx != kk.
            if (CPU.V[hexa[1]] != (hexa[2]<<4) + hexa[3]){
                CPU.PC+=2;
            }
            break;

        case 0x05: // 5xy0
            // Skip next instruction if Vx = Vy.
            if (CPU.V[hexa[1]] == CPU.V[hexa[2]]){
                CPU.PC+=2;
            }
            break;

        case 0x06: // 6xkk
            // Set Vx = kk.
            CPU.V[hexa[1]] =  (hexa[2]<<4) + hexa[3];
            break;

        case 0x07: // 7xkk
            // Set Vx = Vx + kk.
            CPU.V[hexa[1]] += (hexa[2]<<4) + hexa[3];
            break;

        case 0x08: // 8xy0, 8xy1, 8xy2, 8xy3, 8xy4, 8xy5, 8xy6, 8xy7 and 8xyE
            switch(hexa[3]){
                case 0x00: // 8xy0
                    // Set Vx = Vy.
                    CPU.V[hexa[1]] = CPU.V[hexa[2]];
                    break;

                case 0x01: // 8xy1
                    // Set Vx = Vx OR Vy.
                    CPU.V[hexa[1]] = CPU.V[hexa[1]] | CPU.V[hexa[2]];
                    break;

                case 0x02: // 8xy2
                    // Set Vx = Vx AND Vy.
                    CPU.V[hexa[1]] = CPU.V[hexa[1]] & CPU.V[hexa[2]];
                    break;

                case 0x03: // 8xy3
                    // Set Vx = Vx XOR Vy.
                    CPU.V[hexa[1]] = CPU.V[hexa[1]] ^ CPU.V[hexa[2]];
                    break;

                case 0x04: //8xy4
                    // Set Vx = Vx + Vy, set VF = carry.
                    if (CPU.V[hexa[1]] + CPU.V[hexa[2]]> 0xFF ){

                        CPU.V[0xF] = 0x01;
                    } 
                    else {
                        CPU.V[0xF] = 0x00;
                    }
                    CPU.V[hexa[1]] += CPU.V[hexa[2]];
                    break;

                case 0x05: // 8xy5
                    // Set Vx = Vx - Vy, set VF = NOT borrow.
                    if (CPU.V[hexa[1]] > CPU.V[hexa[2]]) {
                        CPU.V[0xF] = 0x1;
                    }
                    else {
                        CPU.V[0xF] = 0x0;
                    }
                    CPU.V[hexa[1]] = CPU.V[hexa[1]] - CPU.V[hexa[2]];
                    break;
                      
                case 0x06: // 8xy6
                    // Set Vx = Vx SHR 1.
                    CPU.V[0xF] = CPU.V[hexa[1]] & 0x1;
                    CPU.V[hexa[1]] = CPU.V[hexa[1]]>>1;
                    break;

                case 0x07: // 8xy7
                    // Set Vx = Vy - Vx, set VF = NOT borrow.
                    if (CPU.V[hexa[2]] > CPU.V[hexa[1]]){
                        CPU.V[0xF] = 0x1;
                    }
                    else {
                        CPU.V[0xF] = 0x0;
                    }
                    CPU.V[hexa[1]] = CPU.V[hexa[2]] - CPU.V[hexa[1]];
                    break;

                case 0x0E: // 8xyE
                    // Set Vx = Vx SHL 1.
                    CPU.V[0xF] = CPU.V[hexa[1]]>>7;
                    CPU.V[hexa[1]] = CPU.V[hexa[1]]<<1;
                    break;
                    
            }
            break;

        case 0x09: // 9xy0
            // Skip next instruction if Vx != Vy.
            if (CPU.V[hexa[1]] != CPU.V[hexa[2]]){
                CPU.PC+=2;
            }
            break;

        case 0x0A: // Annn
            // The value of register I is set to nnn.
            CPU.I = (hexa[1]<<8) + (hexa[2]<<4) + hexa[3];
            break;

        case 0x0B: // Bnnn
            // Jump to location nnn + V0.
            CPU.PC = (hexa[1]<<8) + (hexa[2]<<4) + hexa[3] + CPU.V[0];
            CPU.PC-=2;
            break;

        case 0x0C: // Cxkk
            // Set Vx = random byte AND kk.
            CPU.V[hexa[1]] = (rand()%((hexa[2]<<4) + hexa[3] + 1));
            break;

        case 0x0D: // Dxyn
            // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
            draw_sprite(CPU.V[hexa[1]], CPU.V[hexa[2]], hexa[3]);
            break;
            
        case 0x0E: // Ex9E, ExA1
            if (hexa[2] == 0x9 && hexa[3] == 0xE){
                // Skip next instruction if key with the value of Vx is pressed.
                if (CPU.keyboard[CPU.V[hexa[1]]] == KEY_PRESSED){
                    CPU.PC+=2;
                }
            }
            else if (hexa[2] == 0xA && hexa[3] == 0x1){
                // Skip next instruction if key with the value of Vx is not pressed.
                if (CPU.keyboard[CPU.V[hexa[1]]] == KEY_UNPRESSED){
                    CPU.PC+=2;
                }
            }
            break;

        case 0x0F: // Fx07, Fx0A, Fx15, Fx18, Fx1E, Fx29, Fx33, Fx55 and Fx65
            switch (hexa[2]){
                case 0: // Fx07 and Fx0A
                    if (hexa[3] == 0x7){ 
                        // Set Vx = delay timer value.
                        CPU.V[hexa[1]] = CPU.delay;
                    }
                    else if (hexa[3] == 0xA){
                        // Wait for a key press, store the value of the key in Vx.
                        keep_up = wait_for_key(hexa[1]);
                    }
                    break;

                case 0x01:
                    switch(hexa[3]){
                        case 0x5: // Fx15
                            // Set delay timer = Vx.
                            CPU.delay = CPU.V[hexa[1]];
                            break;

                        case 0x8: // Fx18
                            // Set sound timer = Vx.
                            CPU.sound_timer = CPU.V[hexa[1]];
                            break;

                        case 0xE: // Fx1E
                            // Set I = I + Vx.
                            if (CPU.I + CPU.V[hexa[1]] > 0xFFF){
                                CPU.V[0xF] = 1;
                            }
                            else {
                                CPU.V[0xF] = 0;
                                CPU.I += CPU.V[hexa[1]];
                            }
                            break;
                        default:
                            break;
                    }
                    break;

                case 0x02: // Fx29
                    // Set I = location of sprite for digit Vx.
                    CPU.I = 5*CPU.V[hexa[1]];
                    break;

                case 0x03: // Fx33
                    // Store BCD representation of Vx in memory locations I, I+1, and I+2.
                    CPU.ram[CPU.I] = (CPU.V[hexa[1]] - CPU.V[hexa[1]%100])/100;
                    CPU.ram[CPU.I+1] = (((CPU.V[hexa[1]]-CPU.V[hexa[1]]%10)/10)%10);
                    CPU.ram[CPU.I+2] = CPU.V[hexa[1]] - CPU.ram[CPU.I]*100 - CPU.ram[CPU.I+1]*10;
                    break;

                case 0x05: // Fx55
                    // Store registers V0 through Vx in memory starting at location I.
                    for (uint8_t k = 0x0; k <= hexa[1]; k++){
                        CPU.ram[CPU.I + k] = CPU.V[k];
                    }
                    break;

                case 0x06: // Fx65
                    // Read registers V0 through Vx from memory starting at location I.
                    for (uint8_t k = 0x00; k <= hexa[1]; k++){
                        CPU.V[k] = CPU.ram[CPU.I + k];
                    }
                    break;
                
                default: {
                    break; 
                }

            }
    }
    CPU.PC+= 2;
    return keep_up;
}

/**
 * @brief Store the representation of 1, 2,3 ... C, D and F in ram starting at the 0 address.
 * 
 * @param digit_binary Path to the file conting the sprites. 
 */
void load_digit(char* digit_binary){
    FILE *bin_file = NULL;
    bin_file = fopen(digit_binary, "rb");

    if (bin_file == NULL){
        fprintf(stderr, "Unable to load the digit file. ");
        exit(EXIT_FAILURE);
    }
    fread(&CPU.ram[0], sizeof(uint8_t)*16* HEX_REP_SIZE, 1, bin_file);
    fclose(bin_file);
}

/**
 * @brief Load a game rom to the ram.
 * 
 * @param rom_name Path to a binary game file.
 */
void load_game(char* rom_name){
    FILE* rom = NULL;
    rom = fopen(rom_name,"rb");

    if (rom == NULL){
        fprintf(stderr, "Unable to load the ROM.\n");
        exit(EXIT_FAILURE);
    }

    fread(&CPU.ram[READ_AREA], sizeof(uint8_t) * (MEMORY_SIZE-READ_AREA), 1, rom);
    fclose(rom);
}

/**
 * @brief Draws a sprite for the opcode DXYN.
 * 
 * @param x X-coordinate value (0<=X<64).
 * @param y Y-coordinate value (0<=X<32).
 * @param height Size of the sprite in bytes.
 */
void draw_sprite(uint8_t x, uint8_t y, uint8_t height){
    
    // loading sprite data from ram
    uint8_t loaded_sprite[height];
    for (uint8_t k = 0; k < height; k++){
    loaded_sprite[k] = CPU.ram[CPU.I+k];
    }
    // Set VF at 0 by default
    CPU.V[0x0F] = 0;

    // Drawing the sprite in the screen table
    for (uint8_t j = 0; j < height; j++){
        for (uint8_t k = 0; k < 8; k++){            
            if ((screen[(x+k)%SCREEN_WIDTH][(y+j)%SCREEN_HEIGTH].couleur == 1) && ((loaded_sprite[j]&(0x1<<(7-k)))>>(7-k)) == 1){
                CPU.V[0xF] = 1;
            };
            screen[(x+k)% SCREEN_WIDTH][(y+j)%SCREEN_HEIGTH].couleur ^= (loaded_sprite[j]&(0x1<<(7-k)))>>(7-k);
        }
    }
}

/**
 * @brief Wait for user input for the opcode Fx0A
 * 
 * @param b Register value to store the result. (0<= b < 16)
 */
uint8_t wait_for_key(Uint8 b){
    uint8_t wait = 1;
    uint8_t keep_up = 1;
    while (wait == 1){
        SDL_WaitEvent(&sdl_event);
        switch (sdl_event.type)
        {
        case SDL_QUIT:
            wait = 0;
            keep_up = 0;
            break;

        case SDL_KEYDOWN:
            switch(sdl_event.key.keysym.sym){
                case SDLK_0:{CPU.V[b] = 0x0; CPU.keyboard[0x0] = 1; wait = 0; break;}
                case SDLK_1:{CPU.V[b] = 0x1; CPU.keyboard[0x1] = 1; wait = 0; break;}
                case SDLK_2:{CPU.V[b] = 0x2; CPU.keyboard[0x2] = 1; wait = 0; break;}
                case SDLK_3:{CPU.V[b] = 0x3; CPU.keyboard[0x3] = 1; wait = 0; break;}
                case SDLK_4:{CPU.V[b] = 0x4; CPU.keyboard[0x4] = 1; wait = 0; break;}
                case SDLK_5:{CPU.V[b] = 0x5; CPU.keyboard[0x5] = 1; wait = 0; break;}
                case SDLK_6:{CPU.V[b] = 0x6; CPU.keyboard[0x6] = 1; wait = 0; break;}
                case SDLK_7:{CPU.V[b] = 0x7; CPU.keyboard[0x7] = 1; wait = 0; break;}
                case SDLK_8:{CPU.V[b] = 0x8; CPU.keyboard[0x8] = 1; wait = 0; break;}
                case SDLK_9:{CPU.V[b] = 0x9; CPU.keyboard[0x9] = 1; wait = 0; break;}
                case SDLK_a:{CPU.V[b] = 0xA; CPU.keyboard[0xA] = 1; wait = 0; break;}
                case SDLK_b:{CPU.V[b] = 0xB; CPU.keyboard[0xB] = 1; wait = 0; break;}
                case SDLK_c:{CPU.V[b] = 0xC; CPU.keyboard[0xC] = 1; wait = 0; break;}
                case SDLK_d:{CPU.V[b] = 0xD; CPU.keyboard[0xD] = 1; wait = 0; break;}
                case SDLK_e:{CPU.V[b] = 0xE; CPU.keyboard[0xE] = 1; wait = 0; break;}
                case SDLK_f:{CPU.V[b] = 0xF; CPU.keyboard[0xF] = 1; wait = 0; break;}
                default:
                    break;  
            }
        default:
            break;
        }
    }
    return keep_up;
}
