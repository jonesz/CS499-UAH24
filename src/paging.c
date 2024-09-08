/*
 *   paging.c by MiniOs
 */
#include <stdint.h>

// TODO (Britton): Explain all this
typedef uint8_t PageFlags;

typedef struct _PageDirectory PageDirectory;
typedef struct _PageTable PageTable;
typedef union _PageDirectoryEntry PageDirectoryEntry;
typedef union _PageTableEntry PageTableEntry;

union _PageDirectoryEntry {
  PageFlags flags;
  uint32_t entry;
};

union _PageTableEntry {
  PageFlags flags;
  uint32_t entry;
};

struct _PageDirectory {
  PageDirectoryEntry entries[1024];
};

struct _PageTable {
  PageTableEntry entries[1024];
};

#define PAGE_PRESENT ((PageFlags)1)
#define PAGE_DIRTY ((PageFlags)(1 << 6))
#define PAGE_ACCESSED ((PageFlags)(1 << 5))
#define PAGE_ALLOW_USER_ACCESS ((PageFlags)(1 << 2))
#define PAGE_ALLOW_WRITE ((PageFlags)(1 << 1))

uint32_t get_page_directory_index(void *ptr) {
  uint32_t mask = 0xFFC00000;
  uint32_t result = (uint32_t)ptr & mask;
  result >>= 22;
  return result;
}

uint32_t get_page_table_index(void *ptr) {
  uint32_t mask = 0x003FF000;
  uint32_t result = (uint32_t)ptr & mask;
  result >>= 12;
  return result;
}

uint32_t get_page_offset(void *ptr) {
  uint32_t mask = 0x00000FFF;
  uint32_t result = (uint32_t)ptr & mask;
  return result;
}

// Requires page_directory points to 4096 Bytes of memory for the page directory
// And page_tables points to 1024 blocks of 4096 Bytes for the page tables
// Fully maps the entire 4GiB virtual memory space to the physical memory
// Virtual pointers should correspond exactly to the physical address
void page_map_identity(PageDirectory *page_directory, PageTable *page_tables) {
  PageFlags flags = PAGE_ALLOW_USER_ACCESS | PAGE_PRESENT | PAGE_ALLOW_WRITE;
  for (uint32_t dir_index = 0; dir_index < 1024; dir_index++) {

    page_directory->entries[dir_index].entry =
        (uint32_t)(page_tables + dir_index) | flags;

    for (uint32_t table_index = 0; table_index < 1024; table_index++) {
      page_tables[dir_index].entries[table_index].entry =
          (dir_index << 22) | (table_index << 12) | flags;
    }
  }
}
