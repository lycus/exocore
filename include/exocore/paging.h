#ifndef EXOCORE_PAGING_H
#define EXOCORE_PAGING_H

#include "exocore/common.h"

#define PAGE_SIZE 4096

typedef struct page_table_entry
{
    union
    {
        struct
        {
            bool present : 1; // Should always be 1.
            bool writable : 1; // Indicates whether this page can be written to.
            bool user_mode : 1; // Indicates whether this page is accessible in user mode.
            bool writethrough : 1;
            bool disable_cache : 1;
            bool accessed : 1; // Indicates whether this page has ever been accessed.
            bool dirty : 1; // Indicates whether this page has ever been written to.
            bool pat : 1;
            bool global : 1; // Indicates whether this translation is global.
            ui8 kernel_bits1 : 3;
            ui64 frame : 40; // Physical address of the frame shifted right by 24 bits.
            bool unused : 1; // Must be 0.
            ui16 kernel_bits2 : 10;
            bool disable_execute : 1; // If set, and the IA32_EFER.NXE bit is set, this entry cannot be executed.
        };

        ui64 raw;
    };
} attr(packed) page_table_entry_t;

STATIC_ASSERT(sizeof(page_table_entry_t) == 8);

#define PAGE_TABLE_ENTRIES 512

typedef struct page_table
{
    page_table_entry_t pages[PAGE_TABLE_ENTRIES];
} attr(packed) page_table_t;

STATIC_ASSERT(sizeof(page_table_t) == sizeof(page_table_entry_t) * PAGE_TABLE_ENTRIES);

typedef struct page_directory_entry
{
    union
    {
        struct
        {
            bool present : 1; // Should always be 1.
            bool writable : 1; // Indicates whether this page can be written to.
            bool user_mode : 1; // Indicates whether this page is accessible in user mode.
            bool writethrough : 1;
            bool disable_cache : 1;
            bool accessed : 1; // Indicates whether this entry has ever been used for translation.
            bool kernel_bit : 1;
            bool is_4_mb : 1; // If set, this entry maps a 4 MB page.
            ui8 kernel_bits1 : 4;
            ui64 table : 40; // Physical address of the page table shifted right by 24 bits.
            bool unused : 1; // Must be 0.
            ui16 kernel_bits2 : 10;
            bool disable_execute : 1; // If set, and the IA32_EFER.NXE bit is set, this entry cannot be executed.
        };

        ui64 raw;
    };
} attr(packed) page_directory_entry_t;

STATIC_ASSERT(sizeof(page_directory_entry_t) == 8);

#define PAGE_DIRECTORY_ENTRIES 512

typedef struct page_directory
{
    page_directory_entry_t entries[PAGE_DIRECTORY_ENTRIES];
} attr(packed) page_directory_t;

STATIC_ASSERT(sizeof(page_directory_t) == sizeof(page_directory_entry_t) * PAGE_DIRECTORY_ENTRIES);

typedef struct pml_4_entry
{
    union
    {
        struct
        {
            bool present : 1; // Should always be 1.
            bool writable : 1; // Indicates whether this page can be written to.
            bool user_mode : 1; // Indicates whether this page is accessible in user mode.
            bool writethrough : 1;
            bool disable_cache : 1;
            bool accessed : 1; // Indicates whether this page has ever been accessed.
            bool kernel_bit : 1;
            bool unused1 : 1; // Must be 0.
            ui8 kernel_bits1 : 4;
            ui64 pointer : 40; // Physical address of the page directory pointer table shifted right by 24 bits.
            bool unused2 : 1; // Must be 0.
            ui16 kernel_bits2 : 10;
            bool disable_execute : 1; // If set, and the IA32_EFER.NXE bit is set, this entry cannot be executed.
        };

        ui64 raw;
    };
} attr(packed) pml_4_entry_t;

STATIC_ASSERT(sizeof(pml_4_entry_t) == 8);

#define PML_4_ENTRIES 512

typedef struct pml_4
{
    pml_4_entry_t entries[PML_4_ENTRIES];
} attr(packed) pml_4_t;

STATIC_ASSERT(sizeof(pml_4_t) == sizeof(pml_4_entry_t) * PML_4_ENTRIES);

typedef struct page_directory_pointer_table_entry
{
    union
    {
        struct
        {
            bool present : 1; // Should always be 1.
            bool writable : 1; // Indicates whether this page can be written to.
            bool user_mode : 1; // Indicates whether this page is accessible in user mode.
            bool writethrough : 1;
            bool disable_cache : 1;
            bool accessed : 1; // Indicates whether this page has ever been accessed.
            bool kernel_bit : 1;
            bool is_1_gb : 1; // If set, this entry maps a 1 GB page.
            ui8 kernel_bits1 : 4;
            ui64 pointer : 40; // Physical address of the page directory shifted right by 24 bits.
            bool unused : 1; // Must be 0.
            ui16 kernel_bits2 : 10;
            bool disable_execute : 1; // If set, and the IA32_EFER.NXE bit is set, this entry cannot be executed.
        };

        ui64 raw;
    };
} attr(packed) page_directory_pointer_table_entry_t;

STATIC_ASSERT(sizeof(page_directory_pointer_table_entry_t) == 8);

#define PAGE_DIRECTORY_POINTER_TABLE_ENTRIES 512

typedef struct page_directory_pointer_table
{
    page_directory_pointer_table_entry_t entries[PAGE_DIRECTORY_POINTER_TABLE_ENTRIES];
} attr(packed) page_directory_pointer_table_t;

STATIC_ASSERT(sizeof(page_directory_pointer_table_t) == sizeof(page_directory_pointer_table_entry_t) * PAGE_DIRECTORY_POINTER_TABLE_ENTRIES);

#endif
