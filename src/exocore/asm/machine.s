section .text

global halt_machine

align 8
halt_machine:

    hlt

global spin_pause

align 8
spin_pause:

    pause
    ret

global breakpoint

align 8
breakpoint:

%ifdef EXOCORE_DEBUG
    xchg bx, bx
%endif
    ret
