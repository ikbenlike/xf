#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <ctype.h>

#include "types.h"
#include "wordlist.h"

char *upcase_string(char *s){
    size_t ss = strlen(s);
    char *r = calloc(ss, sizeof(char));
    for(size_t i = 0; i < ss; i++){
        r[i] = (char)toupper((int)s[i]);
    }
    return r;
}

size_t hash_word(char *key){
    size_t output = 0;
    size_t length = strlen(key);
    size_t i = 0;
    while((i + 1) <= length){
        output = output * 17 + (unsigned char)key[i++];
    }
    return output;
}

Wordlist **wordlist_find_with_hash(Wordlist **wl, size_t hash){
    Wordlist *w = *wl;
    const size_t w_hash = w->hash;

    if(hash == w_hash){
        return wl;
    }
    else if(hash < w_hash && w->left != NULL){
        return wordlist_find_with_hash(&w->left, hash);
    }
    else if(hash < w_hash && w->left == NULL){
        return &w->left;
    }
    else if(hash > w_hash && w->right != NULL){
        return wordlist_find_with_hash(&w->right, hash);
    }
    else if(hash > w_hash && w->right == NULL){
        return &w->right;
    }

    return NULL;
}

Word *wordlist_get(Wordlist* wl, char *name){
    size_t hash = hash_word(upcase_string(name));
    Wordlist **wp = wordlist_find_with_hash(&wl, hash);
    if(wp != NULL && *wp != NULL){
        //printf("word {%s} with xt {%x}\n", name, (*wp)->word->fn);
        return (*wp)->word;
    }
    return NULL;
}

Wordlist *wordlist_set(Wordlist *wl, Word *word){
    size_t hash = hash_word(upcase_string(word->name));
    Wordlist **s = wordlist_find_with_hash(&wl, hash);

    if(s == NULL){
        return NULL;
    }

    Wordlist *set = *s;
        
    if(set == NULL){
        Wordlist *tmp = calloc(1, sizeof(Wordlist));
        if(tmp == NULL){
            return NULL;
        }
        *s = tmp;
        set = *s;
        set->word = calloc(1, sizeof(Word));
        if(set->word == NULL){
            free(set);
            return NULL;
        }
    }
    set->hash = hash;
    if(set->word == NULL){
        return NULL;
    }
    memcpy(set->word, word, sizeof(Word));
    return set;
}