#include "esubmodule2/esubmodule2.hpp"
#include <iostream>

int main (int argc, char *argv[])
{
    std::cout << " Example number 1 of using esubmodule2 library but with in-source build of the example" << std::endl;
    Calculator calc;
    std::cout << "Hello World 12+7 = " << calc.add(12, 7) << "\n";
    return 0;
}
