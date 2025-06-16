#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define SIGNATURE "PIERR"
#define SIGNATURE_LEN 5
#define MAX_SIZE 1024

typedef int32_t i32;
typedef int32_t Err;
typedef size_t u32;
typedef char c8;
typedef uint8_t u8;

u8 tile2char(u8 tile) {
  switch (tile) {
    case 0: return '0';
    case 1: return '1';
    case 2: return 'P';
    default: return '?';
  }
}

Err main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s <input.pierr> <output.txt>\n", argv[0]);
    return -1;
  }

  const c8 *inFile = argv[1];
  const c8 *outFile = argv[2];

  FILE *inBuffer = fopen(inFile, "rb");
  if (!inBuffer) {
    perror("[ERROR] error opening input file");
    return -1;
  }

  // 5 Bytes: Signature
  c8 sig[SIGNATURE_LEN + 1] = {0};
  if (fread(sig, 1, SIGNATURE_LEN, inBuffer) != SIGNATURE_LEN) {
    fprintf(stderr, "[ERROR] SIGNATURE NOT FOUND!\n");
    fclose(inBuffer);
    return -1;
  }
  if (strncmp(sig, SIGNATURE, SIGNATURE_LEN) != 0) {
    fprintf(stderr, "[ERROR] Invalid Signature!\n");
    fclose(inBuffer);
    return -1;
  }

  i32 width, height;
  // 4 Byte: Width (i32)
  if (fread(&width, sizeof(i32), 1, inBuffer) != 1) {
    fprintf(stderr, "[ERROR] Unable reading Width!\n");
    fclose(inBuffer);
    return -1;
  }
  // 4 Byte: Height (i32)
  if (fread(&height, sizeof(i32), 1, inBuffer) != 1) {
    fprintf(stderr, "[ERROR] Unable reading Height!\n");
    fclose(inBuffer);
    return -1;
  }
  if (width <= 0 || height <= 0 || width > MAX_SIZE || height > MAX_SIZE) {
    fprintf(stderr, "[ERROR] Invalid Dimension!\n");
    fclose(inBuffer);
    return -1;
  }

  // Other Byte: Tile data (c8)
  u32 tileSize = (u32)width * (u32)height;
  u8 *tile_data = malloc(tileSize);
  if (!tile_data) {
    fprintf(stderr, "[ERROR] Allocation failed\n");
    fclose(inBuffer);
    return -1;
  }

  if (fread(tile_data, 1, tileSize, inBuffer) != tileSize) {
    fprintf(stderr, "[ERROR] reading map data\n");
    free(tile_data);
    fclose(inBuffer);
    return -1;
  }
  fclose(inBuffer);

  FILE *outBuffer = fopen(outFile, "wb");
  if (!outBuffer) {
    perror("[ERROR] error opening output file");
    free(tile_data);
    return -1;
  }

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      c8 c = tile2char(tile_data[y * width + x]);
      fputc(c, outBuffer);
    }
    fputc('\n', outBuffer);
  }
  fclose(outBuffer);
  free(tile_data);

  fprintf(stdout, "Decoded PIERR to <%s> successfully\n", outFile);
  return 0;
}
