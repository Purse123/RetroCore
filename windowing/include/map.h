// TODOS:
// 1. Loading map with text file
// 2. Multiple map levels
// 3. Draw player's position (maybe FOV too)

#pragma once
#include "calam.h"
#include "Canvas.h"

#define GRIDCOLS 10
#define GRIDROWS 10

class GridRenderer {
 public:
  GridRenderer(Canvas &canvas, Calam &calam, int gridCols, int gridRows):
    canvas(canvas), calam(calam), cols(gridCols), rows(gridRows) {
    tileWidth = canvas.getWidth() / cols;
    tileHeight = canvas.getHeight() / rows;
  }

  void draw_minimap (int world[GRIDCOLS][GRIDROWS], RGBA wallColor, RGBA floorColor) {
    for (int row = 0; row <= rows; ++row) {
      for (int col = 0; col <= cols; ++col) {
	int value = world[row][col];
	RGBA color = (value == 1) ? wallColor : floorColor;

	int x = col * tileWidth;
	int y = row * tileHeight;

	int w = canvas.getWidth();
	int h = canvas.getHeight();

	calam.putFillRect({x, y}, {w, h}, color);
      }
    }
  }
  
private:
  Canvas &canvas;
  Calam &calam;
  int cols, rows;
  int tileWidth, tileHeight;
};
