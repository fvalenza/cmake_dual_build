
set(${PROJECT_NAME}_TEST_SOURCES 
    suite.test.cpp
    suites/suite1.cpp
)


set(${PROJECT_NAME}_TEST_HEADERS
    suites/suite1.hpp
)

find_package(minicutest REQUIRED)

add_executable(${PROJECT_NAME}_tests ${${PROJECT_NAME}_TEST_SOURCES})

target_include_directories(${PROJECT_NAME}_tests
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}_tests PRIVATE minicutest)
target_link_libraries(${PROJECT_NAME}_tests PRIVATE libmodule1)

add_test(
    NAME ${PROJECT_NAME}_tests
    COMMAND ${PROJECT_NAME}_tests
    )
