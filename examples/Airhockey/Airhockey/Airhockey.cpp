#include "Airhockey.h"

SIM_BEG(Airhockey)


/// =========================
/// ======== Project ========
/// =========================

void Airhockey_Project::projectPrepare(Layout& layout)
{
    /// Create multiple instance of a single Scene, mount to separate viewports
    layout << create<Airhockey_Scene>(1);
}

/// =========================
/// ========= Scene =========
/// =========================

void Airhockey_Scene::sceneStart()
{
}

void Airhockey_Scene::sceneMounted(Viewport*)
{
    camera->setOriginViewportAnchor(Anchor::CENTER);
    camera->focusWorldRect(r.scaled(1.1));
}

void Airhockey_Scene::sceneDestroy()
{
    // Destroy scene
}

void Airhockey_Scene::sceneProcess()
{
}

void Airhockey_Scene::viewportProcess(Viewport*, double)
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

    if (m1.y < 0 + m1.r)
    {
        // Keep mallet below center of table
        m1.y = 0 + m1.r;
    }
    else if (m1.y > bottom - m1.r)
    {
        // Keep mallet above bottom of table
        m1.y = bottom - m1.r;
    }

    if (m1.x < left + m1.r)
    {
        // Keep the mallet right of left side of table
        m1.x = left + m1.r;
    }
    else if (m1.x > right - m1.r)
    {
        // Keep the mallet left of right side of table
        m1.x = right - m1.r;
    }
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

void Airhockey_Scene::drawAirHockeyTable(Viewport* ctx) const
{
    double line_width = table_width * 0.03; // scalable

    // Table background
    ctx->setFillStyle(30, 25, 40);
    ctx->fillRoundedRect(left, top, table_width, table_height, puck.r);

    // Outer border
    ctx->setLineWidth(line_width);
    ctx->setStrokeStyle(90, 90, 90);
    ctx->strokeRoundedRect(
        left - line_width / 2,
        top - line_width / 2, 
        table_width + line_width, 
        table_height + line_width,
        puck.r + line_width / 2);

    ctx->setLineWidth(table_width * 0.005);

    // Blue lines
    ctx->setStrokeStyle(0, 0, 255);
    double goalLineY1 = top + table_height * 0.3;
    double goalLineY2 = top + table_height * 0.7;
    ctx->beginPath();
    ctx->moveTo(left, goalLineY1);
    ctx->lineTo(left + table_width, goalLineY1);
    ctx->moveTo(left, goalLineY2);
    ctx->lineTo(left + table_width, goalLineY2);
    ctx->stroke();

    // Center circle
    double centerRadius = table_width * 0.15;
    double innerCenterRadius = table_width * 0.12;

    // Center red dotted line
    ctx->setStrokeStyle(255, 0, 0);
    drawDottedLine(ctx,  centerRadius + 3, 0, right, 0, 4, 3);
    drawDottedLine(ctx, -centerRadius - 3, 0, left, 0, 4, 3);

    ctx->setStrokeStyle(0, 0, 255);
    ctx->strokeEllipse(0, 0, centerRadius);
    ctx->strokeEllipse(0, 0, innerCenterRadius);

    // Face-off circles (red)
    //double faceOffRadius = table_width * 0.1;
    //ctx->setStrokeStyle(255, 0, 0);
    //ctx->strokeEllipse(centerX, goalLineY1 + (centerY - goalLineY1) / 2, faceOffRadius);
    //ctx->strokeEllipse(centerX, goalLineY2 - (goalLineY2 - centerY) / 2, faceOffRadius);

    // Goals
    ctx->beginPath();
    ctx->arc(0, bottom, table_width / 6, 0, Math::PI, PathWinding::WINDING_CCW);
    ctx->stroke();

    ctx->beginPath();
    ctx->arc(0, top, table_width / 6, 0, Math::PI, PathWinding::WINDING_CW);
    ctx->stroke();
}

void Airhockey_Scene::viewportDraw(Viewport* ctx) const
{
    ctx->drawWorldAxis();

    drawAirHockeyTable(ctx);

    ctx->setFillStyle(puck.hit ? 255 : 100, 120, 100);
    ctx->fillEllipse(puck.x, puck.y, puck.r);

    m1.draw(ctx);
    m2.draw(ctx);
}

void Airhockey_Scene::onEvent(Event e)
{
    handleWorldNavigation(e, true);
}

//void AirHockey_Scene::onPointerDown(PointerEvent e) {}
//void AirHockey_Scene::onPointerUp(PointerEvent e) {}
void Airhockey_Scene::onPointerMove(PointerEvent)
{

}
//void AirHockey_Scene::onWheel(PointerEvent e) {}
//void AirHockey_Scene::onKeyDown(KeyEvent e) {}
//void AirHockey_Scene::onKeyUp(KeyEvent e) {}


SIM_END(Airhockey)

