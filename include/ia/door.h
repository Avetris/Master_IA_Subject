
#ifndef __DOOR_H__
#define __DOOR_H__ 1

#include <cstdint>
#include <ia/defines.h>
#include <engine\window.h>

class Door {
    public:
        Door(){
        };
        ~Door() {};
        void init(MathLib::Vec2 from, MathLib::Vec2 to) {
            _from = from;
            _to = to;
            rect.x = from.x();
            rect.y = from.y();
            rect.w = to.x() - from.x();
            rect.h = to.y() - from.y();
        }
        void render() const {
            SDL_Renderer* renderer = Window::instance().getRenderer();
            if (_isOpen) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            }
            SDL_RenderFillRect(renderer, &rect);
        }
        bool isOpen() { return _isOpen; }
        void changeStatus() { _isOpen = !_isOpen; }
        std::pair<MathLib::Vec2, MathLib::Vec2> getCoord() { return { _from, _to }; }
    private:
        bool _isOpen = false;
        SDL_Rect rect;
        MathLib::Vec2 _from;
        MathLib::Vec2 _to;
};

#endif