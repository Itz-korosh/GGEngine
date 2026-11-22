#pragma once

#include <cstdint>

namespace UI {

struct GLRGB {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
};

struct GLRGBA {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 1.0f;
};

class RGB {
public:
    std::uint8_t r = 0;
    std::uint8_t g = 0;
    std::uint8_t b = 0;

    RGB() = default;
    RGB(std::uint8_t red, std::uint8_t green, std::uint8_t blue);

    GLRGB ToGL() const;
};

class RGBA {
public:
    std::uint8_t r = 0;
    std::uint8_t g = 0;
    std::uint8_t b = 0;
    std::uint8_t a = 255;   // 255 = fully visible, 0 = invisible

    RGBA() = default;
    RGBA(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha = 255);

    GLRGBA ToGL() const;
};

} // namespace UI