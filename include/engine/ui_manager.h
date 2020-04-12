#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__ 1

#include <unordered_map>
#include <engine\text_sprite.h>
#include <engine\math.h>

#include <SDL/SDL_ttf.h>
#include <engine\defines.h>

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
    void renderPath();
    uint32_t addText(char* text, const uint32_t x = 0, const uint32_t y = 0, const bool isPoint = false);
    void setText(char* text, uint32_t uid);
    void setColor(uint32_t uid, const SDL_Color& textColor);
    void removeText(uint32_t uid);
    void setPath(std::vector<MathLib::Vec2> path, float timeLast);
    void setVisible(uint32_t uid, bool visible) { _spriteMap[uid].setVisible(visible); }
    uint32_t addAlarm();

    void shutdown();

  private:
    UIManager() {}

    uint32_t _UID = 1;
    std::unordered_map<uint32_t, TextSprite> _spriteMap;
    TTF_Font* _font = nullptr;
    TTF_Font* _fontPoint = nullptr;
    uint32_t _pathUID = 0;
    std::vector<MathLib::Vec2> _path;
};
#endif