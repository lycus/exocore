MULTIBOOT_PAGE_ALIGNMENT equ 1 << 0
MULTIBOOT_MEMORY_INFO equ 1 << 1
MULTIBOOT_VIDEO_MODE equ 1 << 2
MULTIBOOT_AOUT_KLUDGE equ 1 << 16

MULTIBOOT_MAGIC equ 0x1badb002 ; Multiboot magic number.
MULTIBOOT_FLAGS equ MULTIBOOT_PAGE_ALIGNMENT | MULTIBOOT_MEMORY_INFO ; Multiboot module flags.
MULTIBOOT_CHECKSUM equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS) ; Fabulous Multiboot checksum.
