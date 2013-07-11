/*
 * ATOMIC OPERATIONS FOR THUMB
 *
 * TODO some potentially useful ops missing...
 *
 * These are entirely based on/derived from the Linux kernel API.
 * Linux knows what it's doing and has a very good set of atomic ops that are
 * already familiar to developers, so why reinvent the wheel.
 *
 * The safety of not being castable to a non-atomic int is also nice, so here
 * the same int-in-a-struct method as Linux is used.
 */

#ifndef _ARCH_INCLUDE_ATOMIC_H
#define _ARCH_INCLUDE_ATOMIC_H

typedef struct {
    volatile int counter;
} atomic_t;

#define ATOMIC_INIT(n)                  {(n)}

/* word aligned reads/writes of words are atomic */
#define atomic_read(v)                  (v)->counter
#define atomic_set(v,i)                 (v)->counter = i

#define atomic_add(i,v) \
    /* (v)->counter += i */             \
    asm volatile(                       \
        "0:\n\t"                        \
        "ldrex  r0, [%[ctr]]\n\t"       \
        "add    r0, r0, %[inc]\n\t"     \
        "strex  r1, r0, [%[ctr]]\n\t"   \
        "cmp    r1, #0\n\t"             \
        "bne    0b\n\t"                 \
        : [ctr]"+l"(&(v)->counter)      \
        : [inc]"g"(i)                   \
        : "r0", "r1", "memory"          \
    )

#define atomic_sub(i,v)                 \
    /* (v)->counter -= i */             \
    asm volatile(                       \
        "0:\n\t"                        \
        "ldrex  r0, [%[ctr]]\n\t"       \
        "sub    r0, r0, %[inc]\n\t"     \
        "strex  r1, r0, [%[ctr]]\n\t"   \
        "cmp    r1, #0\n\t"             \
        "bne    0b\n\t"                 \
        : [ctr]"+l"(&(v)->counter)      \
        : [inc]"g"(i)                   \
        : "r0", "r1", "memory"          \
    )

#define atomic_inc(v)                   \
    /* (v)->counter += 1 */             \
    asm volatile(                       \
        "0:\n\t"                        \
        "ldrex  r0, [%[ctr]]\n\t"       \
        "add    r0, r0, #1\n\t"         \
        "strex  r1, r0, [%[ctr]]\n\t"   \
        "cmp    r1, #0\n\t"             \
        "bne    0b\n\t"                 \
        : [ctr]"+l"(&(v)->counter)      \
        :: "r0", "r1", "memory"         \
    )

#define atomic_dec(v)                   \
    /* (v)->counter -= 1 */             \
    asm volatile(                       \
        "0:\n\t"                        \
        "ldrex  r0, [%[ctr]]\n\t"       \
        "sub    r0, r0, #1\n\t"         \
        "strex  r1, r0, [%[ctr]]\n\t"   \
        "cmp    r1, #0\n\t"             \
        "bne    0b\n\t"                 \
        : [ctr]"+l"(&(v)->counter)      \
        :: "r0", "r1", "memory"         \
    )

/* --(v)->counter == 0 */
static int atomic_dec_and_test(atomic_t *v) {
    register int ret = 0;
    asm volatile(
        "0:\n\t"
        "ldrex  r0, [%[ctr]]\n\t"
        "sub    r0, r0, #1\n\t"
        "strex  r1, r0, [%[ctr]]\n\t"
        "cmp    r1, #0\n\t"
        "bne    0b\n\t"
        "cmp    r0, #1\n\t"
        "bne    1f\n\t"
        "mov    %[ret], #0\n\t"
        "1:\n\t"
        : [ret]"=l"(ret)
        : [ctr]"l"(&v->counter)
        : "r0", "r1", "memory"
    );
}

#endif /* _ARCH_INCLUDE_ATOMIC_H */
