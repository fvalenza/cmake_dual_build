#include "module1/module1.hpp"
#include "esubmodule1/esubmodule1.hpp"
#include <iostream>

#include "submodule1/submod.hpp"
#include "submodule1/submod2.hpp"


int main (int argc, char *argv[])
{
    Printer myPrinter;
    myPrinter.print("toto");
    myPrinter.print("toto", "titi");

    Calculator calc;
    std::cout << "calc esubmodule1:" << calc.add(1,1) << std::endl;

    submod_toto();
    submod_titi();

    return 0;
}
