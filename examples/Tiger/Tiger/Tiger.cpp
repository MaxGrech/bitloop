#include "Tiger.h"
#include "draw_tiger.h"

SIM_DECLARE(Tiger)

using namespace BL;

/// =========================
/// ======== Project ========
/// =========================

void Tiger_Project_Data::populate()
{
    ImGui::SliderInt("Viewport Count", &viewport_count, 1, 8);
}

void Tiger_Project::projectPrepare(Layout& layout)
{
    layout << create<Tiger_Scene>(viewport_count);
}

/// =======================
/// ======== Scene ========
/// =======================


void Tiger_Scene_Data::populate()
{
    ImGui::Checkbox("Transform coordinates", &transform_coordinates);
    ImGui::Checkbox("Scale Lines & Text", &scale_lines);
    ImGui::Checkbox("Scale Sizes", &scale_sizes);
    ImGui::Checkbox("Rotate Text", &rotate_text);

    if (ImGui::SceneSection("View", 0.0f, 2.0f, true)) 
    {
        cam_view.populateUI();
    }
}

void Tiger_Scene::sceneStart()
{}

void Tiger_Scene::sceneMounted(Viewport*)
{
    // Initialize viewport
    camera->setOriginViewportAnchor(Anchor::CENTER);
    camera->setDirectCameraPanning(true);
    camera->focusWorldRect(-100, -100, 1000, 1000);

    cam_view.read(camera);
}

void Tiger_Scene::sceneDestroy()
{}

void Tiger_Scene::sceneProcess()
{}

void Tiger_Scene::viewportProcess(Viewport*, double)
{
    // ======== Camera View ========
    cam_view.apply(camera); // Apply any camera view settings from imgui
}

void Tiger_Scene::viewportDraw(Viewport* ctx) const
{
    ctx->drawWorldAxis();

    camera->worldCoordinates(transform_coordinates);
    camera->scalingLines(scale_lines);
    camera->scalingSizes(scale_sizes);

    draw_tiger(ctx);
}

void Tiger_Scene::onEvent(Event e)
{
    if (e.ctx_owner())
    {
        // Handle camera navigation for viewport that captured the event
        e.ctx_owner()->camera.handleWorldNavigation(e, true);

        cam_view.read(&e.ctx_owner()->camera);
    }
}

SIM_END(Tiger)
