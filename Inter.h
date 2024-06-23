#ifndef INTER_H
#define INTER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The following needs to be preprocessed with AllocPPx.pl */

/*!(alloc-pp __inter_heap)!*/

#define INTER_ALLOC(n) __inter_heap_alloc_notrace(n)
#define INTER_REALLOC(mem, nn) __inter_heap_realloc(mem, nn)
#define INTER_FREE(mem) __inter_heap_free(mem)

struct INTER_LambdaSexp;
struct INTER_String;
struct INTER_Value;

struct INTER_String {
    uint8_t *buffer;
    size_t buf_length;

    struct INTER_String *next;
};

struct INTER_Value {
   enum {
	INTERVAL_String,
	INTERVAL_Number,
	INTERVAL_Argument,
   } kind;

   union {
   	INTER_String *v_string;
	long double v_number;
   };

   struct INTER_Value *next;
};

struct INTER_LambdaSexp {
    struct INTER_String *name;

    enum {
	LAMSEXP_Atom,
	LAMSEXP_List,
    } kind;

    struct INTER_Value *value;
};




#endif
