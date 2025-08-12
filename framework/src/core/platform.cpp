#include "platform.h"
#include "imgui_custom.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <limits.h>
#elif defined(__linux__)
#include <unistd.h>
#include <limits.h>
#endif
#include <filesystem>

PlatformManager* PlatformManager::singleton = nullptr;

#ifdef __EMSCRIPTEN__
EM_JS(int, _is_mobile_device, (), {
  // 1. Prefer UA-Client-Hints when they exist (Chromium >= 89)
  if (navigator.userAgentData && navigator.userAgentData.mobile !== undefined) {
    return navigator.userAgentData.mobile ? 1 : 0;
  }

  // 2. Pointer/hover media-queries � quick and cheap
  if (window.matchMedia('(pointer: coarse)').matches &&
      !window.matchMedia('(hover: hover)').matches) {
    return 1;
  }

  // 3. Fallback to a light regex on the UA string
  return /Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini|Windows Phone/i
           .test(navigator.userAgent) ? 1 : 0;
});
#else
int _is_mobile_device()
{
    return 0;
}
#endif

void PlatformManager::init()
{
    is_mobile_device = _is_mobile_device();
}

void PlatformManager::update()
{
    SDL_GetWindowSizeInPixels(window, &gl_w, &gl_h);
    SDL_GetWindowSize(window, &win_w, &win_h);
}

void PlatformManager::resized()
{
    #ifdef __EMSCRIPTEN__
    emscripten_get_element_css_size("#canvas", &css_w, &css_h);
    float device_dpr = emscripten_get_device_pixel_ratio();
    fb_w = css_w * device_dpr;
    fb_h = css_h * device_dpr;
    SDL_SetWindowSize(window, fb_w, fb_h);
    emscripten_set_canvas_element_size("#canvas", fb_w, fb_h);
    #else
    SDL_GetWindowSizeInPixels(window, &fb_w, &fb_h);
    #endif

    Platform()->update();
}

bool PlatformManager::device_vertical()
{
    #ifdef __EMSCRIPTEN__
    EmscriptenOrientationChangeEvent orientation;
    if (emscripten_get_orientation_status(&orientation) == EMSCRIPTEN_RESULT_SUCCESS)
        return abs(orientation.orientationAngle % 180) < 90;
    #endif

    return false;
}

void PlatformManager::device_orientation(int* orientation_angle, int* orientation_index)
{
    #ifdef __EMSCRIPTEN__
    EmscriptenOrientationChangeEvent orientation;
    if (emscripten_get_orientation_status(&orientation) == EMSCRIPTEN_RESULT_SUCCESS)
    {
        if (orientation_angle) *orientation_angle = orientation.orientationAngle;
        if (orientation_index) *orientation_index = orientation.orientationIndex;
    }
    #else
    if (orientation_angle) *orientation_angle = 0;
    if (orientation_index) *orientation_index = 0;
    #endif
}

#ifdef __EMSCRIPTEN__
std::function<void(int, int)> _onOrientationChangedCB;
EM_BOOL _orientationChanged(int type, const EmscriptenOrientationChangeEvent* e, void* data)
{
    if (_onOrientationChangedCB)
        _onOrientationChangedCB(e->orientationIndex, e->orientationAngle);
    return EM_TRUE;
}
#endif

bool PlatformManager::device_orientation_changed(std::function<void(int, int)> onChanged [[maybe_unused]])
{
    #ifdef __EMSCRIPTEN__
    _onOrientationChangedCB = onChanged;
    EMSCRIPTEN_RESULT res = emscripten_set_orientationchange_callback(NULL, EM_FALSE, _orientationChanged);
    return (res == EMSCRIPTEN_RESULT_SUCCESS);
    #else
    return false;
    #endif
}

bool PlatformManager::is_mobile()
{
    #ifdef DEBUG_SIMULATE_MOBILE
    return true;
    #else
    return is_mobile_device;
    #endif
}

bool PlatformManager::is_desktop_native()
{
    #if defined __EMSCRIPTEN__ || defined FORCE_WEB_UI
    return false;
    #else
    return true;
    #endif
}

bool PlatformManager::is_desktop_browser()
{
    #ifdef __EMSCRIPTEN__
    return !is_mobile(); // Assumed desktop browser if mobile screen not detected
    #else
    return false; // Native desktop application
    #endif
}

float PlatformManager::font_scale()
{
    return is_mobile() ? 1.3f : 1.0f;
}


float PlatformManager::ui_scale_factor(float extra_mobile_mult)
{
    return is_mobile() ? (2.0f * extra_mobile_mult) : 1.0f;
}

float PlatformManager::line_height()
{
    return ImGui::GetFontSize();
}

float PlatformManager::input_height()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& s = ImGui::GetStyle();
    return io.DisplaySize.y / ImGui::GetFontSize() + s.FramePadding.y * 2.0f;
}

float PlatformManager::max_char_rows()
{
    ImGuiIO& io = ImGui::GetIO();
    return io.DisplaySize.y / ImGui::GetFontSize();
}

float PlatformManager::max_char_cols()
{
    ImGuiIO& io = ImGui::GetIO();
    return io.DisplaySize.x / ImGui::GetFontSize();
}

inline std::filesystem::path executable_dir()
{
    #if defined(_WIN32)
    std::wstring buf(MAX_PATH, L'\0');
    DWORD len = GetModuleFileNameW(nullptr, buf.data(), (DWORD)buf.size());
    if (len == 0)
        throw std::runtime_error("GetModuleFileNameW failed");
    buf.resize(len);
    return std::filesystem::path(buf).parent_path();

    #elif defined(__APPLE__)
    char buf[PATH_MAX];
    uint32_t size = sizeof(buf);
    if (_NSGetExecutablePath(buf, &size) != 0)
        throw std::runtime_error("_NSGetExecutablePath failed");
    return std::filesystem::canonical(buf).parent_path();

    #elif defined(__linux__)
    char buf[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (len == -1)
        throw std::runtime_error("readlink failed");
    buf[len] = '\0';
    return std::filesystem::canonical(buf).parent_path();

    #else   // last-ditch fallback
    return std::filesystem::current_path();
    #endif
}

inline std::filesystem::path resource_root()
{
    #ifdef __EMSCRIPTEN__
    return "/";
    #else
    return executable_dir();
    #endif
}

std::string PlatformManager::path(std::string_view virtual_path)
{
    std::filesystem::path p = resource_root();

    if (!virtual_path.empty() && virtual_path.front() == '/')
        virtual_path.remove_prefix(1); // trim leading '/'

    p /= virtual_path; // join
    p = p.lexically_normal(); // clean up
    return p.string();
}
