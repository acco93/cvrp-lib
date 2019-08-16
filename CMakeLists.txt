cmake_minimum_required(VERSION 3.10)

project(cvrp-lib)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(WARNING_FLAGS "-Wall -Wextra -Wpedantic -Wuninitialized")
set(SANITIZERS_FLAGS "-fno-omit-frame-pointer -fsanitize=undefined -fsanitize=leak") # -fsanitize=address
set(OPT_FLAGS "-O3 -march=native -ffat-lto-objects -flto -fwhole-program")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${SANITIZERS_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPT_FLAGS}")

add_subdirectory(googletest-master)
include_directories(googletest-master/googletest/include)
include_directories(googletest-master/googlemock/include)

add_subdirectory(macro)
add_subdirectory(vrp-solution)
add_subdirectory(raw-mm)
add_subdirectory(fixed-size-value-stack)
add_subdirectory(vrp-instance)
