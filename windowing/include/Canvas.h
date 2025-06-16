#pragma once
#include <SDL2/SDL.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <fstream>

typedef uint32_t u32;

class Canvas {
 private:
  int width, height;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  u32* frameBuffer;
  
 public:
  Canvas(int width, int height) : width(width), height(height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      std::cerr << "[SDL ERROR] Init failed: " << SDL_GetError() << std::endl;
      exit(1);
    }

    window = SDL_CreateWindow("Calam", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
      std::cerr << "[SDL ERROR] CreateWindow failed: " << SDL_GetError() << std::endl;
      SDL_Quit();
      exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
      std::cerr << "[SDL ERROR] CreateRenderer failed: " << SDL_GetError() << std::endl;
      SDL_DestroyWindow(window);
      SDL_Quit();
      exit(1);
    }

    texture = SDL_CreateTexture(
      renderer, SDL_PIXELFORMAT_ABGR8888,
      // SDL_TEXTUREACCESS:
      // 1. STATIC: change rarely, not locable
      // 2. STREAMING: change frequently, locable
      // 3. Texture used as render target
      SDL_TEXTUREACCESS_STREAMING,
      width, height
    );
    if (!texture) {
      std::cerr << "[SDL ERROR] CreateTexture failed: " << SDL_GetError() << std::endl;
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);
      SDL_Quit();
      exit(1);
    }

    frameBuffer = new u32[width * height];
    memset(frameBuffer, 0, width * height * sizeof(u32));
  }

  ~Canvas() {
    delete[] frameBuffer;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }

  void clear() {
    SDL_RenderClear(renderer);	/* Clear rendering target */
  }

  void update() {
    // rect = NULL, represent area to update, NULL for entire texture
    // const void *pixels = frameBuffer, ray pixel data in texture format
    // int pitch = no of byte in row including padding
    SDL_UpdateTexture(texture, NULL, frameBuffer, width * sizeof(u32));
  }

  void present() {
    // copy portion of current rendering target
    // SDL_Rect* srcrect: source, dstrect: destination (NULL strech to fill given rect)
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer); /* Present rendered image to screen */
  }

  u32* getBuffer() { return frameBuffer; }
  int getWidth() const { return width; }
  int getHeight() const { return height; }

};
