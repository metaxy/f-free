cmake_minimum_required(VERSION 2.6)
project(f-free)
find_path(
    GUROBI_PATH include/gurobi_c++.h
    PATHS ~/bin/gurobi650/linux64/ /home/users/w/walger/irb-ubuntu/cluster/gruppe1/gurobi
)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS graph)

option(test "Build all tests." OFF)

set( ffree_SRCS
    ggraph.cpp
    vgraph.cpp
    mgraph.cpp
    mergegraph.cpp
    common.cpp
    randomize.cpp
    anyoption.cpp
    forbidden.cpp
    vf.cpp
    state.cpp
    reducednodemapping.cpp
    special_subgraph_isomorphism.cpp
    timer.cpp
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


include_directories(lib/vflib/include/)
set(
  ffree_LIBS
  vflib
  ${Boost_LIBRARIES}
)

ADD_DEFINITIONS(
    -std=c++11
    -Ofast
)
add_executable(ffree_random ${ffree_SRCS} state_random.cpp main_random.cpp)

target_link_libraries(
    ffree_random
    ${ffree_LIBS}
)
add_executable(ffree_random2 ${ffree_SRCS} state_random2.cpp main_random2.cpp)

target_link_libraries(
    ffree_random2
    ${ffree_LIBS}
)
add_executable(ffree_random_half ${ffree_SRCS} state_random_half.cpp main_random_half.cpp)

target_link_libraries(
    ffree_random_half
    ${ffree_LIBS}
)

add_executable(ffree_bottom ${ffree_SRCS} state_bottom.cpp main_bottom.cpp)

target_link_libraries(
    ffree_bottom
    ${ffree_LIBS}
)

add_executable(ffree_bottom_random ${ffree_SRCS} state_bottom_random.cpp main_bottom_random.cpp)

target_link_libraries(
    ffree_bottom_random
    ${ffree_LIBS}
)

add_executable(ffree_grow_reduce ${ffree_SRCS} state_grow_reduce.cpp main_grow_reduce.cpp)

target_link_libraries(
    ffree_grow_reduce
    ${ffree_LIBS}
)


add_executable(ffree_grow_reduce2 ${ffree_SRCS} state_grow_reduce2.cpp main_grow_reduce2.cpp)

target_link_libraries(
    ffree_grow_reduce2
    ${ffree_LIBS}
)

add_executable(ffree_grow_reduce3 ${ffree_SRCS} state_grow_reduce3.cpp main_grow_reduce3.cpp)

target_link_libraries(
    ffree_grow_reduce3
    ${ffree_LIBS}
)

add_executable(ffree_extend ${ffree_SRCS} state_extend.cpp main_extend.cpp)

target_link_libraries(
    ffree_extend
    ${ffree_LIBS}
)

add_executable(ffree_approx2k ${ffree_SRCS} state_approx2k.cpp main_approx2k.cpp)

target_link_libraries(
    ffree_approx2k
    ${ffree_LIBS}
)
add_executable(isomorph ${ffree_SRCS} main_isomorph.cpp)

target_link_libraries(
    isomorph
    ${ffree_LIBS}
)


add_executable(benchmark_iso ${ffree_SRCS} main_benchmark_iso.cpp)

target_link_libraries(
    benchmark_iso
    ${ffree_LIBS}
)



if (test)
  # This adds another subdirectory, which has 'project(gtest)'.
  add_subdirectory(lib/gtest-1.6.0)
  enable_testing()
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

  ##############
  # Unit Tests
  ##############
  add_executable(runUnitTests test_project1.cpp)
  target_link_libraries(runUnitTests gtest gtest_main)
  target_link_libraries(runUnitTests project1_lib)

  add_test(NAME that-test-I-made COMMAND runUnitTests)

  # You can also omit NAME and COMMAND. The second argument could be some other
  # test executable.
  add_test(that-other-test-I-made runUnitTests)
endif()
if(GUROBI_PATH)
    include_directories(
        ${GUROBI_PATH}/include
    )
    add_executable(ffree_blp ${ffree_SRCS} gurobilp.cpp state_blp.cpp main_blp.cpp)

    find_library(G1 libgurobi_g++4.2.a ${GUROBI_PATH}/lib)
    find_library(G2 libgurobi65.so ${GUROBI_PATH}/lib)

    target_link_libraries(
        ffree_blp
        ${ffree_LIBS}
        ${G1}
        ${G2}
    )
endif()

