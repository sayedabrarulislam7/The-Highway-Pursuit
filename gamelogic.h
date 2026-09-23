/*
 * gamelogic.h
 * The Highway Pursuit - Core Game Logic
 *
 * Declares functions that drive the main gameplay loop:
 *   - Timer updates (score accumulation based on elapsed time)
 *   - HUD / timer overlay drawing
 *   - The central drawgamepage() that orchestrates all per-frame
 *     updates: bot movement, bullets, pickups, obstacles, police chase
 *   - Per-difficulty wrappers: draweasypage, drawmediumpage, drawhardpage
 */

#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "globals.h"
#include "structs.h"
#include "background.h"
#include "collision.h"
#include "score.h"

/**
 * changetimer() - Increments the time-based score component every second.
 * Registered with iSetTimer(1000, changetimer) at startup.
 */
void changetimer();

/**
 * drawtimer() - Draws the Score and Lives HUD boxes in the top-right
 * corner of the game screen during active gameplay.
 */
void drawtimer();

/**
 * drawgamepage(speedMultiplier) - Master per-frame render + update call.
 * Handles: background rendering, car/coin drawing, bot movement,
 * bullet updates, obstacle logic, police chase, and collision checks.
 *
 * @param speedMultiplier  Scale factor applied to all movement speeds.
 *                         1.0 = Easy baseline; higher for Medium/Hard.
 */
void drawgamepage(float speedMultiplier);

/**
 * draweasypage()   - Calls drawgamepage with Easy speed (1.0x).
 * drawmediumpage() - Ramps speed every 250 score pts (cap 2.5x), then draws.
 * drawhardpage()   - Ramps speed every 200 score pts (cap 3.5x), then draws.
 */
void draweasypage();
void drawmediumpage();
void drawhardpage();

#endif // GAMELOGIC_H
