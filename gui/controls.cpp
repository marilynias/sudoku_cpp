
#include "controls.h"
#include <SDL2/SDL.h>
// #include "graphics.h"


bool App::doInput()
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
                if (!sudoku->step())
                {
                    sudoku->next = true;
                    sudoku->won = sudoku->sud->check_win();
                };
                return true;
            }
            else if (event.key.keysym.sym == SDLK_RETURN)
            {
                sudoku->won = true;
            } else {
                int x = event.key.keysym.sym - '0';
                if (value_isValid(x))
                {
                    sudoku->set_selected(x);
                    return true;
                }
            }
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            
            mb_held |= event.button.button;
            sudoku->handleEvent(event.button);
        }

        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            mb_held &= ~event.button.button;
        }

        else if (mb_held & SDL_BUTTON_LEFT && event.type == SDL_MOUSEMOTION)
        {
            sudoku->handleEvent(event.motion);
        }
    }
    return false;
}
