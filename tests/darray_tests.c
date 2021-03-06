#include "minunit.h"
#include <lcthw/darray.h>

static DArray *array = NULL;
static int *val1 = NULL;
static int *val2 = NULL;

char *test_create() {
    array = DArray_create(sizeof(int), 100);

    mu_assert(array != NULL, "DArray create failed.");
    mu_assert(array->contents != NULL, "Contents were not initialized properly.");
    mu_assert(array->end == 0, "end wasn't created properly.");
    mu_assert(array->element_size == sizeof(int), "element_size if wrong.");
    mu_assert(array->max == 100, "Wrong initial max length.");

    return NULL;
}

char *test_destroy() {
    DArray_destroy(array);

    return NULL;
}

char *test_new() {
    val1 = DArray_new(array);
    mu_assert(val1 != NULL, "failed to make a new element.");

    val2 = DArray_new(array);
    mu_assert(val2 != NULL, "failed to make a new element.");

    return NULL;
}

char *test_set() {
    DArray_set(array, 0, val1);
    DArray_set(array, 1, val2);

    return NULL;
}

char *test_get() {
    mu_assert(DArray_get(array, 0) == val1, "Wrong first value.");
    mu_assert(DArray_get(array, 1) == val2, "Wrong second value.");

    return NULL;
}

char *test_remove() {
    int *val_check = DArray_remove(array, 0);
    mu_assert(val_check != NULL, "Should not get NULL.");
    mu_assert(*val_check == *val1, "Should get the first value.");
    mu_assert(DArray_get(array, 0) == NULL, "Should be gone.");
    DArray_free(val_check);

    val_check = DArray_remove(array, 1);
    mu_assert(val_check != NULL, "Should not get NULL.");
    mu_assert(*val_check == *val2, "Should get the first value.");
    mu_assert(DArray_get(array, 1) == NULL, "Should be gone.");
    DArray_free(val_check);

    return NULL;
}

char *test_expand_contract() {
    int old_max = array->max;
    DArray_expand(array);
    mu_assert((unsigned int)array->max == old_max + array->expand_rate,
            "Wrong size after expand.");

    DArray_contract(array);
    mu_assert((unsigned int)array->max == array->expand_rate + 1,
            "Should stay at the expand_rate at least.");

    DArray_contract(array);
    mu_assert((unsigned int)array->max == array->expand_rate + 1,
            "Should stay at the expand_rate at least.");

    return NULL;
}

char *test_push_pop() {
    int i = 0;
    for(i = 0; i < 1000; i++) {
        int *val = DArray_new(array);
        *val = i * 333;
        DArray_push(array, val);
    }

    mu_assert(array->max == 1201, "Wrong max size.");

    for(i = 999; i >= 0; i--) {
        int *val = DArray_pop(array);
        mu_assert(val != NULL, "Shouldn't get a NULL.");
        mu_assert(*val == i * 333, "Wrong value.");
        DArray_free(val);
    }

    return NULL;
}

char *test_copy() {
    DArray *orig = DArray_create(sizeof(int), 11);
    DArray *dest = DArray_create(sizeof(int), 11);

    int i = 0;
    for(i = 0; i < 10; i++) {
        int *el_to_add = DArray_new(orig);
        *el_to_add = i;
        mu_assert(DArray_push(orig, el_to_add) == 0, "Pushing to DArray failed.");
    }

    int rc = DArray_copy(orig, dest);
    mu_assert(rc == 0, "Copy failed.")

    mu_assert(orig->max == dest->max, "max did not copy properly.");
    mu_assert(orig->end == dest->end, "end did not copy properly.");
    mu_assert(orig->element_size == dest->element_size,
            "element_size did not copy properly.");
    mu_assert(orig->expand_rate == dest->expand_rate,
            "expand_rate did not copy properly.");

    for(i = 0; i < 10; i++) {
        int *val = DArray_get(dest, i);
        mu_assert(val != NULL, "Got NULL from copy.");
        mu_assert(*val == i,
                "Failed to copy contents correctly.");
    }

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_new);
    mu_run_test(test_set);
    mu_run_test(test_get);
    mu_run_test(test_remove);
    mu_run_test(test_expand_contract);
    mu_run_test(test_push_pop);
    mu_run_test(test_destroy);
    mu_run_test(test_copy);

    return NULL;
}

RUN_TESTS(all_tests);
