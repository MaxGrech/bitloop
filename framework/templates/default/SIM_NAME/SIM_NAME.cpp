#include "{SIM_NAME}.h"

SIM_DECLARE({SIM_NAME}, "New Projects", "{SIM_NAME}")


/// =========================
/// ======== Project ========
/// =========================

void {SIM_NAME}_Project_Vars::populate()
{
    ImGui::SliderInt("Viewport Count", &viewport_count, 1, 8);
}

void {SIM_NAME}_Project::projectPrepare(Layout& layout)
{
    /// Create multiple instance of a single Scene, mount to separate viewports
    layout << create<{SIM_NAME}_Scene>(viewport_count);

    /// Or create a single Scene instance and view on multiple Viewports
    //auto* scene = create<{SIM_NAME}_Scene>();
    //for (int i = 0; i < viewport_count; ++i)
    //    layout << scene;
}

/// =========================
/// ========= Scene =========
/// =========================

void {SIM_NAME}_Scene_Attributes::populate()
{
    //ImGui::Checkbox("option", &option);
    //ImGui::SliderDouble("value", &value, 0.0, 1.0);
}

void {SIM_NAME}_Scene::sceneStart()
{
    // Initialize scene
}

void {SIM_NAME}_Scene::sceneMounted(Viewport*)
{
    // Initialize viewport
    camera->setOriginViewportAnchor(Anchor::CENTER);
    //camera->focusWorldRect(0, 0, 100, 100);
}

void {SIM_NAME}_Scene::sceneDestroy()
{
    // Destroy scene
}

void {SIM_NAME}_Scene::sceneProcess()
{
    // Process scene update
}

void {SIM_NAME}_Scene::viewportProcess(Viewport* ctx, double dt)
{
    // Process viewport update
}

void {SIM_NAME}_Scene::viewportDraw(Viewport* ctx) const
{
    // Draw Scene on this viewport
}

void {SIM_NAME}_Scene::onEvent(Event e)
{
    handleWorldNavigation(e, true);
}

//void {SIM_NAME}_Scene::onPointerDown(PointerEvent e) {{}}
//void {SIM_NAME}_Scene::onPointerUp(PointerEvent e) {{}}
//void {SIM_NAME}_Scene::onPointerMove(PointerEvent e) {{}}
//void {SIM_NAME}_Scene::onWheel(PointerEvent e) {{}}
//void {SIM_NAME}_Scene::onKeyDown(KeyEvent e) {{}}
//void {SIM_NAME}_Scene::onKeyUp(KeyEvent e) {{}}

SIM_END({SIM_NAME})
