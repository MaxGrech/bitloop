// main.cpp
extern "C" 
{

// todo: Go with WebCodecs + mp4 remuxer (mp4box.js or mp4-muxer)

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

    // Call this each time you have a new RGBA frame at ptr (w�h�4 bytes)
    void add_frame(unsigned char* ptr, int w, int h) {
        EM_ASM({
          addFrame($0, $1, $2);
        }, ptr, w, h);
    }

    // Call this when you�re done; it will trigger the download
    void finish_video() {
        EM_ASM({ finishVideo(); });
    }

#else
    void add_frame(unsigned char* ptr, int w, int h) {}
    void finish_video(unsigned char* ptr, int w, int h) {}
#endif

} // extern "C"