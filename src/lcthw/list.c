#include <lcthw/list.h>
#include <lcthw/dbg.h>

List *List_create() {
    List *new_list = calloc(1, sizeof(List));
    check_mem(new_list);

    return new_list;

error:
    return NULL;
}

int List_destroy(List *list) {
    check(list != NULL, "Tried to destroy NULL list.");

    LIST_FOREACH(list, first, next, cur) {
        if(cur->prev) {
            free(cur->prev);
        }
    }

    free(list->last);
    free(list);

    return 0;

error:
    return 1;
}

int List_clear(List *list) {
    check(list != NULL, "Tried to clear NULL list.");

    LIST_FOREACH(list, first, next, cur) {
        free(cur->value);
    }

    return 0;

error:
    return 1;
}

int List_clear_destroy(List *list) {
    check(list != NULL, "Tried to clear and destroy NULL list.");

    LIST_FOREACH(list, first, next, cur) {
        free(cur->value);
        if(cur->prev) {
            free(cur->prev);
        }
    }

    free(list->last);
    free(list);

    return 0;

error:
    return 1;
}

int List_verify(List *list) {
    check(list != NULL, "Tried to verify NULL.");
    check(List_count(list) >= 0, "Count is less than zero.");
    if(List_first(list) != NULL || List_last(list) != NULL) {
        check(List_count(list) > 0, "Count is less than 1 but first is not NULL.");
        check(List_last(list) != NULL && List_first(list) != NULL,
                "First is NULL and last is not or last is NULL and first is not.");
    }

    return 0;

error:
    return 1;
}

int List_push(List *list, void *value) {
    check(list != NULL, "Tried to push to NULL. Create a list with List_create().");

    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if(list->last == NULL) {
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        node->prev = list->last;
        list->last = node;
    }

    list->count++;

    return 0;

error:
    return 1;
}

void *List_pop(List *list) {
    ListNode *node = list->last;
    return node != NULL ? List_remove(list, node) : NULL;
}

int List_fpush(List *list, void *value) {
    check(list != NULL, "Tried to fpush to NULL.");

    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if(list->first == NULL) {
        list->first = node;
        list->last = node;
    } else {
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    }

    list->count++;

    return 0;

error:
    return 1;
}

void *List_fpop(List *list) {
    check(list != NULL, "Tried to fpop from NULL.");

    ListNode *node = list->first;
    return node != NULL ? List_remove(list, node) : NULL;

error:
    return NULL;
}

void *List_remove(List *list, ListNode *node) {
    void *result = NULL;

    check(list->first && list->last, "List is empty.");
    check(node, "node can't be NULL");

    if(node == list->first && node == list->last) {
        list->first = NULL;
        list->last = NULL;
    } else if(node == list->first) {
        list->first = node->next;
        check(list->first != NULL, "Invalid List. Has NULL first.");
        list->first->prev = NULL;
    } else if(node == list->last) {
        list->last = node->prev;
        check(list->last != NULL, "Invalid list. Has NULL last.");
        list->first->next = NULL;
    } else {
        ListNode *after = node->next;
        ListNode *before = node->prev;
        after->prev = before;
        before->next = after;
    }

    list->count--;
    result = node->value;
    free(node);

    return result;

error:
    return NULL;
}

List *List_copy(List *list) {
    check(list != NULL, "Tried to copy NULL.");

    List *list_copy = List_create();
    check(list_copy != NULL, "Creating the list copy failed.");

    LIST_FOREACH(list, first, next, cur) {
        List_push(list_copy, cur->value);
    }

    return list_copy;

error:
    return NULL;
}

List *List_join(List *left, List *right) {
    check(List_verify(left) == 0, "First list does not verify.");
    check(List_verify(right) == 0, "Second list does not verify.");

    List *new = List_create();
    new->count = left->count + right->count;
    new->first = left->first;
    new->last = right->last;
    left->last->next = right->first;

    return new;

error:
    return NULL;
}

int List_compare(List *first, List *second) {
    check(first != NULL, "Tried to compare to NULL. RESULTS ARE JUNK.");
    check(second != NULL, "Tried to compare to NULL. RESULTS ARE JUNK.");

    if(first->count > second->count) return 1;
    else if(first->count < second->count) return -1;

    int i = 0;
    ListNode *cur_first = first->first;
    ListNode *cur_second = second->first;
    for(i = 0; i < first->count; i++) {
        if(cur_first->value > cur_second->value) return 1;
        else if(cur_first->value < cur_second->value) return -1;
    }

    return 0;

error:
    return 0;
}
