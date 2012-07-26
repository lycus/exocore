#include "console.h"

typedef enum interrupt_id : ui32
{
    INTERRUPT_DIVISION_BY_ZERO_EXCEPTION = 0,
    INTERRUPT_DEBUG_EXCEPTION = 1,
    INTERRUPT_NONMASKABLE = 2,
    INTERRUPT_BREAKPOINT_EXCEPTION = 3,
    INTERRUPT_INTO_DETECTED_OVERFLOW = 4,
    INTERRUPT_OUT_OF_BOUNDS_EXCEPTION = 5,
    INTERRUPT_INVALID_OPCODE_EXCEPTION = 6,
    INTERRUPT_NO_COPROCESSOR_EXCEPTION = 7,
    INTERRUPT_DOUBLE_FAULT = 8, // Includes error code.
    INTERRUPT_COPROCESSOR_SEGMENT_OVERRUN = 9,
    INTERRUPT_BAD_TSS = 10, // Includes error code.
    INTERRUPT_SEGMENT_NOT_PRESENT = 11, // Includes error code.
    INTERRUPT_STACK_FAULT = 12, // Includes error code.
    INTERRUPT_GENERAL_PROTECTION_FAULT = 13, // Includes error code.
    INTERRUPT_PAGE_FAULT = 14, // Includes error code.
    INTERRUPT_UNKNOWN_INTERRUPT_EXCEPTION = 15,
    INTERRUPT_COPROCESSOR_FAULT = 16,
    INTERRUPT_ALIGNMENT_CHECK_EXCEPTION = 17,
    INTERRUPT_MACHINE_CHECK_EXCEPTION = 18,
    INTERRUPT_RESERVED_19 = 19,
    INTERRUPT_RESERVED_20 = 20,
    INTERRUPT_RESERVED_21 = 21,
    INTERRUPT_RESERVED_22 = 22,
    INTERRUPT_RESERVED_23 = 23,
    INTERRUPT_RESERVED_24 = 24,
    INTERRUPT_RESERVED_25 = 25,
    INTERRUPT_RESERVED_26 = 26,
    INTERRUPT_RESERVED_27 = 27,
    INTERRUPT_RESERVED_28 = 28,
    INTERRUPT_RESERVED_29 = 29,
    INTERRUPT_RESERVED_30 = 30,
    INTERRUPT_RESERVED_31 = 31,
} interrupt_id_t;

typedef struct interrupt_info
{
    const ui32 ds; // Data segment selector.
    const ui32 edi; // General purpose EDI register.
    const ui32 esi; // General purpose ESI register.
    const ui32 ebp; // General purpose EBP register.
    const ui32 esp; // General purpose ESP register.
    const ui32 ebx; // General purpose EBX register.
    const ui32 edx; // General purpose EDX register.
    const ui32 ecx; // General purpose ECX register.
    const ui32 eax; // General purpose EAX register.
    const interrupt_id_t interrupt_identifier; // Interrupt identifier.
    const ui32 error_code; // Error code (0 if not applicable).
    const ui32 user_eip;
    const ui32 user_cs;
    const ui32 user_eflags;
    const ui32 user_esp;
    const ui32 user_ss;
} attr(packed) interrupt_info_t;

void i386_isr_handler(const interrupt_info_t info)
{
    switch (info.interrupt_identifier)
    {
        case INTERRUPT_DIVISION_BY_ZERO_EXCEPTION:
            PANIC("Received interrupt: INTERRUPT_DIVISION_BY_ZERO_EXCEPTION\n");
            break;
        case INTERRUPT_DEBUG_EXCEPTION:
            PANIC("Received interrupt: INTERRUPT_DEBUG_EXCEPTION\n");
            break;
        case INTERRUPT_NONMASKABLE:
            PANIC("Received interrupt: INTERRUPT_NONMASKABLE\n");
            break;
        case INTERRUPT_BREAKPOINT_EXCEPTION:
            PANIC("Received interrupt: INTERRUPT_BREAKPOINT_EXCEPTION\n");
            break;
        case INTERRUPT_INTO_DETECTED_OVERFLOW:
            PANIC("Received interrupt: INTERRUPT_INTO_DETECTED_OVERFLOW\n");
            break;
        case INTERRUPT_OUT_OF_BOUNDS_EXCEPTION:
            PANIC("Received interrupt: INTERRUPT_OUT_OF_BOUNDS_EXCEPTION\n");
            break;
        case INTERRUPT_INVALID_OPCODE_EXCEPTION:
            PANIC("Received interrupt: INTERRUPT_INVALID_OPCODE_EXCEPTION\n");
            break;
        case INTERRUPT_NO_COPROCESSOR_EXCEPTION:
            PANIC("Received interrupt: INTERRUPT_NO_COPROCESSOR_EXCEPTION\n");
            break;
        case INTERRUPT_DOUBLE_FAULT:
            PANIC("Received interrupt: INTERRUPT_DOUBLE_FAULT\n");
            break;
        case INTERRUPT_COPROCESSOR_SEGMENT_OVERRUN:
            PANIC("Received interrupt: INTERRUPT_COPROCESSOR_SEGMENT_OVERRUN\n");
            break;
        case INTERRUPT_BAD_TSS:
            PANIC("Received interrupt: INTERRUPT_BAD_TSS\n");
            break;
        case INTERRUPT_SEGMENT_NOT_PRESENT:
            PANIC("Received interrupt: INTERRUPT_SEGMENT_NOT_PRESENT\n");
            break;
        case INTERRUPT_STACK_FAULT:
            PANIC("Received interrupt: INTERRUPT_STACK_FAULT\n");
            break;
        case INTERRUPT_GENERAL_PROTECTION_FAULT:
            PANIC("Received interrupt: INTERRUPT_GENERAL_PROTECTION_FAULT\n");
            break;
        case INTERRUPT_PAGE_FAULT:
            PANIC("Received interrupt: INTERRUPT_PAGE_FAULT\n");
            break;
        case INTERRUPT_UNKNOWN_INTERRUPT_EXCEPTION:
            PANIC("Received interrupt: INTERRUPT_UNKNOWN_INTERRUPT_EXCEPTION\n");
            break;
        case INTERRUPT_COPROCESSOR_FAULT:
            PANIC("Received interrupt: INTERRUPT_COPROCESSOR_FAULT\n");
            break;
        case INTERRUPT_ALIGNMENT_CHECK_EXCEPTION:
            PANIC("Received interrupt: INTERRUPT_ALIGNMENT_CHECK_EXCEPTION\n");
            break;
        case INTERRUPT_MACHINE_CHECK_EXCEPTION:
            PANIC("Received interrupt: INTERRUPT_MACHINE_CHECK_EXCEPTION\n");
            break;
        case INTERRUPT_RESERVED_19:
        case INTERRUPT_RESERVED_20:
        case INTERRUPT_RESERVED_21:
        case INTERRUPT_RESERVED_22:
        case INTERRUPT_RESERVED_23:
        case INTERRUPT_RESERVED_24:
        case INTERRUPT_RESERVED_25:
        case INTERRUPT_RESERVED_26:
        case INTERRUPT_RESERVED_27:
        case INTERRUPT_RESERVED_28:
        case INTERRUPT_RESERVED_29:
        case INTERRUPT_RESERVED_30:
        case INTERRUPT_RESERVED_31:
            PANIC("Reserved interrupt identifier received.");
            break;
    }
}
