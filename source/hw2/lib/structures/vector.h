/**
 * @file vector.h
 * @author JacobLinCool <me@jacoblin.cool> (github.com/JacobLinCool)
 * @brief Vector, Dynamic Array.
 * @version 1.0.0
 * @date 2022-03-16
 *
 * @copyright Copyright (c) 2022 JacobLinCool (MIT License)
 *
 */

#define StructVector(_name, _type, _fallback)                                   \
    typedef struct _name {                                                      \
        _type* data;                                                            \
        size_t size;                                                            \
        size_t capacity;                                                        \
        void (*insert)(struct _name*, size_t, _type);                           \
        _type (*remove)(struct _name*, size_t);                                 \
        void (*set)(struct _name*, size_t, _type);                              \
        _type (*get)(struct _name*, size_t);                                    \
        void (*push)(struct _name*, _type);                                     \
        _type (*pop)(struct _name*);                                            \
        _type (*back)(struct _name*);                                           \
        void (*unshift)(struct _name*, _type);                                  \
        _type (*shift)(struct _name*);                                          \
        _type (*front)(struct _name*);                                          \
        void (*clear)(struct _name*);                                           \
        void (*free)(struct _name*);                                            \
    } _name;                                                                    \
    void insert_##_name(_name* vector, size_t _idx, _type val) {                \
        if (vector->size == vector->capacity) {                                 \
            vector->capacity <<= 1;                                             \
            vector->data = realloc(vector->data, vector->capacity * sizeof(_type));\
        }                                                                       \
        for (size_t i = vector->size; i > _idx; i--) {                          \
            vector->data[i] = vector->data[i - 1];                              \
        }                                                                       \
        vector->data[_idx] = val;                                               \
        vector->size++;                                                         \
    }                                                                           \
    _type remove_##_name(_name* vector, size_t _idx) {                          \
        if (_idx >= vector->size || _idx < 0) {                                 \
            return _fallback;                                                   \
        }                                                                       \
        _type val = vector->data[_idx];                                         \
        for (size_t i = _idx; i < vector->size - 1; i++) {                      \
            vector->data[i] = vector->data[i + 1];                              \
        }                                                                       \
        vector->size--;                                                         \
        return val;                                                             \
    }                                                                           \
    void set_##_name(_name* vector, size_t _idx, _type val) {                   \
        if (_idx >= vector->size || _idx < 0) {                                 \
            return;                                                             \
        }                                                                       \
        vector->data[_idx] = val;                                               \
    }                                                                           \
    _type get_##_name(_name* vector, size_t _idx) {                             \
        if (_idx >= vector->size || _idx < 0) {                                 \
            return _fallback;                                                   \
        }                                                                       \
        return vector->data[_idx];                                              \
    }                                                                           \
    void push_##_name(_name* vector, _type val) {                               \
        insert_##_name(vector, vector->size, val);                              \
    }                                                                           \
    _type pop_##_name(_name* vector) {                                          \
        return remove_##_name(vector, vector->size - 1);                        \
    }                                                                           \
    _type back_##_name(_name* vector) {                                         \
        return get_##_name(vector, vector->size - 1);                           \
    }                                                                           \
    void unshift_##_name(_name* vector, _type val) {                            \
        insert_##_name(vector, 0, val);                                         \
    }                                                                           \
    _type shift_##_name(_name* vector) {                                        \
        return remove_##_name(vector, 0);                                       \
    }                                                                           \
    _type front_##_name(_name* vector) {                                        \
        return get_##_name(vector, 0);                                          \
    }                                                                           \
    void clear_##_name(_name* vector) {                                         \
        vector->size = 0;                                                       \
    }                                                                           \
    void free_##_name(_name* vector) {                                          \
        free(vector->data);                                                     \
        free(vector);                                                           \
    }                                                                           \
    _name* create_##_name() {                                                   \
        _name* vector = malloc(sizeof(_name));                                  \
        vector->size = 0;                                                       \
        vector->capacity = 64;                                                  \
        vector->data = malloc(vector->capacity * sizeof(_type));                \
        vector->insert = &insert_##_name;                                       \
        vector->remove = &remove_##_name;                                       \
        vector->set = &set_##_name;                                             \
        vector->get = &get_##_name;                                             \
        vector->push = &push_##_name;                                           \
        vector->pop = &pop_##_name;                                             \
        vector->back = &back_##_name;                                           \
        vector->unshift = &unshift_##_name;                                     \
        vector->shift = &shift_##_name;                                         \
        vector->front = &front_##_name;                                         \
        vector->clear = &clear_##_name;                                         \
        vector->free = &free_##_name;                                           \
        return vector;                                                          \
    }                                                                           
