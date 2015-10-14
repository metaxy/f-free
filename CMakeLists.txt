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
    vf.cpp
)

set( vflib_SRCS
   lib/vflib/src/argedit.cc
   lib/vflib/src/argloader.cc
   lib/vflib/src/argraph.cc
   lib/vflib/src/error.cc
   lib/vflib/src/gene.cc
   lib/vflib/src/gene_mesh.cc
   lib/vflib/src/match.cc
   lib/vflib/src/sd_state.cc
   lib/vflib/src/sortnodes.cc
   lib/vflib/src/sortnodes.h
   lib/vflib/src/ull_state.cc
   lib/vflib/src/ull_sub_state.cc
   lib/vflib/src/vf2_mono_state.cc
   lib/vflib/src/vf2_state.cc
   lib/vflib/src/vf2_sub_state.cc
   lib/vflib/src/vf_mono_state.cc
   lib/vflib/src/vf_state.cc
   lib/vflib/src/vf_sub_state.cc
   lib/vflib/src/xsubgraph.cc
)

set( vflib_INCLUDE
   lib/vflib/include/allocpool.h
   lib/vflib/include/argedit.h
   lib/vflib/include/argloader.h
   lib/vflib/include/argraph.h
   lib/vflib/include/dict.h
   lib/vflib/include/error.h
   lib/vflib/include/gene.h
   lib/vflib/include/gene_mesh.h
   lib/vflib/include/match.h
   lib/vflib/include/sd_state.h
   lib/vflib/include/state.h
   lib/vflib/include/ull_state.h
   lib/vflib/include/ull_sub_state.h
   lib/vflib/include/vf2_mono_state.h
   lib/vflib/include/vf2_state.h
   lib/vflib/include/vf2_sub_state.h
   lib/vflib/include/vf_mono_state.h
   lib/vflib/include/vf_state.h
   lib/vflib/include/vf_sub_state.h
   lib/vflib/include/xsubgraph.h
)

add_library(
    vflib
    STATIC
    ${vflib_SRCS}
    ${vflib_INCLUDE}
)

target_include_directories(vflib PUBLIC lib/vflib/include/)

include_directories(lib/vflib/include/)

set(
  ffree_LIBS
  vflib
)

ADD_DEFINITIONS(
    -std=c++11
    -Ofast
)
add_executable(ffree ${ffree_SRCS} main.cpp)

target_link_libraries(
    ffree
    ${ffree_LIBS}
)
