cmake_minimum_required(VERSION 4.3)
project(uci_swe241_assignment3)

set(CMAKE_CXX_STANDARD 26)

add_executable(uci_swe241_assignment3
        sorting.cpp)

enable_testing()

add_executable(sorting_test sorting_test.cpp)
add_test(NAME sorting_test COMMAND sorting_test)

add_executable(sorting_benchmark sorting_benchmark.cpp)
add_test(NAME sorting_benchmark COMMAND sorting_benchmark)
