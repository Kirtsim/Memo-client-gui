#pragma once

namespace memo::model {

struct Color
{
    int red = 0;
    int green = 0;
    int blue = 0;

    Color() = default;
    Color(int red, int green, int blue);
};

inline Color::Color(int red, int green, int blue)
    : red(red), green(green), blue(blue)
{
}

} // namespace memo::model
