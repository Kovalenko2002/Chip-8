/**
 * @file translator.c
 * @author Xavier Monard
 * @brief 
 * @version 0.1
 * @date 2023-06-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "include/translator.h"
uint8_t rom_code[ROM_SIZE];


void load_rom(char* rom_name){
    FILE *rom = NULL;
    rom = fopen(rom_name, "rb");

    if (rom == NULL){
        fprintf(stderr, "Unable to load the rom file %s\n", rom_name);
        exit(EXIT_FAILURE);
    }

    fread(&rom_code[0], sizeof(uint8_t) * (ROM_SIZE), 1, rom);
    fclose(rom);
}

void translate_opcode(int* hexa){

    switch(hexa[0]){
        case 0x0:
            if (hexa[2] == 0xE && hexa[3] == 0x0){
                printf("CLS");
            } else if (hexa[2] == 0xE && hexa[3] == 0xE){
                printf("RET");
            } else {
                printf("SYS %X", (hexa[1]<<8)+(hexa[2]<<4)+hexa[3]);
            }
            break;

        case 0x1:
            printf("JP %d", (hexa[1]<<8)+(hexa[2]<<4)+hexa[3]);
            break;

        case 0x2:
            printf("CALL %d",(hexa[1]<<8)+(hexa[2]<<4)+hexa[3]);
            break;

        case 0x3:
            printf("SE V%X, %d",hexa[1],((hexa[2]<<4)+hexa[3]));
            break;

        case 0x4:
            printf("SNE V%X, %d",hexa[1],((hexa[2]<<4)+hexa[3]));
            break;

        case 0x5:
            printf("SE V%X,V%X",hexa[1],hexa[2]);
            break;

        case 0x6:
            printf("LD V%X, %d", hexa[1], ((hexa[2]<<4)+hexa[3]));
            break;
            
        case 0x7:
            printf("ADD V%X, V%X",hexa[1],hexa[2]);
            break;

        case 0x8:
            switch(hexa[3]){
                case 0x0:
                    printf("LD V%X, V%X", hexa[1], hexa[2]);
                    break;

                case 0x1:
                    printf("OR V%X, V%X", hexa[1], hexa[2]);
                    break;

                case 0x2:
                    printf("AND V%X, V%X", hexa[1], hexa[2]);
                    break;

                case 0x3:
                    printf("XOR V%X, V%X", hexa[1], hexa[2]);
                    break;

                case 0x4:
                    printf("ADD V%X, V%X", hexa[1], hexa[2]);
                    break;

                case 0x5:
                    printf("SUB V%X, V%X", hexa[1], hexa[2]);
                    break;

                case 0x6:
                    printf("SHR V%X {, V%X}", hexa[1], hexa[2]);
                    break;

                case 0x7:
                    printf("SUBN V%X, V%X", hexa[1], hexa[2]);
                    break;

                case 0xE:
                    printf("SHL V%X {, V%X}", hexa[1], hexa[2]);
                    break;

                default:
                    break;
            }
            break;
            
        case 0x9:
            printf("SNE V%X, V%X",hexa[1],hexa[2]);
            break;

        case 0xA:
            printf("LD I, %d", ((hexa[1]<<8)+(hexa[2]<<4)+hexa[3]));
            break;

        case 0xB:
            printf("JP V0, %d",((hexa[1]<<8)+(hexa[2]<<4)+hexa[3]));
            break;

        case 0xC:
            printf("RND V%X, %X",hexa[1], ((hexa[2]<<4)+hexa[3]));
            break;

        case 0xD:
            printf("DRW V%X, V%X, %d",hexa[1],hexa[2],hexa[3]);
            break;

        case 0xE:
            switch(hexa[3]){
                case 0xE:
                    printf("SKP V%X",hexa[1]);
                    break;

                case 0x1:
                    printf("SKNP V%X",hexa[1]);
                    break;

                default:
                    break;
            }
            break;

        case 0xF:{
            switch(hexa[3]){
                case 0x7:
                    printf("LD V%X, DT",hexa[1]);
                    break;

                case 0xA:
                    printf("LD V%X, K",hexa[1]);
                    break;

                case 0x5:
                    switch(hexa[2]){
                        case 0x1:
                            printf("LD DT, V%X",hexa[1]);
                            break;

                        case 0x5:
                            printf("LD [I], V%X", hexa[1]);
                            break;

                        case 0x6:
                            printf("LD V%X, [I]", hexa[1]);
                            break;

                        default:
                            break;
                    }
                    break;

                case 0x8:
                    printf("LD ST, V%X",hexa[1]);
                    break;

                case 0xE:
                    printf("ADD I, V%X", hexa[1]);
                    break;

                case 0x9:
                    printf("LD F, V%X", hexa[1]);
                    break;

                case 0x3:
                    printf("LD B, V%X", hexa[1]);
                    break;
            }
        }

        default:
            break;
    }
}

void hex2asm(){
    int mask[4] = {0xF000, 0x0F00, 0x00F0, 0x000F};
    int hexa[4];

    for (uint16_t k = 0; k < ROM_SIZE-1; k+=2){
        uint16_t command = (rom_code[k]<<8) + rom_code[k+1];
        // divide opcode
        for (uint8_t i = 0; i < 4; i++){
            hexa[i] = (command & mask[i])>>(12-4*i);
        }
        // call the translation function
        printf("%d.", k+READ_AREA);
        translate_opcode(hexa);
        printf("\n");
    }
}

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("You must give a file to translate.\n");
        return 1;
    }
    load_rom(argv[1]);
    hex2asm();
    return 0;
}
