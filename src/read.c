#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "read.h"

void skip_whitespace(FILE *stream){
    int c = getc(stream);
    if(isspace(c)){
        while(isspace(c)){
            c = getc(stream);
        }
    }
    ungetc(c, stream);
}

char *resize_buffer(char *buffer, size_t *s, size_t index){
    size_t size = *s;
    size += 64;
    char *tmp = realloc(buffer, size);
    if(tmp == NULL){
        return NULL;
    }
    *s = size;
    return tmp;
}

char *downsize_buffer(char *buffer, size_t *s, size_t index){
    size_t size = *s;
    if(index < size){
        size_t diff = size - index;
        char *tmp = realloc(buffer, size - diff + 1);
        if(tmp == NULL){
            return buffer;
        }
        *s = size - diff;
        tmp[*s] = '\0';
        return tmp;
    }

    return NULL;
}

char *get_word(FILE *stream){
    char *word = calloc(64, sizeof(char));
    size_t wl = 64;
    size_t wi = 0;
    skip_whitespace(stream);

    for(int c = getc(stream); !isspace(c) && c != EOF; c = getc(stream)){
        if(wi < wl){
            word[wi++] = (char)c;
        }
        else {
            char *tmp = resize_buffer(word, &wl, wi);
            if(tmp == NULL){
                free(word);
                return NULL;
            }
            word[wi++] = (char)c;
        }
    }

    return downsize_buffer(word, &wl, wi);
}

enum Type classify_word(char *word){
    size_t l = strlen(word);

    if(word[0] != '-' && !isdigit(word[0])){
        return Default;
    }

    for(size_t i = 0; i < l; i++){
        if(!isdigit(word[i])){
            return Default;
        }
    }

    return Number;
}

char *parse(FILE *stream, char test, size_t *length){
    size_t size = 512;
    const size_t increment = size;
    char *buffer = calloc(size, sizeof(char));
    size_t i = 0;
    for(int c = getc(stream); c != EOF && (char)c != test; c = getc(stream)){
        buffer[i++] = (char)c;
        if(i == size){
            char *tmp = realloc(buffer, size + increment);
            if(!tmp){
                *length = i;
                return buffer;
            }
            buffer = tmp;
            memset(buffer + size, 0, increment);
            size += increment;
        }
    }
    size_t l = strlen(buffer);
    char *tmp = realloc(buffer, size - (size - l));
    if(!tmp){
        *length = size;
        return buffer;
    }
    buffer = tmp;
    *length = l;
    return buffer;
}