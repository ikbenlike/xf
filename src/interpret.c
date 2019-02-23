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
                        Func *x = calloc(2, sizeof(Func));
                        if(x == NULL){
                            puts("Internal error.");
                            break;
                        }
                        x[0] = get_xt_from_word(md->wl, "docolon");
                        x[1] = fn;
                        push(RStack)(md->rstack, x);
                        execute(md);
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