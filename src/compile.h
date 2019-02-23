#ifndef __COMPILE_H__
#   define __COMPILE_H__

    Func get_xt_from_word(Wordlist *wl, char *word);
    bool word_is_primitive(Wordlist *wl, char *word);
    Func *compile(char *name, Metadata *md);

#endif