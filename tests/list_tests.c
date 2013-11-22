#include "minunit.h"
#include <lcthw/list.h>
#include <assert.h>

static List *list = NULL;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";

char *test_create() {
    list = List_create();
    mu_assert(list != NULL, "Failed to create list.");
    mu_assert(List_verify(list) == 0, "Verify failed.");
    return NULL;
}

char *test_destroy() {
    mu_assert(List_verify(list) == 0, "Verify failed.");

    int err = List_destroy(list);
    mu_assert(err == 0, "Destroy failed, see [ERROR].");

    return NULL;
}

char *test_push_pop() {
    mu_assert(List_verify(list) == 0, "Verify failed.");

    int err = List_push(list, test1);
    mu_assert(List_last(list) == test1, "Wrong last value.");
    mu_assert(err == 0, "Push failed, see [ERROR].");

    err = List_push(list, test2);
    mu_assert(List_last(list) == test2, "Wrong last value.");
    mu_assert(err == 0, "Push failed, see [ERROR].");

    err = List_push(list, test3);
    mu_assert(List_last(list) == test3, "Wrong last value.");
    mu_assert(List_count(list) == 3, "Wrong count on push.");
    mu_assert(List_first(list) == test1, "Wrong first value.");
    mu_assert(err == 0, "Push failed, see [ERROR].");

    char *val = List_pop(list);
    mu_assert(val == test3, "Wrong value on pop.");

    val = List_pop(list);
    mu_assert(val == test2, "Wrong value on pop.");

    val = List_pop(list);
    mu_assert(val == test1, "Wrong value on pop.");
    mu_assert(List_count(list) == 0, "Wrong count after pop.");

    return NULL;
}

char *test_fpush() {
    mu_assert(List_verify(list) == 0, "Verify failed.");

    int err = List_fpush(list, test1);
    mu_assert(List_first(list) == test1, "Wrong first value.");
    mu_assert(err == 0, "fpush failed, see [ERROR].");

    err = List_fpush(list, test2);
    mu_assert(List_first(list) == test2, "Wrong first value.");
    mu_assert(err == 0, "fpush failed, see [ERROR].");

    err = List_fpush(list, test3);
    mu_assert(List_first(list) == test3, "Wrong first value.");
    mu_assert(List_count(list) == 3, "Wrong count on fpush.");
    mu_assert(err == 0, "fpush failed, see [ERROR].");

    return NULL;
}

char *test_remove() {
    mu_assert(List_verify(list) == 0, "Verify failed.");

    // we only need to test the middle case since pop/fpop
    // already tests the other cases

    char *val = List_remove(list, list->first->next);
    mu_assert(val == test2, "Wrong removed element.");
    mu_assert(List_count(list) == 2, "Wrong count after remove.");
    mu_assert(List_first(list) == test3, "Wrong first after remove.");
    mu_assert(List_last(list) == test1, "Wrong last after remove.");

    return NULL;
}

char *test_fpop() {
    mu_assert(List_verify(list) == 0, "Verify failed.");
    mu_assert(List_count(list) != 0, "Wrong count before fpop.");

    char *val = List_fpop(list);
    mu_assert(val == test3, "Wrong value on fpop.");

    val = List_fpop(list);
    mu_assert(val == test1, "Wrong value on fpop.");
    mu_assert(List_count(list) == 0, "Wrong count after shift.");

    return NULL;
}

char *test_compare() {
    List *list1 = List_create();
    List *list2 = List_create();

    List_push(list1, test1);
    List_push(list1, test2);
    List_push(list1, test3);

    List_push(list2, test1);
    List_push(list2, test2);
    List_push(list2, test3);

    mu_assert(List_compare(list1, list2) == 0,
            "Compare returned non-zero for identical lists.");

    List_destroy(list1);
    List_destroy(list2);

    return NULL;
}

char *test_copy() {
    mu_assert(List_verify(list) == 0, "Verify failed.");

    int err = List_push(list, test1);
    mu_assert(err == 0, "Push failed, see [ERROR].");
    mu_assert(List_count(list) == 1, "Wrong count after push.");
    mu_assert(List_first(list) == List_last(list) && List_first(list) == test1,
            "Value is wrong after push.");

    List *copy = List_copy(list);
    mu_assert(copy != NULL, "Copy failed.");
    mu_assert(List_verify(copy) == 0, "Copy did not verify!");
    mu_assert(List_compare(list, copy) == 0, "Copy is not the same!");
    mu_assert(List_fpop(list) == List_fpop(copy),
            "Fpop returned different values for the copy.");

    mu_assert(List_verify(copy) == 0, "Verify failed.");

    err = List_clear_destroy(copy);
    mu_assert(err == 0, "Destroying the copy failed, see [ERROR].");

    return NULL;
}

char *test_join() {
    List *list = List_create();
    mu_assert(List_verify(list) == 0, "Verify failed.");

    int err = List_push(list, test1);
    mu_assert(err == 0, "Push failed, see [ERROR].");
    mu_assert(List_count(list) == 1, "Wrong count after push.");
    mu_assert(List_first(list) == List_last(list) && List_first(list) == test1,
            "Value is wrong after push.");

    List *copy = List_copy(list);
    mu_assert(copy != NULL, "Copy failed.");

    int total_count = List_count(list) + List_count(copy);
    List *joined = List_join(list, copy);
    mu_assert(List_count(joined) == total_count, "Joined count is wrong.");
    mu_assert(List_first(joined) == List_first(list), "Joined first is wrong.");
    mu_assert(List_last(joined) == List_last(copy), "Joined last is wrong.");

    List_destroy(joined);

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_push_pop);
    mu_run_test(test_fpush);
    mu_run_test(test_remove);
    mu_run_test(test_fpop);
    mu_run_test(test_copy);
    mu_run_test(test_destroy);
    mu_run_test(test_compare);
    mu_run_test(test_join);

    return NULL;
}

RUN_TESTS(all_tests);
