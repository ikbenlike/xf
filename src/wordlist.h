#ifndef __WORDLIST_H__
#   define __WORDLIST_H__

    typedef struct Wordlist {
        size_t hash;
        Word *word;
        struct Wordlist *right;
        struct Wordlist *left;
    } Wordlist;


    size_t hash_word(char *key);
    Word *wordlist_get(Wordlist* wl, char *name);
    Wordlist *wordlist_set(Wordlist *wl, Word *word);

#endif