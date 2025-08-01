﻿/*
 *                 __    _ __  __                
 *                / /_  /_/ /_/ /___  ____  ____ 
 *               / __ \/ / __/ / __ \/ __ \/ __ \
 *              / /_/ / / /_/ / /_/ / /_/ / /_/ /
 *             /_____/_/\__/_/\____/\____/ ____/ 
 *                                      /_/      
 *              
 *  Copyright (C) 2025 Will Hemsworth
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 * 
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.
 * 
 * 
 * ================================================
 * =============  Framework features  =============
 * ================================================
 * 
 * Goals:
 *  ✔ Rapid prototyping of scientific simulations, visualizations, and 
 *     other ideas (games, art, etc.)
 *  ✔ Optimized for high performance
 *  ✘ Seamless video encoding with FFmpeg (desktop-only for now)
 * 
 * Engine:
 *  ✔ Cross-platform (Linux:✔  Windows:✔  Web:✔  macOS:✘  Android:✘  iPhone:✘)
 *  ✔ Takes advantage of latest C++23 features
 *  ✔ Multithreaded ImGui support for non-blocking UI input (updates applied at beginning of each frame)
 *  ✔ SDL3 for window/input handling
 *  ✔ Rich set helpers and 3rd party libraries for scientific simulations
 *  ✘ 128-bit floating-point support for Camera/World
 * 
 * Simulations:
 *  ✔ Highly modular
 *  ✔ Multi-viewport support
 *  ✔ Multiple scenes per project which can be mounted to viewports
 *  ✔ Scene inheriting (e.g. SolarSystem simulation inherits from SpaceEngine)
 * 
 * Graphics:
 *  ✔ High-DPI rendering support
 *  ✔ NanoVG wrapper for 128-bit vector graphics rendering (Javascript Canvas-like API)
 *  ✔ Easy switching between world/screen space rendering
 * 
 * Features:
 *  ✘ Individual simulations can be compiled as standalone-apps for all platforms
 *  ✘ Timeline support with integrated scripting
 *  ✔ Collection of examples for different difficulty levels
 * 
 * 
 * TODO:
 * > Each simulation generates a vcxproj of it's own, and can compile to exe
 *   - It always compiles to .obj, but the child vcxproj can compile it into an exe by linking with "main()"
 *   - implementation:
 *     - make each simulation have it's own complete CMakeLists.txt, which links to SimulationCore
 *     - root "bitloop_gallery" have it's own CMakeLists.txt which links to all simulations
 * 
 * > Make a NEW Visual Studio project, link to bitloop, generate standalone build for project
 *   - The generated projects should use this logic
 *   - They already do, but you can't compile individual projects alone, only the root framework right now
 * 
 * > Make new project, include bitloop as EXTERNAL dependency, build
 *   - Requires moving "Portfolio" logic into it's own translation unit
 * 
 */


#include <memory>

#ifdef __EMSCRIPTEN__
#define SDL_MAIN_HANDLED
#include <GLES3/gl3.h>
#else
#include "glad/glad.h"
//#include <glad/gl.h>
#endif

/// ImGui
#include "bitloop/ui/imgui_custom.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

/// Project files
#include "project_worker.h"
#include "main_window.h"
#include "shared_sync.h"

SDL_Window* window = nullptr;
SharedSync shared_sync;
//std::unordered_map<size_t, size_t> thread_map;

void gui_loop()
{
    // ======== Poll SDL events ========
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        ImGui_ImplSDL3_ProcessEvent(&e);

        switch (e.type)
        {
            case SDL_EVENT_QUIT: shared_sync.quit(); break;
            case SDL_EVENT_WINDOW_RESIZED:
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
                Platform()->resized();
                break;
            default: ProjectWorker::instance()->queueEvent(e); break;
        }
    }

    Platform()->update();

    // ======== Prepare frame ========
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::GetIO().DisplaySize = ImVec2((float)Platform()->fbo_width(), (float)Platform()->fbo_height());
    ImGui::GetIO().DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    ImGui::NewFrame();

    // ======== Draw window ========
    MainWindow::instance()->populateUI();

    // ======== Render ========
    ImGui::Render();
    glClearColor(0.1f, 0.0f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

int bitloop_main(int, char* [])
{
    // ======== SDL Window setup ========
    {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 0);

        //#ifndef __EMSCRIPTEN__
        //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        //#else
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        //#endif

        int fb_w = 1280, fb_h = 720;

        #ifdef __EMSCRIPTEN__
        emscripten_get_canvas_element_size("#canvas", &fb_w, &fb_h);

        // needed?
        EM_ASM({
            const cv = Module['canvas'];
            cv.setAttribute('tabindex', '0');
            cv.setAttribute('contenteditable', 'true');
            cv.addEventListener('mousedown', () => cv.focus());
            });
        #endif

        std::cout << "Creating window...\n";
        window = SDL_CreateWindow("bitloop", fb_w, fb_h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_HIGH_PIXEL_DENSITY);

        if (!window) {
            std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
            return 1;
        }
    }

    SDL_GLContext gl_context;

    // ======== OpenGL setup ========
    {
        gl_context = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, gl_context);
        SDL_GL_SetSwapInterval(1); // enforces 60fps / v-sync

        #ifndef __EMSCRIPTEN__
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            std::cerr << "Failed to initialize GLAD\n";
            return 1;
        }

        // Make colours consistent on desktop build
        glDisable(GL_FRAMEBUFFER_SRGB);
        #endif

    }

    auto platform_manager = std::make_unique<PlatformManager>(window);
    auto project_worker = std::make_unique<ProjectWorker>(shared_sync);
    auto main_window = std::make_unique<MainWindow>(shared_sync);

    // ======== ImGui setup ========
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplSDL3_InitForOpenGL(window, gl_context);

        ImGui_ImplOpenGL3_Init();
        //#ifdef __EMSCRIPTEN__
        //ImGui_ImplOpenGL3_Init("#version 300 es");
        //#else
        //ImGui_ImplOpenGL3_Init("#version 150");
        //#endif
    }

    // ======== Init window & start worker thread ========
    {
        MainWindow::instance()->init();
        ProjectWorker::instance()->startWorker();
    }

    // ======== Start main gui loop ========
    {
        #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(gui_loop, 0, true);
        #else
        {
            while (!shared_sync.quitting.load())
            {
                gui_loop();
                SDL_Delay(0);
            }

            // Gracefully exit
            ProjectWorker::instance()->end();

            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplSDL3_Shutdown();
            ImGui::DestroyContext();
            SDL_GL_DestroyContext(gl_context);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }
        #endif
    }
    return 0;
}