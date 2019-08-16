include_directories (../src)

add_executable (fixed-size-value-stack-test main.cpp)

target_link_libraries (fixed-size-value-stack-test gtest gtest_main)