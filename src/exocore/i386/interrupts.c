#include "common.h"

typedef enum interrupt_id : ui32
{
    INTERRUPT_DIV_ZERO = 0,
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

typedef struct registers
{
    ui32 ds; // Data segment selector.
    ui32 edi; // General purpose EDI register.
    ui32 esi; // General purpose ESI register.
    ui32 ebp; // General purpose EBP register.
    ui32 esp; // General purpose ESP register.
    ui32 ebx; // General purpose EBX register.
    ui32 edx; // General purpose EDX register.
    ui32 ecx; // General purpose ECX register.
    ui32 eax; // General purpose EAX register.
    interrupt_id_t interrupt_identifier; // Interrupt identifier.
    ui32 error_code; // Error code (0 if not applicable).
    ui32 user_eip;
    ui32 user_cs;
    ui32 user_eflags;
    ui32 user_esp;
    ui32 user_ss;
} registers_t;

void i386_isr_handler(registers_t registers attr(unused))
{
}
