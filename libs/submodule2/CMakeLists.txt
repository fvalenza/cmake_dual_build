cmake_minimum_required(VERSION 3.14)


project(
    submodule2
    VERSION 0.1.0
    DESCRIPTION "short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)


OPTION (BUILD_EXAMPLES "Build the examples" ON)

if(NOT TARGET lib_compiler_flags)
    # Compiler flags through an linked library.
    include(cmake/base_compile_options.cmake)
endif()


set(${PROJECT_NAME}_SOURCES
    src/submodule2.cpp
)

set(${PROJECT_NAME}_PUBLIC_HEADERS
    include/submodule2/submodule2.hpp
)

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})

set_target_properties(${PROJECT_NAME} 
    PROPERTIES PUBLIC_HEADER ${${PROJECT_NAME}_PUBLIC_HEADERS}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE lib_compiler_flags)

# ---- Preprocessor macros ----
# target_compile_definitions(${PROJECT_NAME} PUBLIC "PREPROCESSOR_MACRO")  


if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# ---- Install the library----
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
)
install(TARGETS lib_compiler_flags
    EXPORT ${PROJECT_NAME}Targets
)

# ---- Install export target and config for find_package ----
install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  DESTINATION lib/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "lib/cmake/${PROJECT_NAME}"
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}"
    COMPATIBILITY AnyNewerVersion
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION lib/cmake/${PROJECT_NAME}
  )
