

#ifndef DEFS_H
#define DEFS_H


#include <vector>
#include <bitset>
#include <array>
#include <bitset>
#include <iostream>

#include <string>
#include <memory>
#include <stdexcept>

#include <boost/array.hpp>
// #include "classes.h"

template <typename... Args>
std::string string_format(const std::string &format, Args... args)
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
    if (size_s <= 0)
    {
        throw std::runtime_error("Error during formatting.");
    }
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}



using namespace std;

class Tile;
// #define assertm(exp, msg) assert(((void)msg, exp));

inline const int setsize = 9;
inline const int total_size = setsize * setsize;

typedef std::vector<Tile *> tileGrp;
typedef boost::array<Tile *, total_size> tList;
typedef boost::array<Tile *, 20> visList;
// typedef array<Tile *, 9> tSet;


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

template <class T>
bool contains(vector<T> iterable, T tofind) { return find(iterable.begin(), iterable.end(), tofind) != iterable.end(); }

template <class T, size_t N>
bool contains(array<T, N> iterable, T tofind) { return find(iterable.begin(), iterable.end(), tofind) != iterable.end(); }
// void replaceAll(std::string &str, const std::string &from, const std::string &to);

bool value_isValid(int val);


#endif