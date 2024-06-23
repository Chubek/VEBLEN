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

struct INTER_Expr {
   enum {
	EXP_Constant,
	EXP_Variable,
	EXP_Application,
	EXP_Abstraction,
	EXP_Let,
	EXP_Letrec,
	EXP_LetIn,
	EXP_Infix,
	EXP_Case,
	EXP_Pattern,
	EXP_Cons,
   } kind;

   union {
	struct INTER_Const *v_const;
	struct INTER_Variable *v_variable;
	struct INTER_Application *v_application;
	struct INTER_Abstraction *v_abstraction;
	struct INTER_Let *v_let;
	struct INTER_LetRec *v_letrec;
	struct INTER_LetIn *v_letin;
	struct INTER_Infix *v_infix;
	struct INTER_Case *v_case;
	struct INTER_Pattern *v_pattern;
	struct INTER_Cons *v_cons;
   };
};

struct INTER_Const {
   uint8_t repr[MAX_CONST_REPR + 1];
   enum {
	CNR_Number,
	CNR_String,
	CNR_Operator,
	CNR_Intrin,
	CNR_Ident,
	/* TODO: Add more */
   } kind;
};

struct INTER_Variable {
    uint8_t repr[MAX_VARIABLE_REPR + 1];
    struct INTER_Symtab **static_link;
};

#endif
