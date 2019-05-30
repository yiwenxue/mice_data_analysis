cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_C_COMPILER gcc)
set( CMAKE_CXX_FLAGS "-Wall -m64 -O0" )
# set(C_STANDARD 99)
project(main)

include_directories(src)

add_library(mylib 
    src/plot.c
    src/mathematic.c
    src/file.c
    src/utility.c)

set(SRC src/main.c )

add_executable(main ${SRC})
add_executable(cosinor src/cosinor.c)
add_executable(test src/test.c)

FIND_PACKAGE(PkgConfig REQUIRED)

FIND_PACKAGE(GSL REQUIRED)
LINK_DIRECTORIES(${GSL_LIBRARIES})
INCLUDE_DIRECTORIES(${GSL_INCLUDE_DIRS})

FIND_PACKAGE(FFTW3 REQUIRED)
include_directories(${FFTW3_INCLUDE_DIR})
LINK_DIRECTORIES(${FFTW3_LIBRARIES})

TARGET_LINK_LIBRARIES(main mylib m ${GSL_LIBRARIES} ${GSL_CLBAS_LIBRARY} ${FFTW3_LIBRARIES} )
TARGET_LINK_LIBRARIES(cosinor mylib m ${GSL_LIBRARIES} ${GSL_CLBAS_LIBRARY} )
TARGET_LINK_LIBRARIES(test mylib m ${GSL_LIBRARIES} ${GSL_CLBAS_LIBRARY} )
