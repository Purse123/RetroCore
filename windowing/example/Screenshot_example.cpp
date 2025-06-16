#include <iostream>
#include "calam.h"

#define WIDTH 200
#define HEIGHT 100

int main() {
  Canvas canvas(WIDTH, HEIGHT);
  Calam cfx(canvas);

  RGBA cyan = {0, 255, 255, 255};
  RGBA red = {255, 0, 0, 255};
  RGBA green = {0, 255, 0, 0};
  RGBA blue = {0, 0, 255, 255};

  cfx.putFillRect ({ 0, 0}, {25, 25}, blue);

  cfx.calam_save_to_ppm (canvas.getBuffer(), WIDTH, HEIGHT);
  
  canvas.clear();		// clear back buffer
  canvas.update();		// update texture in section
  canvas.present();		// swap the buffer
      
  SDL_Delay(1000);
  return 0;
}
