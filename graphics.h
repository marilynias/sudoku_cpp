#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define TILE_SIZE 64
#define SCREEN_WIDTH width
#define SCREEN_HEIGHT width

#include "classes.h"
#include "gameObjects.h"


extern const int rmask;
extern const int gmask;
extern const int bmask;
extern const int amask;
const inline int border_width = 3;
const inline int width = setsize * TILE_SIZE + setsize + border_width * 3;
const inline int topleft_x = 0;
const inline int topleft_y = 0;
const inline int topright_x = width;
const inline int topright_y = 0;
const inline int bottomleft_x = 0;
const inline int bottomleft_y = width;
const inline int bottomright_x = width;
const inline int bottomright_y = width;


typedef struct
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    GO_Sudoku *sudoku;
} App;

void initSDL(void);

int gameLoop(string sudoku);

void init();

#endif // __GRAPHICS_H__