#include "esubmodule2/esubmodule2.hpp"
#include "internal-esubmodule2.hpp"

int incrementCalculator(int a)
{
    return a + 1;
}

int Calculator::add(int a, int b)
{
    return a + b;
}

int Calculator::addIncrement(int a, int b)
{
    return a + incrementCalculator(b);
}

int Calculator::sub(int a, int b)
{
    return a - b;
}
