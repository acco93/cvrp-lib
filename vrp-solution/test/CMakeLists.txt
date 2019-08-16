include_directories (../src)

add_executable (vrp-solution-test main.cpp)

target_include_directories(vrp-solution-test PRIVATE ../../raw-mm/src)
target_include_directories(vrp-solution-test PRIVATE ../../macro/src)
target_include_directories(vrp-solution-test PRIVATE ../../fixed-size-value-stack/src)
target_include_directories(vrp-solution-test PRIVATE ../../vrp-instance/src)

target_link_libraries (vrp-solution-test vrp-solution-lib fixed-size-value-stack-lib gtest gtest_main raw-mm-lib)