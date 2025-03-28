
if(NOT TARGET lib_compiler_flags)
    # Compiler flags through an linked library.
    include(cmake/base_compile_options.cmake)
endif()

# sub-division of main sources into compile units can be called module or component
add_subdirectory(module1)


# NB: The creation of executables is put here in a macro considering that they have exactly the same prerequisites
# (just the 2 at the end changes). If only one executable is needed, take the code out of the macro and let it be
# the main code of CMakeLists

macro(create_executable_pfl)
    if (ARGC EQUAL 0)
        # Just keep this part of the if for only one executable ( and the rest of the macro)
        set(APP_NAME "product_conf")
    else()
        # Do not keep this part for one executable
        set(APP_NAME "product_conf${ARGV0}")
    endif()

    set(${APP_NAME}_SOURCES
        ${APP_NAME}.main.cpp
        )

    add_executable(${APP_NAME} ${${APP_NAME}_SOURCES})

    target_include_directories(${APP_NAME}
        PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
        )

    target_link_libraries(${APP_NAME} PRIVATE lib_compiler_flags)
    target_link_libraries(${APP_NAME} PRIVATE libmodule1)

    target_link_libraries(${APP_NAME} PRIVATE submodule1)
    target_link_libraries(${APP_NAME} PRIVATE submodule2)


    find_package(esubmodule1 REQUIRED)
    target_link_libraries(${APP_NAME} PRIVATE esubmodule1)
endmacro(create_executable_pfl)



create_executable_pfl()
create_executable_pfl(2)
