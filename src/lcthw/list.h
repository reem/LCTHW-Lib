#ifndef lcthw_List_h
#define lcthw_List_h

#include <stdlib.h>

/* ListNode contains the data of each node in a doubly-linked list.
 * Contains points to the next node and the previous node to allow for
 * traversal in either direction.
 * Contains a value of any type.
 */
struct ListNode;
typedef struct ListNode {
    struct ListNode *next;
    struct ListNode *prev;
    void *value;
} ListNode;

/* List contains the first and last ListNode in a doubly-linked list, which is
 * all you need to work with the entire list and iterate from both directions.
 * Contains a pointer to the first node in the list and the last node in the
 * list.
 * Contains the length of the list.
 */
typedef struct List {
    int count;
    ListNode *first;
    ListNode *last;
} List;

// All functions that return ints return 0 for success and 1 for failure.
// All Lists are allocated using calloc and should be free()d using
// List_destroy() or List_clear_destroy() if their contents need to be free()d.

// calloc's room for a new list and returns the pointer.
List *List_create();

// free's the memory used in the List and all of its ListNodes.
int List_destroy(List *list);

// free's the memory used in storing all the ListNode's values.
int List_clear(List *list);

// Combines List_clear and List_destroy. More efficient than calling one after
// the other.
int List_clear_destroy(List *list);

// Verifies that the List does not violate several invariants.
// Used for testing.
int List_verify(List *list);

// Getters more easily defined as Macros.
#define List_count(A) ((A)->count)
#define List_first(A) ((A)->first != NULL ? (A)->first->value : NULL)
#define List_last(A) ((A)->last != NULL ? (A)->last->value : NULL)

// Pushes a new element onto the end of a List.
int List_push(List *list, void *value);

// Pulls the last element OFF the end of the List.
void *List_pop(List *list);

// Pushes a new element to the front of a List.
int List_fpush(List *list, void *value);

// Pulls the first element OFF the front of the List.
void *List_fpop(List *list);

// Removes a certain ListNode from the List.
// Used under the covers for List_pop and List_fpop.
void *List_remove(List *list, ListNode *node);

// Copies a list and returns a pointer to the copy.
List *List_copy(List *list);

// Joins a list. Frees the second list and adds all of its elements to the
// first list. Pass contents_free = 1 if the contents of the list were malloced
// and need to be freed.
List *List_join(List *left, List *right);

// Compares two lists. TAKES O(N) TIME!!!
int List_compare(List *first, List *second);
/*
 * Faster, easier iteration over Lists.
 *
 * Usage: LIST_FOREACH(List, Start, Move, Called)
 * Typically: LIST_FOREACH(list, first, next, cur)
 *
 * Sample use case:
 * int List_clear(List *list) {
 *     LIST_FOREACH(list, first, next, cur) {
 *         free(cur->value);
 *     }
 * }
 */
#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL;\
    ListNode *V = NULL;\
    for(V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif
