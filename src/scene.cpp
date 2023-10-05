#include "scene.hpp"

void update_field_color(grid_2D<vec3>& field, Grid2d grid) {
    field.fill({ 1,1,1 });
    float const d = 0.1f;
    int const Nf = int(field.dimension.x);

    for (int kx = 0; kx < Nf; ++kx) {
        for (int ky = 0; ky < Nf; ++ky) {

            float f = 0.0f;
            vec3 const p0 = { 2.0f * (kx / (Nf - 1.0f) - 0.5f), 2.0f * (ky / (Nf - 1.0f) - 0.5f), 0.0f };

            for (auto particle: grid.get_all_particles()) {
                vec3 const& pi = particle->p;
                float const r = norm(p0 - pi) / d;
                f += 0.25f * std::exp(-r * r);
            }

            field(kx, Nf - 1 - ky) = vec3(clamp(1 - f, 0, 1), clamp(1 - f, 0, 1), 1);
        }
    }
}

void scene_structure::initialize() {
    camera_projection = camera_projection_orthographic { -1.1f, 1.1f, -1.1f, 1.1f, -10, 10, window.aspect_ratio() };
    camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
    camera_control.look_at({ 0.0f, 0.0f, 2.0f }, {0,0,0}, {0,1,0});
    global_frame.initialize_data_on_gpu(mesh_primitive_frame());

    field.resize(30, 30);
    field_quad.initialize_data_on_gpu(mesh_primitive_quadrangle({ -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }) );
    field_quad.material.phong = { 1,0,0 };
    field_quad.texture.initialize_texture_2d_on_gpu(field);

    grid = Grid2d(sph_parameters);
    grid.create_grid(grid_init_param());

    sphere_particle.initialize_data_on_gpu(mesh_primitive_sphere(1.0,{0,0,0},10,10));
    sphere_particle.model.scaling = 0.01f;
    curve_visual.color = { 1,0,0 };
    curve_visual.initialize_data_on_gpu(curve_primitive_circle());
}

void scene_structure::display_frame() {
    // Set the light to the current position of the camera
    environment.light = camera_control.camera_model.position();

    timer.update(); // update the timer to the current elapsed time
    float const dt = 0.005f * timer.scale;

    simulate(dt, grid, sph_parameters);

    if (gui.display_particles) {
        for (auto particle: grid.get_all_particles()) {
            vec3 const& p = particle->p;
            sphere_particle.model.translation = p;
            draw(sphere_particle, environment);
        }
    }

    if (gui.display_radius) {
        curve_visual.model.scaling = sph_parameters.h;

        auto particles = grid.get_all_particles(); // Get all particles
        for (int i = 0; i < particles.size(); i += 10) {
            auto particle = particles[i]; // Get the particle at every 10th index
            curve_visual.model.translation = particle->p;
            draw(curve_visual, environment);
        }
    }

    if (gui.display_color) {
        update_field_color(field, grid);
        field_quad.texture.update(field);
        draw(field_quad, environment);
    }
}

void scene_structure::display_gui() {
    ImGui::Checkbox("Display color", &gui.display_color);
    ImGui::Checkbox("Display particles", &gui.display_particles);
    ImGui::Checkbox("Display radius", &gui.display_radius);

    if (ImGui::Button("Reset simulation")) {
        grid.create_grid(grid_init_param());
    }

    if (ImGui::Button("Random simulation")) {
        auto param = grid_init_param();
        param.velocity = initial_velocity::RANDOM;
        grid.create_grid(param);
    }
}

void scene_structure::mouse_move_event() {}

void scene_structure::mouse_click_event() {
    camera_control.action_mouse_click(environment.camera_view);
}

void scene_structure::keyboard_event() {
    camera_control.action_keyboard(environment.camera_view);
}

void scene_structure::idle_frame() {
    camera_control.idle_frame(environment.camera_view);
}