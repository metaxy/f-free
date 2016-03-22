cmake_minimum_required(VERSION 2.6)
project(f-free)
find_path(
    GUROBI_PATH include/gurobi_c++.h
    PATHS ~/bin/gurobi650/linux64/ /home/users/w/walger/irb-ubuntu/cluster/gruppe1/gurobi
)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.50.0 COMPONENTS graph)

option(test "Build all tests." OFF)

set( ffree_SRCS
    src/common/common.cpp
    src/common/randomize.cpp
    src/common/anyoption.cpp
    src/graph/ggraph.cpp
    src/graph/vgraph.cpp
    src/iso/reducednodemapping.cpp
    src/state/state.cpp
)

set( ffree_M_SRCS
  src/graph/mgraph.cpp
  src/state/mstate.cpp
  src/iso/vf.cpp
  src/iso/forbidden.cpp
)

set( ffree_B_SRCS
  src/graph/boostgraph.cpp
  src/state/bstate.cpp
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

include_directories(lib/vflib/include/ ${Boost_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR})
set(
  ffree_LIBS
  vflib
  ${Boost_LIBRARIES}
)

ADD_DEFINITIONS(
    -std=c++11
    -Ofast
)
add_executable(ffree_random ${ffree_SRCS} ${ffree_M_SRCS} src/state/random/state_random.cpp src/state/random/main_random.cpp)

target_link_libraries(
    ffree_random
    ${ffree_LIBS}
)
add_executable(ffree_random2 ${ffree_SRCS} ${ffree_M_SRCS} src/state/random2/state_random2.cpp src/state/random2/main_random2.cpp)

target_link_libraries(
    ffree_random2
    ${ffree_LIBS}
)
add_executable(ffree_random_half ${ffree_SRCS} ${ffree_M_SRCS} src/state/random_half/state_random_half.cpp src/state/random_half/main_random_half.cpp)

target_link_libraries(
    ffree_random_half
    ${ffree_LIBS}
)

add_executable(ffree_bottom ${ffree_SRCS} ${ffree_M_SRCS} src/state/bottom/state_bottom.cpp src/state/bottom/main_bottom.cpp)

target_link_libraries(
    ffree_bottom
    ${ffree_LIBS}
)

add_executable(ffree_bottom_random ${ffree_SRCS} ${ffree_M_SRCS} src/state/bottom_random/state_bottom_random.cpp src/state/bottom_random/main_bottom_random.cpp)

target_link_libraries(
    ffree_bottom_random
    ${ffree_LIBS}
)

add_executable(ffree_grow_reduce ${ffree_SRCS} ${ffree_M_SRCS} src/state/grow_reduce/state_grow_reduce.cpp src/state/grow_reduce/main_grow_reduce.cpp)

target_link_libraries(
    ffree_grow_reduce
    ${ffree_LIBS}
)


add_executable(ffree_grow_reduce2 ${ffree_SRCS} ${ffree_M_SRCS} src/state/grow_reduce2/state_grow_reduce2.cpp src/state/grow_reduce2/main_grow_reduce2.cpp)

target_link_libraries(
    ffree_grow_reduce2
    ${ffree_LIBS}
)

add_executable(ffree_grow_reduce3 ${ffree_SRCS} ${ffree_M_SRCS} src/state/grow_reduce3/state_grow_reduce3.cpp src/state/grow_reduce3/main_grow_reduce3.cpp)

target_link_libraries(
    ffree_grow_reduce3
    ${ffree_LIBS}
)

add_executable(ffree_grow_reduce_boost ${ffree_SRCS} ${ffree_B_SRCS} src/state/grow_reduce_boost/state_grow_reduce_boost.cpp src/state/grow_reduce_boost/main_grow_reduce_boost.cpp)

target_link_libraries(
    ffree_grow_reduce_boost
    ${ffree_LIBS}
)


add_executable(ffree_extend ${ffree_SRCS} ${ffree_M_SRCS} src/state/extend/state_extend.cpp src/state/extend/main_extend.cpp)

target_link_libraries(
    ffree_extend
    ${ffree_LIBS}
)

add_executable(ffree_extend_boost ${ffree_SRCS} ${ffree_B_SRCS} src/state/extend_boost/state_extend_boost.cpp src/state/extend_boost/main_extend_boost.cpp)

target_link_libraries(
    ffree_extend_boost
    ${ffree_LIBS}
)


add_executable(ffree_simple_random ${ffree_SRCS} ${ffree_B_SRCS} src/state/simple_random/state_simple_random.cpp src/state/simple_random/main_simple_random.cpp)

target_link_libraries(
    ffree_simple_random
    ${ffree_LIBS}
)

add_executable(ffree_approx2k ${ffree_SRCS} src/graph/mergegraph.cpp src/state/approx2k/state_approx2k.cpp src/state/approx2k/main_approx2k.cpp)

target_link_libraries(
    ffree_approx2k
    ${ffree_LIBS}
)
add_executable(isomorph ${ffree_SRCS} ${ffree_M_SRCS} src/tools/main_isomorph.cpp)

target_link_libraries(
    isomorph
    ${ffree_LIBS}
)


add_executable(benchmark_iso ${ffree_SRCS} ${ffree_M_SRCS} ${ffree_B_SRCS} src/iso/special_subgraph_isomorphism.cpp src/common/timer.cpp src/tools/main_benchmark_iso.cpp)

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
    add_executable(ffree_blp ${ffree_SRCS} ${ffree_M_SRCS} src/common/gurobilp.cpp src/state/blp/state_blp.cpp src/state/blp/main_blp.cpp)

    find_library(G1 libgurobi_g++4.2.a ${GUROBI_PATH}/lib)
    find_library(G2 libgurobi65.so ${GUROBI_PATH}/lib)

    target_link_libraries(
        ffree_blp
        ${ffree_LIBS}
        ${G1}
        ${G2}
    )
endif()

