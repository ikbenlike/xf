#ifndef __RUN_H__
#   define __RUN_H__

    void execute(Metadata *md);

#   define NEXT(NAME) \
    do { \
        Metadata *_n_md__ = (NAME); \
        Word *w = *_n_md__->running++; \
        w->interpreter(w, _n_md__); \
    } while(0)

#endif