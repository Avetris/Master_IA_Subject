#include "engine/ui_manager.h"
#include "ia/defines.h"

#include <cstdio>
#include <engine\window.h>

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
    TextSprite sprite;
    sprite.setPositionUpLeft(x, y);
    _spriteMap.insert({ uid, sprite });
    _spriteMap[uid].loadText(text, SDL_Color FOREGROUND_COLOR, font_, true);
    _spriteMap[uid].setVisible(true);
    return uid;
}

void UIManager::setText(char* text, uint8_t uid)
{
    _spriteMap[uid].setText(text);
}

void UIManager::setColor(uint8_t uid, const SDL_Color& textColor)
{
    _spriteMap[uid].setTextColor(textColor);
}

void UIManager::removeText(uint8_t uid)
{
    _spriteMap.erase(uid);
}

void UIManager::drawPath(std::vector<MathLib::Vec2> path)
{
    const int numRect = path.size();
    SDL_Renderer* renderer = Window::instance().getRenderer();
    SDL_Rect* rect = new SDL_Rect[numRect];
    for (size_t i = 0; i < numRect; i++) {
        rect[i].x = path[i].x();
        rect[i].y = path[i].y();
        rect[i].w = 8;
        rect[i].h = 8;
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 128, 255);
    SDL_RenderFillRects(renderer, rect, numRect);
}

void UIManager::shutdown()
{
    _spriteMap.clear();
}
