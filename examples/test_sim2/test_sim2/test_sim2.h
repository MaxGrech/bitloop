#pragma once
#include <bitloop/core/project.h>
#include <test_sim/test_sim.h>

SIM_BEG(Test2)

using namespace BL;

struct Particle : public DVec2
{
    double fx = 0, fy = 0;
    double vx, vy;
    Particle(double x, double y, double vx, double vy)
        : Vec2(x, y), vx(vx), vy(vy)
    {}
};

struct Test2_Scene_Attributes : VarBuffer
{
    bool transform_coordinates = true;
    bool scale_lines_text = true;
    bool scale_sizes = true;
    bool rotate_text = true;

    double camera_x = 0;
    double camera_y = 0;
    double camera_rotation = 0;
    double zoom_x = 1;
    double zoom_y = 1;
    double zoom_mult = 1;

    void registerSynced() override
    {
        sync(transform_coordinates);
        sync(scale_lines_text);
        sync(scale_sizes);
        sync(rotate_text);
        sync(camera_x);
        sync(camera_y);
        sync(camera_rotation);
        sync(zoom_x);
        sync(zoom_y);
        sync(zoom_mult);
    }
    void populate() override;
};

struct Test2_Scene : public Scene<Test2_Scene_Attributes>
{
    DVec2 ball_pos = { 0, 0 };
    std::vector<Particle> particles;

    // --- Custom Launch Config Example ---
    struct Config
    {
        //double speed = 10.0;
    };

    Test2_Scene(Config&) //:
        //speed(info.speed)
    {}

    // Scene management
    //void _sceneAttributes() override;
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
    //void onPointerDown(PointerEvent e) override;
    //void onPointerUp(PointerEvent e) override;
    //void onPointerMove(PointerEvent e) override;
    //void onWheel(PointerEvent e) override;
    //void onKeyDown(KeyEvent e) override;
    //void onKeyUp(KeyEvent e) override;
};

struct Test2_Project_Vars : public VarBuffer
{
    int viewport_count = 1;

    void populate();
    void registerSynced()
    {
        sync(viewport_count);
    }
};

struct Test2_Project : public Project<Test2_Project_Vars>
{
    void projectPrepare(Layout& layout) override;
};

SIM_END(Test2)
