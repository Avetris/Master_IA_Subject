#include "engine/ui_manager.h"
#include "ia/defines.h"

#include <cstdio>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

void UIManager::init() {
    font_ = TTF_OpenFont(FONT_FILE, FPS_FONT_SIZE);
    if (!font_) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    }
}

void UIManager::render()
{
    for (auto& pair : _spriteMap) {
        pair.second.render();
    }
}

uint8_t UIManager::addText(char* text, const uint32_t x, const uint32_t y)
{
    uint8_t uid = _UID++;
    Sprite sprite;
    sprite.setPositionUpLeft(x, y);
    _spriteMap.insert({ uid, sprite });
    setText(text, uid);
    return uid;
}

void UIManager::setText(char* text, uint8_t uid)
{
    _spriteMap[uid].loadFromRenderedText(text, SDL_Color FOREGROUND_COLOR, font_, true);
    _spriteMap[uid].setVisible(true);
}

void UIManager::removeText(uint8_t uid)
{
    _spriteMap.erase(uid);
}

void UIManager::shutdown()
{
    _spriteMap.clear();
}
