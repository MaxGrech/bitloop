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
    camera->focusWorldRect(TableInfo::r.scaled(1.1));
}

void Airhockey_Scene::sceneDestroy()
{
    // Destroy scene
}

void Airhockey_Scene::sceneProcess()
{
}

void Airhockey_Scene::processAI()
{
    // Make copies of the "current" gamestate to test different possible futures
    int scenario_count = 1;

    scenario_states.clear();
    preview_states.clear();

    for (int i = 0; i < scenario_count; i++)
    {
        // Make 10 copies of the current state
        scenario_states.push_back(current_state);

        // Set the starting condition for each scenario
        GameState& state = scenario_states[i];
        state.m2.taking_shot = true;
    }

    // Think ahead
    for (int i = 0; i < scenario_count; i++)
    {
        // For each scenario, process 2 seconds into the future
        for (int step = 0; step < 360; step++)
        {
            GameState& state = scenario_states[i];
            state.step();

            if (step % 10 == 0)
            {
                preview_states.push_back(state);
            }
        }
    }

    //~ // Think about different scenarios (initial angles and different spins)
}

void GameState::step()
{
    // Detect if puck collides with mallet
    m1.checkPuckCollision(&puck);
    m2.checkPuckCollision(&puck);

    puck.process(TableInfo::r);

    // Make human mallet follow the mouse
    {

        // Set mallet velocity
        m1.vx = (mouse_x - m1.x) * 0.05;
        m1.vy = (mouse_y - m1.y) * 0.05;

        // Apply mallet velocity
        m1.x += m1.vx;
        m1.y += m1.vy;

        // Work out "true" speed of puck
        double speed_limit = 1.0;
        double speed = sqrt(puck.vx * puck.vx + puck.vy * puck.vy);
        if (speed > speed_limit)
        {
            // Work out angle puck is moving
            double angle = atan2(puck.vy, puck.vx);

            // Slow the puck down to the maximum speed limit
            puck.vx = cos(angle) * speed_limit;
            puck.vy = sin(angle) * speed_limit;
        }

        // Vertical walls
        if      (m1.y < 0 + m1.r)                  m1.y = 0 + m1.r;
        else if (m1.y > TableInfo::bottom - m1.r)  m1.y = TableInfo::bottom - m1.r;
        
        // Horizontal walls
        if      (m1.x < TableInfo::left  + m1.r)   m1.x = TableInfo::left  + m1.r;
        else if (m1.x > TableInfo::right - m1.r)   m1.x = TableInfo::right - m1.r;


        
    }

    // Make AI mallet track the puck position (with an offset)
    {
        double offset = TableInfo::table_height * 0.15;

        // Set the target position
        if (m2.taking_shot)
        {
            double angle = atan2(puck.y - m2.y, puck.x - m2.x);
            double hit_strength = 20.0;
            m2.target_x = puck.x + cos(angle) * hit_strength;
            m2.target_y = puck.y + sin(angle) * hit_strength;
        }
        else
        {
            m2.target_x = puck.x;
            m2.target_y = puck.y - offset;
        }

        // Ease towards the target position
        m2.vx = (m2.target_x - m2.x) * 0.02;
        m2.vy = (m2.target_y - m2.y) * 0.02;

        m2.x += m2.vx;
        m2.y += m2.vy;

        // Vertical walls
        if      (m2.y > 0 - m2.r)                  m2.y = 0 - m2.r;
        else if (m2.y < TableInfo::top + m2.r)     m2.y = TableInfo::top + m2.r;
        
        // Horizontal walls
        if      (m2.x < TableInfo::left  + m2.r)   m2.x = TableInfo::left  + m2.r;
        else if (m2.x > TableInfo::right - m2.r)   m2.x = TableInfo::right - m2.r;
    }
}

void Airhockey_Scene::viewportProcess(Viewport*, double)
{
    current_state.mouse_x = mouse->world_x;
    current_state.mouse_y = mouse->world_y;

    processAI();

    
    // Process the "real" game state
    for (int i = 0; i < 3; i++)
    {
        // Process mallet positions/velocity/collisions
        current_state.step();
    }

    //~
    /*
    * 
    > Test hitting the puck at different angles/offsets
      For each "angle of attack", would it lead to a goal?
        - If yes
           - trigger a flag "taking_shot"
           - set the "angle of attack"

    > Once the puck enter's user's table side
        - Reset back to false

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

void Airhockey_Scene::drawAirHockeyTable(Viewport* ctx) const
{
    double line_width = TableInfo::table_width * 0.03; // scalable

    // Table background
    ctx->setFillStyle(30, 25, 40);
    ctx->fillRoundedRect(
        TableInfo::left, 
        TableInfo::top, 
        TableInfo::table_width, 
        TableInfo::table_height, 
        Puck::radius);

    // Outer border
    ctx->setLineWidth(line_width);
    ctx->setStrokeStyle(90, 90, 90);
    ctx->strokeRoundedRect(
        TableInfo::left - line_width / 2,
        TableInfo::top - line_width / 2, 
        TableInfo::table_width + line_width, 
        TableInfo::table_height + line_width,
        Puck::radius + line_width / 2);

    ctx->setLineWidth(TableInfo::table_width * 0.005);

    // Blue lines
    ctx->setStrokeStyle(0, 0, 255);
    double goalLineY1 = TableInfo::top + TableInfo::table_height * 0.3;
    double goalLineY2 = TableInfo::top + TableInfo::table_height * 0.7;
    ctx->beginPath();
    ctx->moveTo(TableInfo::left, goalLineY1);
    ctx->lineTo(TableInfo::left + TableInfo::table_width, goalLineY1);
    ctx->moveTo(TableInfo::left, goalLineY2);
    ctx->lineTo(TableInfo::left + TableInfo::table_width, goalLineY2);
    ctx->stroke();

    // Center circle
    double centerRadius = TableInfo::table_width * 0.15;
    double innerCenterRadius = TableInfo::table_width * 0.12;

    // Center red dotted line
    ctx->setStrokeStyle(255, 0, 0);
    drawDottedLine(ctx,  centerRadius + 3, 0, TableInfo::right, 0, 4, 3);
    drawDottedLine(ctx, -centerRadius - 3, 0, TableInfo::left, 0, 4, 3);

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
    ctx->arc(0, TableInfo::bottom, TableInfo::table_width / 6, 0, Math::PI, PathWinding::WINDING_CCW);
    ctx->stroke();

    ctx->beginPath();
    ctx->arc(0, TableInfo::top, TableInfo::table_width / 6, 0, Math::PI, PathWinding::WINDING_CW);
    ctx->stroke();
}

void Airhockey_Scene::viewportDraw(Viewport* ctx) const
{
    ctx->drawWorldAxis();

    drawAirHockeyTable(ctx);

    current_state.puck.draw(ctx);
    current_state.m1.draw(ctx);
    current_state.m2.draw(ctx);

    for (const GameState& state : preview_states)
    {
        state.m1.draw(ctx, 50);
        state.m2.draw(ctx, 50);
        state.puck.draw(ctx, 50);
    }

    for (const GameState& state : scenario_states)
    {
        state.m2.draw(ctx);
    }
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
void Airhockey_Scene::onKeyDown(KeyEvent e)
{
    if (e.scanCode() == ScanCode::SDL_SCANCODE_P)
    {
        current_state.m2.taking_shot = true;
    }
}
//void AirHockey_Scene::onKeyUp(KeyEvent e) {}


SIM_END(Airhockey)

