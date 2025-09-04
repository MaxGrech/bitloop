#pragma once
#include "project.h"

SIM_BEG(Airhockey)

using namespace BL;

struct Puck
{
    static inline const double radius = 5;

    double x = 0;
    double y = 0;

    double vx = 0;
    double vy = 0;

    bool hit = false;

    void process(DRect rect)
    {
        x += vx;
        y += vy;

        if (x < rect.x1 + radius)
        {
            x = rect.x1 + radius;
            vx *= -1;
        }
        else if (x > rect.x2 - radius)
        {
            x = rect.x2 - radius;
            vx *= -1;
        }

        if (y < rect.y1 + radius)
        {
            y = rect.y1 + radius;
            vy *= -1;
        }
        else if (y > rect.y2 - radius)
        {
            y = rect.y2 - radius;
            vy *= -1;
        }
    }

    void draw(Viewport* ctx, int alpha=255) const
    {
        ctx->setFillStyle(hit ? 255 : 100, 120, 100, alpha);
        ctx->fillEllipse(x, y, Puck::radius);
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

    void draw(Viewport* ctx, int alpha = 255) const
    {
        ctx->setFillStyle(100, 120, 100, alpha);
        ctx->fillEllipse(x, y, r);

        ctx->setFillStyle(100, 150, 100, alpha);
        ctx->fillEllipse(x, y, r / 2);
    }

    void checkPuckCollision(Puck* puck)
    {
        double dx = puck->x - x;
        double dy = puck->y - y;
        double d = std::sqrt(dx * dx + dy * dy);
        double combined_r = puck->radius + r;

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

struct AIMallet : Mallet
{
    double target_angle;
    double target_angle_spin; // How much to change our "angle of attack" each frame

    bool taking_shot = false;

    double target_x;
    double target_y;

    AIMallet(double mx, double my) : Mallet(mx, my)
    {
    }
};

struct TableInfo
{
    // --- Table ---
    static inline const double table_width = 160;
    static inline const double table_height = 200;

   

    static inline const double left = -table_width / 2; // left
    static inline const double right = table_width / 2; // right
    static inline const double top = -table_height / 2; // top
    static inline const double bottom = table_height / 2; // bottom
    static inline const DRect r = DRect(left, top, right, bottom);
};

struct GameState
{
    Puck puck;
    Mallet m1 = Mallet(0, 60);
    AIMallet m2 = AIMallet(0, -60);

    double mouse_x;
    double mouse_y;

    void step();
};

struct Airhockey_Scene : public BasicScene
{
    // --- Custom Launch Config Example ---
    struct Config {};
    //////////////////////////

    // Keep track of the "current" game state
    GameState current_state;
    std::vector<GameState> scenario_states;
    std::vector<GameState> preview_states; // debugging


    Airhockey_Scene(Config&) //:
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
    void onKeyDown(KeyEvent e) override;
    //void onKeyUp(KeyEvent e) override;

    void processAI();
    void drawAirHockeyTable(Viewport* ctx) const;
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

struct Airhockey_Project : public BasicProject
{
    static ProjectInfo info()
    {
        return ProjectInfo({ "Games", "Airhockey" });
    }

    void projectPrepare(Layout& layout) override;
};

SIM_END(Airhockey)
