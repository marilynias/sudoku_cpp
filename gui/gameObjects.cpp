#include "gameObjects.h"
#include "graphics.h"
#include <SDL2/SDL.h>
#include "functions.h"

using namespace std;

TTF_Font *GO_Tile::valFont = nullptr;
SDL_Surface *GO_Tile::poss_Surface = nullptr;
SDL_Rect GO_Tile::poss_rects[9] = {0};
SDL_Renderer *arenderer = nullptr;

Entity::Entity(int x, int y, int w, int h, string title) : rect({x, y, w, h}), title(title) 
{
    rect = {x, y, w, h};
    title = title;
    borderRect = SDL_Rect{rect.x - 1, rect.y - 1, rect.w + 2, rect.h + 2};
}
//                                general position           1px border        block border
GO_Tile::GO_Tile(Tile *tile) : Entity(BORDER_WIDTH + (tile->index % 9 * TILE_SIZE) + (tile->index % 9) + (tile->index % 9 / 3 * BORDER_WIDTH),
                                      BORDER_WIDTH + (tile->index / 9 * TILE_SIZE) + (tile->index / 9) + (tile->index / 9 % 9 / 3 * BORDER_WIDTH),
                                      TILE_SIZE, TILE_SIZE, "")
{

    this->tile = tile;
    value = tile->value;
    possibleVals = tile->possibleVals;
    if (tile->value != ZERO)
        lock();
    
    generate_image();
}

GO_Tile::~GO_Tile()
{
    SDL_DestroyTexture(texture);
}

void GO_Tile::draw_value(SDL_Surface *surface)
{
    title = *tile;
    // value
    SDL_Surface *txt = TTF_RenderText_Solid(valFont, title.c_str(), SDL_Color{0, 0, 0, 255});
    SDL_Rect valRect = SDL_Rect{
        TILE_SIZE / 2 - txt->w / 2, 
        TILE_SIZE / 2 - txt->h / 2, 
        txt->w, txt->h
    };
    
    if (!txt)
        throw std::string("Tile Generate TTF_RenderText_Solid: ") + SDL_GetError();

    if(value != tile->value && tile->value != ZERO){
        SDL_FillRect(surface, &valRect, SDL_MapRGBA(surface->format, 0, 255, 0, 100));
    }

    BlitSurface(txt, NULL, surface, &valRect);
    SDL_FreeSurface(txt);
}

void GO_Tile::draw_index(SDL_Surface *surface)
{
    // DEBUG index
    TTF_Font *possFont = TTF_OpenFont("Padauk-Regular.ttf", TILE_SIZE / 5);

    SDL_Surface *txt = TTF_RenderText_Solid(possFont, to_string(tile->getIndex()).c_str(), SDL_Color{0, 0, 255, 200});
    SDL_Rect indRect = SDL_Rect{TILE_SIZE - 10, 0 - 4, txt->w, txt->h};
    if (txt == NULL)
        throw std::string("Tile Generate TTF_RenderText_Solid: ") + SDL_GetError();
    BlitSurface(txt, NULL, surface, &indRect);

    SDL_FreeSurface(txt);
    TTF_CloseFont(possFont);
}

void GO_Tile::set_bg_color(SDL_Renderer *renderer)
{
    if (selected)
        setRenderColor(renderer, selectedColor);
    else if (locked)
        setRenderColor(renderer, lockedColor);
    else
        setRenderColor(renderer, bgColor);
}


void GO_Tile::draw_possible(SDL_Surface *surface)
{
    // bitset<9> diff = possibleVals ^ tile->possibleVals;
    // possible values
    for (size_t i = 0; i < possibleVals.size(); i++)
    {
        if (!possibleVals.test(i))
            continue;

        // if(diff.test(i) && tile->value == ZERO)
        // {
        //     SDL_FillRect(surface, &poss_rects[i], SDL_MapRGBA(poss_Surface->format, 255, 0, 0, 100));
        // }
        
        BlitSurface(poss_Surface, &poss_rects[i], surface, &poss_rects[i]);

        
    }
}

void GO_Tile::generate_image(){
    SDL_Surface *surface = SDL_CreateRGBSurface(0, TILE_SIZE, TILE_SIZE, 32, rmask, gmask, bmask, amask);
    if (SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 250, 250, 250, 0)) != 0)
        cout << SDL_GetError() << endl;

    if (tile->value != ZERO)
        draw_value(surface);
        
    else
        draw_possible(surface);

    draw_index(surface);

    

    texture = SDL_CreateTextureFromSurface(arenderer, surface);
    if (texture==nullptr)
        throw std::string("Tile Generate CreateTextureFromSurface: ") + SDL_GetError();

    SDL_FreeSurface(surface);
        
}

void GO_Tile::draw(SDL_Renderer *renderer)
{
    SetRenderTarget(renderer, NULL);

    set_bg_color(renderer);

    
    
    RenderFillRect(renderer, &rect) ;
    
    if(SDL_RenderCopy(renderer, texture, NULL, &rect) != 0) throw std::string("RenderCopy: ") + SDL_GetError();

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
    if(possFont == nullptr) 
        throw std::string("TTF_OpenFont: ") + SDL_GetError();

    poss_Surface = SDL_CreateRGBSurface(0, TILE_SIZE, TILE_SIZE, 32, rmask, gmask, bmask, amask);
    if (poss_Surface == nullptr)
        throw std::string("Poss surface: ") + SDL_GetError();

    for (int i = 0; i < 9; i++)
    {
        SDL_Color c = black;
        const char *s = to_string(i + 1).c_str();
        SDL_Surface *txt = TTF_RenderText_Solid(possFont, s, c);
        poss_rects[i] = SDL_Rect{i % 3 * TILE_SIZE / 3 +5,
                                          i / 3 * TILE_SIZE / 3,
                                          txt->clip_rect.w, txt->clip_rect.h};

        BlitSurface(txt, NULL, poss_Surface, &poss_rects[i]);
        SDL_FreeSurface(txt);
    }
    TTF_CloseFont(possFont);
}

void GO_Tile::handleEvent(SDL_MouseButtonEvent e)
{
    if(e.button == SDL_BUTTON_LEFT)
    {
        printf("Click on tile %i, selected is %i\n", tile->index, !selected);
        selected = !selected;
    }
}

void GO_Tile::handleEvent(SDL_MouseMotionEvent e)
{

}

void GO_Tile::set_value(int val)
{
    if (value_isValid(val) != locked)
    {
        if (val==ZERO)
            tile->possibleVals.set();
        else
            tile->possibleVals.reset();
        tile->value = val;
        generate_image();
    }
}

void GO_Tile::apply()
{
    
    value = tile->value;
    possibleVals = tile->possibleVals;
    generate_image();
    
}

GO_Sudoku::GO_Sudoku(string sud_str)
{
    draw_sol_overlay = false;
    sud = new Sudoku(sud_str);
    sud->init_sudoku(sud->getStr());

    rect = SDL_Rect{topleft_x, topleft_y, bottomright_x, bottomright_y};
    // TODO: overlay for changed tiles
    overlay = SDL_CreateTexture(arenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);
    for (Tile *tile : sud->tiles)
        go_tiles[tile->index] = new GO_Tile(tile);
    
    
}

void GO_Sudoku::init(string sud_str)
{
    won = next = false;
    sud->init_sudoku(sud_str);
    for (Tile *tile : sud->tiles)
    {
        if (go_tiles[tile->index] == nullptr)
            go_tiles[tile->index] = new GO_Tile(tile);
        else
            go_tiles[tile->index]->generate_image();
    }
}

GO_Sudoku::~GO_Sudoku()
{
    for (GO_Tile *tile : go_tiles)delete tile;
    
    delete sud;
}

void GO_Sudoku::update_tiles()
{
    for (GO_Tile *tile : go_tiles) tile-> generate_image();
}

bool GO_Sudoku::step()
{
    if (changed())
    {
        apply();
        return true;
    }

    if (sud->remove_existing_numbers())
    {
        cout << "removed existing numbers" << endl;
        update_tiles();
        return true;
    }
    else if (sud->solve_singles())
    {
        cout << "solved singles" << endl;
        update_tiles();
        return true;
    }
    else if (sud->solve_hidden_singles())
    {
        cout << "solved hidden singles" << endl;
        update_tiles();
        return true;
    }
    else if (sud->remove_naked_subset())
    {
        cout << "found a naked subset" << endl;
        update_tiles();
        return true;
    }
    else if (sud->remove_hidden_subset(2))
    {
        cout << "found a hidden pair" << endl;
        update_tiles();
        return true;
    }
    else if (sud->remove_hidden_subset(3))
    {
        cout << "found a hidden triple" << endl;
        update_tiles();
        return true;
    }
    else if (sud->remove_pointing_subset())
    {
        cout << "found a pointing pair/triple" << endl;
        update_tiles();
        return true;
    }
    else if (sud->remove_box_line_reduction())
    {
        cout << "found a box line reduction" << endl;
        update_tiles();
        return true;
    }
    else if (sud->remove_xwing())
    {
        cout << "found an xwing" << endl;
        update_tiles();
        return true;
    }
    else if (sud->remove_hidden_subset(4))
    {
        cout << "found a hidden quad" << endl;
        update_tiles();
        return true;
    }
    Chain *chain = sud->simple_coloring();
    if (chain != nullptr)
    {
        cout << "found a simple chain" << endl;
        _draw_chain(*chain);
        delete chain;
        
        update_tiles();
        return true;
        // TODO: Draw chain
    } else {
        this->won = sud->check_win();
        return false;
    }
    return false;
}

void GO_Sudoku::draw(SDL_Renderer *renderer){
    for (GO_Tile *tile : go_tiles)
        tile->draw(renderer);

    if(draw_sol_overlay)
    {
        SetRenderTarget(arenderer, NULL);
        SDL_RenderCopy(arenderer, overlay, NULL, &rect);
    }
    // if (won)
    // {
    //     SetRenderTarget(renderer, NULL);
    //     SetRenderDrawColor(renderer, 0, 255, 0, 255);
    //     SDL_RenderFillRect(renderer, &rect);
    // }
}


bool GO_Sudoku::_draw_chain(Chain chain){
    bool changed = false;
    for (auto tile : chain.contradictions)
    {
        changed |= tile->remove_possible(chain.num);
    }
    return changed;
}

void GO_Sudoku::handleEvent(SDL_MouseButtonEvent e)
{
    if (e.button != SDL_BUTTON_LEFT) return;

    if (e.clicks == 2)
    {
        select_all_event(e);
        return;
    }
    auto col = group_collide_point({e.x, e.y}, go_tiles);
    auto mods = SDL_GetModState();
    if (mods & KMOD_LSHIFT)
    {
        for (GO_Tile *tile : col)
        {
            tile->handleEvent(e);
            sel_last_changed = tile->is_selected();
        }
    } else if (col.size() >= 1) {
        for (GO_Tile *tile : go_tiles)
        {
            if (contains(col, tile))
            {
                tile->handleEvent(e);
                sel_last_changed = tile->is_selected();
                
            }
            else
                tile->deselect();
        }
        
    }
}
void GO_Sudoku::handleEvent(SDL_MouseMotionEvent e)
{
    auto col = group_collide_point({e.x, e.y}, go_tiles);
    for (GO_Tile *tile : col)
    {
        sel_last_changed ? tile->select() : tile->deselect();
    }
}

GO_tileGrp GO_Sudoku::get_selected()
{
    GO_tileGrp selected;
    for (GO_Tile *tile : go_tiles)
    {
        if (tile->is_selected())
            selected.push_back(tile);
    }
    return selected;
}

void GO_Sudoku::set_selected(int val)
{
    if(value_isValid(val))
    {
        for (GO_Tile *tile : go_tiles)
        {
            if (tile->is_selected())
            {
                tile->set_value(val);
            }
        }
    }
    
}

bool GO_Sudoku::changed()
{
    for (GO_Tile *tile : go_tiles)
    {
        if (tile->changed())
            return true;
    }
    return false;
}

void GO_Sudoku::apply()
{
    for (GO_Tile *tile : go_tiles)
    {
        if (tile->changed())
            tile->apply();
    }
}

bool GO_Sudoku::draw_changed(SolveType step)
{
    assert(step && "step is null");
    SDL_Surface *o_surface = SDL_CreateRGBSurface(0, TILE_SIZE, TILE_SIZE, 32, 0, 0, 0, 0);
    // for (GO_Tile *tile : go_tiles)
    // {
    //     if (contains(step.due_to, tile->get_tile()))
    //     {
            
    //         tile->highlight_possible(o_surface, step.num, red);
    //     }
    //     else if (step.tile == tile->get_tile())
    //     {
    //         tile->highlight_possible(o_surface, step.num, green);
    //     }
    // }
    overlay = SDL_CreateTextureFromSurface(arenderer, o_surface);
}

bool GO_Sudoku::remove_naked_subset()
{
    SolveType step = sud->remove_naked_subset();
    if (step != SolveType::none)
        draw_changed(step);
    
    return step;
}

void GO_Sudoku::select_all_event(SDL_MouseButtonEvent e)
{
    auto col = group_collide_point({e.x, e.y}, go_tiles);
    auto mods = SDL_GetModState();
    
    if (col.size() > 0)
    {
        GO_Tile *t = col[0];
        for (GO_Tile *tile : go_tiles)
        {
            if (tile->get_value() == t->get_value())
                tile->select();
            else if (!mods & KMOD_LSHIFT)
                tile->deselect();
        }
    }
}