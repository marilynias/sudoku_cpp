#ifndef __CLASSES_H__
#define __CLASSES_H__

#include "defs.h"

class Tile
{
private:
    
    std::array<Tile *, 20> visibleTiles;
    
    void printconnected();
    int get_first_possible(bitset<10> b);

public:
    // std::array<bool, 10> possibleVals;
    std::bitset<9> possibleVals;
    int value;
    int index;
    Tile(int ind, int val);
    Tile():Tile(0,0){};
    void init_tSets(tList TileList);
    ~Tile();
    int getValue()
    {
        return value;
    };
    void assign_possible_tiles();
    virtual void update_value(int val);
    std::string getStr();
    std::string getPossStr();

    bool remove_val_from_visible(int val);
    bool remove_possible(int val);
    std::bitset<9> get_possible_vals() { return possibleVals; }
    
};

struct Sudoku
{
private:
    

public:
    tList tiles;
    std::array<tSet, setsize> rows = {};
    std::array<tSet, setsize> columns = {};
    std::array<tSet, setsize> boxes = {};
    std::array<tGroup *, 3> groups = {&rows, &columns, &boxes};

    Sudoku(std::string sud_str);
    ~Sudoku();

    bool check_win();
    void print_board();
    std::string getStr();
    bool solve();

    bool remove_existing_numbers();
    bool remove_naked_subset();
    bool solve_singles();
    bool solve_hidden_singles();
    
};
#endif // __CLASSES_H__