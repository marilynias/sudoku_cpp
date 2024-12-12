#ifndef __CLASSES_H__
#define __CLASSES_H__

#include "defs.h"
#include <map>
#include <boost/smart_ptr.hpp>
// #include <boost/algorithm/cxx11/any_of.hpp>
// #include <boost/algorithm/cxx11/all_of.hpp>

struct tSet;
typedef array<tSet, setsize> tGroup;
const vector<char> e_char = {'.'};

enum SetType
{
    row = 0,
    column = 1,
    box = 2
};

enum SolveType
{
    none,
    remove_existing_numbers,
    remove_naked_subset,
    remove_hidden_subset,
    remove_box_line_reduction,
    remove_pointing_subset,
    solve_singles,
    solve_hidden_singles,
    remove_xwing,
    remove_simple_coloring,
    simple_coloring,
};

class SolveStep
{
private:
    tileGrp possToRemove;

public:
    SolveType type;
    SolveStep(SolveType type) : type(type) {}
    void draw();
    void apply();  
};

class Tile
{
private:
    void printconnected();

public:
    operator std::string();
    // std::array<bool, 10> possibleVals;
    std::bitset<9> possibleVals;
    tSet *column;
    tSet *row;
    tSet *box;
    tSet *groups[3];
    int value;
    int index;
    Tile(int ind, int val, tSet *column, tSet *row, tSet *box);
    Tile():Tile(0,0, nullptr, nullptr, nullptr){};
    void init(int val);
    // void init_tSets(tList TileList);
    ~Tile();
    int getValue() {return value;};
    int getIndex() {return index;};
    void assign_possible_tiles();
    virtual void update_value(int val);
    std::string getStr();
    std::string getPossStr();

    bool remove_val_from_visible(int val);
    bool remove_possible(int val);
    bool has_possible(int val) { return possibleVals.test(val-1); }

    // vector<Tile *> _chain(vector<Tile *>  chain,Tile *tile,int num, bool onoff = true);


};

struct tSet : std::array<Tile *, setsize>
{
    iterator begin() { return std::array<Tile *, setsize>::begin(); };
    bitset<setsize> missingVals = bitset<setsize>();

    inline vector<Tile *> tiles_containing_missing_vals(int num){
        vector<Tile *> v;
        v.reserve(this->size());
        for (Tile *tile : *this){
            if (tile && tile->possibleVals.test(num-1)){
                v.push_back(tile);
            }
        }
        v.shrink_to_fit();
        return v;
    };

    inline bitset<setsize> ind_containing_missing_vals(int num)
    {
        // assert(0<num<=setsize&&"num must be between 1 and 9");
        bitset<setsize> b;
        for (size_t i = 0; i < this->size(); i++)
        {
            b.set(i, this->at(i)->possibleVals.test(num - 1));
        }
        return b;
    };

    tSet() : std::array<Tile *, 9> (){
        missingVals.set();
    };
};

struct invalid
{
    int twice_onoff = 1<<0;
};

struct Chain : map<Tile *, bool>
{
    Chain(Tile *tile, int num);
    // ~Chain();
    int num;
    void _chain(Tile *tile, bool onoff = true);
    vector<Tile*> contradictions;
    bool check_contradiction();
    bool remove_contradictions();
    

private:
    bool twice_onchain();
    bool twice_offchain();
};

struct Sudoku
{
private:
    vector<Tile *> _get_chain(int val);
    string o_sud;
    tGroup rows = {};
    tGroup columns = {};
    tGroup boxes = {};
    std::array<tGroup *, 3> groups = {&rows, &columns, &boxes};
    

public:
    operator std::string();

    tList tiles;
    Sudoku(std::string sud_str);
    ~Sudoku();

    void init_sudoku(std::string sud_str);

    bool check_win();
    void print_board();
    std::string getStr();
    bool solve();
    bool step();

    SolveType remove_existing_numbers();
    SolveType remove_naked_subset(int len = 2);
    SolveType remove_hidden_subset(int len=2);
    SolveType remove_box_line_reduction();
    SolveType remove_pointing_subset();
    SolveType solve_singles();
    SolveType solve_hidden_singles();
    SolveType remove_xwing();
    SolveType remove_simple_coloring();

    Chain *simple_coloring();
};

#endif // __CLASSES_H__