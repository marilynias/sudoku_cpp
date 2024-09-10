#include "graphics.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
rmask = 0xff000000;
gmask = 0x00ff0000;
bmask = 0x0000ff00;
amask = 0x000000ff;
#else
const int rmask = 0x000000ff;
const int gmask = 0x0000ff00;
const int bmask = 0x00ff0000;
const int amask = 0xff000000;
#endif



App app;

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

void doInput(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            exit(0);
        }

        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_SPACE)
            {
                app.sudoku->step();
            }
        }
    }
}

// void drawBorder(void){

//     SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
//     SDL_RenderDrawRect(app.renderer, );
//     SDL_RenderDrawLine(app.renderer, topleft_x, topleft_y, topright_x, topright_y);
//     SDL_RenderDrawLine(app.renderer, topright_x, topright_y, bottomright_x, bottomright_y);
//     SDL_RenderDrawLine(app.renderer, bottomright_x, bottomright_y, bottomleft_x, bottomleft_y);
//     SDL_RenderDrawLine(app.renderer, bottomleft_x, bottomleft_y, topleft_x, topleft_y);
// }

void prepareScene(void)
{
    if(app.sudoku->won){
        SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
    } else {
        SDL_SetRenderDrawColor(app.renderer, 20, 20, 20, 255);
    }
    
    SDL_RenderClear(app.renderer);
    app.sudoku->draw(app.renderer);
}

void presentScene(void)
{
    SDL_RenderPresent(app.renderer);
}

void cleanup(){
    SDL_DestroyRenderer(app.renderer);
    SDL_Quit();
}


void init_GO(string sudoku){
    
    
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


int gameLoop(string sudoku)
{
    app.sudoku = new GO_Sudoku(sudoku);

    while (true){
        if(app.sudoku->won){
            delete(app.sudoku);
            break;
        }


        prepareScene();
        
        doInput();
        presentScene();
        SDL_Delay(200);
    }

    return 1;
}