

#ifndef DEFS_H
#define DEFS_H

#include <string>
#include <vector>
#include <bitset>
#include <array>
#include <bitset>
#include <iostream>


using namespace std;
class Tile;
// #define assertm(exp, msg) assert(((void)msg, exp));

inline const int setsize = 9;
inline const int total_size = setsize * setsize;



typedef array<Tile *, total_size> tList;
typedef array<Tile *, 20> visList;
typedef array<Tile *, 9> tSet;
typedef array<tSet, setsize> tGroup;

inline const int EMPTY = 0;

enum numbers
{
    ZERO = 0,
    ONE = 1 << 1,
    TWO = 1 << 2,
    THREE = 1 << 3,
    FOUR = 1 << 4,
    FIVE = 1 << 5,
    SIX = 1 << 6,
    SEVEN = 1 << 7,
    EIGHT = 1 << 8,
    NINE = 1 << 9
};

void replaceAll(std::string &str, const std::string &from, const std::string &to);

bool value_isValid(int val);

template <std::size_t size>
bool contains_index(array<Tile *, size> v, int index)
{
    for (Tile *t : v)
    {
        if (t->index == index)
        {
            return true;
        }
    }
    return false;
}

template <std::size_t size>
bool lst_contains_value(array<Tile *, size> v, int value)
{
    for (Tile *t : v)
    {
        if (t && t->getValue() == value)
        {
            return true;
        }
    }
    return false;
}

#endif