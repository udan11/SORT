cmake_minimum_required(VERSION 2.8)
project(sort)

list(APPEND CMAKE_MODULE_PATH ".")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -O3 -Wall -Wextra -pedantic")

# ------------------------------------------------------------------------------
# Packages
# ------------------------------------------------------------------------------

# Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS filesystem log program_options system timer)
include_directories(${Boost_INCLUDE_DIRS})
add_definitions(-DBOOST_LOG_DYN_LINK)

# OpenCL
find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})

# OpenMP
find_package(OpenMP REQUIRED)

# TBB
find_package(TBB REQUIRED)

# ------------------------------------------------------------------------------
# benchmark
# ------------------------------------------------------------------------------

file(GLOB benchmark_src "src/*.cpp")
add_executable(benchmark ${benchmark_src})
target_include_directories(benchmark PRIVATE "src")
target_link_libraries(benchmark ${Boost_LIBRARIES} pthread ${OpenCL_LIBRARIES} tbb)

# ------------------------------------------------------------------------------
# experiments
# ------------------------------------------------------------------------------

file(GLOB experiments_src "src_experiments/*.cpp")
add_executable(experiments ${experiments_src})
target_compile_options(experiments PRIVATE ${OpenMP_CXX_FLAGS})
target_include_directories(experiments PRIVATE "src" "experiments")
target_link_libraries(experiments ${Boost_LIBRARIES} pthread ${OpenCL_LIBRARIES} ${OpenMP_CXX_FLAGS} tbb)
