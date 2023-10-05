//
// Created by Chloe Magnier on 05/10/2023.
//

#ifndef INC_06_SPH_GRID_H
#define INC_06_SPH_GRID_H

#include "simulation.hpp"

/**
 * @brief A class to store and retrieve particles in a grid
 *
 * You can use this class to store particles depending on their location and retrieve them efficiently as well as all the ones impacting it
 *
 */
class Grid2d {
private:
    int grid_size; // The size of the grid
    float cell_size; // The size of a cell. Ideally you want it to be the same as the radius of the particles

    std::vector<std::vector<std::vector<particle_element>>> grid; // The grid itself. It is a 3D vector of int. The first two dimensions are the coordinates of the cell in the grid. The third dimension is the list of particles in this cell

    // A vector referencing all the particles in the grid
    std::vector<particle_element*> particles;

    /**
     * @brief Get the cell coordinates of a particle
     *
     * @param p The particle
     * @return The cell coordinates
     */
    std::pair<int, int> get_cell_coordinates(particle_element p);

public:
    // constructor
    Grid2d();
    Grid2d(int grid_size, float cell_size);

    /**
     * @brief creates a grid with the given parameters
     *
     * @param grid_size the size of the grid
     * @param cell_size the size of a cell
     * @param spacing the space between particules. Depends on the size of the particles
     * @param sph_parameters the parameters of the simulation
     *
     * @return the grid
     */
    void fill(float spacing, float h);

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
     * @brief Get all the particles in range of a particle
     *
     * @param particle The particle
     * @param sph_parameters The parameters of the simulation
     * @return A vector of particles in range
     */
    std::vector<particle_element> get_particles_in_range(particle_element particle, sph_parameters_structure const& sph_parameters);

    /**
     * @brief Get all the particles in the grid
     *
     * @return A vector of particles
     */
    std::vector<particle_element *> get_all_particles();
};


#endif //INC_06_SPH_GRID_H
