#pragma once
#include "library.h"
#include "board.h"
#include "utils.h"
#include "point.h"
#include "hazard.h"
class Mario;

// Constant for the total number of barrels
constexpr int SUM_OF_BARRELS = 4;

/**
 * @class barrels
 * @brief Represents a barrel, which is a type of hazard in the game.
 *
 * This class handles the movement, direction, and interactions of barrels.
 */
class barrels : public hazard {

    int fall_count = 0; ///< Counter for the number of steps the barrel has fallen.
    int max_fall_count = 8; ///< Maximum number of steps the barrel can fall before resetting.

public:

    /**
     * @brief Constructor to initialize a barrel with its position, direction, and reference to Mario.
     * @param _x The initial x-coordinate of the barrel.
     * @param _y The initial y-coordinate of the barrel.
     * @param _dir The initial direction of the barrel (-1 for left, 1 for right).
     * @param mario A pointer to the Mario object for interaction.
     */
    barrels(int _x, int _y, int _dir, Mario* mario) : hazard(_x, _y, _dir, mario, 'O') { find_dir(_x, _y); set_start_dir(get_dir()); }

    /**
     * @brief Moves the barrel based on its current direction and environment.
     */
    void move();

    /**
     * @brief Determines the direction of the barrel based on the given position.
     * @param x The x-coordinate to evaluate.
     * @param y The y-coordinate to evaluate.
     */
    void find_dir(int x, int y);

    /**
     * @brief Handles the behavior of an exploding barrel.
     *
     * This function manages what happens when the barrel explodes, such as clearing its position or interacting with the environment.
     */
    void exploding_barrel();

    /**
     * @brief Deletes the barrel from the game.
     *
     * This function is used to remove the barrel when it is no longer needed, such as after an explosion.
     */
    void delete_barrel();

    /**
     * @brief Checks if Mario is at the same position as the barrel.
     * @return True if Mario is at the barrel's position, false otherwise.
     */
    bool is_mario_here() const;

    /**
     * @brief Resets the barrel's state to its initial conditions.
     *
     * This function is used to restart the barrel, resetting its position and counters.
     */
    void restart();
};
