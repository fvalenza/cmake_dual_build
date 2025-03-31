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
    include/submodule1/submod
    include/submodule1/submod2
)

set(SPECIFIC_HEADERS_EXT
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
    message(STATUS "SOURCE_DIR: ${SOURCE_DIR}")
    message(STATUS "SOURCE_NAME: ${SOURCE_NAME}")
    message(STATUS "SOURCE_EXT: ${SOURCE_EXT}")
    set(SOURCE_CLIENT ${SOURCE_DIR}/${SOURCE_NAME}_${CLIENT}${SOURCE_EXT})
    message(STATUS "SOURCE_CLIENT: ${SOURCE_CLIENT}")

    list(APPEND FINAL_SOURCES ${SOURCE_CLIENT})
endforeach()

# Display final sources
message(STATUS "Final sources: ${FINAL_SOURCES}")
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)

# foreach(SPECIFIC_FILE IN LISTS SPECIFIC_HEADERS_EXT)
#     get_filename_component(HEADER_DIR ${SPECIFIC_FILE} DIRECTORY)
#     get_filename_component(HEADER_NAME ${SPECIFIC_FILE} NAME_WE)
#     get_filename_component(HEADER_EXT ${SPECIFIC_FILE} LAST_EXT)
#     message(STATUS "HEADER_DIR: ${HEADER_DIR}")
#     message(STATUS "HEADER_NAME: ${HEADER_NAME}")
#     message(STATUS "HEADER_EXT: ${HEADER_EXT}")
#
#     set(SPECIFIC_FILE_CLIENT ${HEADER_DIR}/${HEADER_NAME}_${CLIENT}${HEADER_EXT})
#     message(STATUS "SPECIFIC_FILE_CLIENT: ${SPECIFIC_FILE_CLIENT}")
#     message(STATUS "SPECIFIC_FILE: ${SPECIFIC_FILE}")
#
#
# endforeach()

set(RENAMED_HEADERS)
set(SPECIFIC_HEADER_NAMES)
set(UNSPECIFIC_HEADER_NAMES)
foreach(HEAD IN LISTS SPECIFIC_HEADERS_EXT)
    set(SOURCE_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/${HEAD}_${CLIENT}.hpp)
    set(DEST_HEADER ${CMAKE_BINARY_DIR}/${HEAD}.hpp)

    get_filename_component(HEADER_DIR ${HEAD} DIRECTORY)
    get_filename_component(HEADER_NAME ${HEAD} NAME_WE)
    get_filename_component(HEADER_EXT ${HEAD} LAST_EXT)
    message(STATUS "HEADER_DIR: ${HEADER_DIR}")
    message(STATUS "HEADER_NAME: ${HEADER_NAME}")
    message(STATUS "HEADER_EXT: ${HEADER_EXT}")

    set(HEADER_CLIENT ${HEADER_DIR}/${HEADER_NAME}_${CLIENT}${HEADER_EXT})
    message(STATUS "HEADER_CLIENT: ${HEADER_CLIENT}")
    message(STATUS "HEAD: ${HEAD}")
    set(SOURCE_HEAD ${CMAKE_CURRENT_SOURCE_DIR}/${HEADER_CLIENT})
    set(SOURCE_HEADER ${SOURCE_HEAD}) # duplicate to remove
    message(STATUS "SOURCE_HEAD: ${SOURCE_HEAD}")
    set(DEST_HEADER ${CMAKE_BINARY_DIR}/${HEAD})
    message(STATUS "DEST_HEADER: ${DEST_HEADER}")

    # Ensure the source header exists before copying
    if(NOT EXISTS ${SOURCE_HEADER})
        message(FATAL_ERROR "Required header file '${SOURCE_HEADER}' does not exist!")
    endif()

    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)

    if(CMAKE_HOST_WIN32)
        message(STATUS "Make accessible specific files from build/ Copying file from ${SOURCE_HEADER} to ${DEST_HEADER}")
        configure_file(${SOURCE_HEADER} ${DEST_HEADER} COPYONLY)
    else()
        get_filename_component(DEST_DIR ${DEST_HEADER} DIRECTORY) # Ensure the directory for DEST_HEADER exists
        file(MAKE_DIRECTORY ${DEST_DIR})  # Create the necessary directory structure

        message(STATUS "Make accessible specific files from build/ Creating symlink from ${DEST_HEADER} to ${SOURCE_HEADER}")
        file(CREATE_LINK ${SOURCE_HEADER} ${DEST_HEADER}  SYMBOLIC)
    endif()

    list(APPEND RENAMED_HEADERS ${SOURCE_HEADER})
    # list(APPEND SPECIFIC_HEADER_NAMES ${HEAD}_${CLIENT}.hpp)
    # list(APPEND UNSPECIFIC_HEADER_NAMES ${HEAD}.hpp)
endforeach()

message(STATUS "Renamed headers: ${RENAMED_HEADERS}")
# message(STATUS "specific header names: ${SPECIFIC_HEADER_NAMES}")
# message(STATUS "unspecific header names: ${UNSPECIFIC_HEADER_NAMES}")

# Concatenate common headers with renamed headers
set(${PROJECT_NAME}_SOURCES ${FINAL_SOURCES})
set(${PROJECT_NAME}_PUBLIC_HEADERS ${COMMON_HEADERS} ${RENAMED_HEADERS})

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

install(CODE "set(SPECIFIC_HEADERS_EXT \"${SPECIFIC_HEADERS_EXT}\")")
install(CODE "set(CLIENT \"${CLIENT}\")")

# ---- Install Post-Install Script with the headers list ----
install(FILES ${HEADERS_LIST_FILE} DESTINATION lib/cmake/${PROJECT_NAME})

install(SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/tools/compilation/PostInstall.cmake)
