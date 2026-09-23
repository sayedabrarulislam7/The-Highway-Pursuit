/*
 * globals.h
 * The Highway Pursuit - Global Variables
 *
 * This header declares all global variables used throughout the game,
 * including page state flags, player state, positions, and game object state.
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include "structs.h"   // HighScore, Bullet, MAX_BULLETS

// ─── Page State Flags ──────────────────────────────────────────────────────
extern int homepage;
extern int startpage;
extern int aboutpage;
extern int instructionpage;
extern int scorepage;
extern int easypage;
extern int mediumpage;
extern int hardpage;
extern int pausepage;
extern int gameoverpage;
extern int nameentrypage;

// ─── Button Click Trackers ─────────────────────────────────────────────────
extern int aboutbuttonclick;
extern int startbuttonclick;
extern int instructionbuttonclick;
extern int scorebuttonclick;
extern int easybuttonclick;
extern int mediumbuttonclick;
extern int hardbuttonclick;
extern int pausebuttonclick;

// ─── Score & Timer ─────────────────────────────────────────────────────────
extern int second;
extern int score;
extern int lives;
extern int coinscore;
extern int botKillScore;
extern int minute;
extern int hour;
extern int totalsecond;
extern char sec[10000];
extern char hr[10000];
extern char point[10000];

// ─── Player ────────────────────────────────────────────────────────────────
extern float playerx;
extern float playery;
extern char playerName[50];
extern int  nameIndex;
extern int  nameFocus;

// ─── Coin Positions ────────────────────────────────────────────────────────
extern int c1Y, c2Y, c3Y;
extern int c1X, c2X, c3X;
extern int c1Xspeed, c2Xspeed, c3Xspeed;

// ─── Bot Car Positions ─────────────────────────────────────────────────────
extern float x,  y;       // v1
extern float a,  b;       // v2
extern float x3, y3;      // v3
extern float v3TargetX;
extern int   v3LaneSwitchTimer;

// ─── Bot Car Health ────────────────────────────────────────────────────────
extern int v1Health;
extern int v2Health;
extern int v3Health;

// ─── Missile / Bullet ──────────────────────────────────────────────────────
extern int missileCount;

// ─── Diamond Pickup ────────────────────────────────────────────────────────
extern float diamondX;
extern float diamondY;
extern int   diamondActive;
extern int   nextDiamondScore;

// ─── Life Pickup ───────────────────────────────────────────────────────────
extern float lifeX;
extern float lifeY;
extern int   lifeActive;
extern int   nextLifeScore;

// ─── Blockade Obstacle ─────────────────────────────────────────────────────
extern float blockadeX;
extern float blockadeY;
extern int   blockadeActive;
extern int   blockadeHealth;

// ─── Secondary Obstacle (obs2) ─────────────────────────────────────────────
extern float obs2X;
extern float obs2Y;
extern int   obs2Active;
extern int   obs2Health;

// ─── Police Car Chase ──────────────────────────────────────────────────────
extern float policeX;
extern float policeY;
extern int   policeActive;
extern int   policeTimer;
extern float policeCurrentSpeed;

// ─── Speed Ramps ───────────────────────────────────────────────────────────
extern float mediumSpeedMultiplier;
extern int   mediumScoreMilestone;
extern float hardSpeedMultiplier;
extern int   hardScoreMilestone;

// ─── Background Image Data ─────────────────────────────────────────────────
extern int   totalimagecount;
extern int   increment;
extern char  backgroundimagepath[30][250];
extern int   imgposition[100];
extern int   backgroundY;
extern int   backgroundimagepathindex;

extern int   bg2totalimagecount;
extern int   bg2increment;
extern char  bg2backgroundimagepath[30][250];
extern int   bg2imgposition[100];

extern int   bg3totalimagecount;
extern int   bg3increment;
extern char  bg3backgroundimagepath[30][250];
extern int   bg3imgposition[100];

#endif // GLOBALS_H
