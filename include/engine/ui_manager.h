#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__ 1

#include <unordered_map>
#include <engine\sprite.h>

#include <SDL/SDL_ttf.h>

class UIManager {
  public:
    ~UIManager() {}
    UIManager(UIManager const&) = delete;
    void operator=(UIManager const&) = delete;

    static UIManager& instance() {
      static UIManager instance;
      return  instance;
    }

    void init();
    void render();
    uint8_t addText(char* text, const uint32_t x = 0, const uint32_t y = 0);
    void setText(char* text, uint8_t uid);
    void removeText(uint8_t uid);
    void shutdown();

  private:
    UIManager() {}

    uint8_t _UID = 1;
    std::unordered_map<uint8_t, Sprite> _spriteMap;
    TTF_Font* font_ = nullptr;
};
#endif