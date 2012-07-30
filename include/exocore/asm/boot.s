PAGE_SIZE equ 0x1000 ; Size of memory pages.
PAGE_DIRECTORY_ENTRIES equ 1024 ; Number of entries in the page directory.

%if EXOCORE_IS_32_BIT
KERNEL_VMA equ 0xC0000000 ; Keep in sync with the linker script.
%else
KERNEL_VMA equ 0xffff800000000000 ; Keep in sync with the linker script.
%endif

KERNEL_PAGE_NUMBER equ KERNEL_VMA / PAGE_SIZE / PAGE_DIRECTORY_ENTRIES
