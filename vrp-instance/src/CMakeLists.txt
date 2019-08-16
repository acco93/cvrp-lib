add_library(vrp-instance-lib Instance.hpp Instance.cpp Instance_parsers.cpp SubInstance.hpp
        AbstractInstance.hpp AbstractInstance.cpp)

target_include_directories(vrp-instance-lib PRIVATE ../../raw-mm/src)

target_link_libraries(vrp-instance-lib raw-mm-lib)