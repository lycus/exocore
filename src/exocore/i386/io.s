section .text

global i386_io_read_ui8

align 4
i386_io_read_ui8:

    mov dx, [esp + 4] ; Port is the first argument.

    ; Read a byte from the port in dx.
    in al, dx

    ret

global i386_io_read_ui16

align 4
i386_io_read_ui16:

    mov dx, [esp + 4] ; Port is the first argument.

    ; Read a word from the port in dx.
    in ax, dx

    ret

global i386_io_read_ui32

align 4
i386_io_read_ui32:

    mov dx, [esp + 4] ; Port is the first argument.

    ; Read a dword from the port in dx.
    in eax, dx

    ret

global i386_io_write_ui8

align 4
i386_io_write_ui8:

    mov al, [esp + 8] ; Value is the second argument.
    mov dx, [esp + 4] ; Port is the first argument.

    ; Write a byte to the port in dx.
    out dx, al

    ret

global i386_io_write_ui16

align 4
i386_io_write_ui16:

    mov ax, [esp + 8] ; Value is the second argument.
    mov dx, [esp + 4] ; Port is the first argument.

    ; Write a word to the port in dx.
    out dx, ax

    ret

global i386_io_write_ui32

align 4
i386_io_write_ui32:

    mov eax, [esp + 8] ; Value is the second argument.
    mov dx, [esp + 4] ; Port is the first argument.

    ; Write a dword to the port in dx.
    out dx, eax

    ret

global i386_io_wait

align 4
i386_io_wait:

    ; We assume the 0x80 port to be free.
    out 0x80, al

    ret
