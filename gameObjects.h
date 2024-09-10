#ifndef __GAMEOBJECTS_H__
#define __GAMEOBJECTS_H__


#include "classes.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class GO_Tile;

class GO_Tile
{
private:
    Tile *tile;
    int *index;
    int *value;
    std::bitset<setsize> *possibleVals;
    SDL_Rect rect;
    // SDL_Surface *surface;
    static SDL_Rect poss_rects[setsize];
    SDL_Texture *texture;
    TTF_Font *possFont; 
    
    

public : 
    
    GO_Tile(int index, int value);
    GO_Tile(Tile *tile);
    // GO_Tile():GO_Tile(0,0){};
    ~GO_Tile();
    void draw(SDL_Renderer *renderer);
    void generate_image();

    static void init_font(SDL_Renderer *renderer);
    static TTF_Font *valFont;
    static SDL_Surface *poss_Surface;
    // static GO_Tile *fromTile(Tile *tile){
    //     GO_Tile *go_tile = new GO_Tile(tile);
    //     go_tile->tile = tile;
    //     return go_tile;
    // };
};

typedef array<GO_Tile *, total_size> GO_tList;
typedef array<GO_Tile *, 20> GO_visList;
typedef array<GO_Tile *, 9> GO_tSet;
typedef array<GO_tSet, setsize> GO_tGroup;

class GO_Sudoku
{
public:
    SDL_Rect rect;
    void draw(SDL_Renderer *renderer);
    bool won = false;
    GO_Sudoku(string sud_str);
    ~GO_Sudoku();
    Sudoku *sud;
    array<GO_Tile*, total_size> go_tiles;
    bool step();
    void update_tiles();
};

#endif // __GAMEOBJECTS_H__