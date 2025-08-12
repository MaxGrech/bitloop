#pragma once
#include <bitloop/core/project.h>

SIM_BEG({SIM_NAME})

struct {SIM_NAME}_Scene_Attributes : VarBuffer
{
    //double speed = 0.0;

    void registerSynced() override
    {
        //sync(speed);
    }
    void populate() override;
};

struct {SIM_NAME}_Scene : public Scene<{SIM_NAME}_Scene_Attributes>
{
    // --- Custom Launch Config Example ---
    struct Config
    {
        //double speed = 10.0;
    };

    {SIM_NAME}_Scene(Config&) //:
        //speed(info.speed)
    {}

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
    //void onPointerDown(PointerEvent e) override;
    //void onPointerUp(PointerEvent e) override;
    //void onPointerMove(PointerEvent e) override;
    //void onWheel(PointerEvent e) override;
    //void onKeyDown(KeyEvent e) override;
    //void onKeyUp(KeyEvent e) override;
};

struct {SIM_NAME}_Project_Vars : public VarBuffer
{
    int viewport_count = 1;

    void populate();
    void registerSynced()
    {
        sync(viewport_count);
    }
};

struct {SIM_NAME}_Project : public Project<{SIM_NAME}_Project_Vars>
{
    void projectPrepare(Layout& layout) override;
};

SIM_END({SIM_NAME})