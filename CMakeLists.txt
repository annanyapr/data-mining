cmake_minimum_required(VERSION 3.17)
project(DataMining)

set(CMAKE_CXX_STANDARD 17)
set(GML ./readgml)
include_directories(${GML})
find_package (Threads)


set(
        SOURCE_FILES
        ${GML}/readgml.c
        comparision.cpp

)
add_executable(Clion ${SOURCE_FILES})
target_link_libraries(Clion PRIVATE Threads::Threads)
