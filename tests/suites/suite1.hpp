#ifndef __TEST_SUITE_1_HPP__
#define __TEST_SUITE_1_HPP__

#include "minicutest/minicutest.h"
#include "module1.hpp"

#define TOTO "FLOOO"
typedef struct
{
	int a;
	int b;
} point2D;


external_declare_test_suite(mcu_suite1);

#endif // __TEST_SUITE_1_HPP__
