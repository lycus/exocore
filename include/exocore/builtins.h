#ifndef EXOCORE_H_BUILTINS
#define EXOCORE_H_BUILTINS

#include "exocore/common.h"

#define builtin_ffs32(VAL) (ui32)__builtin_ffs(VAL)
#define builtin_ffs64(VAL) (ui64)__builtin_ffsll(VAL)

#define builtin_clz32(VAL) (ui32)__builtin_clz(VAL)
#define builtin_clz64(VAL) (ui64)__builtin_clzll(VAL)

#define builtin_ctz32(VAL) (ui32)__builtin_ctz(VAL)
#define builtin_ctz64(VAL) (ui64)__builtin_ctzll(VAL)

#define builtin_popcount32(VAL) (ui32)__builtin_popcount(VAL)
#define builtin_popcount64(VAL) (ui64)__builtin_popcountll(VAL)

#define builtin_parity32(VAL) (ui32)__builtin_parity(VAL)
#define builtin_parity64(VAL) (ui64)__builtin_parityll(VAL)

#endif
