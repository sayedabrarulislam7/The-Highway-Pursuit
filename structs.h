/*
 * structs.h
 * The Highway Pursuit - Data Structures
 *
 * Defines all custom data types (structs) used across the game.
 * Variable instances (topScores, bullets) are declared in globals.h
 * and defined in iMain.cpp.
 */

#ifndef STRUCTS_H
#define STRUCTS_H

// ─── High Score Record ─────────────────────────────────────────────────────
// Stores one leaderboard entry: the player's name and their score.
struct HighScore {
    char name[50];
    int  score;
};

// ─── Bullet / Missile Entity ───────────────────────────────────────────────
// Represents a single projectile in the bullets[] pool.
#define MAX_BULLETS 10

struct Bullet {
    float x;
    float y;
    int   active;
    int   type;  // 0 = normal bullet, 1 = missile
};

#endif // STRUCTS_H
