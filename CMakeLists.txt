cmake_minimum_required(VERSION 3.12)
project(dbase_channels)

set(CMAKE_CXX_STANDARD 17)
include_directories ("/usr/local/include")
link_directories("/usr/local/lib")
link_directories("/usr/local/lib64")

add_executable(dbase_channels src/main.cpp)
target_link_libraries(dbase_channels pq)
target_link_libraries(dbase_channels pqxx)
target_link_libraries(dbase_channels curl)
target_link_libraries(dbase_channels gq)