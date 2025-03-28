This directory is not required.

The `examples/ `directory is reserved for source files related to example and sample usage of the project. The structure and layout of this directory is not prescribed by this document.

Project maintainers must provide a way for consumers to disable the compilation of examples and samples (with a variable in the main CMake. If set to true, include subdir examples/ with a Cmakelists inside).


For big libraries/projects, you can put here some example on very specific parts/class usage.
Basically one .cpp file by "task"/functionnality that you want to highlight to users on how to use your lib.


Note: See static_lib template for integration of examples' CMakeLists with option is root CMakeLists

