#include "classes.h"
#include "defs.h"
#include <cassert>

Tile::Tile(int ind, int val)
{

    index = ind;
    value = val;
    if (val==ZERO){
        possibleVals.set();
    } else {
        possibleVals.reset();
    }
    

    assert(value_isValid(value) && "Invalid value: " + value);
}

Tile::~Tile()
{
}

bool Tile::remove_possible(int val)
{

    bool changed = possibleVals.test(val-1);
    possibleVals.set(val-1, false);
    return changed;
}

bool Tile::remove_val_from_visible(int val)
{
    bool changed = false;
    if (getValue() != ZERO)
    {
        for (Tile *tile : visibleTiles)
        {
            if (tile == nullptr){
                continue;
            }
            changed |= tile->remove_possible(getValue());
        }
    }
    return changed;
}

void Tile::printconnected()
{
    for (size_t i = 0; i < 9; i++)
    {
        if (i % 3 == 0 and i != 0)
        {
            std::cout << "\n---------------\n";
        }
        else
        {
            std::cout << "\n";
        }

        for (size_t j = 0; j < 9; j++)
        {
            if (j % 3 == 0 and j != 0)
            {
                std::cout << " | ";
            }

            auto t = contains_index(visibleTiles, i * 9 + j) ? "X" : " ";
            std::cout << t;
        }
    }
}

void Tile::init_tSets(tList TileList)
{
    /* init visible tiles
    0 | 1 | 2  || 3 | 4 | 5  || 6 | 7 | 8
    9 | 10| 11 || 12| 13| 14 || 15| 16| 17
    18| 19| 20 || 21| 22| 23 || 24| 25| 26
    --------------------------------------
    27| 28| 29 || 30| 31| 32 || 33| 34| 35
    36| 37| 38 || 39| 40| 41 || 42| 43| 44
    45| 46| 47 || 48| 49| 50 || 51| 52| 53
    --------------------------------------
    54| 55| 56 || 57| 58| 59 || 60| 61| 62
    63| 64| 65 || 66| 67| 68 || 69| 70| 71
    72| 73| 74 || 75| 76| 77 || 78| 79| 80

    */
    size_t i = 0;
    for (Tile *tile : TileList)
    {
        if (tile->index == index)
        {
            continue;
        } // ignore self
        else if (tile->index % setsize == index % setsize || // rows
                 tile->index / setsize == index / setsize || // columns
                 tile->index / 27 * 3 + tile->index % 9 / 3 == index / 27 * 3 + index % 9 / 3)
        {
            visibleTiles[i] = tile;
            i++;
        }
    }
    // TileList.
    // std::cout << TileList.size() << ": " <<  visibleTiles.size() << endl;
    // printconnected(visibleTiles, TileList);
}



void Tile::assign_possible_tiles()
{
    if (getValue() != ZERO)
    {
        possibleVals.reset();
        return;
    }
    // possibleVals&= 0x1fd;
    for (size_t i = 1; i < 10; i++)
    {
        // unset bit if its in visible tiles.
        possibleVals.set(i-1, !lst_contains_value(visibleTiles, i) && possibleVals.test(i-1));
    }
    // cout << (possibleVals >> 1) << endl;
}

void Tile::update_value(int val)
{
    assert(value_isValid(val) && "Invalid value: " + val);
    value = val;
    possibleVals.reset();
    for (Tile *t : visibleTiles)
    {
        if (t)
        {
            t->remove_possible(val);
        }
    }
}


string Tile::getStr()
{
    return value == EMPTY ? " " : to_string(value);
}

string Tile::getPossStr()
{
    string s = to_string(getValue()) + ": ";
    for (size_t i = 0; i < possibleVals.size(); i++)
    {
        s.append(possibleVals.test(i) ? to_string(i+1) : " ");
    }

    return s;
}

Sudoku::Sudoku(string sud_str)
{
    // sud_str = "300967001040302080020000070070000090000873000500010003004705100905000207800621004";
    if (sud_str.size() != total_size)
    {
        throw length_error("sudoku not the right length");
    }

    for (string::size_type i = 0; i < sud_str.size(); ++i)
    {
        int num = static_cast<int>(sud_str.at(i) - '0');
        tiles[i] = new Tile(i, num);
    }

    int n = 0;
    for (Tile *tile : tiles)
    {
        columns[tile->index % setsize][tile->index / setsize] = tile;
        rows[tile->index / setsize][tile->index % setsize] = tile;
        uint n_in_box = tile->index / 3 % 9 / 3 * 3 + tile->index % 3;
        boxes[tile->index / 27 * 3 + tile->index % 9 / 3][n_in_box] = tile;
        n++;
    }

    for (Tile *tile : tiles)
    {
        tile->init_tSets(tiles);
    }

    // for (Tile *tile : tiles)
    // {
    //     tile->assign_possible_tiles();
    // }

    print_board();
}



bool Sudoku::remove_existing_numbers()
{
    bool changed = false;
    for (Tile *tile : tiles)
    {
        if (tile->getValue() != EMPTY)
        {

            changed |= tile->remove_val_from_visible(tile->getValue());
        }
    }
    return changed;
}

Sudoku::~Sudoku()
{
    for (Tile *tile : tiles)
    {
        delete tile;
    }
}

bool Sudoku::check_win(){
    for (tGroup *group : groups)
    {
        for (tSet set : *group)
        {
            bitset<9> nums;
            for (Tile *tile : set)
            {
                if (tile->getValue() == EMPTY)
                {
                    return false;
                } else {
                    nums.set(tile->getValue() - 1);
                }
            }
            if(!nums.all()){
                return false;
            }
        }
    }
    
    return true;
}

string Sudoku::getStr()
{
    string str;
    for (Tile *tile : tiles)
    {
        str += tile->getStr();
    }
    return str;
}

bool Sudoku::solve()
{
    solve_hidden_singles();

    return false;
}

void Sudoku::print_board()
{
    for (size_t i = 0; i < 9; i++)
    {

        for (size_t j = 0; j < 9; j++)
        {
            if (j % 3 == 0 and j != 0)
            {
                cout << " |";
            }

            cout << " " << tiles[i * 9 + j]->getStr();
        }
        if (i % 3 == 2 and i != 8)
        {
            cout << "\n-----------------------";
        }
        cout << "\n";
    }
}


int get_last_set_bit(bitset<9> b)
{
    int j = 0;
    while (!b.test(0))
    {
        b >>= 1;
        j++;
    }
    return j;
}

bool Sudoku::solve_hidden_singles()
{
    bool changed = false;
    for (tGroup *group : groups)
    {
        for (tSet set : *group)
        {
            for (size_t i = 1; i < 10; i++)
            {
                bitset<9> num;
                uint b;
                for (size_t t = 0; t < set.size(); t++)
                {
                    Tile *tile = set[t];
                    if (tile->getValue() == EMPTY)
                    {
                        b = tile->get_possible_vals().test(i-1) ? i : b;
                        num.set(t, tile->get_possible_vals().test(i-1));
                    }
                    else
                    {
                        continue;
                    }
                }

                auto n = num;
                if (n.count() == 1)
                {
                    set[get_last_set_bit(n)]->update_value(i);
                    changed = true;
                }
            }
        }
    }
    return changed;
}

bool Sudoku::solve_singles(){
    bool changed = false;
    for (Tile *tile : tiles){
        if (tile->get_possible_vals().count() == 1){
            tile->update_value(get_last_set_bit(tile->get_possible_vals())+1);
            changed = true;
        }
    }
    return changed;
}

bool Sudoku::remove_naked_subset(){
    bool changed = false;
    for (tGroup *group : groups)
    {
        for (tSet set : *group)
        {
            for (Tile *tile : set)
            {
                if (tile->possibleVals.count() == 2|| tile->possibleVals.count() == 3)
                {
                    for (Tile *tile2 : set){
                        if (tile2 != tile && tile2->possibleVals == tile->possibleVals)
                        {
                            if (tile->possibleVals.count() == 2){
                                // only if there is a naked pair
                                for (Tile *t : set)
                                {
                                    if (tile != t && tile2 != t && (t->possibleVals & tile->possibleVals).any())
                                    {
                                        t->possibleVals &= ~tile->possibleVals;
                                        changed = true;
                                    }
                                }
                            }
                            else if (tile->possibleVals.count() == 3){
                                for (Tile *tile3 : set)
                                {
                                    if (tile3 != tile && tile3 != tile2 && tile3->possibleVals == tile->possibleVals)
                                    {
                                        // only if there is a naked triplet
                                        for (Tile *t : set)
                                        {
                                            if (tile != t && tile2 != t && tile3 != t && (t->possibleVals & tile->possibleVals).any())
                                            {
                                                t->possibleVals &= ~tile->possibleVals;
                                                changed = true;
                                            }
                                        }
                                    } 
                                }
                            
                            }

                            
                        }
                    }
                }
            }
        }
    }
    return changed;
}