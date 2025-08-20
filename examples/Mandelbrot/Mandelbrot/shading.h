#pragma once
#include "types.h"


enum class MandelSmoothing
{
    NONE = 0,
    ITER = 1,
    DIST = 2,
    MIX = 3, // ITER | DIST
    COUNT
};

enum class MandelTransform
{
    NONE,
    FLATTEN,
    //COUNT
};

enum ColorGradientTemplate
{
    GRADIENT_CUSTOM,

    GRADIENT_CLASSIC,
    GRADIENT_SINUSOIDAL_RAINBOW_CYCLE,
    GRADIENT_WAVES,

    GRADIENT_TEMPLATE_COUNT
};

static const char* ColorGradientNames[GRADIENT_TEMPLATE_COUNT] = {
    "",
    "CLASSIC",
    "SINUSOIDAL_RAINBOW_CYCLE",
    "WAVES"
};


template<ColorGradientTemplate type>
inline void colorGradientTemplate(double t, uint8_t& r, uint8_t& g, uint8_t& b)
{
    if constexpr (type == GRADIENT_CLASSIC)
    {
        r = (uint8_t)(9 * (1 - t) * t * t * t * 255);
        g = (uint8_t)(15 * (1 - t) * (1 - t) * t * t * 255);
        b = (uint8_t)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
    }
    else if constexpr (type == GRADIENT_SINUSOIDAL_RAINBOW_CYCLE)
    {
        float a = (float)t * 3.14159265f;
        r = (uint8_t)(sin(a) * sin(a) * 255);
        g = (uint8_t)(sin(a + 2.0944f) * sin(a + 2.0944f) * 255);
        b = (uint8_t)(sin(a + 4.1888f) * sin(a + 4.1888f) * 255);
    }
}

inline void colorGradientTemplate(ColorGradientTemplate type, double t, uint8_t& r, uint8_t& g, uint8_t& b)
{
    switch (type)
    {
    case GRADIENT_CLASSIC:                  colorGradientTemplate<GRADIENT_CLASSIC>(t, r, g, b); break;
    case GRADIENT_SINUSOIDAL_RAINBOW_CYCLE: colorGradientTemplate<GRADIENT_SINUSOIDAL_RAINBOW_CYCLE>(t, r, g, b); break;
    default: break;
    }
}