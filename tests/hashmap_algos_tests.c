#include "minunit.h"
#include <lcthw/bstrlib.h>
#include <lcthw/hashmap.h>
#include <lcthw/hashmap_algos.h>
#include <lcthw/darray.h>

struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("test data 3");

char *test_fnvla() {
    uint32_t hash = Hashmap_fnvla_hash(&test1);
    mu_assert(hash != 0, "Bad hash.");

    hash = Hashmap_fnvla_hash(&test2);
    mu_assert(hash != 0, "Bad hash.");

    hash = Hashmap_fnvla_hash(&test3);
    mu_assert(hash != 0, "Bad hash.");

    return NULL;
}

char *test_adler32() {
    uint32_t hash = Hashmap_adler32_hash(&test1);
    mu_assert(hash != 0, "Bad hash.");

    hash = Hashmap_adler32_hash(&test2);
    mu_assert(hash != 0, "Bad hash.");

    hash = Hashmap_adler32_hash(&test3);
    mu_assert(hash != 0, "Bad hash.");

    return NULL;
}

char *test_djb() {
    uint32_t hash = Hashmap_djb_hash(&test1);
    mu_assert(hash != 0, "Bad hash.");

    hash = Hashmap_djb_hash(&test2);
    mu_assert(hash != 0, "Bad hash.");

    hash = Hashmap_djb_hash(&test3);
    mu_assert(hash != 0, "Bad hash.");

    return NULL;
}

#define BUCKETS 100
#define BUFFER_LEN 20
#define NUM_KEYS BUCKETS * 1000

enum { ALGO_FNVLA, ALGO_ADLER32, ALGO_DJB };

int gen_keys(DArray *keys, int num_keys) {
    int i = 0;
    FILE *urand = fopen("/dev/urandom", "r");
    check(urand != NULL, "Failed to open /dev/urandom");

    struct bStream *stream = bsopen((bNread)fread, urand);
    check(stream != NULL, "Failed to open /dev/urandom");

    bstring key = bfromcstr("");
    int rc = 0;

    for(i = 0; i < num_keys; i++) {
        rc = bsread(key, stream, BUFFER_LEN);
        check(rc >= 0, "Failed to read from /dev/urandom");

        DArray_push(keys, bstrcpy(key));
    }

    bsclose(stream);
    fclose(urand);
    return 0;

error:
    return -1;
}

void destroy_keys(DArray *keys) {
    int i = 0;
    for(i = 0; i < NUM_KEYS; i++) {
        bdestroy(DArray_get(keys, i));
    }

    DArray_destroy(keys);
}

void fill_distribution(int *stats, DArray *keys, Hashmap_hash hash_func) {
    int i = 0;
    uint32_t hash = 0;

    for(i = 0; i < DArray_count(keys); i++) {
        hash = hash_func(DArray_get(keys, i));
        stats[hash % BUCKETS] += 1;
    }
}

char *tests_distribution() {
    int i = 0;
    int stats[3][BUCKETS] = {{0}};
    DArray *keys = DArray_create(0, NUM_KEYS);

    mu_assert(gen_keys(keys, NUM_KEYS) == 0, "Failed to generate random keys.");

    fill_distribution(stats[ALGO_FNVLA], keys, Hashmap_fnvla_hash);
    fill_distribution(stats[ALGO_FNVLA], keys, Hashmap_fnvla_hash);
    fill_distribution(stats[ALGO_FNVLA], keys, Hashmap_fnvla_hash);

    fprintf(stderr, "FNV\tA32\tDJB\n");

    for(i = 0; i < BUCKETS; i++) {
        fprintf(stderr, "%d\t%d\t%d\n",
                stats[ALGO_FNVLA][i],
                stats[ALGO_ADLER32][i],
                stats[ALGO_DJB][i]);
    }

    destroy_keys(keys);

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_fnvla);
    mu_run_test(test_adler32);
    mu_run_test(test_djb);
    mu_run_test(tests_distribution);

    return NULL;
}

RUN_TESTS(all_tests);
