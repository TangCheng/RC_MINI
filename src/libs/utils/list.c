#include "utils/list.h"

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

List *List_push(List *list, void *x)
{
    List *p;
    p = (List *)malloc(sizeof(List));
    p->first = x;
    p->rest = list;
    return p;
}

List *List_pop(List *list, void **x)
{
    if (list) {
        List *head = list->rest;
        if (x) {
            *x = list->first;
        }
        free(list);
        return head;
    } else {
        return list;
    }
}

List *List_remove(List *list, void *x)
{
    List *next;
    if (list && list->first == x) {
        next = list->rest;
        free(list);
        return next;
    }
    for (; list; list = next) {
        next = list->rest;
        if (x == next->first) {
            list->rest = next->rest;
            free(next);
            break;
        }
    }
    return list;
}

void List_free(List **list)
{
    List *next;
    for (; *list; *list = next) {
        next = (*list)->rest;
        free(*list);
    }
}

void List_map(List *list, Apply apply, void *cl)
{
    for (; list; list = list->rest) {
        apply(&list->first, cl);
    }
}

#ifdef __cplusplus
}
#endif
