/*
 * collision.h
 * The Highway Pursuit - Collision Detection
 *
 * Declares functions responsible for detecting and responding to
 * collisions between the player car and all game objects:
 *   - Bot cars (v1, v2, v3)
 *   - Collectible coins (c1, c2, c3)
 *
 * All hitboxes use a 5 px inward shrink from the sprite edge to give
 * a forgiving but visually accurate collision feel.
 */

#ifndef COLLISION_H
#define COLLISION_H

#include "globals.h"
#include "structs.h"
#include "score.h"

/**
 * collision() - Tests the player car against bot cars (v1, v2, v3).
 * On hit: decrements lives, resets the offending car off-screen,
 * and calls triggerGameOver() if lives reach zero.
 */
void collision();

/**
 * coincollision() - Tests the player car against the three coin objects.
 * On hit: awards coin points to coinscore, repositions the coin,
 * and refreshes the combined score total.
 */
void coincollision();

#endif // COLLISION_H
