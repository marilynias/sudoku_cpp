cmake_minimum_required(VERSION 3.5.0)
project(SudokuSolver VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB srces "../include/*.h" "../include/*.cpp" "./*.cpp" "./*.h")
add_executable(SudokuSolver main.cpp ${srces})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/sdl2)
find_package( SDL2 REQUIRED )
find_package( SDL2_ttf REQUIRED)
find_package(Boost COMPONENTS filesystem REQUIRED)

target_link_libraries( ${PROJECT_NAME} 
  SDL2::Main 
  SDL2::TTF
  Boost::filesystem
)

enable_testing()
add_executable(test_algos test/test.cpp include/classes.cpp include/defs.cpp)
add_test(NAME test_algos COMMAND test_algos)