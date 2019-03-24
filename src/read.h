#ifndef __READ_H__
#   define __READ_H__

    enum Type {
        Number = 0,
        Default = 1,
    };

    char *get_word(FILE *stream);
    enum Type classify_word(char *word);
    char *parse(FILE *stream, char test, size_t *length);

#endif