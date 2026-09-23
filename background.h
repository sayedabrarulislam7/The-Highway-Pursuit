/*
 * background.h
 * The Highway Pursuit - Background Rendering
 *
 * Manages the three scrolling road backgrounds used across the three
 * difficulty levels:
 *   BG1 (bg1-01..30)  →  Easy level
 *   BG2 (bg2-01..30)  →  Medium level
 *   BG3 (bg3-01..30)  →  Hard level
 *
 * Each background is composed of 30 horizontal image slices that are
 * scrolled downward in a looping fashion to simulate vehicle movement.
 */

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "globals.h"

// ─── Background 1 (Easy) ───────────────────────────────────────────────────

/**
 * initializeimageposition() - Sets the initial Y position of each BG1 slice.
 * Must be called once before the game loop starts.
 */
void initializeimageposition();

/**
 * movebackground() - Scrolls all BG1 slices downward by one step.
 * Registered with iSetTimer so it runs every 10 ms.
 */
void movebackground();

/**
 * rendering() - Draws all BG1 slices at their current positions.
 * Called every frame while the easy page is active.
 */
void rendering();

// ─── Background 2 (Medium) ─────────────────────────────────────────────────

/**
 * initializebg2imageposition() - Sets initial Y positions for BG2 slices.
 */
void initializebg2imageposition();

/**
 * movebg2background() - Scrolls BG2 slices; registered with iSetTimer.
 */
void movebg2background();

/**
 * rendering2() - Draws all BG2 slices for the medium difficulty level.
 */
void rendering2();

// ─── Background 3 (Hard) ───────────────────────────────────────────────────

/**
 * initializebg3imageposition() - Sets initial Y positions for BG3 slices.
 */
void initializebg3imageposition();

/**
 * movebg3background() - Scrolls BG3 slices; registered with iSetTimer.
 */
void movebg3background();

/**
 * rendering3() - Draws all BG3 slices for the hard difficulty level.
 */
void rendering3();

#endif // BACKGROUND_H
