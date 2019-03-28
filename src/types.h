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

    void i_docolon(struct Word *word, Metadata *md);
    void i_literal(struct Word *word, Metadata *md);
    void i_primitive(struct Word *word, Metadata *md);

    typedef int64_t Cell;
    typedef void(*Func)(Metadata*);
    typedef void(*Interpreter)(struct Word*, Metadata*);

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
        struct Word ***fn;
    } RStack;

    typedef struct Word {
        Interpreter interpreter;
        char *name;
        Func fn;
        bool immediate;
    } Word;

    Cell pop_Stack(Stack* stack);
    Stack *push_Stack(Stack *stack, Cell cell);
    Cell pick(Stack *stack, Cell n);
    Word **pop_RStack(RStack *stack);
    RStack *push_RStack(RStack *stack, Word **fn);
    Word *make_word(char *name, Func fn, Interpreter interpreter);

#   define MAKE_WORD(NAME, FN) make_word(NAME, (Func)FN, &i_docolon)
#   define MAKE_PRIM(NAME, FN) make_word(NAME, FN, &i_primitive)

#endif