/**
 * @file display.c
 * @author Xavier Monard
 * @brief 
 * @version 0.1
 * @date 2023-06-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "include/display.h"
#include <stdio.h>

/* Global variables */
pixel screen[SCREEN_WIDTH][SCREEN_HEIGTH];
SDL_Texture *sdl_texture[2];
SDL_Window * sdl_window;
SDL_Renderer * sdl_renderer;
SDL_Event sdl_event;


/// @brief Initialize all the pixel of the screen set at black value.
void initialize_screen(){
    for (uint8_t x = 0; x < SCREEN_WIDTH; x++){
        for (uint8_t y = 0; y < SCREEN_HEIGTH; y++){
            screen[x][y].position.x = x * PIXEL_SIZE;
            screen[x][y].position.y = y * PIXEL_SIZE;
            screen[x][y].position.h = PIXEL_SIZE;
            screen[x][y].position.w = PIXEL_SIZE;
            screen[x][y].couleur = PIXEL_BLACK;
        }
    }
}

/* Load the texture to the buffer of one pixel. */
void draw_pixel(pixel pixel){
    SDL_RenderCopy(sdl_renderer, sdl_texture[pixel.couleur], NULL, &pixel.position);
}

/* Draw the screen in the buffer and update it to the renderer. */
void update_screen(){
    for (uint8_t x = 0; x < SCREEN_WIDTH; x++){
        for (uint8_t y = 0; y < SCREEN_HEIGTH; y++){
            draw_pixel(screen[x][y]);
        }
    }
    SDL_RenderPresent(sdl_renderer);
}

/* Set the value of the whole screen to black. */
void clear_screen(){
     for (uint8_t x = 0; x < SCREEN_WIDTH; x++){
        for (uint8_t y = 0; y < SCREEN_HEIGTH; y++){
            screen[x][y].couleur = PIXEL_BLACK;
        }
    }
}

/* Initialize SDL screen, renderer and the SDL textures. */
void initialize_sdl(){
    sdl_window = NULL;
    sdl_renderer = NULL;

    // Screen creation 
    sdl_window = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);

    if (sdl_window == NULL || sdl_renderer == NULL){
        fprintf(stderr, "SDL initialization has failed. See line %d :\n %s", __LINE__ , SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Texture creation
    SDL_Surface *sdl_surface[2];
    sdl_surface[0] = SDL_CreateRGBSurface(SDL_SWSURFACE, PIXEL_SIZE, PIXEL_SIZE, 32, 0, 0, 0, 0);
    sdl_surface[1] = SDL_CreateRGBSurface(SDL_SWSURFACE, PIXEL_SIZE, PIXEL_SIZE, 32, 0, 0, 0, 0);
    
    if (sdl_surface[0] == NULL || sdl_surface[1] == NULL){
        fprintf(stderr, "Error in texture creation. See line %d :\n %s", __LINE__, SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_FillRect(sdl_surface[0], NULL, SDL_MapRGB(sdl_surface[0]->format, 0x00, 0x00, 0x00));
    SDL_FillRect(sdl_surface[1], NULL, SDL_MapRGB(sdl_surface[1]->format, 0xFF, 0xFF, 0xFF));

    sdl_texture[0] = SDL_CreateTextureFromSurface(sdl_renderer, sdl_surface[0]);
    sdl_texture[1] = SDL_CreateTextureFromSurface(sdl_renderer, sdl_surface[1]);

    if (sdl_texture[0] == NULL || sdl_texture[1] == NULL)
    {
        fprintf(stderr, "Error in texture creation. See line %d :\n %s", __LINE__, SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Surface freeing
    SDL_FreeSurface(sdl_surface[0]);
    SDL_FreeSurface(sdl_surface[1]);

    SDL_RenderClear(sdl_renderer);
}
