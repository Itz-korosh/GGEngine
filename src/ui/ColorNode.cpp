#include "ui/ColorNode.hpp"

namespace UI {

RGB::RGB(std::uint8_t red, std::uint8_t green, std::uint8_t blue)
    : r(red), g(green), b(blue)
{
}

GLRGB RGB::ToGL() const
{
    return GLRGB{ r / 255.0f, g / 255.0f, b / 255.0f };
}

RGBA::RGBA(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha)
    : r(red), g(green), b(blue), a(alpha)
{
}

GLRGBA RGBA::ToGL() const
{
    return GLRGBA{ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
}

} // namespace UI