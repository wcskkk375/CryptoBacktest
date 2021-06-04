PROJECT(Strategy)
 
cmake_minimum_required(VERSION 2.8)
 
add_definitions(-std=c++17)
 
SET(CompilerSetting "clang")

SET(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

include_directories( ../../include ../)
link_directories("../../../build" "../")
link_libraries(BACKTEST)

AUX_SOURCE_DIRECTORY(./ DIR_SRCS)
AUX_SOURCE_DIRECTORY(../ DIR_SRCS)

add_executable(Strategy ${DIR_SRCS} )

TARGET_LINK_LIBRARIES(Strategy libBACKTEST.a)
