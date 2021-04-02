#ifndef __LIST_H__
#define __LIST_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct List_T {
    struct List_T *rest;
    void *first;
} List;

typedef void (*Apply)(void **x, void *cl);

extern List *List_push(List *list, void *x);
extern List *List_pop(List *list, void **x);
extern List *List_remove(List *list, void *x);
extern void List_free(List **list);
extern void List_map(List *list, Apply apply, void *cl);

#ifdef __cplusplus
}
#endif

#endif /* __LIST_H__ */
