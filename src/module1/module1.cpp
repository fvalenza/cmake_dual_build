#include "module1.hpp"
#include <iostream>
#include "esubmodule2/esubmodule2.hpp"

std::string concatString(const std::string string1, const std::string string2)
{
    return string1 + string2;
}

void Printer::print(const std::string& string)
{
    std::cout << string << std::endl;
    Calculator calc;
    std::cout << "calc esubmodule2:" << calc.add(2,2) << std::endl;
}

void Printer::print(const std::string& string1, const std::string& string2)
{
    std::cout << concatString(string1, string2) << std::endl;
}
