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

# Define the client (must be specified when configuring CMake)
if(NOT DEFINED CLIENT)
    message(FATAL_ERROR "CLIENT variable not set! Use -DCLIENT=toto or -DCLIENT=titi when configuring CMake.")
endif()

# Append _${CLIENT} to specific sources and headers
set(FINAL_SOURCES ${COMMON_SOURCES})
set(FINAL_HEADERS ${COMMON_HEADERS})

foreach(SPECIFIC_FILE IN LISTS SPECIFIC_SOURCES)
    list(APPEND FINAL_SOURCES ${SPECIFIC_FILE}_${CLIENT}.cpp)
endforeach()

# Display final sources
message(STATUS "Final sources: ${FINAL_SOURCES}")
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)

set(RENAMED_HEADERS)
set(SPECIFIC_HEADER_NAMES)
set(UNSPECIFIC_HEADER_NAMES)
foreach(SPECIFIC_FILE IN LISTS SPECIFIC_HEADERS)
    set(SOURCE_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/${SPECIFIC_FILE}_${CLIENT}.hpp)
    set(DEST_HEADER ${CMAKE_BINARY_DIR}/${SPECIFIC_FILE}.hpp)

    # Ensure the source header exists before copying
    if(NOT EXISTS ${SOURCE_HEADER})
        message(FATAL_ERROR "Required header file '${SOURCE_HEADER}' does not exist!")
    endif()

    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)
    # Use configure_file to copy and rename headers at configuration time
    # configure_file(${SOURCE_HEADER} ${DEST_HEADER} COPYONLY)

    # Ensure the directory for DEST_HEADER exists
    get_filename_component(DEST_DIR ${DEST_HEADER} DIRECTORY)
    file(MAKE_DIRECTORY ${DEST_DIR})  # Create the necessary directory structure

    message(STATUS "Creating symlink from ${DEST_HEADER} to ${SOURCE_HEADER}")
    # file(CREATE_LINK ${DEST_HEADER} ${SYMLINK_PATH} SYMBOLIC)
    file(CREATE_LINK ${SOURCE_HEADER} ${DEST_HEADER}  SYMBOLIC)

    list(APPEND RENAMED_HEADERS ${SOURCE_HEADER})
    # list(APPEND SPECIFIC_HEADER_NAMES ${SPECIFIC_FILE}_${CLIENT}.hpp)
    # list(APPEND UNSPECIFIC_HEADER_NAMES ${SPECIFIC_FILE}.hpp)
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

install(CODE "set(SPECIFIC_HEADERS \"${SPECIFIC_HEADERS}\")")
install(CODE "set(CLIENT \"${CLIENT}\")")

# ---- Install Post-Install Script with the headers list ----
install(FILES ${HEADERS_LIST_FILE} DESTINATION lib/cmake/${PROJECT_NAME})

install(SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/tools/compilation/PostInstall.cmake)
