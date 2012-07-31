#ifndef EXOCORE_H_MEMORY
#define EXOCORE_H_MEMORY

attr(const) static inline uiptr align_to(const uiptr ptr, const uiptr alignment)
{
    const uiptr val = ptr + alignment - 1;
    return val - val % alignment;
}

attr(const) static inline uiptr is_aligned(const uiptr ptr, const uiptr alignment)
{
    return !(ptr % alignment);
}

#endif
