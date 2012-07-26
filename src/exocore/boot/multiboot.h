#ifndef EXOCORE_H_BOOT_MULTIBOOT
#define EXOCORE_H_BOOT_MULTIBOOT

#include "exocore/common.h"

// The magic number of the Multiboot header.
#define MULTIBOOT_HEADER_MAGIC 0x1badb002

// The magic number passed to the kernel.
#define MULTIBOOT_KERNEL_MAGIC 0x2badb002

// The flags (memory info and module alignment) of the Multiboot header.
#define MULTIBOOT_HEADER_FLAGS 0x00010003

// The Multiboot header.
typedef struct multiboot_header
{
    const ui32 magic; // As defined in boot.s.
    const ui32 flags; // As defined in boot.s.
    const ui32 checksum; // As defined in boot.s.
    const ui32 header_address; // As defined in boot.s. Only present if flags[16] is set.
    const ui32 load_address; // As defined in boot.s. Only present if flags[16] is set.
    const ui32 load_end_address; // As defined in boot.s. Only present if flags[16] is set.
    const ui32 bss_end_address; // As defined in boot.s. Only present if flags[16] is set.
    const ui32 entry_address; // As defined in boot.s. Only present if flags[16] is set.
} attr(packed) multiboot_header_t;

// The Multiboot section header table structure.
typedef struct multiboot_section_header_table
{
    const ui32 count; // Number of section headers. Can be 0.
    const ui32 size; // Size of each section header entry.
    const uiptr address; // Pointer to the section header array.
    const ui8* const string_table; // Symbol string table.
} attr(packed) multiboot_section_header_table_t;

// The module structure.
typedef struct module
{
    const uiptr start; // Start address.
    const uiptr end; // End address.
    const ui8* const arguments; // Zero-terminated string containing module arguments.
    const ui32 reserved; // Do not use. Always 0.
} attr(packed) module_t;

// The Multiboot memory map structure.
typedef struct multiboot_memory_map
{
    const ui64 address; // Starting address of the memory map.
    const ui64 length; // Length in bytes of the memory map.
    const ui32 type; // If equal to 1, RAM. Otherwise, reserved.
} attr(packed) multiboot_memory_map_t;

// The Multiboot memory map pair structure.
typedef struct multiboot_memory_map_pair
{
    const ui32 size; // Bytes needed to skip to next pair.
    const multiboot_memory_map_t memory_map; // The memory map at this location.
} attr(packed) multiboot_memory_map_pair_t;

// The Multiboot information structure.
typedef struct multiboot_info
{
    const ui32 flags; // Feature flags. Used to determine availability of information in this structure.
    const uiptr memory_lower; // Requires flags[0]. Amount of lower memory in kilobytes.
    const uiptr memory_upper; // Requires flags[0]. Amount of upper memory in kilobytes.
    const ui32 boot_device; // Requires flags[1]. Contains info about the device the kernel was booted from.
    const ui8* const command_line; // Requires flags[2]. Zero-terminated string containing kernel arguments.
    const ui32 module_count; // Requires flags[3]. How many modules are loaded with the kernel?
    const module_t* const modules; // Requires flags[3]. Pointer to the module array.
    const multiboot_section_header_table_t header_table; // Requires flags[5].
    const ui32 memory_map_length; // Requires flags[6]. Total size of the memory map pair array in bytes.
    const multiboot_memory_map_pair_t* const memory_map_address; // Requires flags[6]. Pointer to memory map pair array.
    const ui32 drives_length; // Requires flags[7].
    const uiptr drives_address; // Requires flags[7].
    const ui32 configuration_table; // Requires flags[8]. Address of the ROM configuration table given by the BIOS.
    const ui32 bootloader_name; // Requires flags[9]. Zero-terminated string containing the bootloader's name.
    const ui32 apm_table; // Requires flags[10].
    const ui32 vbe_control_info; // Requires flags[11].
    const ui32 vbe_mode_info; // Requires flags[11].
    const ui16 vbe_mode; // Requires flags[11].
    const ui16 vbe_interface_segment; // Requires flags[11].
    const ui16 vbe_interface_offset; // Requires flags[11].
    const ui16 vbe_interface_length; // Requires flags[11].
} attr(packed) multiboot_info_t;

#endif
