#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_WIDTH 10
#define MAX_HEIGHT 10
#define SIGNATURE "PIERR"

typedef int32_t i32;
typedef int32_t Err;
typedef size_t u32;
typedef char c8;
typedef uint8_t u8;

u8 char2tile(c8 c) {
  switch (c) {
    case '0': return 0;		/* empty space */
    case '1': return 1;		/* Boundary    */
    case 'P': return 2;		/* Player      */
  default: return 255;		/* Invalid     */
  }
}

Err main(i32 argc, c8 *argv[]) {
  if (argc != 3) {
    printf("Usage: %s <input.txt> <output.pierr>\n", argv[0]);
    return -1;
  }

  const c8 *inBuffer = argv[1];
  const c8 *outBuffer = argv[2];

  FILE *in = fopen(inBuffer, "r");
  if (!in) {
    perror("error opening input file\n");
    return -1;
  }

  c8 *lines[MAX_HEIGHT];
  i32 width = -1 , height = 0;
  c8 buffer[MAX_WIDTH + 2];	/* 2 byte for `\r\n` */
  
  while (fgets(buffer, sizeof(buffer), in)) {
    i32 dataLen = strcspn(buffer, "\r\n");
    /* memset(buffer, dataLen, 0); */
    buffer[dataLen] = '\0';

    // width initialized as -1, width = no of char in that line
    if (width == -1) width = dataLen;
    
    // for every line check length matches width from if condition
    // prevent loader and renderer breaks
    else if (dataLen != width) {
      fprintf(stderr, "[ERROR] Line %d width mismatch: expected %d, got %d\n", height + 1, width, dataLen);
      fclose(in);
      return -1;
    }

    lines[height] = strdup(buffer);   /* when buffer reuse previous line are safe */
    if (!lines[height]) {
      perror("Memory Error");
      fclose(in);
      return -1;
    }

    height++;
    if (height > MAX_HEIGHT) {
      fprintf(stderr, "[ERROR] MAP HEIGHT MISMATCH\n");
      fclose(in);
      return -1;
    }
  }
  fclose(in);
  
  FILE *out = fopen(outBuffer, "wb");
  if (!out) {
    perror("error opening output file\n");
    return -1;
  }
  
  // 6 Bytes: *signature* "PIERR" (null-terminated)
  fwrite(SIGNATURE, 1, 5, out);
  
  // 4 Bytes: Width (int32)
  // 4 Bytes: Height (int32)
  i32 w = width;
  i32 h = height;

  fwrite(&w, sizeof(i32), 1, out);
  fwrite(&h, sizeof(i32), 1, out);
  
  // Other Bytes: Tile data (1 byte each tile)
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      u8 tile = char2tile(lines[y][x]);
      
      if (tile == 255) {
	fprintf(stderr, "Invalid character %c, at line %d col %d\n", lines[y][x], y + 1, x + 1);
	fclose(out);
	return -1;
      }
      fwrite(&tile, 1, 1, out);
    }
    free(lines[y]);
  }
  
  fclose(out);
  fprintf(stdout, "PIERR file %s created succesfully\n", outBuffer);
  return 0;
}
