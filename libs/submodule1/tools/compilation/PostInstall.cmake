message(STATUS "Running Post-Install: Creating symlinks for installed headers...")

# Define install prefix
set(INSTALL_DIR "${CMAKE_INSTALL_PREFIX}")

message(STATUS "List of specific Headers: ${SPECIFIC_HEADERS_EXT}")
message(STATUS "CLIENT: ${CLIENT}")

foreach(file IN LISTS SPECIFIC_HEADERS_EXT)
    get_filename_component(NAME ${file} NAME)
    get_filename_component(DIR ${file} DIRECTORY)

    get_filename_component(SOURCE_DIR ${file} DIRECTORY)
    get_filename_component(SOURCE_NAME ${file} NAME_WE)
    get_filename_component(SOURCE_EXT ${file} LAST_EXT)

    # Define full installed file path
    set(INSTALLED_HEADER "${INSTALL_DIR}/${SOURCE_DIR}/${SOURCE_NAME}_${CLIENT}${SOURCE_EXT}")

    # Define symlink path
    set(SYMLINK_PATH "${INSTALL_DIR}/${SOURCE_DIR}/${SOURCE_NAME}${SOURCE_EXT}")

    message(STATUS "Check if INSTALLED_HEADER exists: ${INSTALLED_HEADER}")
    # Create the symlink
    if(EXISTS ${INSTALLED_HEADER})
        if(CMAKE_HOST_WIN32)
            # execute_process(COMMAND ln -s ${INSTALLED_HEADER} ${SYMLINK_PATH})
            # message(STATUS "Created symlink: ${SYMLINK_PATH} -> ${INSTALLED_HEADER}")
            # file(COPY ${INSTALLED_HEADER} DESTINATION ${INSTALL_DIR}/${SOURCE_DIR})
            configure_file(${INSTALLED_HEADER} ${SYMLINK_PATH} COPYONLY)
            message(STATUS "Copied file: ${INSTALLED_HEADER} to ${INSTALL_DIR}/${SOURCE_DIR}")
        else()
            # copy the file instead of creating a symlink
            # file(COPY ${INSTALLED_HEADER} DESTINATION ${INSTALL_DIR}/${SOURCE_DIR})
            configure_file(${INSTALLED_HEADER} ${SYMLINK_PATH} COPYONLY)
            message(STATUS "Copied file: ${INSTALLED_HEADER} to ${INSTALL_DIR}/${SOURCE_DIR}")
        endif()
    else()
        message(WARNING "Post-Install: Header file ${INSTALLED_HEADER} not found. Skipping symlink.")
    endif()
endforeach()

message(STATUS "Post-Install: Symlink creation completed.")
