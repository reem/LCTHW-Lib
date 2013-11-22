#ifndef lcthw_List_algos_h
#define lcthw_List_algos_h

#include <lcthw/list.h>

typedef int (*List_val_compare)(const void *a, const void *b);

int List_bubble_sort(List *list, List_val_compare cmp);

List *List_merge_sort(List *list, List_val_compare cmp);

#endif
