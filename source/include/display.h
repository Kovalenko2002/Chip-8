#ifndef DISPLAY_H
#define DISPLAY_H

/* Includes */

#include <stdint.h>
#include <SDL2/SDL.h>

/* Macros */

#define PIXEL_SIZE 8
#define SCREEN_WIDTH  64
#define SCREEN_HEIGTH 32
#define WIDTH SCREEN_WIDTH * PIXEL_SIZE
#define HEIGHT SCREEN_HEIGTH * PIXEL_SIZE
#define PIXEL_BLACK 0
#define PIXEL_WHITE 1
#define HEX_REP_SIZE 5

/* Structs */

/**
 * @brief Structure representing a screen pixel
 * 
 * @param position A SDL structure definng x,y,h and w.
 * @param couleur The color of the pixel.
 */
typedef struct {
    SDL_Rect position;
    uint8_t couleur;
} pixel;

/* Globals */

/**
 * @brief Screen of our emulator.
 * 
 */
extern pixel screen[SCREEN_WIDTH][SCREEN_HEIGTH];
extern SDL_Texture *sdl_texture[2];
extern SDL_Window * sdl_window;
extern SDL_Renderer * sdl_renderer;
extern SDL_Event sdl_event;

/* Functions*/

void initialize_screen();
void draw_pixel(pixel pixel);
void update_screen();
void clear_screen();
void initialize_sdl();

#endif /* DISPLAY_H */
