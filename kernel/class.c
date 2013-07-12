#include <stdint.h>
#include <kernel/class.h>
#include <mm/mm.h>

struct obj *__instantiate(char *name, struct class *class, void *ops, size_t size) {
    struct obj *o;
    void *container;

    container = kmalloc(size);

    if(!container)
        return NULL;

    /* Hack, but needed for now */
    o = (struct obj *)((uintptr_t)container + class->type->offset);

    atomic_set(&o->refcount, 1);
    o->type = class->type;
    o->ops = ops;
    o->name = name;

    /* add to class collection of instances */
    o->parent = &class->obj;
    collection_add(&class->instances, o);

    return o;
}
