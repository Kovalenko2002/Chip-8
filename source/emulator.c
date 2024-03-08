/**
 * @file emulator.c
 * @author Xavier Monard
 * @brief 
 * @version 0.1
 * @date 2023-06-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <SDL2/SDL.h>
#include <stdio.h>
#include "include/cpu.h"
#include "include/display.h"

void activate_sdl();
void deactivate_sdl();
void pause();
uint8_t listen();



int main(int argc, char* argv[] ){
    if (argc != 2){
        printf("You muste give a name.\n");
        return EXIT_SUCCESS;
    }
    
    activate_sdl();
    initialize_sdl();
    initialize_screen();
    initialize();
    load_game(argv[1]);

    uint8_t keep_up = 1;
    do {
        keep_up = listen();

        // Interpret 4 opcode each 16ms
        for (int actions = 0; actions<CPU_SPEED && keep_up == 1; actions++){
            keep_up = interpret_opcode(get_opcode());
        }
        update_screen();
        time_count();
        SDL_Delay(FPS);
    } while (keep_up == 1);
    pause();

    return EXIT_SUCCESS;
}

/**
 * @brief Function that launches SDL.
 * 
 */
void activate_sdl(){
    atexit(deactivate_sdl);
    if (SDL_Init(SDL_INIT_VIDEO) == -1){
        fprintf(stderr, "Unable to launch SDL :\n %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

/* Function that stops SDL and free allocated texture elements. */
void deactivate_sdl(){
    // Freeing textures
    SDL_DestroyTexture(sdl_texture[0]);
    SDL_DestroyTexture(sdl_texture[1]);

    // Freeing renderer and window
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);

    // Quitting
    SDL_Quit();
}

/* Listen for events detected by SDL.*/
void pause(){
    uint8_t keep = 1;
    do {
        SDL_WaitEvent(&sdl_event);
        switch (sdl_event.type)
        {
        case SDL_QUIT:
            keep = 0;
            break;
        
        case SDL_KEYDOWN:
            keep = 0;
            break;
        }
    } while (keep == 1);
}

uint8_t listen(){
    uint8_t keep_up = 1;

    while(SDL_PollEvent(&sdl_event)) {

        switch(sdl_event.type){
            case SDL_QUIT: {keep_up = 0; break;}
            case SDL_KEYDOWN:
                switch(sdl_event.key.keysym.sym){
                    case SDLK_0: { CPU.keyboard[0x0] = KEY_PRESSED; break;}
                    case SDLK_1: { CPU.keyboard[0x1] = KEY_PRESSED; break;}
                    case SDLK_2: { CPU.keyboard[0x2] = KEY_PRESSED; break;}
                    case SDLK_3: { CPU.keyboard[0x3] = KEY_PRESSED; break;}
                    case SDLK_4: { CPU.keyboard[0x4] = KEY_PRESSED; break;}
                    case SDLK_5: { CPU.keyboard[0x5] = KEY_PRESSED; break;}
                    case SDLK_6: { CPU.keyboard[0x6] = KEY_PRESSED; break;}
                    case SDLK_7: { CPU.keyboard[0x7] = KEY_PRESSED; break;}
                    case SDLK_8: { CPU.keyboard[0x8] = KEY_PRESSED; break;}
                    case SDLK_9: { CPU.keyboard[0x9] = KEY_PRESSED; break;}
                    case SDLK_a: { CPU.keyboard[0xa] = KEY_PRESSED; break;}
                    case SDLK_b: { CPU.keyboard[0xb] = KEY_PRESSED; break;}
                    case SDLK_c: { CPU.keyboard[0xc] = KEY_PRESSED; break;}
                    case SDLK_d: { CPU.keyboard[0xd] = KEY_PRESSED; break;}
                    case SDLK_e: { CPU.keyboard[0xe] = KEY_PRESSED; break;}
                    case SDLK_f: { CPU.keyboard[0xf] = KEY_PRESSED; break;}
                    default: {break;}
                }
                break;
            case SDL_KEYUP:
                switch(sdl_event.key.keysym.sym){
                    case SDLK_0: { CPU.keyboard[0x0] = KEY_UNPRESSED; break;}
                    case SDLK_1: { CPU.keyboard[0x1] = KEY_UNPRESSED; break;}
                    case SDLK_2: { CPU.keyboard[0x2] = KEY_UNPRESSED; break;}
                    case SDLK_3: { CPU.keyboard[0x3] = KEY_UNPRESSED; break;}
                    case SDLK_4: { CPU.keyboard[0x4] = KEY_UNPRESSED; break;}
                    case SDLK_5: { CPU.keyboard[0x5] = KEY_UNPRESSED; break;}
                    case SDLK_6: { CPU.keyboard[0x6] = KEY_UNPRESSED; break;}
                    case SDLK_7: { CPU.keyboard[0x7] = KEY_UNPRESSED; break;}
                    case SDLK_8: { CPU.keyboard[0x8] = KEY_UNPRESSED; break;}
                    case SDLK_9: { CPU.keyboard[0x9] = KEY_UNPRESSED; break;}
                    case SDLK_a: { CPU.keyboard[0xa] = KEY_UNPRESSED; break;}
                    case SDLK_b: { CPU.keyboard[0xb] = KEY_UNPRESSED; break;}
                    case SDLK_c: { CPU.keyboard[0xc] = KEY_UNPRESSED; break;}
                    case SDLK_d: { CPU.keyboard[0xd] = KEY_UNPRESSED; break;}
                    case SDLK_e: { CPU.keyboard[0xe] = KEY_UNPRESSED; break;}
                    case SDLK_f: { CPU.keyboard[0xf] = KEY_UNPRESSED; break;}
                    default: {break;}
                }
                break;

            default:
                break;
        }
    }
    return keep_up;
}
