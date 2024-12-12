#include "functions.h"

bool collide_rect(SDL_Rect r1, SDL_Rect r2)
{
    return r1.x < r2.x + r2.w &&
           r1.x + r1.w > r2.x &&
           r1.y < r2.y + r2.h &&
           r1.y + r1.h > r2.y;
}

bool collide_point(SDL_Point p, SDL_Rect r)
{
    return r.x < p.x &&
           r.x + r.w > p.x &&
           r.y < p.y &&
           r.y + r.h > p.y;
}

void setRenderColor(SDL_Renderer* renderer, SDL_Color color)
{
    if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) != 0)
        throw std::string("SetRenderDrawColor: ") + SDL_GetError();
}

void SetRenderTarget(SDL_Renderer *renderer, SDL_Texture *texture)
{
    if (SDL_SetRenderTarget(renderer, texture) != 0)
        throw std::string("SetRenderTarget: ") + SDL_GetError();
}

void RenderFillRect(SDL_Renderer *renderer, SDL_Rect *rect)
{
    if (SDL_RenderFillRect(renderer, rect) != 0)
        throw std::string("RenderFillRect: ") + SDL_GetError();
}

void RenderCopy(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Rect *srcrect, const SDL_Rect *dstrect)
{
    if(SDL_RenderCopy(renderer, texture, srcrect, dstrect) != 0)
        throw std::string("RenderCopy: ") + SDL_GetError();
}

    void
    BlitSurface(SDL_Surface *surf, const SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect)
{
    if (SDL_BlitSurface(surf,srcrect,dst,dstrect) != 0)
        throw std::string("BlitSurface: ") + SDL_GetError();
}

SDL_Point center(SDL_Rect r)
{
    return {r.x + r.w/2, r.y + r.h/2};
}