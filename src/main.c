#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>

#include "types.h"
#include "read.h"
#include "wordlist.h"
#include "compile.h"
#include "interpret.h"
#include "init_kernel.h"

int main(int argc, char **argv){
    puts("cl");
    Metadata *md = calloc(1, sizeof(Metadata));

    Stack *stack = calloc(1, sizeof(Stack));
    stack->cells = calloc(4096, sizeof(Cell));
    stack->size = 4096;

    RStack *rstack = calloc(1, sizeof(RStack));
    rstack->fn = calloc(64, sizeof(Func*));
    rstack->size = 64;

    md->stack = stack;
    md->rstack = rstack;
    md->stream = stdin;
    md->mode = Interpret_Mode;

    Wordlist *wl = init_kernel();

    md->wl = wl;
    repl(md);
}