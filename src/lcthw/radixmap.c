/*
 * Based on code by Andre Reinald then heavily modified by Zed A. Shaw
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <lcthw/radixmap.h>
#include <lcthw/dbg.h>

RadixMap *RadixMap_create(size_t max) {
    RadixMap *map = calloc(sizeof(RadixMap), 1);
    check_mem(map);

    map->contents = calloc(sizeof(RMElement), max + 1);
    check_mem(map->contents);

    map->temp = calloc(sizeof(RMElement), max + 1);
    check_mem(map->temp);

    map->max = max;
    map->end = 0;
    map->high = 0;

    return map;

error:
    return NULL;
}

void RadixMap_destroy(RadixMap *map) {
    if(map) {
        free(map->contents);
        free(map->temp);
        free(map);
    }
}

#define ByteOf(x, y) (((uint8_t *)x)[(y)])

static inline void radix_sort(short offset, uint64_t max,
        uint64_t *source, uint64_t *dest) {
    uint64_t count[256] = {0};
    uint64_t *cp = NULL;
    uint64_t *sp = NULL;
    uint64_t *end = NULL;
    uint64_t s = 0;
    uint64_t c = 0;

    // count occurences of every byte value
    for(sp = source, end = source + max; sp < end; sp++) {
        count[ByteOf(sp, offset)]++;
    }

    // transform count into index by summing elements and storing into same
    // array
    for(s = 0, cp = count, end = count + 256; cp < end; cp++) {
        c = *cp;
        *cp = s;
        s += c;
    }

    // fill dest with right values in the right place
    for(sp = source, end = source + max; sp < end; sp++) {
        cp = count + ByteOf(sp, offset);
        dest[*cp] = *sp;
        ++(*cp);
    }
}

static inline uint8_t getByte(uint32_t x, int n) {
    return (x >> (n << 3)) & 0xff;
}

void RadixMap_sort(RadixMap *map, int start, int end) {
    if(map->high == 0) return;
    uint64_t *source = &map->contents[start].raw;
    uint64_t *temp = &map->temp[start].raw;

    uint8_t highest = 3;
    while(getByte(map->high, highest) == 0x00) {
        highest--;
    }

    uint8_t i = 0;
    for(i = 0; i <= highest; i++) {
        if(!(i & 1)) {
            radix_sort(i, end, source, temp);
        }
        else {
            radix_sort(i, end, temp, source);
        }
    }

    if(!(highest & 1)) {
        memcpy(source, temp, sizeof(RMElement) * (map->end - 1));
    }
}

int RadixMap_bisect(RadixMap *map, uint32_t to_find) {
    int low = 0;
    int high = map->end - 1;
    RMElement *data = map->contents;

    while(low <= high) {
        int middle = low + (high - low)/2;
        uint32_t key = data[middle].data.key;

        if(to_find < key) {
            high = middle - 1;
        } else if(to_find > key) {
            low = middle + 1;
        } else {
            return middle;
        }
    }

    return -1;
}

int RadixMap_bisect_left(RadixMap *map, uint32_t to_find) {
    int low = 0;
    int high = map->end - 1;
    RMElement *data = map->contents;

    while(low <= high) {
        int middle = low + (high - low)/2;
        uint32_t key = data[middle].data.key;

        if(to_find < key) {
            high = middle - 1;
        } else if(to_find > key) {
            low = middle + 1;
        } else {
            return middle;
        }
    }

    return high;
}

int RadixMap_bisect_right(RadixMap *map, uint32_t to_find) {
    int low = 0;
    int high = map->end - 1;
    RMElement *data = map->contents;

    while(low <= high) {
        int middle = low + (high - low)/2;
        uint32_t key = data[middle].data.key;

        if(to_find < key) {
            high = middle - 1;
        } else if(to_find > key) {
            low = middle + 1;
        } else {
            return middle;
        }
    }

    return high;
}

RMElement *RadixMap_find(RadixMap *map, uint32_t to_find) {
    RMElement *data = map->contents;

    int index = RadixMap_bisect(map, to_find);
    if(index != -1) return &data[index];

    return NULL;
}

int RadixMap_add_no_sort(RadixMap *map, uint32_t key, uint32_t value) {
    check(key < UINT32_MAX, "Key can't be equal to UINT32_MAX.");
    if(key > map->high)
        map->high = key;

    RMElement element = {.data = {.key = key, .value = value}};
    check(map->end + 1 < map->max, "RadixMap is full.");

    map->contents[map->end++] = element;


    return 0;

error:
    return -1;
}

int RadixMap_add(RadixMap *map, uint32_t key, uint32_t value) {
    check(RadixMap_add_no_sort(map, key, value) == 0, "Value add failed.");

    int start = RadixMap_bisect_left(map, key);

    RadixMap_sort(map, start, map->end);

    return 0;

error:
    return -1;
}

int RadixMap_delete(RadixMap *map, RMElement *el) {
    check(map->end > 0, "There is nothing to delete.");
    check(el != NULL, "Can't delete NULL element.");

    el->data.key = UINT32_MAX;

    if(map->end > 1) {
        // don't mother resorting a map with length 1
        RadixMap_sort(map, 0, map->end);
    }

    map->end--;

    return 0;

error:
    return -1;
}
