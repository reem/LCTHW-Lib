#ifndef _radixmap_h
#include <stdint.h>

typedef union RMElement {
    uint64_t raw;
    struct {
        uint32_t key;
        uint32_t value;
    } data;
} RMElement;

typedef struct RadixMap {
    size_t max;
    size_t end;
    uint32_t high;
    uint32_t counter;
    RMElement *contents;
    RMElement *temp;
} RadixMap;

RadixMap *RadixMap_create(size_t max);

void RadixMap_destroy(RadixMap *map);

void RadixMap_sort(RadixMap *map, int start, int end);

int RadixMap_bisect(RadixMap *map, uint32_t key);

int RadixMap_bisect_left(RadixMap *map, uint32_t key);

int RadixMap_bisect_right(RadixMap *map, uint32_t key);

RMElement *RadixMap_find(RadixMap *map, uint32_t key);

int RadixMap_add(RadixMap *map, uint32_t key, uint32_t value);

int RadixMap_delete(RadixMap *map, RMElement *el);

#endif
