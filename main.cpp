#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include "main.h"
#include "classes.h"
#include "graphics.h"
#include "defs.h"
#include <cassert>




class sudoku_solver
{
    // all_of<Tile*> tiles = all_of<Tile*>();
    sudoku_solver()
    {
    }
};

int main(int argc, char *args[])
{
    init();
    ifstream sud_file;
    sud_file.open("sudoku17_shuffled.csv");
    if (sud_file.is_open())
    {
        int n = 0;
        string line;
        while (std::getline(sud_file, line))
        {
            if (n < 1)
            {
                n++;
                continue;
            }
            
            
            // std::cout << line << endl;
            // Sudoku sud(line);
            // string str = sud.getStr();
            // replaceAll(str, " ", to_string(EMPTY));
            // assert((str==line) && "generated sudoku different from input");
            // sud.solve();
            gameLoop(line);
            // break;
        }
        
        std::cout << endl;
        sud_file.close();
        
    }
    return 0;
}
