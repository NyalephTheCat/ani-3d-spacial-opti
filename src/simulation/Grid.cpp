//
// Created by Chloe Magnier on 05/10/2023.
//

#include "Grid.h"

#include <cgp/cgp.hpp>
#include <cassert>
#include <vector>

std::pair<int, int> Grid2d::get_cell_coordinates(particle_element p) {
    assert (p.p.x >= 0 && p.p.y >= 0);

    return std::make_pair((int) (p.p.x / cell_size), (int) (p.p.y / cell_size));
}

Grid2d::Grid2d(int grid_size, float cell_size) {
    this->grid_size = grid_size;
    this->cell_size = cell_size;

    // Initialize the grid
    grid = std::vector<std::vector<std::vector<particle_element>>>(grid_size, std::vector<std::vector<particle_element>>(grid_size, std::vector<particle_element>()));

    // Initialize the particles vector
    particles = std::vector<particle_element*>();
}

void Grid2d::add_particle(particle_element p) {
    // Add the particle to the grid
    std::pair<int, int> cell_coordinates = get_cell_coordinates(p);
    grid[cell_coordinates.first][cell_coordinates.second].push_back(p);

    // Add the particle to the particles vector
    particles.push_back(&grid[cell_coordinates.first][cell_coordinates.second].back());
}

std::vector<particle_element> Grid2d::get_particles_in_range(particle_element particle, sph_parameters_structure const& sph_parameters) {
    // Get the range of the particle
    float range = sph_parameters.h;

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
    std::vector<particle_element> particles_in_range;

    for (std::pair<int, int> cell : cells_in_range) {
        for (particle_element p : grid[cell.first][cell.second]) {
            if (norm(p.p - particle.p) <= range) {
                particles_in_range.push_back(p);
            }
        }
    }

    return particles_in_range;
}

std::vector<particle_element *> Grid2d::get_all_particles() {
    return this->particles;
}

void Grid2d::fill(float spacing, float h) {
    clear();

    for (float x = h; x < 1.0f - h; x = x + spacing * h)
    {
        for (float y = -1.0f + h; y < 1.0f - h; y = y + spacing * h)
        {
            particle_element particle;
            particle.p = { x + h / 8.0 * cgp::rand_interval(),y + h / 8.0 * cgp::rand_interval(),0 }; // a zero value in z position will lead to a 2D simulation
            add_particle(particle);
        }
    }
}

void Grid2d::clear() {
    grid = std::vector<std::vector<std::vector<particle_element>>>(grid_size, std::vector<std::vector<particle_element>>(grid_size, std::vector<particle_element>()));
    particles = std::vector<particle_element*>();
}

Grid2d::Grid2d() {
    const int grid_size = 100;
    const float cell_size = (new sph_parameters_structure())->h;

    this->grid_size = grid_size;
    this->cell_size = cell_size;

    // Initialize the grid
    grid = std::vector<std::vector<std::vector<particle_element>>>(grid_size, std::vector<std::vector<particle_element>>(grid_size, std::vector<particle_element>()));
}
