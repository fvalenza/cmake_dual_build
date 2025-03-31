cmake_minimum_required(VERSION 3.14)

project(
    submodule1
    VERSION 0.1.0
    DESCRIPTION "short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

OPTION(BUILD_EXAMPLES "Build the examples" ON)

if(NOT TARGET lib_compiler_flags)
    # Compiler flags through a linked library.
    include(cmake/base_compile_options.cmake)
endif()

set(CLIENT "toto")

set(COMMON_SOURCES
    src/submodule1.cpp
)
set(SPECIFIC_SOURCES
    src/submod
    src/submod2
)

set(COMMON_HEADERS
    include/submodule1/submodule1.hpp
)

set(SPECIFIC_HEADERS
    include/submodule1/submod.hpp
    include/submodule1/submod2.hpp
)


# Define the client (must be specified when configuring CMake)
if(NOT DEFINED CLIENT)
    message(FATAL_ERROR "CLIENT variable not set! Use -DCLIENT=toto or -DCLIENT=titi when configuring CMake.")
endif()

# Append _${CLIENT} to specific sources and headers
set(FINAL_SOURCES ${COMMON_SOURCES})
set(FINAL_HEADERS ${COMMON_HEADERS})

foreach(SRC IN LISTS SPECIFIC_SOURCES)
    get_filename_component(SOURCE_DIR ${SRC} DIRECTORY)
    get_filename_component(SOURCE_NAME ${SRC} NAME_WE)
    get_filename_component(SOURCE_EXT ${SRC} LAST_EXT)
    set(SOURCE_CLIENT ${SOURCE_DIR}/${SOURCE_NAME}_${CLIENT}${SOURCE_EXT})

    list(APPEND FINAL_SOURCES ${SOURCE_CLIENT})
endforeach()



file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/symlinks)

set(COPIED_FILES)
set(CLIENT_HEADERS)

message(STATUS "SPECIFIC_HEADERS: ${SPECIFIC_HEADERS}")

foreach (head ${SPECIFIC_HEADERS})
    message (STATUS "FOREACH")
    message(STATUS "head: ${head}")
    set(COPIED_FILE ${CMAKE_BINARY_DIR}/${head})
    list(APPEND COPIED_FILES ${COPIED_FILE})

    get_filename_component(HEADER_DIR ${head} DIRECTORY)
    get_filename_component(HEADER_NAME ${head} NAME_WE)
    get_filename_component(HEADER_EXT ${head} LAST_EXT)
    set(CLIENT_HEAD ${HEADER_DIR}/${HEADER_NAME}_${CLIENT}${HEADER_EXT})
    list(APPEND CLIENT_HEADERS ${CLIENT_HEAD})

    message(STATUS "Trying to copy ${CLIENT_HEAD} to ${COPIED_FILE}")

    add_custom_command(
    OUTPUT ${COPIED_FILE}
    COMMAND ${CMAKE_COMMAND} -E make_directory
            ${CMAKE_BINARY_DIR}/${HEADER_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${CLIENT_HEAD}
            ${COPIED_FILE}
    DEPENDS ${CLIENT_HEAD}
    )

    message(STATUS "Creating link from  ${CMAKE_BINARY_DIR}/symlinks/${head} --> ${CMAKE_CURRENT_SOURCE_DIR}/${CLIENT_HEAD}")
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/symlinks/${HEADER_DIR})  # Create the necessary directory structure
    file(CREATE_LINK   ${CMAKE_CURRENT_SOURCE_DIR}/${CLIENT_HEAD} ${CMAKE_BINARY_DIR}/symlinks/${head} SYMBOLIC)


endforeach()

# Create a target that depends on all copied files
add_custom_target(copy_files ALL DEPENDS ${COPIED_FILES})






# Concatenate common headers with renamed headers
set(${PROJECT_NAME}_SOURCES ${FINAL_SOURCES})
set(${PROJECT_NAME}_PUBLIC_HEADERS ${COMMON_HEADERS} ${COPIED_FILES})

# Create the static library
add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})

set_target_properties(${PROJECT_NAME}
    PROPERTIES PUBLIC_HEADER "${${PROJECT_NAME}_PUBLIC_HEADERS}"
)

# Display PUBLIC HEADERS
get_target_property(MY_PUBLIC_HEADERS ${PROJECT_NAME} PUBLIC_HEADER)
message(STATUS "MY PUBLIC HEADERS: ${MY_PUBLIC_HEADERS}")

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/symlinks>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE lib_compiler_flags)

# ---- Install the library ----
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

# ---- Write headers list to a file ----
set(HEADERS_LIST_FILE "${CMAKE_BINARY_DIR}/installed_headers.txt")
file(WRITE ${HEADERS_LIST_FILE} "")

# foreach(header IN LISTS RENAMED_HEADERS)
#     file(APPEND ${HEADERS_LIST_FILE} "${header}\n")
# endforeach()

