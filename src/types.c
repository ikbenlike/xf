#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include "types.h"

Cell pop_Stack(Stack* stack){
    return stack->cells[--stack->index];
}

Stack *push_Stack(Stack *stack, Cell cell){
    stack->cells[stack->index++] = cell;
    return stack;
}

Cell pick(Stack *stack, Cell n){
    return stack->cells[stack->index - n - 1];
}

Func *pop_RStack(RStack *stack){
    return stack->fn[--stack->index];
}

RStack *push_RStack(RStack *stack, Func *fn){
    stack->fn[stack->index++] = fn;
    return stack;
}

Word *make_word(char *name, Func fn, bool primitive){
    Word *word = calloc(1, sizeof(Word));
    word->primitive = primitive;
    word->name = name;
    word->fn = fn;
    return word;
}