#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__
#include <SDL2/SDL.h>
#include <vector>
#include "../include/defs.h"
#include "gameObjects.h"

bool collide_point(SDL_Point p1, SDL_Rect r1);

bool collide_rect(SDL_Rect r1, SDL_Rect r2);

void setRenderColor(SDL_Renderer* renderer, SDL_Color color);

void SetRenderTarget(SDL_Renderer *renderer, SDL_Texture *texture);

void BlitSurface(SDL_Surface *surf, const SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect);

void RenderFillRect(SDL_Renderer *renderer, SDL_Rect *rect);

void RenderCopy(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Rect *srcrect, const SDL_Rect *dstrect);

template <class T> std::vector<T> group_collide_point(SDL_Point p1, vector<T> grp)
{
vector<T> ret;
for (T obj : grp) 
{
    if (collide_point(p1, obj->rect))
        ret.push_back(obj);
}
return ret;
};

template <class T, size_t s>
std::vector<T> group_collide_point(SDL_Point p1, array<T, s> grp)
{
    vector<T> ret;
    for (T obj : grp)
    {
        if (collide_point(p1, obj->rect))
            ret.push_back(obj);
    }
    return ret;
};

SDL_Point center(SDL_Rect r);

#endif // __FUNCTIONS_H__