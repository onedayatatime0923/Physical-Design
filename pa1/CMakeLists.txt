cmake_minimum_required(VERSION 2.8)

project(pa1)

############################################################
# Create variables
############################################################
set(BASEPATH    ${PROJECT_SOURCE_DIR})
set(BINPATH     ${BASEPATH}/bin)
set(INCPATH     ${BASEPATH}/include)
set(LIBPATH     ${BASEPATH}/lib)
SET(SRCPATH     ${BASEPATH}/src)
############################################################
# Create compile flags
############################################################
# SET(CMAKE_CXX_FLAGS "-Wall -O3 -std=c++11")
SET(CMAKE_CXX_FLAGS "-O3 -std=c++0x -DNDEBUG")
############################################################
# process variables
############################################################
IF (NOT EXISTS "${BINPATH}")
  FILE(MAKE_DIRECTORY ${BINPATH})
ENDIF()
IF (NOT EXISTS "${INCPATH}")
  FILE(MAKE_DIRECTORY ${INCPATH})
ENDIF()
IF (NOT EXISTS "${LIBPATH}")
  FILE(MAKE_DIRECTORY ${LIBPATH})
ENDIF()
IF (NOT EXISTS "${SRCPATH}")
  FILE(MAKE_DIRECTORY ${SRCPATH})
ENDIF()
############################################################
# executable file output directory
############################################################
set(EXECUTABLE_OUTPUT_PATH ${BASEPATH})
############################################################
# include file                    
############################################################
# include directory
INCLUDE_DIRECTORIES(${INCPATH} ${SRCPATH})
############################################################
# library                         
############################################################
set(LIBRARY_OUTPUT_PATH ${LIBPATH})
# LIB
file(GLOB LIB ${LIBPATH}/*.a)
############################################################
# Create SRCLIB       
############################################################
# SRC
file(GLOB SUBDIR RELATIVE ${SRCPATH} ${SRCPATH}/*)
foreach(dir ${SUBDIR})
    IF(IS_DIRECTORY ${SRCPATH}/${dir} AND NOT ${dir} MATCHES "main")
        file(GLOB FILE ${SRCPATH}/${dir}/*)
        set(SRC ${SRC} ${FILE})
    ENDIF()
endforeach()
add_library(srclib STATIC ${SRC})
target_link_libraries(srclib PRIVATE ${LIB})
############################################################
# Create an executable
############################################################
# MAINPATH
set(MAINPATH ${SRCPATH}/main/)
# Add an executable with the above sources
add_executable(main ${MAINPATH}main.cpp)
# add_executable(test ${MAINPATH}test.cpp)
# link the new target with the target
target_link_libraries(main PRIVATE ${LIB} srclib)
# target_link_libraries(test PRIVATE ${LIB} srclib)
# link the new hello_library target with the hello_binary target
set_target_properties(main PROPERTIES OUTPUT_NAME fm)
# set_target_properties(test PROPERTIES OUTPUT_NAME test)
