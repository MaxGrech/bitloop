#include "AirHockey.h"

SIM_DECLARE(AirHockey, "Lessons", "AirHockey")


/// =========================
/// ======== Project ========
/// =========================

void AirHockey_Project::projectPrepare(Layout& layout)
{
    /// Create multiple instance of a single Scene, mount to separate viewports
    layout << create<AirHockey_Scene>(1);
}

/// =========================
/// ========= Scene =========
/// =========================

void AirHockey_Scene::sceneStart()
{
}

void AirHockey_Scene::sceneMounted(Viewport*)
{
    camera->setOriginViewportAnchor(Anchor::CENTER);
    camera->focusWorldRect(r.scaled(1.1));
}

void AirHockey_Scene::sceneDestroy()
{
    // Destroy scene
}

void AirHockey_Scene::sceneProcess()
{
}

void AirHockey_Scene::viewportProcess(Viewport*, double)
{
    // Game logic



    // Detect if puck collides with mallet
    m1.checkPuckCollision(&puck);
    m2.checkPuckCollision(&puck);

    puck.process(r);

    double mx = mouse->world_x;
    double my = mouse->world_y;

    m1.vx = (mx - m1.x) * 0.1;
    m1.vy = (my - m1.y) * 0.1;

    m1.x += m1.vx;
    m1.y += m1.vy;

    /*if (m1.y < 0 + m1.r)
    {
        // Keep mallet below center of table
        m1.y = 0 + m1.r;
    }
    else if (m1.y > y2 - m1.r)
    {
        // Keep mallet above bottom of table
        m1.y = y2 - m1.r;
    }

    if (m1.x < x1 + m1.r)
    {
        // Keep the mallet right of left side of table
        m1.x = x1 + m1.r;
    }
    else if (m1.x > x2 - m1.r)
    {
        // Keep the mallet left of right side of table
        m1.x = x2 - m1.r;
    }
    */
}

void drawDottedLine(Viewport* ctx, double x1, double y1, double x2, double y2, double dashLength, float gapLength) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    double dist = std::sqrt(dx * dx + dy * dy);
    double dashGap = dashLength + gapLength;
    double dashCount = dist / dashGap;
    dx /= dist;
    dy /= dist;

    ctx->beginPath();
    for (int i = 0; i < dashCount; i++) {
        double sx = x1 + dx * (i * dashGap);
        double sy = y1 + dy * (i * dashGap);
        double ex = sx + dx * dashLength;
        double ey = sy + dy * dashLength;
        ctx->moveTo(sx, sy);
        ctx->lineTo(ex, ey);
    }
    ctx->stroke();
}

void drawAirHockeyTable(Viewport* ctx, double x1, double y1, double table_width, double table_height)
{
    double line_width = table_width * 0.01; // scalable

    // Table background
    ctx->setFillStyle(100, 100, 100);
    ctx->fillRect(x1, y1, table_width, table_height);

    // Outer border
    ctx->setLineWidth(line_width);
    ctx->setStrokeStyle(0, 0, 0);
    ctx->strokeRect(x1, y1, table_width, table_height);

    // Blue goal lines (10% from ends)
    ctx->setStrokeStyle(0, 0, 255);
    double goalLineY1 = y1 + table_height * 0.1;
    double goalLineY2 = y1 + table_height * 0.9;
    ctx->beginPath();
    ctx->moveTo(x1, goalLineY1);
    ctx->lineTo(x1 + table_width, goalLineY1);
    ctx->moveTo(x1, goalLineY2);
    ctx->lineTo(x1 + table_width, goalLineY2);
    ctx->stroke();

    // Center red dotted line
    ctx->setStrokeStyle(255, 0, 0);
    drawDottedLine(ctx,
        x1, y1 + table_height / 2,
        x1 + table_width, y1 + table_height / 2,
        10, 5);

    // Center circle
    double centerX = x1 + table_width / 2;
    double centerY = y1 + table_height / 2;
    double centerRadius = table_width * 0.15;
    ctx->strokeEllipse(centerX, centerY, centerRadius);

    // Face-off circles (red)
    double faceOffRadius = table_width * 0.1;
    ctx->setStrokeStyle(255, 0, 0);
    ctx->strokeEllipse(centerX, goalLineY1 + (centerY - goalLineY1) / 2, faceOffRadius);
    ctx->strokeEllipse(centerX, goalLineY2 - (goalLineY2 - centerY) / 2, faceOffRadius);

    ctx->setFillStyle(255, 0, 0);
    ctx->setStrokeStyle(0, 255, 0);
    ctx->fillRoundedRect(50, 50, 400, 400, 30);
    ctx->strokeRoundedRect(50, 50, 400, 400, 30);
}

void AirHockey_Scene::viewportDraw(Viewport* ctx) const
{
    ctx->drawWorldAxis();

    drawAirHockeyTable(ctx, x1, y1, table_width, table_height);

    ctx->setFillStyle(puck.hit ? 255 : 100, 120, 100);
    ctx->fillEllipse(puck.x, puck.y, puck.r);

    m1.draw(ctx);
    m2.draw(ctx);
}

void AirHockey_Scene::onEvent(Event e)
{
    handleWorldNavigation(e, true);
}

//void AirHockey_Scene::onPointerDown(PointerEvent e) {}
//void AirHockey_Scene::onPointerUp(PointerEvent e) {}
void AirHockey_Scene::onPointerMove(PointerEvent)
{

}
//void AirHockey_Scene::onWheel(PointerEvent e) {}
//void AirHockey_Scene::onKeyDown(KeyEvent e) {}
//void AirHockey_Scene::onKeyUp(KeyEvent e) {}


SIM_END(AirHockey)

