

#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <chrono>
#include <filesystem>
#include "include/classes.h"
#include "gui/graphics.h"
#include "include/defs.h"
#include <cassert>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::duration<float> fsec;
typedef std::chrono::milliseconds ms;

const vector<char> comment_symbol = {'#', '/'};


string sanetize(string s)
{
    for (char c : e_char)
        replace(s.begin(), s.end(), c, '0');
    boost::trim(s);
    return s;
}

int main(int argc, char *args[])
{
    init();

    for (int i = 0; i < argc; ++i)
    {
        cout << "args[" << i << "]: " << args[i] << endl;
    }

    int max_games = argc > 1 ? atoi(args[1]) : 100;
    cout << "total games: " << max_games << endl;
    boost::filesystem::path path("data");
    if (!boost::filesystem::exists(path))
    {
        printf("data directory does not exist: %s\n", (boost::filesystem::current_path() / path).c_str());
        return 1;
    }
    boost::filesystem::path sud_filepath("data/puzzles3_magictour_top1465");
    ifstream sud_file;
    sud_file.open(sud_filepath);
    
    if (sud_file.is_open())
    {
        string line;
        while (getline(sud_file, line))
        {
            string sudstr = sanetize(line);
            if (sudstr.empty() || contains(comment_symbol, sudstr[0]))
                continue;
            sudstr = "200041006400602010016090004300129640142060590069504001584216379920408165601900482"; 
            mainLoop(sudstr);
        }

        sud_file.close();
    }
    else
    {
        cout << "Could not open file: " << sud_filepath << endl;
    }
    return 0;
}
