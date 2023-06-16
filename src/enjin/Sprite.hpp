#ifndef SPRITE_HPP
#define SPRITE_HPP
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "utils/Types.hpp"
class Sprite
{
public:
    Sprite()
    {
        _texture = nullptr;
        _texture1bit = nullptr;
        _width = 0;
        _height = 0;
        _matte = 0;
        _position.x = 0;
        _position.y = 0;
        _frame = 0;
    };
    Sprite(const uint8_t texture[], uint8_t width, uint8_t height, uint8_t matte = 0x8)
    {
        _texture = texture;
        _texture1bit = nullptr;
        _width = width;
        _height = height;
        _matte = matte;
        _position.x = 0;
        _position.y = 0;
        _frame = 0;
    };
    Sprite(const bool texture[], uint8_t width, uint8_t height, uint8_t matte = 0x8)
    {
        _texture = nullptr;
        _texture1bit = texture;
        _height = height;
        _width = width;
        _matte = matte;
        _position.x = 0;
        _position.y = 0;
        _frame = 0;
    };

    void draw(GFXcanvas8 &canvas)
    {
        if (_texture)
        {
            const uint8_t *texture = (const uint8_t *)(_texture + (_frame * _width * _height));
            canvas.drawGrayscaleBitmap(_position.x, _position.y, texture, _matte, _width, _height);
        }
        else if (_texture1bit && _matte < 2)
        {
            const bool *texture = (const bool *)(_texture1bit + (_frame * _width * _height));
            canvas.drawGrayscaleBitmap(_position.x, _position.y, texture, (bool)_matte, _width, _height);
        }
        else if (_texture1bit && _matte > 2)
        {
            const bool *texture = (const bool *)(_texture1bit + (_frame * _width * _height));
            canvas.drawGrayscaleBitmap(_position.x, _position.y, texture, _width, _height);
        }
    };

    void setTexture(const uint8_t texture[], uint8_t width, uint8_t height, uint8_t matte = 16)
    {
        _texture = texture;
        _width = width;
        _height = height;
        _matte = matte;
    };
    void setTexture(const bool texture[], uint8_t width, uint8_t height, uint8_t matte = 16)
    {
        _texture1bit = texture;
        _width = width;
        _height = height;
        _matte = matte;
    };
    void setTexture(const uint8_t texture[], uint8_t frameId)
    {
        _texture = texture;
        _frame = frameId;
    };

    void setTexture(const bool texture[], uint8_t frameId)
    {
        _texture1bit = texture;
        _frame = frameId;
    };

    void setTexture(uint8_t frameId)
    {
        _frame = frameId;
    };

    void setPosition(int8_t x, int8_t y)
    {
        _position.x = x;
        _position.y = y;
    };
    void setPosition(Vector2 pos)
    {
        _position.x = pos.x;
        _position.y = pos.y;
    };
    void setMatte(uint8_t matte)
    {
        _matte = matte;
    };

    uint8_t _width, _height, _frame;
    Vector2 _position;
    uint8_t _matte;

protected:
    const uint8_t *_texture;
    const bool *_texture1bit;
};

#endif // !SPRITE_H