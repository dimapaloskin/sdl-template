#include <SDL.h>
#include <log/log.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define FRAME_TARGET_TIME_MS 1000.0f / 60.0f

typedef struct {
  bool is_running;
  float delta;
  uint32_t prev_frame_time;
  SDL_Window *window;
  SDL_Renderer *renderer;
} app_t;

bool init_sdl(app_t *app) {
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    log_error("can not init sdl: %s", SDL_GetError());
    return false;
  }

  app->window = SDL_CreateWindow(
    "App",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    800,
    600,
    SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE
  );
  if (app->window == NULL) {
    log_error("can not initialize window: %s", SDL_GetError());
    return false;
  }

  app->renderer =
    SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (app->renderer == NULL) {
    log_error("can not initialize renderer: %s", SDL_GetError());
    return false;
  }

  SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);

  return true;
}

void free_sdl(app_t *app) {
  SDL_DestroyRenderer(app->renderer);
  SDL_DestroyWindow(app->window);
  SDL_Quit();
}

void sync_frame_rate(app_t *app) {
  if (app->prev_frame_time == 0) {
    app->prev_frame_time = SDL_GetTicks();
  }

  uint32_t time_to_wait = FRAME_TARGET_TIME_MS - (SDL_GetTicks() - app->prev_frame_time);
  if (time_to_wait > 0 && time_to_wait < FRAME_TARGET_TIME_MS) {
    SDL_Delay(time_to_wait);
  }

  app->delta = (SDL_GetTicks() - app->prev_frame_time) / 1000.f;
  app->prev_frame_time = SDL_GetTicks();
}


void process_input(app_t *app) {
  SDL_Event event;

  if (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      app->is_running = false;
    }

    if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          app->is_running = false;
          break;
        default:
          break;
      }
    }
  }
}

void render(app_t *app) {
  SDL_SetRenderDrawColor(app->renderer, 0xDC, 0xDC, 0xDC, 0);
  SDL_RenderClear(app->renderer);

  SDL_RenderPresent(app->renderer);
}

int main() {
  log_debug("Running...");

  app_t app = {
    .is_running = true,
  };

  if (!init_sdl(&app)) {
    log_debug("Can not initialize SDL");
    return false;
  }

  while (app.is_running) {
    sync_frame_rate(&app);
    process_input(&app);
    render(&app);
  }

  free_sdl(&app);
}
