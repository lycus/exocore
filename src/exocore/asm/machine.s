section .text

global halt_machine

align 8
halt_machine:

    hlt

global breakpoint

align 8
breakpoint:

%ifdef EXOCORE_DEBUG
    xchg bx, bx
%endif
    ret
