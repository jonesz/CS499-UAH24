#include <stdint.h>

typedef struct _BootInformation BootInformation;
struct _BootInformation {
  uint32_t flags;

  uint32_t mem_lower;
  uint32_t mem_upper;

  uint32_t boot_device;
  uint32_t cmd_line;

  uint32_t mods_count;
  uint32_t *mods_addr;

  uint32_t syms[4];

  uint32_t mmap_length;
  uint32_t *mmap_addr;

  uint32_t drives_length;
  uint32_t *drives_addr;

  uint32_t *config_table;
  uint32_t boot_loader_name;
  uint32_t *apm_table;

  uint32_t vbe_control_info;
  uint32_t vbe_mode_info;
  uint32_t vbe_mod;
  uint32_t vbe_interface_seg;
  uint32_t vbe_interface_off;
  uint32_t vbe_interface_len;

  uint32_t framebuffer_addr;
  uint32_t framebuffer_pitch;
  uint32_t framebuffer_width;
  uint32_t framebuffer_height;
  uint32_t framebuffer_bpp;
  uint32_t framebuffer_type;
  uint32_t color_info;
};