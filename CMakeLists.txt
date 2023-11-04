cmake_minimum_required(VERSION 3.22)
project(Mello VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

include(cmake/CPM.cmake)
include(CTest)
enable_testing()

CPMAddPackage(
    NAME JUCE
    GITHUB_REPOSITORY juce-framework/JUCE
    GIT_TAG 7.0.8
    VERSION 7.0.8
    SOURCE_DIR ${LIB_DIR}/juce
)

if(MSVC)
    add_compile_options(/Wall /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic) 
endif()

add_subdirectory(src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-weak_reference_mismatches,weak")
include(CPack)
