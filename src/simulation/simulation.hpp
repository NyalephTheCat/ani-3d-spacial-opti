#pragma once

#include "cgp/cgp.hpp"

class Grid2d;

struct particle_element {
    cgp::vec3 p; // Position
    cgp::vec3 v; // Speed
    cgp::vec3 f; // Force

    float rho;      // density at this particle position
    float pressure; // pressure at this particle position

    particle_element() : p{0,0,0},v{0,0,0},f{0,0,0},rho(0),pressure(0) {}
    particle_element(particle_element const &p) = default;
};

struct sph_parameters_structure {
    float h = 0.12f / 2.0; // Influence distance of a particle (size of the kernel)

    float rho0 = 1; // Rest density (normalized to 1 - real unit should be 1000kg/m^2)

    float m = rho0 * h * h; // Total mass of a particle (consider rho0 h^2)

    float nu = 0.02f; // viscosity parameter

    float stiffness = 8.0f; // Stiffness converting density to pressure
};

void simulate(float dt, Grid2d &grid, sph_parameters_structure const& sph_parameters);