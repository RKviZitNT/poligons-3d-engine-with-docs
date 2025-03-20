cmake_minimum_required(VERSION 3.29)
project(Engine CXX)

set(CMAKE_CXX_STANDARD 17)

file(COPY "resources" DESTINATION "${CMAKE_BINARY_DIR}")

file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

find_package(SFML 3 REQUIRED COMPONENTS Graphics)

add_executable(Engine ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(Engine PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(Engine PRIVATE SFML::Graphics)