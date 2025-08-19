#pragma once
#include <project.h>

SIM_BEG(Tiger)

using namespace BL;

struct Tiger_Scene_Data : VarBuffer
{
    bool transform_coordinates = true;
    bool scale_lines = true;
    bool scale_sizes = true;
    bool rotate_text = true;

    CameraViewController cam_view;
    
    void registerSynced() override
    {
        sync(transform_coordinates);
        sync(scale_lines);
        sync(scale_sizes);
        sync(rotate_text);

        sync(cam_view);
    }
    void populate() override;
};

struct Tiger_Scene : public Scene<Tiger_Scene_Data>
{
    // --- Custom Launch Config Example ---
    struct Config {};
    Tiger_Scene(Config&) {}

    // Scene management
    void sceneStart() override;
    void sceneMounted(Viewport* viewport) override;
    void sceneDestroy() override;

    // --- Simulation processing ---
    void sceneProcess() override;

    // Viewport handling
    void viewportProcess(Viewport* ctx, double dt) override;
    void viewportDraw(Viewport* ctx) const override;

    // Input
    void onEvent(Event e) override;
};

struct Tiger_Project_Data : public VarBuffer
{
    int viewport_count = 1;

    void populate();
    void registerSynced()
    {
        sync(viewport_count);
    }
};

struct Tiger_Project : public Project<Tiger_Project_Data>
{
    static ProjectInfo info()
    {
        return ProjectInfo({ "Tests", "Vector Graphics", "Tiger" });
    }

    void projectPrepare(Layout& layout) override;
};

SIM_END(Tiger)
