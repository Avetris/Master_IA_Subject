
#ifndef __TEXT_SPRITE_H__
#define __TEXT_SPRITE_H__ 1

#include <engine/sprite.h>

class TextSprite : public Sprite {
public:
    TextSprite() {};
    ~TextSprite() {};

    bool loadText(char* textureText, const SDL_Color& textColor, TTF_Font* font, const bool shadow = false, const bool wrapped = false) {
        _text = textureText;
        _textColor = textColor;
        _font = font;
        _shadow = shadow;
        _wrapped = wrapped;
        return loadFromRenderedText(textureText, textColor, font, shadow, wrapped);
    }

    void setText(char* text) {
        free();
        _text = text;
        loadFromRenderedText(text, _textColor, _font, _shadow, _wrapped);
    }


    void setTextColor(const SDL_Color& color) {
        free();
        _textColor = color;
        loadFromRenderedText(_text, color, _font, _shadow, _wrapped);
    }

private:
    char* _text;
    SDL_Color _textColor;
    TTF_Font* _font;
    bool _shadow, _wrapped;
};

#endif