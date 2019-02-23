#ifndef __TYPES_H__
#   define __TYPES_H__

    struct Stack;
    struct RStack;
    struct Word;
    struct Wordlist;

    enum Mode {
        Interpret_Mode = 0,
        Compiler_Mode = 1,
    };

    typedef struct {
        struct Stack *stack;
        struct RStack *rstack;
        struct Word *last_defined_word;
        enum Mode mode;
        FILE *stream;
        struct Wordlist *wl;
    } Metadata;

    typedef int64_t Cell;
    typedef void(*Func)(Metadata*);

#   define pop(WHICH) pop_ ## WHICH
#   define push(WHICH) push_ ## WHICH

    typedef struct Stack {
        size_t size;
        size_t index;
        Cell *cells;
    } Stack;

    typedef struct RStack {
        size_t size;
        size_t index;
        Func **fn;
    } RStack;

    typedef struct Word {
        bool primitive;
        char *name;
        Func fn;
        bool immediate;
    } Word;

    Cell pop_Stack(Stack* stack);
    Stack *push_Stack(Stack *stack, Cell cell);
    Cell pick(Stack *stack, Cell n);
    Func *pop_RStack(RStack *stack);
    RStack *push_RStack(RStack *stack, Func *fn);
    Word *make_word(char *name, Func fn, bool primitive);

#   define MAKE_WORD(NAME, FN) make_word(NAME, (Func)FN, false)
#   define MAKE_PRIM(NAME, FN) make_word(NAME, FN, true)

#endif