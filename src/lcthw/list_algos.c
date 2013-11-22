#include <lcthw/list_algos.h>
#include <lcthw/dbg.h>

inline void ListNode_swap(ListNode *a, ListNode *b) {
    void *temp = a->value;
    a->value = b->value;
    b->value = temp;
}

int List_bubble_sort(List *unsorted, List_val_compare cmp) {
    check(unsorted != NULL, "Tried to sort NULL.");
    if(List_count(unsorted) <= 1) return 0;

    int sorted = 1;

    do {
        sorted = 1;

        LIST_FOREACH(unsorted, first, next, cur) {
            if(cur->next) {
                if(cmp(cur->value, cur->next->value) > 0) {
                    ListNode_swap(cur, cur->next);
                    sorted = 0;
                }
            }
        }
    } while(!sorted);

    return 0;

error:
    return 1;
}

inline List *List_merge(List *left, List *right, List_val_compare cmp) {
    check((left != NULL) || (right != NULL), "Tried to merge NULL.");

    List *merged = List_create();
    void *val = NULL;

    while(List_count(left) > 0 || List_count(right) > 0) {
        if(List_count(left) > 0 && List_count(right) > 0) {
            if(cmp(List_first(left), List_first(right)) <= 0) {
                val = List_fpop(left);
            } else {
                val = List_fpop(right);
            }

            List_push(merged, val);
        } else if(List_count(left) > 0) {
            merged = List_join(merged, left);
            break;
        } else if(List_count(right) > 0) {
            merged = List_join(merged, right);
            break;
        }
    }

    return merged;

error:
    return NULL;
}


List *List_merge_sort(List *list, List_val_compare cmp) {
    if(List_count(list) < 2) {
        return list;
    } else {
        List *left = List_create();
        List *right = List_create();
        int middle = List_count(list) / 2;

        LIST_FOREACH(list, first, next, cur) {
            if(middle > 0) {
                List_push(left, cur->value);
            } else {
                List_push(right, cur->value);
            }

            middle--;
        }

        List *sort_left = List_merge_sort(left, cmp);
        List *sort_right = List_merge_sort(right, cmp);

        if(sort_left != left) List_destroy(left);
        if(sort_right != right) List_destroy(right);

        return List_merge(sort_left, sort_right, cmp);
    }
}
