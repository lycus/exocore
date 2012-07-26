#ifndef EXOCORE_H_COMMON
#define EXOCORE_H_COMMON

typedef unsigned char ui8;
typedef signed char i8;
typedef unsigned short ui16;
typedef signed short i16;
typedef unsigned int ui32;
typedef signed int i32;
typedef unsigned long long ui64;
typedef signed long long i64;

typedef float f32;
typedef double f64;

#ifdef EXOCORE_IS_32_BIT
typedef ui32 uiptr;
typedef i32 iptr;
#else
typedef ui64 uiptr;
typedef i64 iptr;
#endif

typedef _Bool bool;

#define true 1
#define false 0

#define null ((void*)0)

#define attr(X) __attribute__((X))

#define _STRINGIFY(X) #X
#define STRINGIFY(X) _STRINGIFY(X)

attr(noreturn) void panic(const char* const message);

#define PANIC(MSG) panic("Kernel panic at " __FILE__ ":" STRINGIFY(__LINE__) ": " MSG)

#endif
