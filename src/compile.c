#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#include "types.h"
#include "read.h"
#include "wordlist.h"
#include "compile.h"

typedef struct {
    Func *array;
    size_t size;
    size_t index;
} Funcvector;

Func get_xt_from_word(Wordlist *wl, char *word){
    Word *w = wordlist_get(wl, word);
    if(w == NULL){
        printf("Invalid {%s} word not found\n", word);
        return NULL;
    }
    return w->fn;
}

bool word_is_primitive(Wordlist *wl, char *word){
    Word *w = wordlist_get(wl, word);
    if(w == NULL){
        printf("Invalid {%s} word not primitive\n", word);
        return false;
    }
    return w->primitive;
}

Funcvector *init_fv(size_t size){
    Funcvector *fv = calloc(1, sizeof(fv));
    if(fv == NULL){
        return NULL;
    }
    fv->array = calloc(size, sizeof(Func));
    if(fv->array == NULL){
        return NULL;
    }
    fv->size = size;
    fv->index = 0;
    return fv;
}

Funcvector *append_fv(Funcvector *fv, Func f){
    puts("append_fv");
    if(fv->index + 1 < fv->size){
        puts("yeah");
        fv->array[fv->index++] = f;
        return fv;
    }
    puts("nah");

    size_t ns = fv->size + 32;
    Func *tmp = realloc(fv->array, ns);
    if(tmp == NULL){
        puts("append NULL");
        return NULL;
    }
    fv->array = tmp;
    memset(fv->array + fv->size, 0, 32 * sizeof(Func));
    fv->size = ns;
    fv->array[fv->index++] = f;

    return fv;
}

Funcvector *crop_fv(Funcvector *fv){
    if(fv->size - 1 == fv->index){
        return fv;
    }
    Func *tmp = realloc(fv->array, sizeof(Func) * (fv->index + 1));
    if(tmp == NULL){
        return NULL;
    }
    fv->array = tmp;
    fv->size = fv->index;
    return fv;
}

Funcvector *handle_default(Metadata *md, Funcvector *fv, char *name, char *word){
    Func xt = get_xt_from_word(md->wl, word);
    if(word_is_primitive(md->wl, word)){
        if(append_fv(fv, xt) == NULL){
            printf("Fatal error while compiling primitive `%s` into word `%s`.\n", word, name);
            return NULL;
        }
    }
    else {
        if(append_fv(fv, get_xt_from_word(md->wl, "docolon")) == NULL || append_fv(fv, xt) == NULL){
            printf("Fatal error while compiling word `%s` into word `%s`.\n", word, name);
            return NULL;
        }
    }
    return fv;
}

Funcvector *handle_number(Metadata *md, Funcvector *fv, char *name, char *word){
    Func litxt = get_xt_from_word(md->wl, "LIT");
    Func n = (Func)atoll(word);
    if(append_fv(fv, litxt) == NULL || append_fv(fv, n) == NULL){
        printf("Fatal error when adding literal `%s` to word `%s`.\n", word, name);
        return NULL;
    }
    return fv;
}

Func *compile(char *name, Metadata *md){
    Funcvector *fv = init_fv(64);
    if(fv == NULL){
        printf("Fatal error while compiling word `%s`.\n", name);
    }
    const Func scxt = get_xt_from_word(md->wl, ";");
    if(scxt == NULL){
        printf("Fatal error while compiling word `%s`.\n", name);
        return NULL;
    }
    while(true){
        puts("compile while");
        char *word = get_word(md->stream);
        if(word == NULL){
            printf("Error while compiling `%s`.\n", name);
            return NULL;
        }
        switch(classify_word(word)){
            case Default: {
                if(handle_default(md, fv, name, word) == NULL){
                    puts("NULL");
                    free(word);
                    return NULL;
                }
                if(get_xt_from_word(md->wl, word) == scxt){
                    free(word);
                    goto compile_finish;
                }
                break;
            }
            case Number: {
                if(handle_number(md, fv, name, word) == NULL){
                    puts("NULL");
                    free(word);
                    return NULL;
                }
                break;
            }
            default: {
                printf("Incorrect word `%s` found while compiling `%s`.\n", word, name);
                free(word);
                return NULL;
            }
        }
        free(word);
    }
compile_finish:
    if(crop_fv(fv) == NULL){
        free(fv->array);
        free(fv);
        printf("Fatal error while compiling word `%s`.\n", name);
    }
    Func *fs = fv->array;
    free(fv);
    return fs;
}