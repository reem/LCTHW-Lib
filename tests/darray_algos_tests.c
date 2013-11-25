#include "minunit.h"
#include <lcthw/darray_algos.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

static inline int intcmp(int **a, int **b) {
    return **a - **b;
}

static inline int sintcmp(int *a, int *b) {
    return *a - *b;
}

int make_random(DArray *array, size_t n) {
    srand(time(NULL));
    size_t i = 0;
    for(i = 0; i < n; i++) {
        int *random = DArray_new(array);
        *random = rand();
        check(DArray_push(array, random) == 0, "Inserting random values failed.");
    }

    return 0;

error:
    return -1;
}

int is_sorted(DArray *array, DArray_compare cmp) {
    int i = 0;
    for(i = 0; i < DArray_count(array) - 1; i++) {
        if(cmp(DArray_get(array, i), DArray_get(array, i+1)) > 0) {
            return 0;
        }
    }

    return 1;
}

char *run_sort_test(int (*func)(DArray *, DArray_compare), const char *name) {
    DArray *nums = DArray_create(sizeof(int *), 20);
    int rc = make_random(nums, 20);
    mu_assert(rc == 0, "Randomization failed.");

    mu_assert(!is_sorted(nums, (DArray_compare)sintcmp), "Numbers should start not sorted.");

    debug("--- Testing %s sorting algorithm", name);
    rc = func(nums, (DArray_compare)intcmp);
    mu_assert(rc == 0, "Sort failed.");

    mu_assert(is_sorted(nums, (DArray_compare)sintcmp), "Sort didn't sort properly.");

    DArray_clear_destroy(nums);

    return NULL;
}

char *test_qsort() {
    return run_sort_test(DArray_qsort, "qsort");
}

char *test_heapsort() {
    return run_sort_test(DArray_heapsort, "heapsort");
}

char *test_mergesort() {
    return run_sort_test(DArray_mergesort, "mergesort");
}

char *speed_sort_test(int (*func)(DArray *, DArray_compare), const char *name) {
    size_t N = 10000;
    debug("--- Testing the speed of %s", name);
    DArray *source = DArray_create(sizeof(void *), N+1);

    clock_t fastest = LONG_MAX;

    int rc = make_random(source, N);
    mu_assert(rc == 0, "Randomizing the source DArray failed.");

    int i = 0;
    for(i = 0; i < 25; i++) {
        DArray *test = DArray_create(sizeof(int *), N+1);
        rc = DArray_copy(source, test);
        mu_assert(rc == 0, "Copy failed.");

        clock_t elapsed = -clock();
        rc = func(test, (DArray_compare)intcmp);
        elapsed += clock();
        mu_assert(rc == 0, "Sort failed.");

        mu_assert(is_sorted(test, (DArray_compare)sintcmp), "Sort didn't sort properly.");

        if(elapsed < fastest) fastest = elapsed;
        DArray_destroy(test);
    }

    debug("Fastest time for sort: %s, size %zu: %f", name, N, ((float)fastest)/CLOCKS_PER_SEC);

    DArray_clear_destroy(source);
    return NULL;
}

char *test_speed_qsort() {
    return speed_sort_test(DArray_qsort, "quicksort");
}

char *test_speed_mergesort() {
    return speed_sort_test(DArray_mergesort, "mergesort");
}

char *test_speed_heapsort() {
    return speed_sort_test(DArray_heapsort, "heapsort");
}

char *test_cmp() {
    DArray *fake = DArray_create(sizeof(int), 10);
    int *num1 = DArray_new(fake);
    int *num2 = DArray_new(fake);

    *num1 = 100;
    *num2 = 20;
    mu_assert(sintcmp(num1, num2) > 0, "Comparison fails on 100, 20.");

    *num1 = 50;
    *num2 = 50;
    mu_assert(sintcmp(num1, num2) == 0, "Comparison fails on 50, 50.");

    *num1 = 30;
    *num2 = 60;
    mu_assert(sintcmp(num1, num2) < 0, "Comparison fails on 30, 60.");

    DArray_clear_destroy(fake);

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_cmp);
    mu_run_test(test_qsort);
    mu_run_test(test_heapsort);
    mu_run_test(test_mergesort);
    mu_run_test(test_speed_qsort);
    mu_run_test(test_speed_mergesort);
    mu_run_test(test_speed_heapsort);

    return NULL;
}

RUN_TESTS(all_tests);
