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

std::vector<particle_element *> Grid2d::get_particles_influencing(particle_element particle) {
    // Get the range of the particle
    float range = cell_size;

    // Get the cell coordinates of the particle
    std::pair<int, int> cell_coordinates = get_cell_coordinates(particle);

    // Get the cells in range
    std::vector<std::pair<int, int>> cells_in_range;

    // Get the cells in range
    for (int i = cell_coordinates.first - 1; i <= cell_coordinates.first + 1; i++) {
        for (int j = cell_coordinates.second - 1; j <= cell_coordinates.second + 1; j++) {
            if (i >= 0 && i < grid_size && j >= 0 && j < grid_size) {
                cells_in_range.emplace_back(i, j);
            }
        }
    }

    // Get the particles in range
    std::vector<particle_element *> particles_in_range;

    for (std::pair<int, int> cell : cells_in_range) {
        for (particle_element &p : grid[cell.first][cell.second]) {
            if (norm(p.p - particle.p) <= range) {
                particles_in_range.push_back(&p);
            }
        }
    }

    return particles_in_range;
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