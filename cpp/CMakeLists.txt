cmake_minimum_required(VERSION 3.30)
project(tools)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(run_all test/test_main.cpp
        test/unit_Reader.cpp
        test/unit_Options.cpp
        test/unit_Logger.cpp
)

add_executable(run_unit_reader test/test_main.cpp test/unit_Reader.cpp)
add_executable(run_unit_options test/test_main.cpp test/unit_Options.cpp)
add_executable(run_unit_logger test/test_main.cpp test/unit_Logger.cpp)

target_link_libraries(run_all gtest gmock gtest_main)
target_link_libraries(run_unit_reader gtest gmock gtest_main)
target_link_libraries(run_unit_options gtest gmock gtest_main)
target_link_libraries(run_unit_logger gtest gmock gtest_main)

add_executable(tools src/main.cpp)
