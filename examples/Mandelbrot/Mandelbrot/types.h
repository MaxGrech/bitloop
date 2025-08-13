#include <math.h>
#include <cmath>

enum MandelFlag : uint32_t
{
    // bits
    MANDEL_DYNAMIC_ITERS = 1u << 0,
    MANDEL_SHOW_AXIS = 1u << 1,
    MANDEL_FLATTEN = 1u << 2,
    MANDEL_DYNAMIC_COLOR_CYCLE = 1u << 3,
    MANDEL_NORMALIZE_DEPTH = 1u << 4,

    // bitmasks
    MANDEL_FLAGS_MASK = 0x000FFFFFu, // max 24 bit-flags
    MANDEL_SMOOTH_MASK = 0x00F00000u, // max 16 smooth types
    MANDEL_VERSION_MASK = 0xFF000000u, // max 255 versions

    MANDEL_SMOOTH_BITSHIFT = 20,
    MANDEL_VERSION_BITSHIFT = 24
};

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

struct EscapeFieldPixel
{
    double depth;
    double dist;

    double final_depth;
    double final_dist;
};

struct EscapeField : public std::vector<EscapeFieldPixel>
{
    int compute_phase;

    double min_depth = 0.0;
    double max_depth = 0.0;

    double min_dist = 0.0;
    double max_dist = 0.0;

    int w = 0, h = 0;

    EscapeField(int phase) : compute_phase(phase)
    {
    }

    void setAllDepth(double value)
    {
        for (int i = 0; i < size(); i++)
            std::vector<EscapeFieldPixel>::at(i) = { value, value };
    }
    void setDimensions(int _w, int _h)
    {
        if (size() >= (_w * _h))
            return;
        w = _w;
        h = _h;
        resize(w * h, { -1.0, -1.0 });
    }

    EscapeFieldPixel& operator ()(int x, int y)
    {
        return std::vector<EscapeFieldPixel>::at(y * w + x);
    }

    EscapeFieldPixel& at(int x, int y)
    {
        return std::vector<EscapeFieldPixel>::at(y * w + x);
    }

    EscapeFieldPixel* get(int x, int y)
    {
        int i = y * w + x;
        if (i < 0 || i >= size()) return nullptr;
        return data() + i;
    }
};