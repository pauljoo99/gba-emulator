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

void CreateBackground(const Memory::Memory &memory, Pixels &pixels) {
  ABORT("Unimplemented");
}

void CreateSprite(const Memory::Memory &memory, Pixels &pixels) {
  ABORT("Unimplemented");
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
    CreateSprite(memory, pixels);
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
  pixels[0] = {.r = 255, .g = 0, .b = 0};

  CreateImage(memory, pixels);

  write_ppm(argv[2], pixels);

  return 0;
}
