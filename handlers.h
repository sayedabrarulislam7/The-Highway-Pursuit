/*
 * handlers.h
 * The Highway Pursuit - Button & Event Click Handlers
 *
 * Declares all click-handler functions that respond to in-game button
 * presses or mouse-click events.  Each handler updates the relevant
 * page state flags (declared in globals.h) to transition between screens
 * and resets game state where necessary.
 *
 * Navigation handlers:
 *   startbuttonclickhandler       → Homepage → Difficulty Selection
 *   aboutbuttonclickhandler       → Homepage → About Page
 *   instructionbuttonclickhandler → Homepage → Instructions Page
 *   scorebuttonclickhandler       → Homepage → High Scores Page
 *   backbuttonclickhandler        → Any sub-page → Homepage
 *
 * Level start handlers (also reset all game state for a fresh run):
 *   easybuttonclickhandler        → Starts Easy mode
 *   mediumbuttonclickhandler      → Starts Medium mode
 *   hardbuttonclickhandler        → Starts Hard mode
 *
 * Pause handlers:
 *   pausebuttonclickhandler       → Opens Pause screen
 *   pausebuttonclickhandler2      → Resumes game from Pause screen
 */

#ifndef HANDLERS_H
#define HANDLERS_H

#include "globals.h"
#include "structs.h"
#include "score.h"

// ─── Navigation Handlers ───────────────────────────────────────────────────
void startbuttonclickhandler();
void aboutbuttonclickhandler();
void instructionbuttonclickhandler();
void scorebuttonclickhandler();
void backbuttonclickhandler();

// ─── Level Start Handlers ──────────────────────────────────────────────────
void easybuttonclickhandler();
void mediumbuttonclickhandler();
void hardbuttonclickhandler();

// ─── Pause Handlers ────────────────────────────────────────────────────────
void pausebuttonclickhandler();
void pausebuttonclickhandler2();

#endif // HANDLERS_H
