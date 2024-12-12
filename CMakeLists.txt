cmake_minimum_required(VERSION 3.5.0)
project(SudokuSolver VERSION 0.1.0 LANGUAGES C CXX)


file(GLOB srces "include/*.h" "include/*.cpp")
add_executable(SudokuSolver main.cpp ${srces})
find_package(Boost COMPONENTS filesystem REQUIRED)
target_link_libraries( SudokuSolver
  Boost::filesystem
)

# GUI App
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)
file(GLOB guisrces "gui/*.h" "gui/*.cpp")
find_package( SDL2 REQUIRED )
find_package( SDL2_ttf REQUIRED)
add_executable(Sudoku_GUI GUI.cpp ${guisrces} ${srces} )
target_link_libraries( Sudoku_GUI
  Boost::filesystem
  SDL2::Main 
  SDL2::TTF
)

# Tests
option(ENABLE_DOCTESTS "Include tests in the library. Setting this to OFF will remove all doctest related code.
                    Tests in tests/*.cpp will still be enabled." ON)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/doctest)

include(Doctest)
add_executable(test_algos test/test.cpp include/classes.cpp include/defs.cpp)
target_link_libraries(test_algos PUBLIC doctest)
# add_test(NAME All COMMAND test_algos)
add_test(NAME Create_Sudoku           COMMAND test_algos -tc='create Sudoku')
add_test(NAME Assign_Possible_Values  COMMAND test_algos -tc='assign possible values')
add_test(NAME Solve_Singles           COMMAND test_algos -tc='solve_singles')
add_test(NAME Solve_Hidden_Singles    COMMAND test_algos -tc='solve_hidden_singles')
add_test(NAME Remove_Naked_Subset     COMMAND test_algos -tc='remove_naked_subset')
add_test(NAME Hidden_Subsets          COMMAND test_algos -tc='Hidden Subsets')
add_test(NAME remove_box_line_reduction COMMAND test_algos -tc='remove_box_line_reduction)
add_test(NAME remove_x_wing           COMMAND test_algos -tc='remove_x_wing')
enable_testing()