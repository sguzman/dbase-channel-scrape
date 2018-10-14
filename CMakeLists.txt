cmake_minimum_required(VERSION 3.12)
project(dbase_channels)

set(CMAKE_CXX_STANDARD 17)
include_directories ("/usr/local/include")

add_executable(dbase_channels src/main.cpp)
target_link_libraries(dbase_channels pq)
target_link_libraries(dbase_channels curl)