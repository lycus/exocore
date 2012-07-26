#include "exocore/console.h"
#include "exocore/i386/descriptors.h"
#include "exocore/i386/interrupts.h"
#include "exocore/i386/io.h"
#include "exocore/i386/pic.h"

static isr_t interrupt_handlers[IDT_SIZE];

void set_interrupt_handler(const interrupt_id_t id, const isr_t handler)
{
    interrupt_handlers[id] = handler;
}

void i386_isr_handler(const interrupt_info_t info)
{
    switch (info.id)
    {
        case INTERRUPT_DIVISION_BY_ZERO_EXCEPTION:
            DEBUG("Received interrupt: INTERRUPT_DIVISION_BY_ZERO_EXCEPTION\n");
            break;
        case INTERRUPT_DEBUG_EXCEPTION:
            DEBUG("Received interrupt: INTERRUPT_DEBUG_EXCEPTION\n");
            break;
        case INTERRUPT_NONMASKABLE:
            DEBUG("Received interrupt: INTERRUPT_NONMASKABLE\n");
            break;
        case INTERRUPT_BREAKPOINT_EXCEPTION:
            DEBUG("Received interrupt: INTERRUPT_BREAKPOINT_EXCEPTION\n");
            break;
        case INTERRUPT_INTO_DETECTED_OVERFLOW:
            DEBUG("Received interrupt: INTERRUPT_INTO_DETECTED_OVERFLOW\n");
            break;
        case INTERRUPT_OUT_OF_BOUNDS_EXCEPTION:
            DEBUG("Received interrupt: INTERRUPT_OUT_OF_BOUNDS_EXCEPTION\n");
            break;
        case INTERRUPT_INVALID_OPCODE_EXCEPTION:
            DEBUG("Received interrupt: INTERRUPT_INVALID_OPCODE_EXCEPTION\n");
            break;
        case INTERRUPT_NO_COPROCESSOR_EXCEPTION:
            DEBUG("Received interrupt: INTERRUPT_NO_COPROCESSOR_EXCEPTION\n");
            break;
        case INTERRUPT_DOUBLE_FAULT:
            DEBUG("Received interrupt: INTERRUPT_DOUBLE_FAULT\n");
            break;
        case INTERRUPT_COPROCESSOR_SEGMENT_OVERRUN:
            DEBUG("Received interrupt: INTERRUPT_COPROCESSOR_SEGMENT_OVERRUN\n");
            break;
        case INTERRUPT_BAD_TSS:
            DEBUG("Received interrupt: INTERRUPT_BAD_TSS\n");
            break;
        case INTERRUPT_SEGMENT_NOT_PRESENT:
            DEBUG("Received interrupt: INTERRUPT_SEGMENT_NOT_PRESENT\n");
            break;
        case INTERRUPT_STACK_FAULT:
            DEBUG("Received interrupt: INTERRUPT_STACK_FAULT\n");
            break;
        case INTERRUPT_GENERAL_PROTECTION_FAULT:
            DEBUG("Received interrupt: INTERRUPT_GENERAL_PROTECTION_FAULT\n");
            break;
        case INTERRUPT_PAGE_FAULT:
            DEBUG("Received interrupt: INTERRUPT_PAGE_FAULT\n");
            break;
        case INTERRUPT_UNKNOWN_INTERRUPT_EXCEPTION:
            DEBUG("Received interrupt: INTERRUPT_UNKNOWN_INTERRUPT_EXCEPTION\n");
            break;
        case INTERRUPT_COPROCESSOR_FAULT:
            DEBUG("Received interrupt: INTERRUPT_COPROCESSOR_FAULT\n");
            break;
        case INTERRUPT_ALIGNMENT_CHECK_EXCEPTION:
            DEBUG("Received interrupt: INTERRUPT_ALIGNMENT_CHECK_EXCEPTION\n");
            break;
        case INTERRUPT_MACHINE_CHECK_EXCEPTION:
            DEBUG("Received interrupt: INTERRUPT_MACHINE_CHECK_EXCEPTION\n");
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
        case INTERRUPT_IRQ_TIMER:
            //DEBUG("Received interrupt: INTERRUPT_IRQ_TIMER\n");
            break;
        case INTERRUPT_IRQ_KEYBOARD:
            //DEBUG("Received interrupt: INTERRUPT_IRQ_KEYBOARD\n");
            break;
        case INTERRUPT_IRQ_CASCADE:
            //PANIC("Received interrupt: INTERRUPT_IRQ_CASCADE\n");
            break;
        case INTERRUPT_IRQ_COM_1:
            //DEBUG("Received interrupt: INTERRUPT_IRQ_COM_1\n");
            break;
        case INTERRUPT_IRQ_COM_2:
            //DEBUG("Received interrupt: INTERRUPT_IRQ_COM_2\n");
            break;
        case INTERRUPT_IRQ_LPT_2:
            //DEBUG("Received interrupt: INTERRUPT_IRQ_LPT_2\n");
            break;
        case INTERRUPT_IRQ_FLOPPY_DISK:
            //DEBUG("Received interrupt: INTERRUPT_IRQ_FLOPPY_DISK\n");
            break;
        case INTERRUPT_IRQ_LPT_1:
            //DEBUG("Received interrupt: INTERRUPT_IRQ_LPT_1\n");
            break;
        case INTERRUPT_IRQ_CMOS_CLOCK:
            //DEBUG("Received interrupt: INTERRUPT_IRQ_CMOS_CLOCK\n");
            break;
        case INTERRUPT_IRQ_LEGACY_SCSI_NIC:
            //DEBUG("Received interrupt: INTERRUPT_IRQ_LEGACY_SCSI_NIC\n");
            break;
        case INTERRUPT_IRQ_SCSI_NIC_1:
            //DEBUG("Received interrupt: INTERRUPT_IRQ_SCSI_NIC_1\n");
            break;
        case INTERRUPT_IRQ_SCSI_NIC_2:
            //DEBUG("Received interrupt: INTERRUPT_IRQ_SCSI_NIC_1\n");
            break;
        case INTERRUPT_IRQ_PS2_MOUSE:
            //DEBUG("Received interrupt: INTERRUPT_IRQ_PS2_MOUSE\n");
            break;
        case INTERRUPT_IRQ_COPROCESSOR:
            //DEBUG("Received interrupt: INTERRUPT_IRQ_COPROCESSOR\n");
            break;
        case INTERRUPT_IRQ_PRIMARY_ATA_DISK:
            //DEBUG("Received interrupt: INTERRUPT_IRQ_PRIMARY_ATA_DISK\n");
            break;
        case INTERRUPT_IRQ_SECONDARY_ATA_DISK:
            //DEBUG("Received interrupt: INTERRUPT_IRQ_SECONDARY_ATA_DISK\n");
            break;
    }

    // Call an interrupt handler, if there is one.
    if (interrupt_handlers[info.id])
        interrupt_handlers[info.id](info);
}

void i386_irq_handler(const interrupt_info_t info)
{
    if (info.id >= INTERRUPT_IRQ_CMOS_CLOCK)
    {
        i386_io_write_ui8(PIC_SLAVE_COMMAND, PIC_OCW2_EOI);
        i386_io_wait();
    }

    i386_io_write_ui8(PIC_MASTER_COMMAND, PIC_OCW2_EOI);
    i386_io_wait();

    // Just handle stuff in the ISR handler.
    i386_isr_handler(info);
}
