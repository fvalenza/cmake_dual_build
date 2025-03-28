add_executable(example1-submodule2 example1.cpp)

target_link_libraries(example1-submodule2 PRIVATE submodule2)
