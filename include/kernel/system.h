/*
 * Copyright (C) 2013 F4OS Authors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * SYSTEMS - Collections of classes for some purpose - Michael Yenik
 *
 * Systems, themselves a class of objects, are collections of other classes
 * that can be visible to the user.
 *
 * For example, you could have dev and sys be systems. sys would be for
 * tweaking OS paramaters at runtime, and dev for exposing devices available
 * for interaction. In the case of dev, one would make a new "system",
 * instantiated with type system_class and name dev, and then one could
 * register other classes like accel and gpio with this system.
 *
 * SYSTEM API (still in flux, more may be added but these will stay)
 *
 * void register_with_system(struct system *system, struct class *class)
 * ~ registers class with system, so that /system_name/class_name/...
 *   enumerates all instances of class
 *
 * struct obj *get_by_name_from_system(struct system *sys, char *cls_name, char *inst_name)
 * ~ finds the obj named inst_name in the class name cls_name if there is one
 *
 * struct obj *get_system_by_name(char *name)
 * ~ gets the obj for system named "name"
 */

#ifndef KERNEL_SYSTEM_H_INCLUDED
#define KERNEL_SYSTEM_H_INCLUDED

#include <kernel/class.h>

typedef struct system {
    char        *name;
    struct collection classes;
    struct obj  obj;
} system_t;

#define to_system(__obj) container_of((__obj), struct system, obj)

#define INIT_SYSTEM(__symbol, __name) { \
    .name = #__name, \
    .classes = INIT_COLLECTION((__symbol).classes), \
    }

extern struct system dev_system;
extern struct class system_class;
extern struct collection systems;

void register_with_system(struct system *sys, struct class *cls);
struct obj *get_by_name_from_system(struct system *sys, char *cls_name, char *inst_name);
struct obj *get_system_by_name(char *name);

#endif
