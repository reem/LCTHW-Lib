#include "minunit.h"
#include <lcthw/radixmap.h>
#include <time.h>
#include <limits.h>

static int make_random(RadixMap *map) {
    size_t i = 0;

    for(i = 0; i < map->max - 1; i++) {
        uint32_t key = (uint32_t)(rand() | (rand() << 16));
        check(RadixMap_add_no_sort(map, key, i) == 0, "Failed to add key %u.", key);
    }

    return 1;
error:
    return 0;
}

static int check_order(RadixMap *map) {
    RMElement d1, d2;
    unsigned int i = 0;

    // only signal errors if any (should not be)
    for(i = 0; map->end > 0 && i < map->end-1; i++) {
        d1 = map->contents[i];
        d2 = map->contents[i+1];

        if(d1.data.key > d2.data.key) {
            debug("FAIL:i=%u, key: %u, value: %u, equals max? %d\n",
                    i, d1.data.key, d1.data.value, d2.data.key == UINT32_MAX);
            return 0;
        }
    }

    return 1;
}

static int test_search(RadixMap *map) {
    unsigned i = 0;
    RMElement *d = NULL;
    RMElement *found = NULL;

    for(i = map->end / 2; i < map->end; i++) {
        d = &map->contents[i];
        found = RadixMap_find(map, d->data.key);
        check(found != NULL, "Didn't find %u at %u.", d->data.key, i);
        check(found->data.key == d->data.key,
                "Got the wrong result: %p:%u looking for %u at %u",
                found, found->data.key, d->data.key, i);
    }

    return 1;

error:
    return 0;
}

// test for big number of element
static char *test_operations() {
    size_t N = 200;

    RadixMap *map = RadixMap_create(N);
    mu_assert(map != NULL, "Failed to make the map.");
    mu_assert(make_random(map), "Didn't make a random fake radix map.");

    RadixMap_sort(map, 0, map->end);
    mu_assert(check_order(map), "Failed to properly sort the RadixMap.");

    mu_assert(test_search(map), "Failed the search test.");
    mu_assert(check_order(map), "RadixMap didn't stay sorted after search.");

    while(map->end > 0) {
        RMElement *el = RadixMap_find(map, map->contents[map->end / 2].data.key);
        mu_assert(el != NULL, "Should get a result.");

        size_t old_end = map->end;

        mu_assert(RadixMap_delete(map, el) == 0, "Didn't delete it.");
        mu_assert(old_end - 1 == map->end, "Wrong size after delete.");

        // test that end is old value, but uint32 max so it trails off.
        mu_assert(check_order(map), "RadixMap didn't stay sorted after delete.");
    }

    RadixMap_destroy(map);

    return NULL;
}

char *test_copy() {
    size_t N = 100;
    RadixMap *src = RadixMap_create(N);
    RadixMap *copy = RadixMap_create(N);

    RadixMap_copy(src, copy);

    mu_assert(src->max == copy->max, "Didn't copy max.");
    mu_assert(src->end == copy->end, "Didn't copy end.");
    mu_assert(src->high == copy->high, "Didn't copy high.");
    mu_assert(src->low == copy->low, "Didn't copy low.");
    mu_assert(src->counter == copy->counter, "Didn't copy counter.");
    mu_assert(src->counter == copy->counter, "Didn't copy counter.");

    RadixMap_destroy(copy);
    RadixMap_destroy(src);

    return NULL;
}

char *test_speed() {
    size_t N = 100000;
    int i = 0;
    clock_t fastest = LONG_MAX;

    RadixMap *source_map = RadixMap_create(N);
    mu_assert(source_map != NULL, "Failed to make the map.");
    mu_assert(make_random(source_map), "Didn't make a random fake radix map.");

    for(i = 0; i < 100; i++) {
        RadixMap *map = RadixMap_create(N);
        mu_assert(map != NULL, "Failed to make the map.");

        mu_assert(RadixMap_copy(source_map, map) == 0,
                "Copying unsorted RadixMap failed.");

        clock_t elapsed = -clock();
        RadixMap_sort(map, 0, map->end);
        elapsed += clock();

        if(elapsed < fastest) fastest = elapsed;
        mu_assert(check_order(map), "Failed to sort the RadixMap.");

        RadixMap_destroy(map);
    }

    RadixMap_destroy(source_map);
    debug("Fastest time for size %zu: %f", N, ((float)fastest)/CLOCKS_PER_SEC);

    return NULL;
}

char *all_tests() {
    mu_suite_start();
    srand(time(NULL));

    mu_run_test(test_operations);
    mu_run_test(test_copy);
    mu_run_test(test_speed);

    return NULL;
}

RUN_TESTS(all_tests);
