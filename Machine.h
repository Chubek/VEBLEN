#ifndef MACHINE_H
#define MACHINE_H

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Veblen.h"

struct VM_String;
struct VM_Value;
struct VM_ConstPool;
struct VM_Semispace;
struct VM_ValueStack;
struct VM_ControlStack;
struct VM_InstStream;
struct VM_Richp;

enum VM_Opcode;

struct VM_String {
  uint8_t *buffer;
  size_t buf_size;

  struct VM_String *next;
};

struct VM_Value {
  enum {
    VALUE_Int,
    VALUE_Float,
    VALUE_Bool,
    VALUE_Char,
    VALUE_Pointer,
  } kind;

  union {
    intmax_t v_int;
    double v_float;
    bool v_bool;
    uint16_t v_char;
    uintptr_t v_pointer;
  };

  struct VM_Value *next;
};

struct VM_ConstPool {
  struct VM_Value *values[MAX_POOL_SIZE];
  struct VM_String *strings[MAX_POOL_SIZE];
};

struct VM_Address {
  char label[MAX_LABEL + 1];
  uintptr_t address;

  struct VM_Address *next;
};

struct VM_HeapBlock {
  enum {
    HBLK_Record,
    HBLK_Thunk,
    HBLK_Closure,
    HBLK_Constr,
    HBLK_Forward,
    HBLK_String,
    HBLK_Boxed,
  } tag;

  struct VM_Value *values;
  size_t num_values;
  atomic_bool busy;
  atomic_bool alive;
  atomic_bool forwarded;
  struct VM_HeapBlock *next;
  struct VM_HeapBlock *forwarding_address;
};

struct VM_Semispace {
  struct VM_HeapBlock *from_space;
  struct VM_HeapBlock *to_space;

  struct VM_Semispace *next;
};

struct VM_ValueStack {
  struct VM_Value *values;
  size_t num_values;
  size_t pointer;
};

struct VM_ControlStack {
  struct VM_Address *base_pointers;
  struct VM_Address *returns_addrs;

  size_t num_addrs;
  size_t pointer;
};

enum VM_Opcode {
  OP_Push,
  OP_PushFn,
  OP_Swap,
  OP_Updsp,
  OP_Dup,
  OP_Load,
  OP_Store,
  OP_LoadAdd,
  OP_Op,

  OP_IfOp,
  OP_IfCmp,
  OP_IfLt,
  OP_IfLe,
  OP_IfGt,
  OP_IfGe,
  OP_IfEq,
  OP_IfNe,
  OP_IfLtLv,
  OP_Jmp,
  OP_Call,
  OP_TailCall,
  OP_JmpTable,
  OP_Jmpt,
  OP_Return,
  OP_GetConst,
  OP_ReturnConst,
  OP_Apply,

  OP_Create,
  OP_CCreate,
  OP_Get,
  OP_Getn,
  OP_Update,
  OP_GetAll,
  OP_NewArray,

  OP_Stop,
  OP_Print,

  OP_Add,
  OP_Sub,
  OP_Mult,
  OP_Div,
  OP_Mod,
  OP_Inc,
  OP_Dec,
  OP_FltAdd,
  OP_FlatSub,
  OP_FltMult,
  OP_FltDiv,
  OP_Cos,
  OP_Sin,
  OP_Tan,
  OP_Asin,
  OP_Acos,
  OP_Atan,
  OP_log,
  OP_log10,
  OP_Exp,
  OP_Sqrt,
  OP_Floor,
  OP_Pow,

  OP_Eval,
  OP_Exec,
};

struct VM_InstStream {
  enum VM_Opcode *opcodes;
  size_t num_opcodes;
  size_t program_counter;
};

struct VM_Frame {
  struct VM_Address *frame_label;
  struct VM_InstStream *inst_stream;
  struct VM_ValueStack *value_stack;
  struct VM_ControlStack *control_stack;
  struct VM_ConstPool *const_pool;
  struct VM_Semispace *gc_heap;

  bool is_global;
  struct VM_Frame *next;
};

struct VM_Richp {
  FILE *bytecode_source;
  struct VM_Frame *frames;
  size_t num_frames;
  size_t top_frame;
};

#endif /* machine.h */
