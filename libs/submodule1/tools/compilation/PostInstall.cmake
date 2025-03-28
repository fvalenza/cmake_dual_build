message(STATUS "Running Post-Install: Creating symlinks for installed headers...")

# Define install prefix
set(INSTALL_DIR "${CMAKE_INSTALL_PREFIX}")

message(STATUS "List of specific Headers: ${SPECIFIC_HEADERS}")
message(STATUS "CLIENT: ${CLIENT}")

foreach(file IN LISTS SPECIFIC_HEADERS)
    get_filename_component(NAME ${file} NAME)
    get_filename_component(DIR ${file} DIRECTORY)

    # Define full installed file path
    set(INSTALLED_HEADER "${INSTALL_DIR}/${DIR}/${NAME}_${CLIENT}.hpp")

    # Define symlink path
    set(SYMLINK_PATH "${INSTALL_DIR}/${DIR}/${NAME}.hpp")

    message(STATUS "Check if INSTALLED_HEADER exists: ${INSTALLED_HEADER}")
    # Create the symlink
    if(EXISTS ${INSTALLED_HEADER})
        execute_process(COMMAND ln -s ${INSTALLED_HEADER} ${SYMLINK_PATH})
        message(STATUS "Created symlink: ${SYMLINK_PATH} -> ${INSTALLED_HEADER}")
    else()
        message(WARNING "Post-Install: Header file ${INSTALLED_HEADER} not found. Skipping symlink.")
    endif()
endforeach()

message(STATUS "Post-Install: Symlink creation completed.")
