#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>
#include <stdbool.h>

#include "types.h"
#include "read.h"
#include "wordlist.h"
#include "compile.h"
#include "interpret.h"
#include "init_kernel.h"

#define DEFINE_MATH(NAME, OPERATOR) \
    void NAME(Metadata *md){ \
        Cell b = pop(Stack)(md->stack); \
        Cell a = pop(Stack)(md->stack); \
        push(Stack)(md->stack, a OPERATOR b); \
    }

DEFINE_MATH(forth_add, +)
DEFINE_MATH(forth_sub, -)
DEFINE_MATH(forth_mul, *)
DEFINE_MATH(forth_div, /)
DEFINE_MATH(forth_mod, %)
DEFINE_MATH(equality, ==)

void forth_pick(Metadata *md){
    Cell n = pop(Stack)(md->stack);
    push(Stack)(md->stack, pick(md->stack, n));
}

void dot(Metadata *md){
    printf("%"PRId64" ", pop(Stack)(md->stack));
}

void cr(Metadata *md){
    putchar('\n');
}

void bye(Metadata *md){
    exit(0);
}

void blank(Metadata *md){
    push(Stack)(md->stack, (Cell)' ');
}

void emit(Metadata *md){
    Cell n = pop(Stack)(md->stack);
    if(isascii(n)){
        putchar(n);
    }
}

void colon(Metadata *md){
    md->mode = Compiler_Mode;
    char *name = get_word(md->stream);
    if(name == NULL || !strcmp(name, ";")){
        puts("Invalid name for defined word.");
        return;
    }
    Func *fs = compile(name, md);
    wordlist_set(md->wl, MAKE_WORD(name, fs));
    md->mode = Interpret_Mode;
}

void semicolon(Metadata *md){
    pop(RStack)(md->rstack);
}

void literal(Metadata *md){
    Func *n = pop(RStack)(md->rstack);
    push(RStack)(md->rstack, n + 1);
    push(Stack)(md->stack, (Cell)(*n));
}

void to_r(Metadata *md){
    puts(">r");
    Cell n = pop(Stack)(md->stack);
    Func *tmp = pop(RStack)(md->rstack);
    push(RStack)(md->rstack, (Func*)n);
    push(RStack)(md->rstack, tmp);
}

void from_r(Metadata *md){
    puts("r>");
    Func *tmp = pop(RStack)(md->rstack);
    Func *n = pop(RStack)(md->rstack);
    push(RStack)(md->rstack, tmp);
    push(Stack)(md->stack, (Cell)n);
}

void fetch_r(Metadata *md){
    puts("r@");
    Func *tmp = pop(RStack)(md->rstack);
    Func *n = pop(RStack)(md->rstack);
    push(RStack)(md->rstack, n);
    push(RStack)(md->rstack, tmp);
    push(Stack)(md->stack, (Cell)n);
}

void swap(Metadata *md){
    Cell b = pop(Stack)(md->stack);
    Cell a = pop(Stack)(md->stack);
    push(Stack)(md->stack, b);
    push(Stack)(md->stack, a);
}

void docolon(Metadata *md){
    Func *tmp = pop(RStack)(md->rstack);
    push(RStack)(md->rstack, tmp + 1);
    Func *w = (Func*)*tmp;
    push(RStack)(md->rstack, w);
}

Wordlist *init_kernel(){
    Wordlist *wl = calloc(1, sizeof(Wordlist));
    /*Func *_bye = calloc(2, sizeof(Func));
    _bye[0] = &bye;
    Func *_pick = calloc(2, sizeof(Func));
    _pick[0] = &forth_pick;
    Func *_add = calloc(2, sizeof(Func));
    _add[0] = &forth_add;
    Func *_sub = calloc(2, sizeof(Func));
    _sub[0] = &forth_sub;
    Func *_mul = calloc(2, sizeof(Func));
    _mul[0] = &forth_mul;
    Func *_div = calloc(2, sizeof(Func));
    _div[0] = &forth_div;
    Func *_mod = calloc(2, sizeof(Func));
    _mod[0] = &forth_mod;
    Func *_dot = calloc(2, sizeof(Func));
    _dot[0] = &dot;
    Func *_cr = calloc(2, sizeof(Func));
    _cr[0] = &cr;
    Func *_blank = calloc(2, sizeof(Func));
    _blank[0] = &blank;
    Func *_emit = calloc(2, sizeof(Func));
    _emit[0] = &emit;
    Func *_colon = calloc(2, sizeof(Func));
    _colon[0] = &colon;
    Func *_semicolon = calloc(2, sizeof(Func));
    _semicolon[0] = &semicolon;
    Func *_literal = calloc(2, sizeof(Func));
    _literal[0] = &literal;
    Func *_to_r = calloc(2, sizeof(Func));
    _to_r[0] = &to_r;
    Func *_from_r = calloc(2, sizeof(Func));
    _from_r[0] = &from_r;
    Func *_fetch_r = calloc(2, sizeof(Func));
    _fetch_r[0] = &fetch_r;
    Func *_equality = calloc(2, sizeof(Func));
    _equality[0] = &equality;
    Func *_swap = calloc(2, sizeof(Func));
    _swap[0] = &swap;
    Func *_quit = calloc(2, sizeof(Func));
    _quit[0] = &repl;
    wordlist_set(wl, make_word("bye", _bye));
    wordlist_set(wl, make_word("pick", _pick));
    wordlist_set(wl, make_word("+", _add));
    wordlist_set(wl, make_word("-", _sub));
    wordlist_set(wl, make_word("*", _mul));
    wordlist_set(wl, make_word("/", _div));
    wordlist_set(wl, make_word("%", _mod));
    wordlist_set(wl, make_word(".", _dot));
    wordlist_set(wl, make_word("cr", _cr));
    wordlist_set(wl, make_word("bl", _blank));
    wordlist_set(wl, make_word("emit", _emit));
    wordlist_set(wl, make_word(":", _colon));
    wordlist_set(wl, make_word(";", _semicolon));
    wordlist_set(wl, make_word("lit", _literal));
    wordlist_set(wl, make_word(">r", _to_r));
    wordlist_set(wl, make_word("r>", _from_r));
    wordlist_set(wl, make_word("r@", _fetch_r));
    wordlist_set(wl, make_word("=", _equality));
    wordlist_set(wl, make_word("swap", _swap));
    wordlist_set(wl, make_word("quit", _quit));*/
    wordlist_set(wl, MAKE_PRIM("bye", &bye));
    wordlist_set(wl, MAKE_PRIM("pick", &forth_pick));
    wordlist_set(wl, MAKE_PRIM("+", &forth_add));
    wordlist_set(wl, MAKE_PRIM("-", &forth_sub));
    wordlist_set(wl, MAKE_PRIM("*", &forth_mul));
    wordlist_set(wl, MAKE_PRIM("/", &forth_div));
    wordlist_set(wl, MAKE_PRIM("%", &forth_mod));
    wordlist_set(wl, MAKE_PRIM(".", &dot));
    wordlist_set(wl, MAKE_PRIM("cr", &cr));
    wordlist_set(wl, MAKE_PRIM("bl", &blank));
    wordlist_set(wl, MAKE_PRIM("emit", &emit));
    wordlist_set(wl, MAKE_PRIM(":", &colon));
    wordlist_set(wl, MAKE_PRIM(";", &semicolon));
    wordlist_set(wl, MAKE_PRIM("lit", &literal));
    wordlist_set(wl, MAKE_PRIM(">r", &to_r));
    wordlist_set(wl, MAKE_PRIM("r>", &from_r));
    wordlist_set(wl, MAKE_PRIM("r@", &fetch_r));
    wordlist_set(wl, MAKE_PRIM("=", &equality));
    wordlist_set(wl, MAKE_PRIM("swap", &swap));
    wordlist_set(wl, MAKE_PRIM("quit", &repl));
    wordlist_set(wl, MAKE_PRIM("docolon", &docolon));
    return wl;
}

bool is_primitive(Func fn){
    if(fn == &bye ||
       fn == &forth_pick ||
       fn == &forth_add  ||
       fn == &forth_sub  ||
       fn == &forth_mul  ||
       fn == &forth_div  ||
       fn == &forth_mod  ||
       fn == &dot        ||
       fn == &cr         ||
       fn == &blank      ||
       fn == &emit       ||
       fn == &colon      ||
       fn == &semicolon  ||
       fn == &literal    ||
       fn == &to_r       ||
       fn == &from_r     ||
       fn == &fetch_r    ||
       fn == &equality   ||
       fn == &swap){
        return true;
    }
    return false;
}