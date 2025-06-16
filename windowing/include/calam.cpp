#include "calam.h"

Calam::Calam(Canvas &canvas) : canvas(canvas) {}

void Calam::putPixel(vec2D p, RGBA color) {
  if (p.x < 0 || p.y < 0 || p.x >= canvas.getWidth() || p.y >= canvas.getHeight()) return;
  canvas.getBuffer()[p.y * canvas.getWidth() + p.x] =
    (color .a << 24) | (color .b << 16) | (color .g << 8) | color.r;
}

void Calam::putLine(vec2D p1, vec2D p2, RGBA c) {
  int dx = abs(p2.x - p1.x);
  int dy = abs(p2.y - p1.y);

  int incrementer_x = (p2.x > p1.x) ? 1 : -1;
  int incrementer_y = (p2.y > p1.y) ? 1 : -1;

  if (dy <= dx) {
    int desPar = 2 * dy - dx;
    for (int i = 0; i <= dx; ++i) {
      putPixel(p1, c);
      p1.x += incrementer_x;
      if (desPar < 0) {
        desPar += 2 * dy;
      } else {
        desPar += 2 * dy - 2 * dx;
        p1.y += incrementer_y;
      }
    }
  } else {
    int desPar = 2 * dx - dy;
    for (int i = 0; i <= dy; ++i) {
      putPixel(p1, c);
      p1.y += incrementer_y;
      if (desPar < 0) {
        desPar += 2 * dx;
      } else {
        desPar += 2 * dx - 2 * dy;
        p1.x += incrementer_x;
      }
    }
  }
}

void Calam::putRect(vec2D p1, vec2D p2, RGBA c) {
  putLine(p1, {p2.x, p1.y}, c);
  putLine(p1, {p1.x, p2.y}, c);
  putLine(p2, {p1.x, p2.y}, c);
  putLine(p2, {p2.x, p1.y}, c);
}

void Calam::putFillRect(vec2D p1, vec2D p2, RGBA c) {
  int x1 = std::min(p1.x, p2.x);
  int y1 = std::min(p1.y, p2.y);
  int x2 = std::max(p1.x, p2.x);
  int y2 = std::max(p1.y, p2.y);

  x1 = std::max(0, x1);
  y1 = std::max(0, y1);
  x2 = std::min(canvas.getWidth(), x2);
  y2 = std::min(canvas.getHeight(), y2);

  for (int y = y1; y < y2; y++) {
    for (int x = x1; x < x2; x++) {
      putPixel({x, y}, c);
    }
  }
}


void Calam::putTriangle (vec2D p1, vec2D p2, vec2D p3, RGBA c) {
  putLine(p1, p2, c);
  putLine(p3, p2, c);
  putLine(p1, p3, c);
}

void Calam::putFillTriangle (vec2D v0, vec2D v1, vec2D v2, RGBA c) {
  if (v1.y < v0.y) std::swap(v0, v1);
  if (v2.y < v0.y) std::swap(v0, v2);
  if (v2.y < v1.y) std::swap(v1, v2);

  auto edgeInterp = [](int y0, int y1, int x0, int x1, int y) {
    if (y1 == y0) return x0;
    return x0 + (x1 - x0) * (y - y0) / (y1 - y0);
  };

  auto drawScanline = [&](int y, int xStart, int xEnd) {
    if (xStart > xEnd) std::swap(xStart, xEnd);
    for (int x = xStart; x <= xEnd; x++) {
      putPixel({x, y}, c);
    }
  };

    // Fill bottom part (v0-v1-v2)
  for (int y = v0.y; y <= v1.y; y++) {
    int xa = edgeInterp(v0.y, v1.y, v0.x, v1.x, y);
    int xb = edgeInterp(v0.y, v2.y, v0.x, v2.x, y);
    drawScanline(y, xa, xb);
  }

  // Fill top part (v1-v2)
  for (int y = v1.y + 1; y <= v2.y; y++) {
    int xa = edgeInterp(v1.y, v2.y, v1.x, v2.x, y);
    int xb = edgeInterp(v0.y, v2.y, v0.x, v2.x, y);
    drawScanline(y, xa, xb);
  }
}

void Calam::putCircle (vec2D center, int radius, RGBA c) {
  int x = 0;
  int y = radius;
  int p0 = 1 - radius;

  while (x <= y) {
    putPixel ({center.x + x, center.y + y}, c);
    putPixel ({center.x - x, center.y + y}, c);
    putPixel ({center.x + x, center.y - y}, c);
    putPixel ({center.x - x, center.y - y}, c);

    putPixel ({center.x + y, center.y + x}, c);
    putPixel ({center.x - y, center.y + x}, c);
    putPixel ({center.x + y, center.y - x}, c);
    putPixel ({center.x - y, center.y - x}, c);

    if (p0 < 0)
      p0 += 2 * x + 1;
    else {
      p0 += 2 * x + 1 - 2 * y;
      y--;
    }
    x++;
  }
}

void Calam::putEllipse (vec2D center, vec2D radius, RGBA c) {
  int x = 0, y = radius.y;

  // initial paramter for region 1
  // slope < 1, x increments & y-plot
  int p10 = pow(radius.y, 2)
          - pow(radius.x, 2) * radius.y
          + pow(radius.x, 2) / 4;
  int dx = 2 * pow(radius.y, 2) * x;
  int dy = 2 * pow(radius.x, 2) * y;

  while (dx < dy) {
    putPixel ({center.x + x, center.y + y}, c);
    putPixel ({center.x - x, center.y + y}, c);
    putPixel ({center.x + x, center.y - y}, c);
    putPixel ({center.x - x, center.y - y}, c);

    x++;
    if (p10 < 0) {
      dx += 2 * pow(radius.y, 2);
      p10 += dx + pow(radius.y, 2);
    } else {
      y--;
      dx += 2 * pow(radius.y, 2);
      dy -= 2 * pow(radius.x, 2);
      p10 += dx - dy + pow(radius.y, 2);
    }
  }

  // initial paramter for region 2
  // slope > 1, y increments & x-plot
  int p20 = pow(radius.y, 2) * pow(x + 0.5, 2)
          + pow(radius.x, 2) * pow(y - 1, 2)
          - pow(radius.x, 2) * pow(radius.y, 2);
  while (y >= 0) {
    putPixel ({center.x + x, center.y + y}, c);
    putPixel ({center.x - x, center.y + y}, c);
    putPixel ({center.x + x, center.y - y}, c);
    putPixel ({center.x - x, center.y - y}, c);

    y--;
    if (p20 > 0) {
      dy -= 2 * pow(radius.x, 2);
      p20 += pow(radius.x, 2) - dy;
    } else {
      x++;
      dx += 2 * pow(radius.y, 2);
      dy -= 2 * pow(radius.x, 2);
      p20 += dx - dy + pow(radius.x, 2);
    }
  }
}

/*
// Portable Pixel Map
Errno Calam::calam_save_to_ppm (int width, int height) {
  Errno resultNo;
    
  FILE *fp = fopen("output.ppm", "wb");
  if (fp == NULL) return_defer(-1);

  fprintf(fp, "P6\n%zu %zu\n255\n", width, height);
  if (ferror(fp)) return_defer(-1);

  u32 *frameBuffer = canvas_getBuffer(canvas);
  for (int i = 0; i < canvas_getWidth(canvas) * canvas_getHeight(canvas); i++) {
    fputc((buf[i] >> 8 * 0) & 0xFF, fp);  // r
    fputc((buf[i] >> 8 * 1) & 0xFF, fp);  // g
    fputc((buf[i] >> 8 * 2) & 0xFF, fp);  // b
  }
  if (ferror(fp)) return_defer(-1);

 defer_return:
  if (fp) fclose(fp);
  return resultNo;
}
*/
