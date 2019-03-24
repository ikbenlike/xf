#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#include "types.h"
#include "read.h"
#include "wordlist.h"
#include "compile.h"
#include "interpret.h"
#include "init_kernel.h"
#include "run.h"

void load_file(Metadata *md, char *path){
    FILE *before = md->stream;
    md->stream = fopen(path, "r");
    repl(md);
    fclose(md->stream);
    md->stream = before;
}

void repl(Metadata *md){
    char *word;
    enum Type type;
    bool prim;
    while(true){
        word = get_word(md->stream);
        type = classify_word(word);
        switch(type){
            case Default: {
                prim = word_is_primitive(md->wl, word);
                Func fn = get_xt_from_word(md->wl, word);
                if(fn != NULL){
                    if(prim){
                        fn(md);
                    }
                    else{
                        Func *x = calloc(3, sizeof(Func));
                        if(x == NULL){
                            puts("Internal error.");
                            break;
                        }
                        x[0] = get_xt_from_word(md->wl, "docolon");
                        x[1] = fn;
                        x[2] = &none;
                        push(RStack)(md->rstack, x);
                        execute(md);
                        free(x);
                    }
                }
            }
                break;
            case Number:
                push(Stack)(md->stack, (Cell)atol(word));
                break;
            default:
                puts("Wrong word!");
                break;
        }
    }
}
