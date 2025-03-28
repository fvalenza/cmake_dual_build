#include <iostream>

#define VERBOSITY_USER (0x01)
#include <minicutest/minicutest.h>
#include "suites/suite1.hpp"

int main (int argc, char *argv[])
{
    std::cout << " Example number 1 of using dummy-calculator library but with in-source build of the example" << TOTO << std::endl;

    test_group_initialize(example_minicutest_ts_group);
	
	test_suite_run(mcu_suite1);

	test_group_finalize();

    return 0;
}
