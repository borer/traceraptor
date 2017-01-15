#include <stdio.h>
#include "minunit.h"

char* test_bar() {

	int bar = 1000;
	mu_assert(bar == 1000, "bar should be 1000");

	return NULL;
}

char* test_all() {
	mu_suite_start();
	mu_run_test(test_bar);

	return NULL;
}

RUN_TESTS(test_all)
