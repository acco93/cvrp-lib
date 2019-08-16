include_directories (../src)

add_executable (raw-mm-test main.cpp)

target_link_libraries (raw-mm-test raw-mm-lib gtest gtest_main)