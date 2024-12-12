#include "graphics.h"
#include "controls.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
const int rmask = 0xff000000;
const int gmask = 0x00ff0000;
const int bmask = 0x0000ff00;
const int amask = 0x000000ff;
#else
const int rmask = 0x000000ff;
const int gmask = 0x0000ff00;
const int bmask = 0x00ff0000;
const int amask = 0xff000000;
#endif

static App app;

void initSDL(void)
{
    int rendererFlags, windowFlags;
    
    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    app.window = SDL_CreateWindow("Sudoku", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

    if (!app.window)
    {
        printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

    if (!app.renderer)
    {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() != 0)
    {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        SDL_Quit();
        exit(1);
    }
}


void prepareScene(void)
{
    if(app.sudoku->won)
        SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
    else 
        SDL_SetRenderDrawColor(app.renderer, 20, 20, 20, 255);
    
    
    SDL_RenderClear(app.renderer);
    app.sudoku->draw(app.renderer);
}

void presentScene(void)
{
    SDL_RenderPresent(app.renderer);
}

void cleanup(){
    delete app.sudoku;
    TTF_CloseFont(GO_Tile::valFont);
    SDL_FreeSurface(GO_Tile::poss_Surface);
    TTF_Quit();
    SDL_DestroyRenderer(app.renderer);
    SDL_Quit();
    cout << "Goodbye!" << endl;
}


void init(){
    memset(&app, 0, sizeof(App));
    initSDL();
    atexit(cleanup);

    GO_Tile::init_font(app.renderer);
    TTF_Font *possFont = TTF_OpenFont("Padauk-Regular.ttf", TILE_SIZE / 5);
    GO_Tile::valFont = TTF_OpenFont("Padauk-Regular.ttf", TILE_SIZE);
    SDL_assert(GO_Tile::valFont != NULL && possFont != NULL);
}

int mainLoop(string sudoku)
{
    if (app.sudoku != nullptr)
        delete app.sudoku;
    app.sudoku = new GO_Sudoku(sudoku);

    while (true)
    {
        if (app.sudoku->next)
        {
            app.sudoku->step();
            app.sudoku->next = false;
            app.sudoku->won = app.sudoku->sud->check_win();
            break;
        };

        app.doInput();
        prepareScene();
        presentScene();
        SDL_Delay(20);
    }
    // app.sudoku->sud->print_board();
    return app.sudoku->won;
}

int solve_all(string sudoku)
{
    if(app.sudoku != nullptr) delete app.sudoku;
    app.sudoku = new GO_Sudoku(sudoku);
    

    while (!app.sudoku->next)
    {
        if(!app.sudoku->step()){
            app.sudoku->next = true;
            app.sudoku->won = app.sudoku->sud->check_win();
        };

        prepareScene();
        
        app.doInput();
        presentScene();
        SDL_Delay(20);
    }
    // app.sudoku->sud->print_board();
    return app.sudoku->won;
}
