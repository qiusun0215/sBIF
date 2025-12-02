#####
cmake_minimum_required (VERSION 3.5)

project(sBIF)

#####Adding sources
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)

include_directories(${PROJECT_SOURCE_DIR}/src)
add_executable(sBIF ${SRC_LIST})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

if(UNIX)
    add_definitions(-std=c++11 )
else()
    add_definitions(-D_SCL_SECURE_NO_WARNINGS
    -D_CRT_SECURE_NO_WARNINGS -D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

#####Running openmp
FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
message("PENMP FOUND")
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS})
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS})
endif()

