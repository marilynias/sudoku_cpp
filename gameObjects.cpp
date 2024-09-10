#include "gameObjects.h"
#include "graphics.h"
#include <SDL2/SDL.h>

using namespace std;

TTF_Font *GO_Tile::valFont = nullptr;
SDL_Surface *GO_Tile::poss_Surface = nullptr;
SDL_Rect GO_Tile::poss_rects[9] = {0};
SDL_Renderer *arenderer = nullptr;



GO_Tile::GO_Tile(int index, int value)
{
    tile = new Tile(index, value);
    rect = {index % 9 * TILE_SIZE + index % 9, index / 9 * TILE_SIZE + index / 9, TILE_SIZE, TILE_SIZE};
    generate_image();
    // surface = SDL_CreateRGBSurface(0, TILE_SIZE, TILE_SIZE, 32, 0, 0, 0, 255);
}

GO_Tile::GO_Tile(Tile *tile){
    
    this->tile = tile;
    index = &tile->index;
    value = &tile->value;
    possibleVals = &tile->possibleVals;
    //                      general position           1px border        block border
    rect = {border_width + (*index % 9 * TILE_SIZE) + (*index % 9) + (*index % 9 / 3 * border_width),
            border_width + (*index / 9 * TILE_SIZE) + (*index / 9) + (*index / 9 % 9/3 * border_width), 
            TILE_SIZE, TILE_SIZE};
    generate_image();

}

GO_Tile::~GO_Tile()
{
    // SDL_DestroyTexture(texture);

    // SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    // SDL_FreeSurface(poss_Surface);
    // TTF_CloseFont(GO_Tile::valFont);
}

void GO_Tile::generate_image(){
    SDL_Surface *surface = SDL_CreateRGBSurface(0, TILE_SIZE, TILE_SIZE, 32, rmask, gmask, bmask, amask);
    if (SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 250, 250, 250, 255)) != 0)
    {
        cout << SDL_GetError() << endl;
    }
    // if(*value == ZERO) {
        for (size_t i = 0; i < possibleVals->size(); i++)
        {
            if (possibleVals->test(i))
            {
                if (SDL_BlitSurface(poss_Surface, &poss_rects[i], surface, &poss_rects[i]) != 0)
                {
                    std::cout << SDL_GetError() << std::endl;
                    exit(1);
                }
            }
        }
    // } 
    // else{
        SDL_Surface *txt = TTF_RenderText_Solid(valFont, tile->getStr().c_str(), SDL_Color{0, 0, 0, 255});
        if(txt == NULL){
            cout << SDL_GetError() << endl;
        }
        if (SDL_BlitSurface(txt, NULL, surface, NULL) != 0)
        {
            cout << SDL_GetError() << endl;
        }
        SDL_FreeSurface(txt);
    // }
    
    texture = SDL_CreateTextureFromSurface(arenderer, surface);
    SDL_FreeSurface(surface);
};

void GO_Tile::draw(SDL_Renderer *renderer)
{

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);

    
    if(SDL_SetRenderTarget(renderer, NULL) != 0){
        cout << SDL_GetError() << endl;
    }
    
    if(SDL_RenderCopy(renderer, texture, NULL, &rect) != 0){
        cout << SDL_GetError() << endl;
    }
}

// void GO_Tile::update_value(int val)
// {
//     Tile::update_value(val);
//     generate_image();
//     // Tile::update_value(val);
// }

void GO_Tile::init_font(SDL_Renderer *renderer)
{
    arenderer=renderer;
    TTF_Font *possFont = TTF_OpenFont("Padauk-Regular.ttf", TILE_SIZE / 4);
    SDL_assert(possFont != nullptr);
    poss_Surface = SDL_CreateRGBSurface(0, TILE_SIZE, TILE_SIZE, 32, rmask, gmask, bmask, amask);
    // SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 255, 0, 255, 255));
    if (poss_Surface == nullptr)
    {
        std::cout << SDL_GetError() << std::endl;
    }
    
    SDL_assert(possFont != nullptr);
    for (int i = 0; i < 9; i++)
    {
        SDL_Color c = SDL_Color{0, 0, 0, 255};
        const char *s = to_string(i + 1).c_str();
        SDL_Surface *txt = TTF_RenderText_Solid(possFont, s, c);
        poss_rects[i] = SDL_Rect{i % 3 * TILE_SIZE / 3 +5,
                                          i / 3 * TILE_SIZE / 3,
                                          txt->clip_rect.w, txt->clip_rect.h};

        SDL_BlitSurface(txt, NULL, poss_Surface, &poss_rects[i]);
        SDL_FreeSurface(txt);
    }
    // poss_Surface = SDL_CreateTextureFromSurface(renderer, surface);
    // if(poss_Surface == nullptr){
    //     std::cout << SDL_GetError() << std::endl;
    // }
    TTF_CloseFont(possFont);
}

GO_Sudoku::GO_Sudoku(string sud_str)
{
    sud = new Sudoku(sud_str);
    rect = SDL_Rect{topleft_x, topleft_y, bottomright_x, bottomright_y};
    for (Tile *tile : sud->tiles)
    {
        go_tiles[tile->index] = new GO_Tile(tile);
    }
    
}

GO_Sudoku::~GO_Sudoku()
{
    for (GO_Tile *tile : go_tiles)
    {
        delete tile;
    }
    delete sud;
}

void GO_Sudoku::update_tiles()
{
    for (GO_Tile *tile : go_tiles)
    {
        tile-> generate_image();
    }
}

bool GO_Sudoku::step()
{
    if (sud->check_win())
    {
        this->won=true;
        sud->print_board();
        return true;
    }
    if (sud->remove_existing_numbers()){
        update_tiles();
        return true;
    } else if (sud->solve_singles()){
        update_tiles();
        return true;
    } else if (sud->solve_hidden_singles()){
        update_tiles();
        return true;
    } else if (sud->remove_naked_subset()){
        update_tiles();
        return true;
    }
    return false;
}

void GO_Sudoku::draw(SDL_Renderer *renderer){
    for (GO_Tile *tile : go_tiles)
    {
        tile->draw(renderer);
    }
    // if (won)
    // {
    //     SDL_SetRenderTarget(renderer, NULL);
    //     SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    //     SDL_RenderFillRect(renderer, &rect);
    // }
}