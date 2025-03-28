see https://github.com/vector-of-bool/pitchfork and https://api.csswg.org/bikeshed/?force=1&url=https://raw.githubusercontent.com/vector-of-bool/pitchfork/develop/data/spec.bs#intro

## Top-level directories

build    ... Directory for the build artifacts. (Not part of the repo.)
cmake    ... Optional. Contains all user/projects CMake functions/macros
data     ... Optional. Directory for the non-source files, like INI files.
docs     ... Optional. Directory for the documentation.
examples ... Optional. Directory for the samples and the examples. Essentially useful for libraries projects
external ... Optional. Directory for packages/projects to be used by the project, but not edited as part of the project.
extras   ... Directory for the submodules/extra programs not build by default but related to main project. (extras/optional ones)
include  ... Directory for the public headers.TODO: Perhaps this directory is useful only for libraries projects. Determine if contains public headers of modules (from src/) ? If so, it should be a lib instead of a module no ? In what cases it should not be a lib but put here the "public headers" ?
libs     ... Directory for the main __submodules__ build by default.
src      ... Directory for the source files and the private headers.
tests    ... Optional. Directory for the tests cases.
tools    ... Optional. Directory for the development utilities, such as build and refactoring scripts


## Source code
public headers go in include/
private headers go in src/, next to associated source file

File hierarchy is conserved in include/ directory


## Examples

Should have their own "root" CMakeLists.txt and not adding it as add_subdirectory(examples) because in it, we consider its a full example "built in its own repo". So the libraries should have been installed before.

## Testing

Apparently they consider differently unit tests and other tests. Unit test being placed next to src files. (tests/ subdirectory for example, or classfile.test.cpp)

## Executables
files containing main function shoud follow this convention: <app>.main.cpp, with <app> being the target in CMakeLists.txt

# Compilation
First compile and install each externals/ submodule. Then proceed to build project from root CMakeLists.txt
