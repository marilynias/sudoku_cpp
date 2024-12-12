#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define TILE_SIZE 64
#define SCREEN_WIDTH width
#define SCREEN_HEIGHT width
#define BORDER_WIDTH 3

#include "../include/classes.h"
#include "gameObjects.h"

extern const int rmask;
extern const int gmask;
extern const int bmask;
extern const int amask;

const inline int width = setsize * TILE_SIZE + setsize + BORDER_WIDTH * 4;
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
    bool doInput();
    int mb_held;
} App;



void initSDL(void);

int solve_all(string sudoku);
int mainLoop(string sudoku);

void init();

void cleanup();

#endif // __GRAPHICS_H__