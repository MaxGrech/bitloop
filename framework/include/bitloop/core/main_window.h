#pragma once

#include <vector>
#include "threads.h"

#include "imgui_custom.h"
#include "nano_canvas.h"
#include "project.h"

namespace BL {

struct ToolbarButtonState 
{
    ImVec4 bgColor;
    ImVec4 symbolColor;
    bool active;
};

extern ImDebugLog project_log;
extern ImDebugLog debug_log;

class MainWindow
{
    static MainWindow* singleton;

    bool initialized = false;
    bool done_first_focus = false;
    bool update_docking_layout = false;
    bool vertical_layout = false;

    bool need_draw = false;

    ToolbarButtonState play = { ImVec4(0.1f, 0.6f, 0.1f, 1.0f), ImVec4(1, 1, 1, 1), false };
    ToolbarButtonState stop = { ImVec4(0.6f, 0.1f, 0.1f, 1.0f), ImVec4(1, 1, 1, 1), false };
    ToolbarButtonState pause = { ImVec4(0.3f, 0.3f, 0.3f, 1.0f), ImVec4(1, 1, 1, 1), false };
    ToolbarButtonState record = { ImVec4(0.8f, 0.0f, 0.0f, 1.0f), ImVec4(1, 1, 1, 1), false };

    Canvas canvas;
    SharedSync& shared_sync;

    const int window_flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove;

public:

    static MainWindow* instance() { 
        return singleton; 
    }

    MainWindow(SharedSync& _shared_sync) : shared_sync(_shared_sync) {
        singleton = this;
    }

    [[nodiscard]] Canvas* getCanvas() {
        return &canvas;
    }

    void init();
    void checkChangedDPR();

    void initStyles();
    void initFonts();

    /// Determine whether *any* ImGui input is likely being altered
    bool isEditingUI();

    /// Toolbar
    void drawToolbarButton(ImDrawList* drawList, ImVec2 pos, ImVec2 size, const char* symbol, ImU32 color);
    bool toolbarButton(const char* id, const char* symbol, const ToolbarButtonState& state, ImVec2 size);
    void populateToolbar();

    /// Project Tree
    void populateProjectTreeNodeRecursive(ProjectInfoNode& node, int& i, int depth);
    void populateProjectTree(bool expand_vertical);
    void populateProjectUI();
    
    /// Main Window populate
    bool manageDockingLayout();
    bool focusWindow(const char* id);

    void populateCollapsedLayout();
    void populateViewport();
    void populateExpandedLayout();
    void populateUI();
};

} // end namespace BL

