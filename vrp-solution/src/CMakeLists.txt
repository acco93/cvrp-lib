add_library(vrp-solution-lib Solution.hpp Solution.cpp Solution_initializer.cpp Solution_parsers.cpp)

target_include_directories(vrp-solution-lib PRIVATE ../../fixed-size-value-stack/src)
target_include_directories(vrp-solution-lib PRIVATE ../../macro/src)
target_include_directories(vrp-solution-lib PRIVATE ../../vrp-instance/src)
target_include_directories(vrp-solution-lib PRIVATE ../../raw-mm/src)

target_link_libraries(vrp-solution-lib
        raw-mm-lib
        fixed-size-value-stack-lib
        macro-lib
        vrp-instance-lib)