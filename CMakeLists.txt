cmake_minimum_required(VERSION 2.6)
project(f-free)
#find_path(
#    GUROBI_PATH include/gurobi_c++.h
#    PATHS ~/bin/gurobi600/linux64/ /afs/akt.tu-berlin.de/user/pwalger/gruppe1/gurobi /home/users/w/walger/irb-ubuntu/cluster/gruppe1/gurobi
#)
set( ffree_SRCS
    graph.cpp
    mgraph.cpp
    common.cpp
    randomize.cpp
    anyoption.cpp
)
ADD_DEFINITIONS(
    -std=c++11
    -Ofast
)
add_executable(ffree ${ffree_SRCS} main.cpp)

