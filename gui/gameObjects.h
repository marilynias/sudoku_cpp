#ifndef __GAMEOBJECTS_H__
#define __GAMEOBJECTS_H__

#include "../include/classes.h"
// #include "controls.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

inline const SDL_Color black = {10, 10, 10, 255};
inline const SDL_Color grey = {200, 200, 200, 255};
inline const SDL_Color white = {250, 250, 250, 255};
inline const SDL_Color red = {250, 10, 10, 255};
inline const SDL_Color green = {10, 250, 10, 255};
inline const SDL_Color blue = {10, 10, 250, 255};

// SDL_Color bgColor = {250, 250, 250, 255};
// SDL_Color sucsessColor = {10, 10, 250, 255};
// SDL_Color borderColor = {250, 10, 10, 255};

class Entity
{
public:
    Entity(int x, int y, int w, int h, string title="") ;
    virtual ~Entity(){};
    virtual void draw(SDL_Renderer *renderer) = 0;
    bool is_selected() { return selected; };
    void select() {selected = true;};
    void deselect(){selected = false;};
    void lock(){locked=true;};
    void unlock() { locked = false; };
    SDL_Rect rect;
    SDL_Rect borderRect;
    string title;
protected:
    bool selected = false;
    SDL_Color bgColor = white;
    SDL_Color selectedColor = green;
    SDL_Color borderColor = red;
    SDL_Color lockedColor = grey;
    SDL_Texture *texture;
    bool locked = false;
};

class GO_Tile : public Entity
{
private:
    Tile *tile;
    int value;
    std::bitset<setsize> possibleVals;
    // SDL_Surface *surface;
    static SDL_Rect poss_rects[setsize];

    void draw_value(SDL_Surface *surface);
    void draw_possible(SDL_Surface *surface);

    void draw_index(SDL_Surface *surface);
    void set_bg_color(SDL_Renderer *renderer);

public : 
    
    // GO_Tile(int index, int value);
    GO_Tile(Tile *tile);
    // GO_Tile():GO_Tile(0,0){};
    ~GO_Tile();
    
    void draw(SDL_Renderer *renderer);
    void generate_image();
    void handleEvent(SDL_MouseButtonEvent event);
    void handleEvent(SDL_MouseMotionEvent event);
    // bitset<9> get_changed_possibleVals(){return possibleVals ^ tile->possibleVals;};

    bool highlight_possible(SDL_Surface *draw_to, int num, SDL_Color color);

    void set_value(int val);
    int get_value() { return value; };
    Tile *get_tile() { return tile; };

    void apply();

    static void init_font(SDL_Renderer *renderer);
    static TTF_Font *valFont;
    static SDL_Surface *poss_Surface;

    bool changed(){return value != tile->getValue() || possibleVals != tile->possibleVals;};
};

typedef array<GO_Tile *, total_size> GO_tList;
typedef array<GO_Tile *, 20> GO_visList;
typedef array<GO_Tile *, 9> GO_tSet;
typedef array<GO_tSet, setsize> GO_tGroup;
typedef vector<GO_Tile *> GO_tileGrp;

class GO_Sudoku
{
public:
    
    void draw(SDL_Renderer *renderer);
    bool won = false;
    bool next = false;
    GO_Sudoku(string sud_str);
    ~GO_Sudoku();
    void init(string sud_str);
    Sudoku *sud;
    array<GO_Tile*, total_size> go_tiles;
    bool step();
    void update_tiles();
    void handleEvent(SDL_MouseButtonEvent event);
    void handleEvent(SDL_MouseMotionEvent event);
    void set_selected(int val);

private:
    SDL_Rect rect;
    SDL_Texture *overlay;
    bool sel_last_changed;
    bool changed();
    void apply();
    bool draw_sol_overlay;
    bool draw_changed(SolveType step);

    bool remove_existing_numbers();
    bool solve_singles();
    bool solve_hidden_singles();
    bool remove_naked_subset();
    bool remove_hidden_subset(int len=2);
    bool remove_box_line_reduction();
    bool remove_pointing_subset();
    bool remove_xwing();
    bool simple_coloring();

    bool _draw_chain(Chain chain);
    GO_tileGrp get_selected();
    
    void select_all_event(SDL_MouseButtonEvent e);
};

#endif // __GAMEOBJECTS_H__