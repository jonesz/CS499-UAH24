/*
 *   paging.c by MiniOs
 *
 *  NOTE(Britton): How paging works

With paging enabled in 32-bit protected mode, virtual memory addressed with a
2-layer table scheme

The first layer table is the 'Page Directory' (PD) and there is only one
The second layer tables are the 'Page Tables' (PT) and there are up to 1024 (1K)
Both the PD and PT contain 1K 4-Byte long entries, making them 4K Bytes long in
total: exactly 1 Page

Entries in the PD and PT are identical in format,
but a PD entry contains data pointing to an entire page table, while a PT entry
poitns to a specific page Entries are formatted as follows: The bottom 12 bits
(11-0) are reserved for bit flags and metadata about the PT/Page pointed to by
the entry The top 20 bits (31-12) contain the 4K aligned PHYSICAL ADDRESS of the
PT/Page pointed to

Because there are only 20 Bits of address information available in a PD/PT
entry, PDs and PTs must be 4K aligned

With paging enabled, pointers are de-referenced as follows:
Suppose a pointer points to X
Bits 11-0 contain the offset of X's location in a page
Some PT contains an entry pointing to the page where X is located.
Bits 21-12 contain the index of the entry inside that PT which points to X's
page The PD contains an entry pointing to the aforementioned PT Bits 31-22
contain the index of the PD entry which points to X's PT

Put concisely:
(PD(i).addr->PT(j).addr + k)->X
 */
#include <mem/multiboot.h>
#include <stdint.h>

// TODO (Britton): Explain all this
typedef uint8_t PageFlags;

// This pointer should only be accessible by kernel code,
// and only works after init_paging has been executed successfully
#define PAGE_DIRECTORY_POINTER ((PageDirectory *)0xFFFFF000)

// These bit fields can be used to read/write data from a PD OR PT entry
#define PAGE_PRESENT ((PageFlags)1)
#define PAGE_DIRTY ((PageFlags)(1 << 6))
#define PAGE_ACCESSED ((PageFlags)(1 << 5))
#define PAGE_ALLOW_USER_ACCESS ((PageFlags)(1 << 2))
#define PAGE_ALLOW_WRITE ((PageFlags)(1 << 1))

#define Kilobytes(x) 1024 * x
#define Megabytes(x) 1024 * Kilobytes(x)

typedef struct _PageDirectory PageDirectory;
typedef struct _PageTable PageTable;
typedef union _PageDirectoryEntry PageDirectoryEntry;
typedef union _PageTableEntry PageTableEntry;

// Global variable
static uint32_t *pages[Megabytes(1)];
static uint32_t page_alloc_index;

// Global variable holds
static uint32_t page_tables[Kilobytes(1)];
static uint32_t pt_alloc_index;

// Both PD and PT entries are formatted identically,
// But separating them may help the compiler to catch errors
// flags field can be used to easily read/write metadata,
// But since the address field is not byte-aligned,
// There is no good way to make an 'addr' field
union _PageDirectoryEntry {
  PageFlags flags;
  uint32_t entry;
};

union _PageTableEntry {
  PageFlags flags;
  uint32_t entry;
};

// Both PD and PT contain exactly 1K entries
struct _PageDirectory {
  PageDirectoryEntry entries[1024];
};

struct _PageTable {
  PageTableEntry entries[1024];
};

extern multiboot_info_t *boot_info;

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

// Initializes paging to be used by the operating system
// Paging should be disabled during initialization
void init_paging(PageDirectory *page_directory) {

  // TODO(Britton): Use mem_info to identity page or otherwise smartly set up:
  // mmaped I/O, kernel memory, etc

  // Set The last page directory entry to point to the PD itself
  // This way, virtual addr 0xFFFFF000 points to the base of the PD
  // Now, 0xFFFFF000 + i can be used to edit PD entries after setting up paging
  page_directory->entries[1023].entry = (uint32_t)page_directory;
  page_directory->entries[1023].flags = PAGE_PRESENT | PAGE_ALLOW_WRITE;

  // Select the memory region to use for paging
  uint8_t *mmap = (uint8_t *)boot_info->mmap_addr;
  uint8_t *cur = mmap;
  uint32_t biggest_region_size = 0;
  uint8_t *biggest_region = 0;
  while (cur < mmap + boot_info->mmap_length) {
    uint64_t *base = (uint64_t *)(cur + 4);
    uint64_t *length = (uint64_t *)(cur + 8 + 4);
    uint32_t *flags = (uint32_t *)(cur + 16 + 4);
    if (*flags == 1) {
      if (*length > biggest_region_size) {
        biggest_region = (uint8_t *)*base;
        biggest_region_size = *length;
      }
    }
    cur += *(uint64_t *)cur + 4;
  }
  // Fill 'pages' with all available physical addresses of pages
  for (uint32_t i = 0; i < (sizeof(pages) / sizeof(pages[0])); i++) {

    // TODO(Britton): Use mem_info to determine which pages are unavailable
    // For the rest, put an entry in 'pages'
    if (biggest_region + 4096 * i < biggest_region + biggest_region_size) {
      pages[i] = (uint32_t *)(biggest_region + 4096 * i);
    }
  }
}

/*
 * Begin paging API: should be usable by other parts of the kernel
 * TODO(Britton): Test all this
 */

// Returns the phys addr of one free page
// and pops it from the list of free pages
uint32_t *page_alloc() {

  // Check if there are any available pages
  // Zero should never be an allocatable phys addr
  // If page_alloc_index is greater than one megabyte,
  // then it had an underflow and there are no pages left
  if (pages[page_alloc_index] == 0 || page_alloc_index > Megabytes(1)) {
    return 0;
  }

  uint32_t *result = pages[page_alloc_index];
  pages[page_alloc_index] = 0;
  page_alloc_index--;
  return result;
}

// Adds a page back to the list of available pages
// Takes the physical address of a page
// TODO(Britton): Consider security
void page_free(uint32_t *phys_addr) {
  page_alloc_index++;
  pages[page_alloc_index] = phys_addr;
}

// Adds a new userland, writable page table to the page directory
// Returns the PD entry index of the new PT
uint32_t page_table_alloc() {

  uint32_t *pt_addr = page_alloc();
  uint32_t pt_index = page_tables[pt_alloc_index];
  page_tables[pt_alloc_index] = 0;
  pt_alloc_index--;
  PAGE_DIRECTORY_POINTER->entries[pt_index].entry = (uint32_t)pt_addr;
  PAGE_DIRECTORY_POINTER->entries[pt_index].flags =
      PAGE_PRESENT | PAGE_ALLOW_USER_ACCESS | PAGE_ALLOW_WRITE;
  return pt_index;
}

// Frees a PT from the PD
void page_table_free(uint32_t pt_index) {

  PageDirectoryEntry *entry_to_free =
      &(PAGE_DIRECTORY_POINTER->entries[pt_index]);
  uint32_t *phys_addr = (uint32_t *)((entry_to_free->entry) & 0xFFFFF000);
  page_free(phys_addr);
  entry_to_free->entry = 0;
  pt_alloc_index++;
  page_tables[pt_alloc_index] = pt_index;
}
