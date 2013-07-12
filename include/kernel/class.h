/*
 * CLASS - a group of related objs and a way to operate on them - Michael Yenik
 *
 * "classes" provide a useful abstraction from objs. A class is meant to have
 * one obj_type and contain a collection of all objs that have that type
 * (all "instances" of that class, if you'll excuse the OO terminology).
 * Classes also provide a collection of operations that all objs of the
 * class's type should be able to do ("methods"). These are pointers to 
 *
 * Classes might be used for something like:
 * Accelerometers
 * ~ Give a set of methods like initialize, get_data, ...
 * ~ These methods change from obj to obj, each specific accelerometer instance
 *   may provide different pointers to a common per-class set of operations
 * Resources
 * ~ Give a set of methods all resources implement (read, write...)
 * ~ Each resource will wrap some device or underlying thing, and so
 *   they each have their own read/write
 *
 * CLASS API
 * struct obj *instantiate(char *name, struct class *class, void *ops, type)
 */

#ifndef _INCLUDE_KERNEL_CLASS_H
#define _INCLUDE_KERNEL_CLASS_H

#include <stdint.h>
#include <kernel/collection.h>

typedef struct class {
    void(*dtor)(struct obj *);
    struct collection instances;
    struct obj_type *type;
    struct obj      obj;
} class_t;

#define to_class(__obj) container_of((__obj), struct class, obj)

#define instantiate(name, class, ops, type) \
    __instantiate(name, class, ops, sizeof(type))

struct obj *__instantiate(char *name, struct class *class, void *ops, size_t size);

#endif /* _INCLUDE_KERNEL_CLASS_H */
