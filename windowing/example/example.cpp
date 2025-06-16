#include <iostream>
#include "../include/calam.h"

#define FACTOR 90
#define WIDTH (16 * FACTOR)
#define HEIGHT (9 * FACTOR)

int main() {
  Canvas canvas(WIDTH, HEIGHT);
  Calam cfx(canvas);

  // point
  vec2D p = {300, 100};
  RGBA cyan = {0, 255, 255, 255};
  cfx.putPixel (p, cyan);

  // line w BSA
  vec2D p1 = {200, 400};
  vec2D p2 = {400, 200};
  cfx.putLine (p1, p2, cyan);

  // rectangle
  vec2D rect_p1 = {600, 400};
  vec2D rect_p2 = {700, 500};
  cfx.putRect (rect_p1, rect_p2, cyan); // make rectangle
  cfx.putFillRect ({ 0, 0}, {50, 50}, cyan);

  // triangle
  vec2D t_p1 = {350, 350};
  vec2D t_p2 = {650, 350};
  vec2D t_p3 = {500, 500};
  // cfx.putTriangle (t_p1, t_p2, t_p3, cyan);
  cfx.putFillTriangle (t_p1, t_p2, t_p3, cyan);

  // circle
  vec2D circle_center = {100, 100};
  int radius = 50;
  cfx.putCircle (circle_center, radius, cyan);

  // ellipse
  vec2D ellipse_center = {200, 200};
  vec2D ellipse_radius = {100, 50};
  cfx.putEllipse (ellipse_center, ellipse_radius, cyan);

  canvas.clear();		// clear back buffer
  canvas.update();		// update texture in section
  canvas.present();		// swap the buffer
      
  SDL_Delay(1000);
  return 0;
}
