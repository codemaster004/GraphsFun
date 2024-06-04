cmake_minimum_required(VERSION 3.28)
project(GraphsFun)

set(CMAKE_CXX_STANDARD 17)

add_executable(GraphsFun src/main.cpp
        src/Utilities.cpp
        src/Utilities.h
        src/Graph.cpp
        src/Graph.h
        src/List.h
        src/Vector.h
        src/PriorityQueue.h
        src/HashSet.h
        src/HashMap.h
)
