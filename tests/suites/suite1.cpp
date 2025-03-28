

#define VERBOSITY_USER (0x01)

#include "suite1.hpp"
#include "module1.hpp"
#include <stdio.h>




static int equal_2d_points(const point2D a, const point2D b)
{
	return ((a.a == b.a) && (a.b == b.b));
}


TEST_CASE_BEGIN(tc1)

	mcu_assert_true(1);
	mcu_assert_true(0);
	int false_toto = 0;
	mcu_assert_true(false_toto);

TEST_CASE_END()

TEST_CASE_BEGIN(tc2)

	mcu_assert(1 == 2);
	mcu_assert(1 >= 2);
	mcu_assert_message(1 == 2, "custom message");
	mcu_assert('a' >= 'b');
	mcu_assert('a' <= 'b');

TEST_CASE_END()

TEST_CASE_BEGIN(tc3)

	point2D pa = {1, 2};
	point2D pb = {3, 4};
	// mcu_assert_equal_custom_cmp(equal_2d_points, pa, pb);
	mcu_assert_not_equal_custom_cmp(equal_2d_points, pa, pb);

TEST_CASE_END()

TEST_CASE_BEGIN(tc4)
    Printer myPrinter;
    myPrinter.print("toto");
    myPrinter.print("toto", "titi");
TEST_CASE_END()



TEST_SUITE_BEGIN(mcu_suite1)

	test_case_run(tc1);
	test_case_run(tc2);
	test_case_run(tc3);
	test_case_run(tc4);

TEST_SUITE_END()
