#ifndef _STACK_H_
#define _STACK_H_


typedef struct _stack_
{
    void *elem;
    int  elem_size;
    int  logical_len;
    int  alloc_len;
}stack_t;



void stack_new(stack_t *stack, int elem_size);
void stack_dispos(stack_t *stack);
void stack_pop(stack_t *stack, void *elem_addr);
void stack_push(stack_t *stack, void *elem);

#endif
