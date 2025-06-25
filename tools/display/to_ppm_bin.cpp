#include <stdint.h>
#include <stdio.h>

#include "src/datatypes.h"
#include "src/display_utils.h"
#include "src/logging.h"
#include "src/memory.h"

using namespace Emulator;

constexpr U32 WIDTH = 240;
constexpr U32 HEIGHT = 160;

struct Pixel {
  U8 r;
  U8 g;
  U8 b;
};

typedef Pixel Pixels[WIDTH * HEIGHT];

inline void AddPixel(Pixels &pixels, U32 x, U32 y, Pixel p) {
  if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0) {
    return;
  }
  pixels[y * WIDTH + x] = p;
}

inline Pixel GbaColorToPixelColor(U16 gba_color) {
  return {
      .r = U8(((gba_color >> 10u) & 0b11111) * 256 / 32),
      .g = U8(((gba_color >> 5u) & 0b11111) * 256 / 32),
      .b = U8(((gba_color >> 0u) & 0b11111) * 256 / 32),
  };
}

void ProcessTile(Tile8 &tile, U32 x_tl, U32 y_tl, U32 abs_x_offset,
                 U32 abs_y_offset, Pixels &pixels, U16 *palette_buffer) {
  U32 offset_x = x_tl * 8;
  U32 offset_y = y_tl * 8;
  for (U32 row = 0; row < 8; ++row) {
    U8 row_color_ptrs[8];
    memcpy(row_color_ptrs, &tile[row * 2], 4);
    memcpy(&row_color_ptrs[4], &tile[row * 2 + 1], 4);
    for (U32 col = 0; col < 8; ++col) {
      AddPixel(pixels, abs_x_offset + offset_x + col,
               abs_y_offset + offset_y + row,
               GbaColorToPixelColor(palette_buffer[row_color_ptrs[col]]));
    }
  }
}

struct Shape {
  U32 width_tl;
  U32 height_tl;
};

Shape GetShape(U32 shape, U32 size) {
  if (shape == 0b00 && size == 0b00) {
    return {.width_tl = 1, .height_tl = 1};
  } else if (shape == 0b01 && size == 0b00) {
    return {.width_tl = 2, .height_tl = 1};
  } else if (shape == 0b10 && size == 0b00) {
    return {.width_tl = 1, .height_tl = 2};
  } else if (shape == 0b00 && size == 0b01) {
    return {.width_tl = 2, .height_tl = 2};
  } else if (shape == 0b01 && size == 0b01) {
    return {.width_tl = 4, .height_tl = 1};
  } else if (shape == 0b10 && size == 0b01) {
    return {.width_tl = 1, .height_tl = 4};
  } else if (shape == 0b00 && size == 0b10) {
    return {.width_tl = 4, .height_tl = 4};
  } else if (shape == 0b01 && size == 0b10) {
    return {.width_tl = 4, .height_tl = 2};
  } else if (shape == 0b10 && size == 0b10) {
    return {.width_tl = 2, .height_tl = 4};
  } else if (shape == 0b00 && size == 0b11) {
    return {.width_tl = 8, .height_tl = 8};
  } else if (shape == 0b01 && size == 0b11) {
    return {.width_tl = 8, .height_tl = 4};
  } else if (shape == 0b10 && size == 0b11) {
    return {.width_tl = 4, .height_tl = 8};
  } else {
    ABORT("Invalid Combo of Shape and Size");
  }
}

void CreateBackground(const Memory::Memory &memory, Pixels &pixels) {
  ABORT("Unimplemented");
}

void CreateNormalSprite(const Memory::Memory &memory, OAM_t oam, Pixels &pixels,
                        DISPCNT_t DISPCNT) {

  if (DISPCNT.fields.om == 1) {
    ABORT("1D-tile mapping not supported");
  }

  Tile4 *tile_block = &((Tile4 *)&memory.VRAM[0x10000])[oam.fields.tid];
  LOG("TID: 0x%04x", oam.fields.tid);
  if (oam.fields.cm == 0) {
    LOG("Using 4bpp");
    ABORT("Unimplemented");
  } else {
    LOG("Using 8bpp");
    Tile8 *tiles = (Tile8 *)tile_block;
    Shape shape = GetShape(oam.fields.sh, oam.fields.sz);
    for (int w = 0; w < shape.width_tl; ++w) {
      for (int h = 0; h < shape.height_tl; ++h) {
        for (int i = 0; i < 16; ++i) {
          ProcessTile(tiles[h * (32 / 2) + w], w, h, oam.fields.x, oam.fields.y,
                      pixels, (U16 *)&memory.PaletteRAM[0x200]);
        }
      }
    }
  }
}

void CreateSprites(const Memory::Memory &memory, Pixels &pixels,
                   DISPCNT_t DISPCNT) {
  OAM_t *oams = (OAM_t *)memory.OAM;

  bool skip_next = false;
  for (U32 i = 0; i < sizeof(memory.OAM) / sizeof(OAM_t); ++i) {
    if (skip_next) {
      skip_next = false;
      continue;
    }

    // This is an affine sprite.
    if (oams[i].fields.om == 0b01 || oams[i].fields.om == 0b11) {
      skip_next = true;
    }

    if (oams[i].fields.om == 0b00) {
      if (oams[i].value != 0) {
        LOG("Sprite enabled for normal rendering!");
        CreateNormalSprite(memory, oams[i], pixels, DISPCNT);
      }
    }
  }
}

void CreateImage(const Memory::Memory &memory, Pixels &pixels) {
  DISPCNT_t DISPCNT = ReadHalfWordFromGBAMemory(memory, Emulator::DISPCNT_ADDR);
  LOG("0x%04x", DISPCNT.value);
  if (DISPCNT.fields.bg0) {
    CreateBackground(memory, pixels);
  }
  if (DISPCNT.fields.bg1) {
    CreateBackground(memory, pixels);
  }
  if (DISPCNT.fields.bg2) {
    CreateBackground(memory, pixels);
  }
  if (DISPCNT.fields.bg3) {
    CreateBackground(memory, pixels);
  }
  if (DISPCNT.fields.obj) {
    CreateSprites(memory, pixels, DISPCNT);
  }
}

void write_ppm(const char *outfile, const Pixels &pixels) {
  FILE *file_ptr;
  file_ptr = fopen(outfile, "w");
  if (file_ptr == NULL) {
    perror("Error writing to file");
    return;
  }
  fprintf(file_ptr, "P3\n");
  fprintf(file_ptr, "%d %d\n", WIDTH, HEIGHT);
  fprintf(file_ptr, "255\n");

  // Write pixel data
  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      Pixel p = pixels[y * WIDTH + x];
      fprintf(file_ptr, "%d %d %d ", p.r, p.g, p.b);
    }
    fprintf(file_ptr, "\n");
  }
  fclose(file_ptr);
}

int main(int argc, char *argv[]) {

  char *memory_bin = argv[1];

  FILE *file_pointer;
  U64 size;

  file_pointer = fopen(memory_bin, "rb");
  if (file_pointer == NULL) {
    perror("Error opening file");
    return 0;
  }

  // Determine file size
  fseek(file_pointer, 0, SEEK_END);
  size = ftell(file_pointer);
  rewind(file_pointer);

  // Read data from file
  Emulator::Memory::Memory *memory_ptr = new Emulator::Memory::Memory();
  Emulator::Memory::Memory &memory = *memory_ptr;
  size_t bytes_read = fread(memory_ptr, sizeof(char), size, file_pointer);
  if (bytes_read != size) {
    perror("Error reading file");
    fclose(file_pointer);
    return 0;
  }

  // Close the file
  fclose(file_pointer);

  U16 DISPCNT = ReadHalfWordFromGBAMemory(memory, Emulator::DISPCNT_ADDR);
  LOG("DISPCNT: 0x%04x", DISPCNT);

  Pixels pixels{};
  CreateImage(memory, pixels);

  write_ppm(argv[2], pixels);

  return 0;
}
