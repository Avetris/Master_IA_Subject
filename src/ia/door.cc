#include <cstdint>
#include <ia/defines.h>
#include <ia/door.h>
#include <engine\window.h>
#include <ia\pathfinding\path_manager.h>


void Door::init(MathLib::Vec2 from, MathLib::Vec2 to) {
    _from = from;
    _to = to;
    rect.x = from.x();
    rect.y = from.y();
    rect.w = to.x() - from.x();
    rect.h = to.y() - from.y();
}

void Door::render() const {
    SDL_Renderer* renderer = Window::instance().getRenderer();
    if (_isOpen) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }
    SDL_RenderFillRect(renderer, &rect);
}

void Door::changeStatus() {
    _isOpen = !_isOpen;
    PathManager::instance().applyDoor(this);
}

bool Door::isDoor(MathLib::Vec2 coord) {
    for (float x = _from.x(); x < _to.x(); x++) {
        for (float y = _from.y(); y < _to.y(); y++) {
            if (x == coord.x() && y == coord.y()) {
                return true;
            }
        }
    }
    return false;
}