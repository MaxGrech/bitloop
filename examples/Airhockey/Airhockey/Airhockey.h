#pragma once
#include "project.h"

SIM_BEG(AirHockey)

using namespace BL;

struct Puck
{
    const double r = 5;

    double x = 0;
    double y = 0;

    double vx = 0;
    double vy = 0;

    bool hit = false;

    void process(DRect rect)
    {
        x += vx;
        y += vy;

        if (x < rect.x1 + r)
        {
            x = rect.x1 + r;
            vx *= -1;
        }
        else if (x > rect.x2 - r)
        {
            x = rect.x2 - r;
            vx *= -1;
        }

        if (y < rect.y1 + r)
        {
            y = rect.y1 + r;
            vy *= -1;
        }
        else if (y > rect.y2 - r)
        {
            y = rect.y2 - r;
            vy *= -1;
        }
    }
};

struct Mallet
{
    const double r = 7;

    double x = 0;
    double y = 0;

    double vx = 0;
    double vy = 0;



    Mallet(double mx, double my)
    {
        x = mx;
        y = my;
    }

    void draw(Viewport* ctx) const
    {
        ctx->setFillStyle(100, 120, 100);
        ctx->fillEllipse(x, y, r);

        ctx->setFillStyle(100, 150, 100);
        ctx->fillEllipse(x, y, r / 2);
    }

    void checkPuckCollision(Puck* puck)
    {
        double dx = puck->x - x;
        double dy = puck->y - y;
        double d = std::sqrt(dx * dx + dy * dy);
        double combined_r = puck->r + r;

        if (d < combined_r)
        {
            // Puck and mallet have collided
            double overlap = combined_r - d;
            dx /= d; // normalize
            dy /= d;

            puck->x += dx * overlap / 2.0;
            puck->y += dy * overlap / 2.0;
            x -= dx * overlap / 2.0;
            y -= dy * overlap / 2.0;

            // Relative velocity
            double rvx = puck->vx - vx;
            double rvy = puck->vy - vy;

            // Relative velocity along normal
            double velAlongNormal = rvx * dx + rvy * dy;

            // If objects are separating already, skip
            if (velAlongNormal > 0) return;

            // Assume equal masses (simpler)
            double restitution = 1.0; // 1.0 = perfectly elastic, <1 = loses energy

            double impulse = -(1 + restitution) * velAlongNormal / 2.0; // divide by 2 for equal mass
            double impulseX = impulse * dx;
            double impulseY = impulse * dy;

            puck->vx += impulseX;
            puck->vy += impulseY;
            vx -= impulseX;
            vy -= impulseY;
        }
        else
        {
        }
    }
};


struct AirHockey_Scene : public BasicScene
{
    // --- Custom Launch Config Example ---
    struct Config {};
    //////////////////////////

    // --- Table ---
    const double table_width = 160;
    const double table_height = 200;
    const double x1 = -table_width / 2;
    const double x2 = table_width / 2;
    const double y1 = -table_height / 2;
    const double y2 = table_height / 2;
    const DRect r = DRect(x1, y1, x2, y2);

    // --- Objects ---
    Puck puck;
    Mallet m1 = Mallet(0, 60);
    Mallet m2 = Mallet(0, -60);



    AirHockey_Scene(Config&) //:
        //speed(info.speed)
    {
    }

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
    void onPointerMove(PointerEvent e) override;
    //void onWheel(PointerEvent e) override;
    //void onKeyDown(KeyEvent e) override;
    //void onKeyUp(KeyEvent e) override;
};

struct ImageTest_Project_Vars : public VarBuffer
{
    int viewport_count = 1;

    void populate();
    void registerSynced()
    {
        sync(viewport_count);
    }
};

struct AirHockey_Project : public BasicProject
{
    void projectPrepare(Layout& layout) override;
};

SIM_END(AirHockey)