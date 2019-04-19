cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_C_COMPILER clang)
project(main)

include_directories(src)

add_library(mylib 
    src/plot.c
    src/mathematic.c
    src/file.c)

set(SRC src/main.c )

add_executable(main ${SRC})

FIND_PACKAGE(PkgConfig REQUIRED)

FIND_PACKAGE(GSL REQUIRED)
LINK_DIRECTORIES(${GSL_LIBRARIES})
INCLUDE_DIRECTORIES(${GSL_INCLUDE_DIRS})

FIND_PACKAGE(FFTW3 REQUIRED)
include_directories(${FFTW3_INCLUDE_DIR})
LINK_DIRECTORIES(${FFTW3_LIBRARIES})

TARGET_LINK_LIBRARIES(main mylib m ${GSL_LIBRARIES} ${GSL_CLBAS_LIBRARY} ${FFTW3_LIBRARIES} )
