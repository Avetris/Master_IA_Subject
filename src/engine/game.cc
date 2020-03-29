//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "engine/game.h"
#include "engine/debug_draw.h"
#include "engine/window.h"
#include "engine/ui_manager.h"
#include "ia/defines.h"

#include <cstdio>
#include "engine/math.h"

Game::~Game()
{
    UIManager::instance().shutdown();
}

void Game::init() {
  UIManager::instance().init();
}

void Game::start() {
  uint32_t fps_time{ 0 };
  uint32_t fps_time_acc{ 0 };

  uint32_t next_game_tick = SDL_GetTicks();
  uint32_t update_loops = 0;
  uint32_t render_loops = 0;

  const uint32_t skip_ticks = 1000 / TICKS_PER_SECOND;

  while (!quit_) {
    uint32_t loops = 0;
    while ((SDL_GetTicks() > next_game_tick) && (loops < MAX_FRAME_SKIP)) {
      handleInput();
      update(skip_ticks);

      next_game_tick += skip_ticks;
      ++loops;
      ++update_loops;
    }

    render();

    const uint32_t c_time = SDL_GetTicks();
    fps_time_acc += (c_time - fps_time);
    fps_time = c_time;
    ++render_loops;
    if (render_loops > 100) {        //show stats each 100 frames
      const float fps = 1000.0f / (fps_time_acc / 100.0f);
      const float ratio = static_cast<float>(render_loops) / static_cast<float>(update_loops);
      char text[255];
      sprintf_s(text, "%d RFPS      %d UFPS", static_cast<uint32_t>(fps), static_cast<uint32_t>(fps / ratio));
      if (fps_sprite_ > 0) {
          UIManager::instance().setText(text, fps_sprite_);
      }
      else {
          fps_sprite_ = UIManager::instance().addText(text);
      }

      render_loops = 0;
      update_loops = 0;
      fps_time_acc = 0;
    }
  }
}

void Game::shutdown() {
  scenes_[curr_scene_]->shutdown();
  for (uint8_t i = 0; i < SCENE_NUMBER; ++i) {
    delete scenes_[i];
  }
}

void Game::handleInput() {
  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    if (e.type == SDL_QUIT) {
      quit_ = true;
    }
    if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
        scenes_[curr_scene_]->handleMouseEvent(e);
    }    
    if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_ESCAPE: quit_ = true; break;
        case SDLK_F1:   nextScene(-1); break;
        case SDLK_F2:  nextScene(+1); break;
        default:{}
      }
      scenes_[curr_scene_]->handleKeyEvent(e.key.keysym.sym);
    }
  }
}

void Game::update(const uint32_t dt) {
  scenes_[curr_scene_]->update(dt);
}

void Game::render() {
  SDL_Renderer* renderer = Window::instance().getRenderer();
  SDL_SetRenderDrawColor(renderer, 0xD0, 0xD0, 0xD0, 0xFF);
  SDL_RenderClear(renderer);

  scenes_[curr_scene_]->render();
  DebugDraw::render();
  UIManager::instance().render();

  SDL_RenderPresent(renderer);
}

void Game::nextScene(const int8_t sign) {
  scenes_[curr_scene_]->shutdown();
  curr_scene_ += sign;
  curr_scene_ = clamp<int8_t>(curr_scene_, 0, SCENE_NUMBER - 1);
  scenes_[curr_scene_]->init();
}