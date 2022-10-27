cmake_minimum_required(VERSION 3.5)
project(client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Rescan project to ensure the copy is done.
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../picture.jpeg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
#set(LIB_DIR "${SRC_DIR}/..")
set(XCREATOR_DIR "${SRC_DIR}")
set(SOURCES
    # main.cpp is only used to tell cmake it has to be compiled for c++ language.
    "${XCREATOR_DIR}/main.cpp"
    )
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}/")
#target_include_directories(${PROJECT_NAME} PRIVATE "${LIB_DIR}/")
#add_library(zlib SHARED IMPORTED)
#set_target_properties(zlib PROPERTIES IMPORTED_LOCATION ${LIB_DIR}/zlib/build/libz.so)
#target_link_libraries(${PROJECT_NAME} PRIVATE zlib)


