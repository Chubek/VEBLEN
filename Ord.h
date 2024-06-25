#ifndef ORD_H
#define ORD_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Veblen.h"

/* The following needs to be preprocessed with AllocPPx.pl */

/*!(alloc-pp __ord_heap)!*/

#define ORD_ALLOC(n) __ord_heap_alloc_notrace(n)
#define ORD_REALLOC(mem, nn) __ord_heap_realloc(mem, nn)
#define ORD_FREE(mem) __ord_heap_free(mem)

struct ORD_Repr {

};



#endif
