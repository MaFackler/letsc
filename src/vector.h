#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct {
    size_t size;
    size_t capacity;
} VectorHeader;

#define vec_header(v) (((VectorHeader*) v) - 1)
#define vec_size(v) ((v) ? vec_header(v)->size : 0)
#define vec_capacity(v) ((v) ? vec_header(v)->capacity : 0)
#define vec_add(v) (vec__check_size(v), &(v[vec_header(v)->size++]))
#define vec_push(v, e) (*vec_add(v) = e)
#define vec_clear(v) ((v) ? (vec_header(v)->size = 0) : 0)
#define vec_free(v) (((v) ? free(vec_header(v)) : 0), v = NULL)
#define vec__check_size(v) \
    ((vec_size(v) == vec_capacity(v)) ? vec_grow((void **)&v, sizeof(*v)) : 0)

void vec_grow(void **v, size_t element_size) {
    size_t new_capacity = (vec_size(*v) + 1) * 2;
    if (new_capacity < 8) {
        new_capacity = 8;
    }
    size_t amount_bytes = sizeof(VectorHeader) + new_capacity * element_size;
    VectorHeader *header = NULL;
    if (*v) {
        header = (VectorHeader *) realloc(vec_header(*v), amount_bytes);
    } else {
        header = (VectorHeader *) malloc(amount_bytes);
        header->size = 0;
    }
    header->capacity = new_capacity;
    *v = (void *) (header + 1);
}

#endif // VECTOR_H
