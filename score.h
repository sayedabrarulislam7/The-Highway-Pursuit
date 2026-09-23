/*
 * score.h
 * The Highway Pursuit - Score & High Score Management
 *
 * Provides functions to save, load, and update the top-5 leaderboard
 * stored in highscores.txt, and to trigger the game-over sequence.
 */

#ifndef SCORE_H
#define SCORE_H

#include "structs.h"
#include "globals.h"

// ─── Function Declarations ─────────────────────────────────────────────────

/**
 * saveScores() - Persists the current topScores array to highscores.txt.
 * Called every time a new high score is registered.
 */
void saveScores();

/**
 * loadScores() - Reads topScores from highscores.txt.
 * If the file does not exist, initialises all slots to "Empty" / 0.
 */
void loadScores();

/**
 * updateHighScores(newScore) - Inserts newScore into topScores if it
 * qualifies, shifting lower entries down, then saves to disk.
 */
void updateHighScores(int newScore);

/**
 * triggerGameOver() - Switches the game to the game-over screen and
 * records the current score into the leaderboard.
 * Safe to call multiple times (idempotent via gameoverpage guard).
 */
void triggerGameOver();

#endif // SCORE_H
