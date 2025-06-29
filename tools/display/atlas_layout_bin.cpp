#include <stdint.h>
#include <stdio.h>

#include "src/datatypes.h"
#include "src/display_utils.h"
#include "src/logging.h"
#include "src/memory.h"

using namespace Emulator;

constexpr U32 WIDTH = 256;
constexpr U32 HEIGHT = 256;

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

void CreateAtlasLayout(const Memory::Memory &memory, Pixels &pixels) {
  Tile8 *tiles = (Tile8 *)&memory.VRAM[0x10000];
  for (int w = 0; w < 16; ++w) {
    for (int h = 0; h < 32; ++h) {
      ProcessTile(tiles[h * (32 / 2) + w], w, h, 0, 0, pixels,
                  (U16 *)&memory.PaletteRAM[0x200]);
    }
  }
}

void CreateImage(const Memory::Memory &memory, Pixels &pixels) {
  CreateAtlasLayout(memory, pixels);
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

  Pixels pixels{};
  CreateImage(memory, pixels);

  write_ppm(argv[2], pixels);

  return 0;
}
