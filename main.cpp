

#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <chrono>
#include <filesystem>
#include <cassert>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "include/classes.h"
#include "include/defs.h"


typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::duration<float> fsec;
typedef std::chrono::milliseconds ms;

const vector<char> comment_symbol = {'#', '/'};

string sanetize(string s){
    for (char c : e_char)
        replace(s.begin(), s.end(), c, '0');
    boost::trim(s);
    return s;
}

int main(int argc, char *args[])
{

    for (int i = 0; i < argc; ++i)
        cout << "args["<< i <<"]: " << args[i] << endl;

    int max_games = argc>1 ? atoi(args[1]) : 100;
    cout << "total games: " << max_games << endl;
    boost::filesystem::path path("data");
    if(!boost::filesystem::exists(path)) {
        printf("data directory does not exist: %s\n", (boost::filesystem::current_path() / path).c_str());
        return 1;
    }
    // assert(boost::filesystem::exists(path) && (boost::format("data directory does not exist: %s",  boost::filesystem::current_path() / path)));
    for (const auto &sud_filepath : boost::filesystem::directory_iterator(path))
    {
        int num_games = 0;
        int num_games_last=0;
        int num_won = 0;
        int num_won_last=0;
        int n = 0;

        auto t0 = Time::now();

        string filename = sud_filepath.path().filename().string();
        printf("parsing %s\n", filename.c_str());
        ifstream sud_file;
        int linecount = 0;
        if (!sud_filepath.is_regular_file() || sud_filepath.path().filename().c_str()[0]=='.')
            continue;
        sud_file.open(sud_filepath.path().string());
        int lines = std::count(std::istreambuf_iterator<char>(sud_file),
                                std::istreambuf_iterator<char>(), '\n');
        sud_file.close();
        sud_file.open(sud_filepath.path().string());
        // sud_file.; 
        int num_lines = min(max_games, lines);

        // string sud_filepath = "data/puzzles0_kaggle";
        // sud_file.open(sud_filepath);
        string line;

        if (sud_file.is_open())
        {
            while (getline(sud_file, line))
            {
                // line = "200041006400602010016090004300129640142060590069504001584216379920408165601900482";
                string sudstr = sanetize(line);
                if (sudstr.empty() ||contains(comment_symbol, sudstr[0]))
                    continue;
                if (n >= num_lines)
                    break;

                Sudoku sud(sudstr);
                num_won += sud.solve();

            n++;
            num_games += 1;
            if (n % (num_lines / 20) == 0)
            {

                printf("%i: solved %i out of the last %i games: %.1f%%\n",
                       n,
                       num_won - num_won_last,
                       num_games - num_games_last,
                       ((num_won - num_won_last) * 100.f) / (num_games - num_games_last));
                num_games_last = num_games;
                num_won_last = num_won;
                }
                
                // sud->print_board();
                // break;
            }
            if (n < num_lines)
                printf("end of file reached; %i lines \n", n);

            fsec time = Time::now() - t0;
            printf("time taken: %.2fs\n", time.count());
            printf("solved %d out of %d: %.2f%%\n\n", num_won, num_games, num_won * 100.0 / num_games);
            
            sud_file.close();
                
        } else 
            cout << "Could not open file: " << sud_filepath << endl;
        

    }
    return 0;
}
