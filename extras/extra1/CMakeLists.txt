add_executable(extra1-product_conf extra1.cpp)

find_package(libmodule1)
target_link_libraries(example1-product_conf PRIVATE libmodule1)
