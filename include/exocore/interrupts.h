#ifndef EXOCORE_H_INTERRUPTS
#define EXOCORE_H_INTERRUPTS

#include "exocore/common.h"

typedef enum interrupt_id : uiptr
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
    INTERRUPT_IRQ_TIMER = 32,
    INTERRUPT_IRQ_KEYBOARD = 33,
    INTERRUPT_IRQ_CASCADE = 34,
    INTERRUPT_IRQ_COM_1 = 35,
    INTERRUPT_IRQ_COM_2 = 36,
    INTERRUPT_IRQ_LPT_2 = 37,
    INTERRUPT_IRQ_FLOPPY_DISK = 38,
    INTERRUPT_IRQ_LPT_1 = 39,
    INTERRUPT_IRQ_CMOS_CLOCK = 40,
    INTERRUPT_IRQ_LEGACY_SCSI_NIC = 41,
    INTERRUPT_IRQ_SCSI_NIC_1 = 42,
    INTERRUPT_IRQ_SCSI_NIC_2 = 43,
    INTERRUPT_IRQ_PS2_MOUSE = 44,
    INTERRUPT_IRQ_COPROCESSOR = 45,
    INTERRUPT_IRQ_PRIMARY_ATA_DISK = 46,
    INTERRUPT_IRQ_SECONDARY_ATA_DISK = 47,
} interrupt_id_t;

typedef struct interrupt_info
{
    const uiptr ds; // Data segment selector.
#ifndef EXOCORE_IS_32_BIT
    const uiptr r15; // General purpose R15 register.
    const uiptr r14; // General purpose R14 register.
    const uiptr r13; // General purpose R13 register.
    const uiptr r12; // General purpose R12 register.
    const uiptr r11; // General purpose R11 register.
    const uiptr r10; // General purpose R10 register.
    const uiptr r9; // General purpose R9 register.
    const uiptr r8; // General purpose R8 register.
#endif
    const uiptr edi; // General purpose EDI register.
    const uiptr esi; // General purpose ESI register.
    const uiptr ebp; // General purpose EBP register.
    const uiptr esp; // General purpose ESP register.
    const uiptr ebx; // General purpose EBX register.
    const uiptr edx; // General purpose EDX register.
    const uiptr ecx; // General purpose ECX register.
    const uiptr eax; // General purpose EAX register.
    const interrupt_id_t id; // Interrupt identifier.
    const uiptr error_code; // Error code (0 if not applicable).
    const uiptr user_eip;
    const uiptr user_cs;
    const uiptr user_eflags;
    const uiptr user_esp;
    const uiptr user_ss;
} attr(packed) interrupt_info_t;

typedef void (^ isr_t)(const interrupt_info_t);

// Sets a block to be called when an interrupt fires.
void set_interrupt_handler(const interrupt_id_t id, const isr_t handler);

// Enables the receiving of interrupts from hardware.
void enable_interrupts(void);

// Disables the receiving of interrupts from hardware.
void disable_interrupts(void);

#endif
