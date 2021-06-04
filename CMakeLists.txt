PROJECT(BACKTEST)
 
cmake_minimum_required(VERSION 2.8)
 
add_definitions(-std=c++17)
 
SET(CompilerSetting "clang")

SET(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
include_directories(${PROJECT_SOURCE_DIR}/include)
# add_subdirectory(rapidjson)
AUX_SOURCE_DIRECTORY(./ DIR_SRCS)
AUX_SOURCE_DIRECTORY(./src DIR_SRCS)
# add_executable(BACKTEST ${DIR_SRCS})
add_library ( BACKTEST STATIC ${DIR_SRCS} )
# TARGET_LINK_LIBRARIES(BACKTEST BACKTEST.a)


# cmake_minimum_required(VERSION 3.0)
# project(output_cmake)
# set(CMAKE_CXX_STANDARD 11)
# INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
# LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
# add_executable(output_cmake main.cpp)
# TARGET_LINK_LIBRARIES(output_cmake libfunction.a)
