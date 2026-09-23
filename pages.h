/*
 * pages.h
 * The Highway Pursuit - UI Page Drawing Functions
 *
 * Declares all functions that render the game's non-gameplay screens:
 *   - Main Menu (Homepage)
 *   - Difficulty Selection (Start Page)
 *   - About Page
 *   - Instructions Page
 *   - High Scores Page
 *   - Pause Screen
 *   - Game Over Screen
 *   - Name Entry Screen
 *
 * Each function clears or decorates the screen appropriate to the
 * current page state flag set in globals.h.
 */

#ifndef PAGES_H
#define PAGES_H

#include "globals.h"
#include "structs.h"

/**
 * drawHomepage() - Renders the main menu background and title.
 */
void drawHomepage();

/**
 * drawstartpage() - Renders the difficulty selection screen.
 */
void drawstartpage();

/**
 * drawAboutPage() - Renders the About / credits screen.
 */
void drawAboutPage();

/**
 * drawInstructionPage() - Renders the game instruction screen.
 */
void drawInstructionPage();

/**
 * drawScorePage() - Renders the top-5 high scores leaderboard.
 */
void drawScorePage();

/**
 * drawpausepage() - Renders the pause overlay screen.
 */
void drawpausepage();

/**
 * drawgameoverpage() - Renders the game-over screen with final score,
 *                      Retry and Home buttons.
 */
void drawgameoverpage();

/**
 * drawNameEntryPage() - Renders the player name entry form shown before
 *                       starting a game level.
 */
void drawNameEntryPage();

#endif // PAGES_H
