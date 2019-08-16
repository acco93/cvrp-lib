include_directories (../src)

add_executable (vrp-instance-test main.cpp)

target_link_libraries (vrp-instance-test vrp-instance-lib gtest gtest_main)