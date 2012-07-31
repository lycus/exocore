#ifndef EXOCORE_PAGING_H
#define EXOCORE_PAGING_H

#include "exocore/common.h"

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
            ui8 kernel_bits : 3; // Unused; free for kernel use.
            ui32 frame : 20; // Physical address of the frame shifted right by 12 bits.
        };

        ui32 raw;
    };
} attr(packed) page_table_entry_t;

STATIC_ASSERT(sizeof(page_table_entry_t) == 4);

#define PAGE_TABLE_ENTRIES 1024

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
            bool unused1 : 1;
            bool is_4_mb : 1; // If set, this entry maps a 4 MB page.
            bool unused2 : 1;
            ui8 kernel_bits : 3; // Unused; free for kernel use.
            ui32 table : 20; // Physical address of the table shifted right by 12 bits.
        };

        ui32 raw;
    };
} attr(packed) page_directory_entry_t;

STATIC_ASSERT(sizeof(page_directory_entry_t) == 4);

#define PAGE_DIRECTORY_ENTRIES 1024

typedef struct page_directory
{
    page_directory_entry_t physical_tables[PAGE_DIRECTORY_ENTRIES];
    page_table_t tables[PAGE_DIRECTORY_ENTRIES];
    uiptr physical_address;
} attr(packed) page_directory_t;

#define PAGE_SIZE 4096

#endif
