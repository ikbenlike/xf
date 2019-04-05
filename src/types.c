#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include "types.h"
#include "run.h"

void i_docolon(struct Word *word, Metadata *md){
    /*puts("doc");
    Word **tmp = pop(RStack)(md->rstack);
    push(RStack)(md->rstack, tmp + 1);
    Word **w = (Word**)*tmp;
    push(RStack)(md->rstack, w);*/
    push(RStack)(md->rstack, md->running);
    md->running = word->def;
    NEXT(md);
}

void i_literal(struct Word *word, Metadata *md){
    //puts("lit");
    push(Stack)(md->stack, (Cell)word->fn);
    NEXT(md);
}

void i_primitive(struct Word *word, Metadata *md){
    //puts("prm");
    word->fn(md);
}

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

Word **pop_RStack(RStack *stack){
    return stack->fn[--stack->index];
}

RStack *push_RStack(RStack *stack, Word **fn){
    stack->fn[stack->index++] = fn;
    return stack;
}

Word *make_word(char *name, Func fn, Interpreter interpreter){
    Word *word = calloc(1, sizeof(Word));
    word->interpreter = interpreter;
    word->name = name;
    word->fn = fn;
    return word;
}