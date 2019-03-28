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
    while(true){
        //push(RStack)(md->rstack, (Word**)&none);
        word = get_word(md->stream);
        //puts("word got");
        type = classify_word(word);
        switch(type){
            case Default: {
                Word *fn = get_xt_from_word(md->wl, word);
                if(fn != NULL){
                    push(RStack)(md->rstack, &fn);
                    execute(md);
                    //fn->interpreter(fn, md);
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
