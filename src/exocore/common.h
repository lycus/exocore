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

typedef ui8* str;

#define attr(X) __attribute__((X))

#endif
