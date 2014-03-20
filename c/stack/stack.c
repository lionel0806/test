#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "stack.h"


void stack_new(stack_t *stack,int elem_size)
{
    if(stack == NULL)
    {
        return;
    }

    stack->logical_len = 0;
    stack->alloc_len = 4;
    stack->elem_size = elem_size;
    stack->elem = malloc(stack->alloc_len * elem_size);
    assert(stack->elem != NULL);
}


void stack_dispos(stack_t *stack)
{
    free(stack->elem);
}


void stack_pop(stack_t *stack, void *elem_addr)
{
    void *src_addr;

    stack->logical_len--;
    src_addr = (char *)stack->elem + stack->logical_len * stack->elem_size;
    memcpy(elem_addr, src_addr, stack->elem_size);
}


static void _add_size(stack_t *s)
{
    s->alloc_len *= 2;
    s->elem = realloc(s->elem, s->alloc_len * s->elem_size);
}

void stack_push(stack_t *stack, void *elem)
{
    void *dst_addr;

    if(stack->logical_len >= stack->alloc_len)
    {
        _add_size(stack);    
    }

    dst_addr = (char *)stack->elem + stack->logical_len *stack->elem_size;
    memcpy(dst_addr, elem, stack->elem_size);
    stack->logical_len++;
}


