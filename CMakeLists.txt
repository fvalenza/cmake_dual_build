cmake_minimum_required(VERSION 3.14)

project(
    product_conf
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://example.com"
    LANGUAGES CXX
)


OPTION (BUILD_DOC "Build documentation" ON)
OPTION (BUILD_EXTRAS "Build extras/optional programs" OFF)
OPTION (BUILD_UNIT_TESTS "Build the unitary tests" ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Generate target "doc" in case doxygen is found and BUILD_DOC is ON
include(cmake/documentation.cmake)

add_subdirectory(libs)

if(BUILD_UNIT_TESTS)
    include(cmake/CodeCoverage.cmake)
    append_coverage_compiler_flags()
    enable_testing()
    add_subdirectory(tests)
endif()

add_subdirectory(src)


if(BUILD_EXTRAS)
    add_subdirectory(extras)
endif()

