#pragma once

#include "grid2D.hpp"

#include <iostream>

using namespace cgp;

Grid2d::Grid2d(const sph_parameters_structure &sph_parameters) {
    // Example: Initialize grid_size and cell_size based on sph_parameters
    cell_size = sph_parameters.h;

    // Calculate grid_size based on cell_size and domain size
    grid_size = static_cast<int>(2 / cell_size);

    grid.resize(grid_size);
    for (int i = 0; i < grid_size; ++i) {
        grid[i].resize(grid_size);
    }
}

std::pair<int, int> Grid2d::get_cell_coordinates(particle_element p) {
    // Calculate the normalized coordinates within the grid
    float normalizedX = (p.p.x + 1.0f) * 0.5f;
    float normalizedY = (p.p.y + 1.0f) * 0.5f;

    // Calculate the cell coordinates based on the normalized coordinates
    int x = static_cast<int>(normalizedX * grid_size);
    int y = static_cast<int>(normalizedY * grid_size);

    // Make sure the coordinates are within the grid boundaries
    x = std::max(0, std::min(x, grid_size - 1));
    y = std::max(0, std::min(y, grid_size - 1));

    return std::make_pair(x, y);
}

void Grid2d::clear() {
    // Clear the grid
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            grid[i][j].clear();
        }
    }

    // Clear the particles vector
    particles.clear();
}

void Grid2d::add_particle(particle_element p) {
    // Add the particle to the grid
    std::pair<int, int> cell_coordinates = get_cell_coordinates(p);

    grid[cell_coordinates.first][cell_coordinates.second].push_back(p);

    // Add the particle to the particles vector
    particles.push_back(&grid[cell_coordinates.first][cell_coordinates.second].back());
}

std::vector<particle_element*> Grid2d::get_particles_influencing(particle_element particle) {
    std::vector<particle_element*> influencing_particles;

    // Get the cell coordinates of the particle
    std::pair<int, int> cell_coords = get_cell_coordinates(particle);
    int cell_x = cell_coords.first;
    int cell_y = cell_coords.second;

    // Define a range of neighboring cells to check
    int min_x = std::max(0, cell_x - 1);
    int max_x = std::min(grid_size - 1, cell_x + 1);
    int min_y = std::max(0, cell_y - 1);
    int max_y = std::min(grid_size - 1, cell_y + 1);

    // Iterate over neighboring cells and add particles from those cells
    for (int x = min_x; x <= max_x; ++x) {
        for (int y = min_y; y <= max_y; ++y) {
            std::vector<particle_element>& cell = grid[x][y];
            for (particle_element& neighbor_particle : cell) {
                // TODO FIX
                if (norm(neighbor_particle.p - particle.p) < cell_size) {
                    influencing_particles.push_back(&neighbor_particle);
                }
            }
        }
    }

    /* This works but is slower
    for (auto p: get_all_particles()) {
        if (norm(p->p - particle.p) < cell_size) {
            influencing_particles.push_back(p);
        }
    }
     */

    return influencing_particles;
}

vec3 get_initial_velocity(initial_velocity velocity) {
    switch (velocity) {
        case initial_velocity::DOWN:
            return vec3{0, -1, 0};
        case initial_velocity::UP:
            return vec3{0, 1, 0};
        case initial_velocity::LEFT:
            return vec3{-1, 0, 0};
        case initial_velocity::RIGHT:
            return vec3{1, 0, 0};
        case initial_velocity::NONE:
            return vec3{0, 0, 0};
        case initial_velocity::RANDOM:
            return vec3{rand_interval(-1.0f, 1.0f), rand_interval(-1.0f, 1.0f), 0};
        default:
            return vec3{0, 0, 0};
    }
}

void Grid2d::create_grid(const grid_init_param &grid_init_param) {
    clear();

    float true_spacing = grid_init_param.spacing * cell_size;

    for (float i = -1 + grid_init_param.padding.x; i <= 1 - grid_init_param.padding.x; i += true_spacing) {
        for (float j = -1 + grid_init_param.padding.y; j <= 1 - grid_init_param.padding.y; j += true_spacing) {
            particle_element particle;
            particle.p = vec3{i + cell_size / 8.0 * rand_interval(), j + cell_size / 8.0 * rand_interval(), 0};
            particle.v = get_initial_velocity(grid_init_param.velocity);

            add_particle(particle);
        }
    }
}