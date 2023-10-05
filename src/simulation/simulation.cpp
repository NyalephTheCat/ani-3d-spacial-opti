#include "simulation.hpp"
#include "grid2D.hpp"

using namespace cgp;

// Convert a density value to a pressure
float density_to_pressure(float rho, float rho0, float stiffness)
{
    return stiffness * (rho - rho0);
}

float W_laplacian_viscosity(vec3 const& p_i, vec3 const& p_j, float h)
{
    return 45.0f / (Pi * std::pow(h,6)) * (h-norm(p_i - p_j));
}

vec3 W_gradient_pressure(vec3 const& p_i, vec3 const& p_j, float h)
{
    return -45.0f / (Pi*std::pow(h,6)) * std::pow(h-norm(p_i-p_j),2) * (p_i - p_j) / norm(p_i - p_j);
}

float W_density(vec3 const& p_i, const vec3& p_j, float h)
{
    float const r = norm(p_i-p_j);
    assert_cgp_no_msg(r<=h);
    return 315.0/(64.0*3.14159f*std::pow(h,9)) * std::pow(h*h-r*r, 3.0f);
}

void update_density(Grid2d &grid, sph_parameters_structure const& sph_parameters) {
    float const h = sph_parameters.h;
    float const m = sph_parameters.m;

    for (auto particle: grid.get_all_particles()) {
        particle->rho = 0.0f;

        for (auto neighbour: grid.get_particles_influencing(*particle)) {
            particle->rho += m * W_density(particle->p, neighbour->p, h);
        }
    }
}

void update_pressure(Grid2d &grid, sph_parameters_structure const& sph_parameters) {
    float const rho0 = sph_parameters.rho0;
    float const stiffness = sph_parameters.stiffness;

    for (auto particle: grid.get_all_particles()) {
        particle->pressure = density_to_pressure(particle->rho, rho0, stiffness);
    }
}

void update_force(Grid2d &grid, sph_parameters_structure const& sph_parameters) {
    float const gravity = 9.81f;
    float const m = sph_parameters.m;
    float const h = sph_parameters.h;
    float const nu = sph_parameters.nu;

    // Apply gravity to the force
    for (auto particle: grid.get_all_particles()) {
        particle->f = sph_parameters.m * vec3{0, -gravity, 0};
    }

    // Apply viscosity to the force
    for (auto particle: grid.get_all_particles()) {
        vec3 viscosity_force = vec3{0, 0, 0};
        vec3 pressure_force = vec3{0, 0, 0};

        for (auto neighbour: grid.get_particles_influencing(*particle)) {
            if (neighbour == particle || norm(neighbour->p - particle->p) > h) {
                continue;
            }

            pressure_force += m * (particle->pressure + neighbour->pressure) / (2.0f * neighbour->rho) *
                              W_gradient_pressure(particle->p, neighbour->p, h);

            viscosity_force += m * (neighbour->v - particle->v) / neighbour->rho *
                               W_laplacian_viscosity(particle->p, neighbour->p, h);
        }

        particle->f += -m / particle->rho * pressure_force + m * nu * viscosity_force;
    }
}

void simulate(float dt, Grid2d &grid, sph_parameters_structure const& sph_parameters) {
    update_density(grid, sph_parameters);
    update_pressure(grid, sph_parameters);
    update_force(grid, sph_parameters);

    float const damping = 0.005f;
    float const m = sph_parameters.m;
    for (auto particle: grid.get_all_particles()) {
        vec3& p = particle->p;
        vec3& v = particle->v;
        vec3& f = particle->f;

        v = (1 - damping) * v + dt * f / m;
        p += dt * v;
    }

    // Collision
    float const epsilon = 1e-3f;
    for (auto particle: grid.get_all_particles()) {
        vec3& p = particle->p;
        vec3& v = particle->v;

        if (p.y < -1) { // Bottom
            p.y = -1 + epsilon * rand_interval();
            v.y *= -0.5f;
        }
        if (p.y > 1) { // Top
            p.y = 1 - epsilon * rand_interval();
            v.y *= -0.5f;
        }
        if (p.x < -1) { // Left
            p.x = -1 + epsilon * rand_interval();
            v.x *= -0.5f;
        }

        if (p.x > 1) { // Right
            p.x = 1 - epsilon * rand_interval();
            v.x *= -0.5f;
        }
    }
}