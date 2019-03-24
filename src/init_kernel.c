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
DEFINE_MATH(forth_xor, ^)
DEFINE_MATH(forth_bor, |)
DEFINE_MATH(forth_and, &)

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
    putchar(n);
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

void noname_colon(Metadata *md){
    md->mode = Compiler_Mode;
    Func *fs = compile("noname", md);
    md->mode = Interpret_Mode;
    push(Stack)(md->stack, (Cell)fs);
}

void literal(Metadata *md){
    Func *n = pop(RStack)(md->rstack);
    push(RStack)(md->rstack, n + 1);
    push(Stack)(md->stack, (Cell)(*n));
}

void to_r(Metadata *md){
    Cell n = pop(Stack)(md->stack);
    Func *tmp = pop(RStack)(md->rstack);
    push(RStack)(md->rstack, (Func*)n);
    push(RStack)(md->rstack, tmp);
}

void from_r(Metadata *md){
    Func *tmp = pop(RStack)(md->rstack);
    Func *n = pop(RStack)(md->rstack);
    push(RStack)(md->rstack, tmp);
    push(Stack)(md->stack, (Cell)n);
}

void fetch_r(Metadata *md){
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

void load(Metadata *md){
    Cell length = pop(Stack)(md->stack);
    Cell addr = pop(Stack)(md->stack);
    char *path = calloc(length + 1, sizeof(char));
    strncpy(path, (char*)addr, length);
    load_file(md, path);
    free(path);
}

void forth_exec(Metadata *md){
    Word *word = (Word*)pop(Stack)(md->stack);
    if(word->primitive){
        word->fn(md);
        return;
    }
    docolon(md);
}

void tick(Metadata *md){
    char *word = get_word(md->stream);
    //Func fn = get_xt_from_word(md->wl, word);
    push(Stack)(md->stack, (Cell)wordlist_get(md->wl, word));
}

void forth_parse(Metadata *md){
    char test = (char)pop(Stack)(md->stack);
    size_t length = 0;
    char *buffer = parse(md->stream, test, &length);
    push(Stack)(md->stack, (Cell)buffer);
    push(Stack)(md->stack, (Cell)length);
}

void type(Metadata *md){
    size_t length = (size_t)pop(Stack)(md->stack);
    char *addr = (char*)pop(Stack)(md->stack);
    for(size_t i = 0; i < length; i++){
        putchar((int)addr[i]);
    }
}

void forth_free(Metadata *md){
    void *addr = (void*)pop(Stack)(md->stack);
    free(addr);
}

void forth_char(Metadata *md){
    int c = getc(md->stream);
    push(Stack)(md->stack, (Cell)c);
}

void none(Metadata *md){
    return;
}

Wordlist *init_kernel(){
    Wordlist *wl = calloc(1, sizeof(Wordlist));
    wordlist_set(wl, MAKE_PRIM("bye", &bye));
    wordlist_set(wl, MAKE_PRIM("pick", &forth_pick));
    wordlist_set(wl, MAKE_PRIM("+", &forth_add));
    wordlist_set(wl, MAKE_PRIM("-", &forth_sub));
    wordlist_set(wl, MAKE_PRIM("*", &forth_mul));
    wordlist_set(wl, MAKE_PRIM("/", &forth_div));
    wordlist_set(wl, MAKE_PRIM("%", &forth_mod));
    wordlist_set(wl, MAKE_PRIM("=", &equality));
    wordlist_set(wl, MAKE_PRIM("xor", &forth_xor));
    wordlist_set(wl, MAKE_PRIM("or", &forth_bor));
    wordlist_set(wl, MAKE_PRIM("and", &forth_and));
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
    wordlist_set(wl, MAKE_PRIM("swap", &swap));
    wordlist_set(wl, MAKE_PRIM("quit", &repl));
    wordlist_set(wl, MAKE_PRIM("docolon", &docolon));
    wordlist_set(wl, MAKE_PRIM("load", &load));
    wordlist_set(wl, MAKE_PRIM("execute", &forth_exec));
    wordlist_set(wl, MAKE_PRIM("'", &tick));
    wordlist_set(wl, MAKE_PRIM("parse", &forth_parse));
    wordlist_set(wl, MAKE_PRIM("type", &type));
    wordlist_set(wl, MAKE_PRIM("free", &forth_free));
    wordlist_set(wl, MAKE_PRIM("char", &forth_char));
    wordlist_set(wl, MAKE_PRIM("noname:", &noname_colon));
    return wl;
}
