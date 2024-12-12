#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../include/classes.h"

using namespace std;

TEST_CASE("create Sudoku")
{
    string line = "020000094760910050090002081070050010000709000080031067240100070010090045900000100";
    Sudoku sud = Sudoku(line);
    CHECK(sud.getStr() == line);
}

TEST_CASE("assign possible values")
{
    string line = "020000094760910050090002081070050010000709000080031067240100070010090045900000100";
    Sudoku sud = Sudoku(line);
    for (Tile *tile : sud.tiles)
    {
        CHECK(tile->possibleVals.size() == 9);
    }
    CHECK(sud.remove_existing_numbers());
    CHECK(sud.tiles[0]->possibleVals.test(0));
    CHECK(!sud.tiles[0]->possibleVals.test(1));
    CHECK(sud.tiles[0]->possibleVals.test(2));
    CHECK(!sud.tiles[0]->possibleVals.test(3));
    CHECK(sud.tiles[0]->possibleVals.test(4));
    CHECK(!sud.tiles[0]->possibleVals.test(5));
    CHECK(!sud.tiles[0]->possibleVals.test(6));
    CHECK(sud.tiles[0]->possibleVals.test(7));
    CHECK(!sud.tiles[0]->possibleVals.test(8));
}

TEST_CASE("solve_singles")
{
    string line = "003008700800000006170000490026091000009030064030270000000000000000000912090000003";
    Sudoku sud = Sudoku(line);
    CHECK(sud.remove_existing_numbers());
    CHECK(sud.tiles[41]->getValue() != 5);
    

    CHECK(sud.solve_singles());
    CHECK(sud.tiles[41]->getValue() == 5);
}

TEST_CASE("solve_hidden_singles")
{
    string line = "200070038000006070300040600008020700100000006007030400004080009060400000910060002";
    Sudoku sud = Sudoku(line);
    CHECK(sud.remove_existing_numbers());
    CHECK(sud.tiles[1]->getValue() != 4);
    CHECK(sud.tiles[2]->getValue() != 6);
    CHECK(sud.tiles[12]->getValue() != 3);
    CHECK(sud.tiles[17]->getValue() != 4);
    CHECK(sud.tiles[19]->getValue() != 7);
    CHECK(sud.tiles[61]->getValue() != 6);
    CHECK(sud.tiles[63]->getValue() != 8);
    CHECK(sud.tiles[71]->getValue() != 7);
    CHECK(sud.tiles[79]->getValue() != 4);

    
    CHECK(sud.solve_hidden_singles());

    CHECK(sud.tiles[1]->getValue() == 4);
    CHECK(sud.tiles[2]->getValue() == 6);
    CHECK(sud.tiles[12]->getValue() == 3);
    CHECK(sud.tiles[17]->getValue() == 4);
    CHECK(sud.tiles[19]->getValue() == 7);
    CHECK(sud.tiles[61]->getValue() == 6);
    CHECK(sud.tiles[63]->getValue() == 8);
    CHECK(sud.tiles[71]->getValue() == 7);
    CHECK(sud.tiles[79]->getValue() == 4);
}

TEST_CASE("remove_naked_subset")
{
    string line = "400000938032094100095300240370609004529001673604703090957008300003900400240030709";
    Sudoku sud = Sudoku(line);
    CHECK(sud.remove_existing_numbers());
    CHECK(sud.tiles[3]->possibleVals.test(0));
    CHECK(sud.tiles[4]->possibleVals.test(0));
    CHECK(sud.tiles[4]->possibleVals.test(5));
    CHECK(sud.tiles[18]->possibleVals.test(0));
    CHECK(sud.tiles[18]->possibleVals.test(6));
    CHECK(sud.tiles[22]->possibleVals.test(5));
    CHECK(sud.tiles[22]->possibleVals.test(6));
    CHECK(sud.tiles[31]->possibleVals.test(7));
    CHECK(sud.tiles[34]->possibleVals.test(4));
    CHECK(sud.tiles[34]->possibleVals.test(7));
    CHECK(sud.tiles[49]->possibleVals.test(7));
    CHECK(sud.tiles[53]->possibleVals.test(4));
    
    CHECK(sud.remove_naked_subset());

    CHECK(!sud.tiles[3]->possibleVals.test(0));
    CHECK(!sud.tiles[4]->possibleVals.test(0));
    CHECK(!sud.tiles[4]->possibleVals.test(5));
    CHECK(!sud.tiles[18]->possibleVals.test(0));
    CHECK(!sud.tiles[18]->possibleVals.test(6));
    CHECK(!sud.tiles[22]->possibleVals.test(5));
    CHECK(!sud.tiles[22]->possibleVals.test(6));
    CHECK(!sud.tiles[31]->possibleVals.test(7));
    CHECK(!sud.tiles[34]->possibleVals.test(4));
    CHECK(!sud.tiles[34]->possibleVals.test(7));
    CHECK(!sud.tiles[49]->possibleVals.test(7));
    CHECK(!sud.tiles[53]->possibleVals.test(4));
}

TEST_CASE("Hidden Subsets")
{
    SUBCASE("hidden 4"){
        string line = "901500046425090081860010020502000000019000460600000002196040253200060817000001694";
        Sudoku sud = Sudoku(line);

        CHECK(sud.remove_existing_numbers());
        CHECK(sud.remove_naked_subset());
        CHECK(sud.tiles[30]->possibleVals.test(2));
        CHECK(sud.tiles[30]->possibleVals.test(6));
        CHECK(sud.tiles[30]->possibleVals.test(7));

        CHECK(sud.remove_hidden_subset(4));
        CHECK(!sud.tiles[30]->possibleVals.test(2));
        CHECK(!sud.tiles[30]->possibleVals.test(6));
        CHECK(!sud.tiles[30]->possibleVals.test(7));
    }

    SUBCASE("hidden 3"){
        // Trip
        string line = "000001030231090000065003100678924300103050006000136700009360570006019843300000000";
        Sudoku sud = Sudoku(line);

        CHECK(sud.remove_existing_numbers());
        CHECK(sud.remove_naked_subset());
        CHECK(sud.tiles[3]->possibleVals.test(3));
        CHECK(sud.tiles[3]->possibleVals.test(6));
        CHECK(sud.tiles[3]->possibleVals.test(7));
        INFO(sud.tiles[3]->possibleVals);

        CHECK(sud.remove_hidden_subset(3));
        INFO(sud.tiles[3]->possibleVals);

        CHECK(!sud.tiles[3]->possibleVals.test(3));
        CHECK(!sud.tiles[3]->possibleVals.test(6));
        CHECK(!sud.tiles[3]->possibleVals.test(7));
    }
    
    SUBCASE("hidden 2"){
        string line = "000001030231090000065003100678924300103050006000136700009360570006019843300000000";
        Sudoku sud = Sudoku(line);

        CHECK(sud.remove_existing_numbers());
        CHECK(sud.remove_naked_subset());
        CHECK(sud.tiles[3]->possibleVals.test(3));
        CHECK(sud.tiles[3]->possibleVals.test(6));
        CHECK(sud.tiles[3]->possibleVals.test(7));
        CHECK(sud.tiles[3]->possibleVals.test(1));
        CHECK(sud.tiles[3]->possibleVals.test(4));
        CHECK(sud.tiles[3]->possibleVals.test(5));
        INFO(sud.tiles[3]->possibleVals);

        CHECK(sud.remove_hidden_subset(3));
        INFO(sud.tiles[3]->possibleVals);

        CHECK(!sud.tiles[3]->possibleVals.test(3));
        CHECK(!sud.tiles[3]->possibleVals.test(6));
        CHECK(!sud.tiles[3]->possibleVals.test(7));

        CHECK(sud.tiles[3]->possibleVals.test(1));
        CHECK(sud.tiles[3]->possibleVals.test(4));
        CHECK(sud.tiles[3]->possibleVals.test(5));
    }
}

TEST_CASE("remove_hidden_subset3")
{

    // Trip
    string line = "000001030231090000065003100678924300103050006000136700009360570006019843300000000";
    Sudoku sud = Sudoku(line);

    CHECK(sud.remove_existing_numbers());
    CHECK(sud.remove_naked_subset());
    CHECK(sud.tiles[3]->possibleVals.test(3));
    CHECK(sud.tiles[3]->possibleVals.test(6));
    CHECK(sud.tiles[3]->possibleVals.test(7));
    INFO(sud.tiles[3]->possibleVals);

    CHECK(sud.remove_hidden_subset(3));
    INFO(sud.tiles[3]->possibleVals);

    CHECK(!sud.tiles[3]->possibleVals.test(3));
    CHECK(!sud.tiles[3]->possibleVals.test(6));
    CHECK(!sud.tiles[3]->possibleVals.test(7));
}

TEST_CASE("remove_hidden_subset2")
{

    // Trip
    string line = "000001030231090000065003100678924300103050006000136700009360570006019843300000000";
    Sudoku sud = Sudoku(line);

    CHECK(sud.remove_existing_numbers());
    CHECK(sud.remove_naked_subset());
    CHECK(sud.tiles[3]->possibleVals.test(3));
    CHECK(sud.tiles[3]->possibleVals.test(6));
    CHECK(sud.tiles[3]->possibleVals.test(7));
    CHECK(sud.tiles[3]->possibleVals.test(1));
    CHECK(sud.tiles[3]->possibleVals.test(4));
    CHECK(sud.tiles[3]->possibleVals.test(5));
    INFO(sud.tiles[3]->possibleVals);

    CHECK(sud.remove_hidden_subset(3));
    INFO(sud.tiles[3]->possibleVals);

    CHECK(!sud.tiles[3]->possibleVals.test(3));
    CHECK(!sud.tiles[3]->possibleVals.test(6));
    CHECK(!sud.tiles[3]->possibleVals.test(7));

    CHECK(sud.tiles[3]->possibleVals.test(1));
    CHECK(sud.tiles[3]->possibleVals.test(4));
    CHECK(sud.tiles[3]->possibleVals.test(5));
}

TEST_CASE("remove_box_line_reduction")
{

    string line = "020943715904000600750000040500480000200000453400352000042000081005004260090208504";
    Sudoku sud = Sudoku(line);
    CHECK(sud.remove_existing_numbers());
    CHECK(sud.remove_naked_subset());
    CHECK(sud.remove_pointing_subset());

    CHECK(sud.tiles[29]->possibleVals.test(5));
    CHECK(sud.tiles[64]->possibleVals.test(2));

    CHECK(sud.remove_box_line_reduction());

    CHECK(!sud.tiles[29]->possibleVals.test(5));
    CHECK(!sud.tiles[64]->possibleVals.test(2));
}

TEST_CASE("remove_x_wing")
{
    // horizontal
    string line = "100000569492056108056109240009640801064010000218035604040500016905061402621000005";
    Sudoku sud = Sudoku(line);
    CHECK(sud.remove_existing_numbers());
    CHECK(sud.remove_naked_subset());
    CHECK(sud.remove_pointing_subset());

    CHECK(sud.tiles[3]->possibleVals.test(6));
    CHECK(sud.tiles[39]->possibleVals.test(6));

    CHECK(sud.remove_xwing());

    CHECK(!sud.tiles[3]->possibleVals.test(6));
    CHECK(!sud.tiles[39]->possibleVals.test(6));

    // vertical
}

TEST_CASE("remove simple offchain")
{
    string line = "200041006400602010016090004300129640142060590069504001584216379920408165601900482";
    Sudoku sud = Sudoku(line);
    CHECK(sud.remove_existing_numbers());
    CHECK(sud.tiles[13]->has_possible(3));
    CHECK(sud.tiles[17]->has_possible(8));
    CHECK(sud.remove_simple_coloring());
    CHECK(!sud.tiles[13]->has_possible(3));
    CHECK(sud.remove_simple_coloring());
    CHECK(!sud.tiles[17]->has_possible(8));
}

TEST_CASE("remove simple onchain")
{
    string line = "300000560092765300500013209714500603053040100900137405630928704009301806080000930";
    Sudoku sud = Sudoku(line);
    CHECK(sud.remove_existing_numbers());
    CHECK(sud.remove_naked_subset());
    CHECK(sud.tiles[8]->has_possible(1));
    CHECK(sud.tiles[9]->has_possible(1));
    CHECK(sud.tiles[16]->has_possible(1));
    CHECK(sud.tiles[56]->has_possible(1));
    CHECK(sud.tiles[80]->has_possible(1));
    CHECK(sud.remove_simple_coloring());
    CHECK(!sud.tiles[8]->has_possible(1));
    CHECK(!sud.tiles[9]->has_possible(1));
    CHECK(!sud.tiles[16]->has_possible(1));
    CHECK(!sud.tiles[56]->has_possible(1));
    CHECK(!sud.tiles[80]->has_possible(1));
}

SCENARIO("vectors can be sized and resized")
{
    GIVEN("A vector with some items")
    {
        std::vector<int> v(5);

        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 5);

        WHEN("the size is increased")
        {
            v.resize(10);

            THEN("the size and capacity change")
            {
                CHECK(v.size() == 10);
                CHECK(v.capacity() >= 10);
            }
        }
        WHEN("the size is reduced")
        {
            v.resize(0);

            THEN("the size changes but not capacity")
            {
                CHECK(v.size() == 0);
                CHECK(v.capacity() >= 5);
            }
        }
        WHEN("more capacity is reserved")
        {
            v.reserve(10);

            THEN("the capacity changes but not the size")
            {
                CHECK(v.size() == 5);
                CHECK(v.capacity() >= 10);
            }
        }
        WHEN("less capacity is reserved")
        {
            v.reserve(0);

            THEN("neither size nor capacity are changed")
            {
                CHECK(v.size() == 5);
                CHECK(v.capacity() >= 5);
            }
        }
    }
}