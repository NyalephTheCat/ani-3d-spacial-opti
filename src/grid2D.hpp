#pragma once

#include "cgp/cgp.hpp"
#include "simulation/simulation.hpp"

enum initial_velocity {
    NONE,
    RANDOM,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct grid_init_param {
    float spacing; // spacing is relative to the particle size
    cgp::vec2 padding; // Padding is under the form (top/bottom, left/right)
    initial_velocity velocity;

    grid_init_param() : spacing(1.2f), padding(cgp::vec2(0.2f, 0.2f)), velocity(NONE) {}

    grid_init_param(
            float spacing,
            cgp::vec2 padding,
            initial_velocity velocity) : spacing(spacing), padding(padding), velocity(velocity) {}
};

/**
 * @brief A 2D grid to optimize the search of particles
 *
 * Currently this method uses a 2D grid to store the particles. It is not the most efficient method but it allows for a
 * much better result than a naive search
 *
 * Grid will always be a square between (-1, -1) and (1, 1)
 */
class Grid2d {
public:
    explicit Grid2d(const sph_parameters_structure& sph_parameters);

    Grid2d(const Grid2d& other) = default;

    /**
     * @brief creates a grid with the given parameters
     *
     * @param grid_init_param the parameters of the grid
     */
     void create_grid(grid_init_param const& grid_init_param);

    /**
     * @brief clears the grid
     */
    void clear();

    /**
     * @brief Add a particle to the grid
     *
     * @param p The particle to add
     */
    void add_particle(particle_element p);

    /**
     * @brief a getter for the number of particles in the grid
     *
     * @return the number of particles in the grid
     */
    inline unsigned long get_number_of_particles() { return particles.size(); }

    /**
     * @brief Get particles
     *
     * @return A vector of particles
     */
    inline std::vector<particle_element*> get_all_particles() { return particles; }

    /**
     * @brief Get all the particles influencing a particle
     *
     * @param particle The particle
     * @return A vector of particles that influence the particle
     */
     std::vector<particle_element*> get_particles_influencing(particle_element particle);
private:
    float cell_size;
    int grid_size;

    std::vector<std::vector<std::vector<particle_element>>> grid;

    // A vector referencing all the particles in the grid
    std::vector<particle_element*> particles;

    /**
     * @brief Get the cell coordinates of a particle
     *
     * @param p The particle
     * @return The cell coordinates
     */
    std::pair<int, int> get_cell_coordinates(particle_element p);
};