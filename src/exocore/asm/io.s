section .text

global io_read_ui8

align 8
io_read_ui8:

%if EXOCORE_IS_32_BIT
    mov dx, [esp + 4] ; Port is the first argument.
%else
    mov rdx, rdi ; Port is the first argument.
%endif

    ; Read a byte from the port in dx.
    in al, dx

    ret

global io_read_ui16

align 8
io_read_ui16:

%if EXOCORE_IS_32_BIT
    mov dx, [esp + 4] ; Port is the first argument.
%else
    mov rdx, rdi ; Port is the first argument.
%endif

    ; Read a word from the port in dx.
    in ax, dx

    ret

global io_read_ui32

align 8
io_read_ui32:

%if EXOCORE_IS_32_BIT
    mov dx, [esp + 4] ; Port is the first argument.
%else
    mov rdx, rdi ; Port is the first argument.
%endif

    ; Read a dword from the port in dx.
    in eax, dx

    ret

global io_write_ui8

align 8
io_write_ui8:

%if EXOCORE_IS_32_BIT
    mov al, [esp + 8] ; Value is the second argument.
    mov dx, [esp + 4] ; Port is the first argument.
%else
    mov rdx, rdi ; Port is the first argument.
    mov eax, esi ; Value is the second argument.
%endif

    ; Write a byte to the port in dx.
    out dx, al

    ret

global io_write_ui16

align 8
io_write_ui16:

%if EXOCORE_IS_32_BIT
    mov al, [esp + 8] ; Value is the second argument.
    mov dx, [esp + 4] ; Port is the first argument.
%else
    mov rdx, rdi ; Port is the first argument.
    mov eax, esi ; Value is the second argument.
%endif

    ; Write a word to the port in dx.
    out dx, ax

    ret

global io_write_ui32

align 8
io_write_ui32:

%if EXOCORE_IS_32_BIT
    mov al, [esp + 8] ; Value is the second argument.
    mov dx, [esp + 4] ; Port is the first argument.
%else
    mov rdx, rdi ; Port is the first argument.
    mov eax, esi ; Value is the second argument.
%endif

    ; Write a dword to the port in dx.
    out dx, eax

    ret

global io_wait

align 8
io_wait:

    ; We assume the 0x80 port to be free.
    out 0x80, al

    ret
