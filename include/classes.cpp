#include "classes.h"
#include <cassert>
#include <set>



Chain::Chain(Tile *tile, int num){
    assert(num > 0 && num <= 9 && "num must be between 1 and 9");
    this->num = num;
    _chain(tile, true);
}

// Chain::~Chain(){
//     delete contradictions;

// }

void Chain::_chain(Tile *tile, bool onoff){
    // dont do anything if that tile is already present.
    if (count(tile)) return;

    emplace(tile, onoff);
    for(tSet *group : tile->groups)
    {
        vector<Tile *> v = group->tiles_containing_missing_vals(num);
        if (v.size() != 2) continue;
        
        for (Tile *t : v)
        {
            if (t != tile) _chain(t, !onoff);
        }
        
    }
}

bool Chain::check_contradiction(){
    if (this->size() < 2)
        return false;
    for(auto const& [root_tile, color] : *this){
        for (tSet *group : root_tile->groups)
        {
            vector<Tile *> v = group->tiles_containing_missing_vals(num);
            if (v.size() < 2) continue;
            for (Tile *err_tile : v)
            {
                auto pair = this->find(err_tile);
                if(pair == this->end())
                {   //off-chain
                    for (tSet *g2 : err_tile->groups)
                    {
                        if (g2 == group) continue;
                        vector<Tile *> v2 = g2->tiles_containing_missing_vals(num);
                        if (v2.size() < 3) continue;
                        for (Tile *t2 : v2)
                        {
                            auto pair2 = this->find(t2);
                            if (pair2 != end()){
                                if( t2 != root_tile && pair2->second != color && 
                                !contains(contradictions, err_tile))
                                {
                                    // cout << "found offchain contradiction: " << num << " cant be in position " << err_tile->index << " due to " << root_tile->index << " (" << pair2->second << ")" << " and " << t2->index << " (" << color << ")" << endl;
                                    // t->remove_possible(num);
                                    contradictions.push_back(err_tile);
                                }
                            }
                        }
                    }
                }
                else if (err_tile != root_tile && pair->second == color &&
                        !contains(contradictions, err_tile))
                {
                    // cout << "found onchain contradiction num: " << num << " cant be in positions: ";
                    for (auto &[t, c] : *this)
                    {
                        if(c==color) {
                            contradictions.push_back(t);
                            // cout << t->index << ", ";
                        }  
                    }
                    // cout << endl;
                }
            }
            
        }
    }
    return contradictions.size() > 0;
}


int rFromInd(int ind)
{
    return ind / setsize;
};
int cFromInd(int ind)
{
    return ind % setsize;
};
int bFromInd(int ind)
{
    return ind / 9 / 3 * 3 + ind % 9 / 3;
};

Tile::Tile(int ind, int val, tSet *_column, tSet *_row, tSet *_box)
{

    index = ind;
    groups[SetType::row] = row = _row;
    groups[SetType::column] = column = _column;
    groups[SetType::box] = box = _box;
    init(val);

    assert(value_isValid(value) && "Invalid value: " + value);
}

Tile::~Tile()
{
}

void Tile::init(int val){
    value = val;
    if (val == ZERO)
    {
        possibleVals.set();
    }
    else
    {
        possibleVals.reset();
        column->missingVals.set(val - 1, false);
        row->missingVals.set(val - 1, false);
        box->missingVals.set(val - 1, false);
    }
}

bool Tile::remove_possible(int val)
{

    bool changed = possibleVals.test(val-1);
    possibleVals.set(val-1, false);
    assert((possibleVals.any() || val) && "possibleVals must have at least one bit set");
    return changed;
}

bool Tile::remove_val_from_visible(int val)
{
    bool changed = false;
    if (getValue() != ZERO)
    {
        for (tSet *tSet : {column, row, box})
        {
            for (Tile *t : *tSet)
            {
                if (t!=this)
                {
                    changed |= t->remove_possible(getValue());
                }
            }
        }
    }
    return changed;
}


void Tile::update_value(int val)
{
    assert(value_isValid(val) && "Invalid value: " + val);
    value = val;
    possibleVals.reset();
    column->missingVals.set(val-1, false);
    row->missingVals.set(val-1, false);
    box->missingVals.set(val-1, false);
    for (tSet *tSet : {column, row, box}){
        for (Tile *t : *tSet)
        {
            if (t)
            {
                t->remove_possible(val);
            }
        }
    }
}

Tile::operator std::string() { return value == EMPTY ? " " : to_string(value); }


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
    o_sud = sud_str;
    
    if (sud_str.size() != total_size)
    {
        throw length_error("sudoku not the right length");
    }
    

    for (string::size_type i = 0; i < sud_str.size(); ++i)
    {
        int num = static_cast<int>(sud_str.at(i) - '0');
        tiles[i] = new Tile(i, num, &columns[cFromInd(i)], &rows[rFromInd(i)], &boxes[bFromInd(i)]);
    }

    for (Tile *tile : tiles)
    {
        columns[cFromInd(tile->index)][rFromInd(tile->index)] = tile;
        rows[rFromInd(tile->index)][cFromInd(tile->index)] = tile;
        uint n_in_box = tile->index / 3 % 9 / 3 * 3 + tile->index % 3;
        boxes[bFromInd(tile->index)][n_in_box] = tile;
    }

    groups = { &rows,
               &columns,
               &boxes };
    // for (Tile *tile : tiles)
    // {
    //     tile->init_tSets(tiles);
    // }

    // for (Tile *tile : tiles)
    // {
    //     tile->assign_possible_tiles();
    // }

    // print_board();
}

void Sudoku::init_sudoku(std::string sud_str)
{
    
    for (string::size_type i = 0; i < sud_str.size(); ++i)
    {
        int num = static_cast<int>(sud_str.at(i) - '0');
        if (tiles[i] == nullptr)
        {
            tiles[i] = new Tile(i, num, &columns[cFromInd(i)], &rows[rFromInd(i)], &boxes[bFromInd(i)]);
        }
        else
        {
            tiles[i]->init(num);
        }
    }
}

SolveType Sudoku::remove_existing_numbers()
{
    SolveType changed = SolveType::none;
    for (Tile *tile : tiles)
    {
        if (tile->getValue() != EMPTY)
        {

            if (tile->remove_val_from_visible(tile->getValue()))
                changed = SolveType::remove_existing_numbers;
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

bool Sudoku::check_win()
{
    for (tGroup *group : groups)
    {
        for (tSet set : *group)
        {
            bitset<9> nums;
            for (Tile *tile : set)
            {
                if (tile->getValue() == EMPTY && tile->possibleVals.count() == 0)
                {
                    print_board();
                    throw logic_error("Sudoku has no solution");
                }
                else if (tile->getValue() == EMPTY)
                {
                    // print_board();
                    return false;
                }
                else
                {
                    nums.set(tile->getValue() - 1);
                }
            }
            if (!nums.all())
            {
                print_board();
                return false;
            }
        }
    }

    return true;
}

Sudoku::operator std::string() { return getStr(); }

string Sudoku::getStr()
{
    string str;
    for (Tile *tile : tiles)
    {
        str += to_string(tile->getValue());
    }
    return str;
}

bool Sudoku::step(){
    #ifndef __DEBUG__
    return remove_existing_numbers() || 
        solve_singles() || 
        solve_hidden_singles() || 
        remove_naked_subset() || 
        remove_hidden_subset(2) || 
        remove_hidden_subset(3) || 
        remove_pointing_subset() || 
        remove_box_line_reduction() || 
        remove_xwing() || 
        remove_hidden_subset(4) || 
        remove_simple_coloring();
    
    #else
    if (remove_existing_numbers())
    {
        cout << "removed existing numbers" << endl;
        return true;
    }
    else if (solve_singles()){
        cout << "solved singles" << endl;
        return true;
    }
    else if (solve_hidden_singles())
    {
        cout << "solved hidden singles" << endl;
        return true;
    }
    else if (remove_naked_subset())
    {
        cout << "found a naked subset" << endl;
        return true;
    }
    else if (remove_hidden_subset(2))
    {
        cout << "found a hidden pair" << endl;
        return true;
    }
    else if (remove_hidden_subset(3))
    {
        cout << "found a hidden triple" << endl;
        return true;
    }
    else if (remove_pointing_subset())
    {
        cout << "found a pointing pair/triple" << endl;
        return true;
    }
    else if (remove_box_line_reduction())
    {
        cout << "found a box line reduction" << endl;
        return true;
    } 
    else if (remove_xwing())
    {
        cout << "found an xwing" << endl;
        return true;
    }
    else if (remove_hidden_subset(4))
    {
        cout << "found a hidden quad" << endl;
        return true;
    }
    else if (remove_simple_coloring())
    {
        cout << "found a simple chain" << endl;
        return true;
    }
    #endif
    return false;
}

bool Sudoku::solve()
{
    while (step());
    // cout << getStr() << endl;
    return check_win();
}

void Sudoku::print_board()
{
    cout << endl;
    cout << o_sud << endl;
    cout << this << endl;
    for (size_t i = 0; i < 9; i++)
    {

        for (size_t j = 0; j < 9; j++)
        {
            if (j % 3 == 0 and j != 0)
            {
                cout << " |";
            }

            cout << " " << tiles[i * 9 + j];
        }
        if (i % 3 == 2 and i != 8)
        {
            cout << "\n-----------------------";
        }
        cout << "\n";
    }
}


int get_nth_set_bit(bitset<setsize> b, int n = 1)
{
    assert(b.count() >= n && "must have at least n set bit(s)");
    int count = 0;
    for (size_t i = 0; i < b.size(); i++)
    {
        count += b.test(i);
        if (count >= n)
            return i;
    }
    assert(false && "coundnt get set bit even though I checked enough bits are set");
    return -1;
}

int get_first_set_bit(bitset<setsize> b)
{
    assert(b.count()>0 && "must have a set bit");
    int j = setsize-1;
    while (!b.test(j))
    {
        j--;
    }
    return j;
}

SolveType Sudoku::solve_hidden_singles()
{
    SolveType changed = SolveType::none;
    for (tGroup *group : groups)
    {
        for (tSet set : *group)
        {
            for (size_t i = 1; i < 10; i++)
            {
                bitset<9> miss = set.ind_containing_missing_vals(i);
                if (miss.count() != 1)
                    continue;
                set[get_first_set_bit(miss)]->update_value(i);
                changed = SolveType::solve_hidden_singles;
            }
        }
    }
    return changed;
}

SolveType Sudoku::solve_singles()
{
    SolveType changed = SolveType::none;
    for (Tile *tile : tiles)
    {
        if (tile->possibleVals.count() == 1)
        {
            tile->update_value(get_nth_set_bit(tile->possibleVals) + 1);
            changed = SolveType::solve_singles;
        }
    }
    return changed;
}

SolveType Sudoku::remove_naked_subset(int len)
{
    SolveType changed = SolveType::none;
    for (tGroup *group : groups)
    {
        for (tSet set : *group)
        {
            if (set.missingVals.count() <= len)
                continue;

            bitset<9> current;
            for (int i = 0; i < (1 << set.missingVals.count()); i++)
            {
                current = i;
                if ((current & set.missingVals).count() != len)
                    continue;
                bitset<9> ind_tiles_with_comb;
                ind_tiles_with_comb.reset();
                for (int j = 0; j < set.missingVals.size(); j++)
                {
                    if (current.test(j) && set.missingVals.test(j))
                        ind_tiles_with_comb |= set.ind_containing_missing_vals(j + 1);
                }
            }
        }
    }
    return changed;
}


SolveType Sudoku::remove_hidden_subset(int len){
    SolveType changed = SolveType::none;
    assert(len == 2 || len == 3 || len == 4 &&"length of subset should be 2,3 or 4");
    for(tGroup *group : groups){
        for(tSet sset : *group){
            if (sset.missingVals.count() <= len) continue;

            bitset<9> current;
            // loop through all possible combinations of missing values with size len
            for (int i = 0; i < (1 << sset.missingVals.count()); i++)
            {
                current = i;
                if ((current & sset.missingVals).count() != len)  continue;
                bitset<9> ind_tiles_with_comb;
                ind_tiles_with_comb.reset();
                for (int j = 0; j < sset.missingVals.size(); j++)
                {
                    if (current.test(j) && sset.missingVals.test(j))
                        ind_tiles_with_comb |= sset.ind_containing_missing_vals(j + 1);
                }
                if (ind_tiles_with_comb.count() != len) continue;
                // the combination can only occur in those tiles, no other numbers can be in those tiles
                for (int j=0; j<sset.size(); j++){
                    // for tile in set if it contains the combination and other numbers can be in it, eliminate those
                    if(ind_tiles_with_comb.test(j) && (sset[j]->possibleVals & ~current).any()) {
                        // if (len == 4 && !changed){
                        //     cout << "holy smokes, found a 4 length subset" << endl;
                        //     cout << this << endl;
                        // }
                        sset[j]->possibleVals &= current;
                        changed = SolveType::remove_hidden_subset;
                    }
                }
            }
        }
    }
    return changed;
}

SolveType Sudoku::remove_box_line_reduction()
{
    SolveType changed = SolveType::none;
    for (int i = 0; i < 2; i++)
    {
        for (tSet sset : *groups[i])
        {
            for (int i = 0; i < 9; i++)
            {
                if (!sset.missingVals.test(i))
                    continue;
                vector<Tile *> v = sset.tiles_containing_missing_vals(i + 1);
                if ((v.size() != 2 && v.size() != 3) ||
                    // check if all tiles in the subset are in the same box
                    any_of(v.begin() + 1, v.end(),
                                            [&](const Tile *r)
                                            { return r->box != v.front()->box; }))

                { 
                    continue;
                }
                for (Tile *tile : *v.front()->box)
                {
                    if ((*tile->row != sset && *tile->column != sset) && tile->possibleVals.test(i))
                    {
                        // cout << "holy smokes, found a box line reduction" << endl;
                        tile->possibleVals.set(i, false);
                        changed = SolveType::remove_box_line_reduction;
                    }
                }
                
            }
        }
    }
    // for (tSet column : columns)
    // {
    //     for (int i = 0; i < 9; i++)
    //     {
    //         if (!column.missingVals.test(i))
    //             continue;
    //         vector<Tile *> v = column.tiles_containing_missing_vals(i + 1);
    //         if ((v.size() == 2 || v.size() == 3) &&
    //             boost::algorithm::all_of(v.begin() + 1, v.end(),
    //                                       [&](const Tile *r)
    //                                       { return r->box == v.front()->box; }))

    //         {
    //             for (Tile *tile : *v.front()->box)
    //             {
    //                 if ((*tile->column != column) && tile->possibleVals.test(i))
    //                 {
    //                     tile->possibleVals.set(i, false);
    //                     changed = SolveType;
    //                 }
    //             }
    //         }
    //     }
    // }
    return changed;
}

SolveType Sudoku::remove_pointing_subset()
{
    SolveType changed = SolveType::none;
    for (tSet box : boxes)
    {
        for (int i = 0; i < 9; i++)
        {
            if (!box.missingVals.test(i))
                continue;
            vector<Tile *> v = box.tiles_containing_missing_vals(i + 1);
            if ((v.size() == 2 || v.size() == 3) &&
                all_of(v.begin() + 1, v.end(),
                                            [&](const Tile *r)
                                            { return r->column == v.front()->column; }))

            {
                for (Tile *tile : *v.front()->column)
                {
                    if (*tile->box != box && tile->possibleVals.test(i))
                    {
                        tile->possibleVals.set(i, false);
                        changed = SolveType::remove_pointing_subset;
                    }
                }
            }
            else if ((v.size() == 2 || v.size() == 3) && all_of(v.begin() + 1, v.end(),
                                                                                    [&](const Tile *r)
                                                                                    { return r->row == v.front()->row; }))
            {
                for (Tile *tile : *v.front()->row)
                {
                    if (*tile->box != box && tile->possibleVals.test(i))
                    {
                        tile->possibleVals.set(i, false);
                        changed = SolveType::remove_pointing_subset;
                    }
                }
            }
        }
    }
    return changed;
}

SolveType Sudoku::remove_xwing(){

    SolveType changed = SolveType::none;
    for (int pairGrp = 0; pairGrp < 2; pairGrp++)
    {
        int other = (int)!pairGrp;  // 0 if 1 else 1
        tGroup sets = *groups[pairGrp];

        //dont need to check sets[6+] since the second set already covers the right most column
        // and since an xwing cant be in the same box, we can ignore the entire last box on our 1st set
        for (int i1 = 0; i1 < 6; i1++)
        {
            tSet set1 = sets[i1];
            for (int i = 0; i < 9; i++){
                if (!set1.missingVals.test(i))
                    continue;
                vector<Tile *> pair1 = set1.tiles_containing_missing_vals(i + 1);
                if (pair1.size() != 2)
                    continue;

                // only check sets after i1 since the first set already covers everything before it.
                for (int i2 = i1+1; i2 < 9; i2++)
                {
                    tSet set2 = sets[i2];
                    assert(i2 > i1 && " i2 > i1");
                    vector<Tile *> pair2 = set2.tiles_containing_missing_vals(i + 1);

                    tSet *otherSet1 = pair1.front()->groups[other];
                    tSet *otherSet2 = pair1.back()->groups[other];
                    vector<Tile *> toRemove1 = otherSet1->tiles_containing_missing_vals(i + 1);
                    vector<Tile *> toRemove2 = otherSet2->tiles_containing_missing_vals(i + 1);

                    if (pair2.size() != 2 ||
                        !(otherSet1 == pair2.front()->groups[other]) ||
                        !(otherSet2 == pair2.back()->groups[other]) ||
                        !(toRemove1.size() > 2 || toRemove2.size() > 2))
                        continue;

                    for (Tile *tile : toRemove1)
                    {
                        if (toRemove1.size() > 2 && tile != pair1.front() && tile != pair2.front() && tile->possibleVals.test(i))
                        {
                            tile->remove_possible(i+1);
                            changed = SolveType::remove_xwing;
                            // cout << "xwing: " << i + 1 << " from " << tile->index << endl;
                        }
                    }
                    for (Tile *tile : toRemove2)
                    {
                        if (toRemove2.size() > 2 && tile != pair1.back() && tile != pair2.back() && tile->possibleVals.test(i))
                        {
                            tile->remove_possible(i+1); 
                            changed = SolveType::remove_xwing;
                            // cout << "xwing: " << i+1 << " from " << tile->index << endl;
                        }
                    }
                }

            }
        }
    }
    return changed;
}

SolveType Sudoku::remove_simple_coloring()
{
    Chain *chain = simple_coloring();
    if (chain == nullptr)
        return SolveType::none;
    SolveType changed = SolveType::none;
    for (auto tile: chain->contradictions)
    {
        if (tile->remove_possible(chain->num))
                changed = SolveType::remove_simple_coloring;
    }
    delete chain;
    return changed;
}

Chain *Sudoku::simple_coloring(){
    for (int i = 0; i < 9; i++)
    {
        for (Tile *tile : tiles)
        {
        
            if (!tile->possibleVals.test(i))
                continue;
            Chain *c = new Chain(tile, i + 1);
            if (c->check_contradiction())
            {
                return c;
            }
            else delete c;
        }
    }

    return nullptr;
}