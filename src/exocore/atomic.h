#ifndef EXOCORE_H_ATOMIC
#define EXOCORE_H_ATOMIC

#include "common.h"

#define atomic_fence() __atomic_thread_fence(__ATOMIC_SEQ_CST)

#define atomic_load(PTR) __atomic_load_n(PTR, __ATOMIC_SEQ_CST)
#define atomic_load_ret(PTR, RET) __atomic_load(PTR, RET, __ATOMIC_SEQ_CST)

#define atomic_store(PTR, VAL) __atomic_store_n(PTR, VAL, __ATOMIC_SEQ_CST)
#define atomic_store_ind(PTR, VAL) __atomic_store(PTR, VAL, __ATOMIC_SEQ_CST)

#define atomic_exchange(PTR, VAL) __atomic_exchange_n(PTR, VAL, __ATOMIC_SEQ_CST)
#define atomic_exchange_ret(PTR, VAL, RET) __atomic_exchange(PTR, VAL, RET, __ATOMIC_SEQ_CST)

#define atomic_compare_exchange(PTR, CMP, VAL) __atomic_compare_exchange_n(ptr, cmp, val, true, __ATOMIC_SEQ_CST)
#define atomic_compare_exchange_ind(PTR, CMP, VAL) __atomic_compare_exchange(ptr, cmp, val, true, __ATOMIC_SEQ_CST)

#define atomic_add_fetch(PTR, VAL) __atomic_add_fetch(PTR, VAL, __ATOMIC_SEQ_CST)
#define atomic_sub_fetch(PTR, VAL) __atomic_sub_fetch(PTR, VAL, __ATOMIC_SEQ_CST)
#define atomic_and_fetch(PTR, VAL) __atomic_and_fetch(PTR, VAL, __ATOMIC_SEQ_CST)
#define atomic_xor_fetch(PTR, VAL) __atomic_xor_fetch(PTR, VAL, __ATOMIC_SEQ_CST)
#define atomic_or_fetch(PTR, VAL) __atomic_or_fetch(PTR, VAL, __ATOMIC_SEQ_CST)
#define atomic_nand_fetch(PTR, VAL) __atomic_nand_fetch(PTR, VAL, __ATOMIC_SEQ_CST)

#define atomic_fetch_add(PTR, VAL) __atomic_fetch_add(PTR, VAL, __ATOMIC_SEQ_CST)
#define atomic_fetch_sub(PTR, VAL) __atomic_fetch_sub(PTR, VAL, __ATOMIC_SEQ_CST)
#define atomic_fetch_and(PTR, VAL) __atomic_fetch_and(PTR, VAL, __ATOMIC_SEQ_CST)
#define atomic_fetch_xor(PTR, VAL) __atomic_fetch_xor(PTR, VAL, __ATOMIC_SEQ_CST)
#define atomic_fetch_or(PTR, VAL) __atomic_fetch_or(PTR, VAL, __ATOMIC_SEQ_CST)
#define atomic_fetch_nand(PTR, VAL) __atomic_fetch_nand(PTR, VAL, __ATOMIC_SEQ_CST)

#endif
