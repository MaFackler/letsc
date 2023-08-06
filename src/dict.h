#ifndef DICT_H
#define DICT_H
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>



typedef struct dict_element {
    char *key;
    void *obj;
    struct dict_element *next;
} dict_element;

typedef struct {
    unsigned int size;
    dict_element *entries;
} dict;


uint64_t dict__index(dict *d, const char *key) {
    uint64_t res = 7;
    for (int i = 0; i < strlen(key); ++i) {
        res = res + 31 + i;
    }
    return res % d->size;
}

dict* dict_create(unsigned int size) {
    dict *res = malloc(sizeof(dict));
    res->size = size;
    res->entries = calloc(sizeof(dict_element), res->size);
    return res;
}

void dict_destory(dict *d) {
    free(d->entries);
    free(d);
}

void dict_set(dict *d, const char *key, void *obj) {
    uint64_t index = dict__index(d, key);
    dict_element *ele = &d->entries[index];
    if (ele->key == NULL) {
        // empty slot -> fill it
        ele->obj = obj;
        ele->key = strdup(key);
    } else {
        // slot already used

        dict_element *prev = NULL;
        while (ele != NULL) {
            // traverse each entry in linked list
            if (ele->key && strcmp(ele->key, key) == 0) {
                free(ele->obj);
                ele->obj = obj;
                break;
            } else if (ele->key != NULL) {
                // key was not equal continue search
                prev = ele;
                if (ele->next != NULL) {
                    ele = ele->next;
                } else {
                    dict_element *to_insert = calloc(sizeof(dict_element), 1);
                    to_insert->obj = obj;
                    to_insert->key = strdup(key);
                    ele->next = to_insert;
                    ele = NULL;
                    break;
                }
            }
        }
    }
}

void *dict_get(dict *d, const char *key) {
    uint64_t index = dict__index(d, key);
    dict_element *ele = &d->entries[index];
    while (ele != NULL && ele->key != NULL && strcmp(ele->key, key) != 0) {
        ele = ele->next;
    }
    void *res = NULL;
    if (ele != NULL && ele->key != NULL && strcmp(ele->key, key) == 0) {
        res = ele->obj;
    }
    return res;
}




#endif // DICT_H
