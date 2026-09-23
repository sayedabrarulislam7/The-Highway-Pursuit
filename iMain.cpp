/*
 * iMain.cpp
 * The Highway Pursuit - Main Entry Point
 *
 * Contains the iGraphics callback functions (iDraw, iKeyboard,
 * iSpecialKeyboard, iMouse, iMouseMove, iPassiveMouseMove, fixedUpdate)
 * and the program entry point (main).
 *
 * All game logic is organised into the following header modules:
 *   globals.h    - Global variable declarations
 *   structs.h    - Data structures (HighScore, Bullet)
 *   score.h      - Score / leaderboard management
 *   background.h - Scrolling road background rendering
 *   collision.h  - Collision detection between car and objects
 *   gamelogic.h  - Core game-loop functions and per-difficulty page draw
 *   pages.h      - UI screen rendering (menu, about, pause, gameover, etc.)
 *   handlers.h   - Button and mouse click event handlers
 */

#include "iGraphics.h"   // iGraphics library (OpenGL/GLUT wrapper)
#include <stdio.h>       // File I/O (fopen_s, fprintf, fscanf_s)
#include <string.h>      // String utilities (strcpy_s, sprintf_s)

// ─── Game Header Files ────────────────────────────────────────────────────
#include "globals.h"     // All global variable declarations
#include "structs.h"     // HighScore and Bullet data structures
#include "score.h"       // Score saving, loading, and updating
#include "background.h"  // Background image scroll and rendering
#include "collision.h"   // Collision detection functions
#include "gamelogic.h"   // Core game logic and per-frame update
#include "pages.h"       // UI screen drawing functions
#include "handlers.h"    // Button and navigation click handlers



int aboutbuttonclick = 0;
int startbuttonclick = 0;
int instructionbuttonclick = 0;
int scorebuttonclick = 0;
int easybuttonclick = 0;
int mediumbuttonclick = 0;
int hardbuttonclick = 0;
int pausebuttonclick = 0;



int homepage = 1;
int startpage = 0;
int aboutpage = 0;
int instructionpage = 0;
int scorepage = 0;
int easypage = 0;
int mediumpage = 0;
int hardpage = 0;
int pausepage = 0;
int gameoverpage = 0;
int nameentrypage = 0;

int second = 0;
int score = 0;
int lives = 4;
int coinscore = 0;

int c1Y = 50;
int c2Y = 300;
int c3Y = 500;
int c1X = 300;
int c2X = 500;
int c3X = 700;
int c1Xspeed = 0;
int c2Xspeed = 0;
int c3Xspeed = 0;

char playerName[50] = "Player";
int nameIndex = 6;
int nameFocus = 0;

// HighScore struct is defined in structs.h (included via globals.h)
HighScore topScores[5];

void saveScores() {
	FILE *fp;
	errno_t err = fopen_s(&fp, "highscores.txt", "w");
	if (err == 0 && fp) {
		for (int i = 0; i < 5; i++) {
			fprintf(fp, "%s %d\n", topScores[i].name, topScores[i].score);
		}
		fclose(fp);
	}
}

void loadScores() {
	FILE *fp;
	errno_t err = fopen_s(&fp, "highscores.txt", "r");
	if (err == 0 && fp) {
		for (int i = 0; i < 5; i++) {
			fscanf_s(fp, "%s %d", topScores[i].name, (unsigned int)sizeof(topScores[i].name), &topScores[i].score);
		}
		fclose(fp);
	} else {
		for (int i = 0; i < 5; i++) {
			strcpy_s(topScores[i].name, sizeof(topScores[i].name), "Empty");
			topScores[i].score = 0;
		}
	}
}

void updateHighScores(int newScore) {
	int i, j;
	for (i = 0; i < 5; i++) {
		if (newScore > topScores[i].score) {
			for (j = 4; j > i; j--) {
				topScores[j] = topScores[j - 1];
			}
			topScores[i].score = newScore;
			if (playerName[0] == '\0') {
				strcpy_s(topScores[i].name, sizeof(topScores[i].name), "Player");
			} else {
				strcpy_s(topScores[i].name, sizeof(topScores[i].name), playerName);
			}
			saveScores();
			break;
		}
	}
}

void triggerGameOver() {
	if (gameoverpage == 0) {
		gameoverpage = 1;
		easypage = 0;
		mediumpage = 0;
		hardpage = 0;
		updateHighScores(score);
	}
}


float playerx = 470, playery = 0;
float x = 400, y = 601;
float a = 600, b = 601;
float x3 = 500, y3 = 601; // v3 position
float v3TargetX = 500;    // Target X for lane switching in hard mode
int v3LaneSwitchTimer = 0;





//collision
// Image sizes: usercar=60x60, v1/v2=40x76, policecar=50x50, blockade=80x17, bullet=8x50
// Using 5px inward shrink on player and enemies so hitbox sits inside the visible car body.
void collision(){
	float pL = playerx + 5,  pR = playerx + 55;
	float pB = playery + 5,  pT = playery + 55;

	if (pR >= x + 4 && pL <= x + 36 && pT >= y + 4 && pB <= y + 72){
		lives--;
		y = 601;
		x = 300 + rand() % 340;
		if(lives <= 0) triggerGameOver(); 
	}

	//v2  (v2.bmp: 40 wide x 76 tall, shrink 4px inward)
	if (pR >= a + 4 && pL <= a + 36 && pT >= b + 4 && pB <= b + 72){
		lives--;
		b = 601;
		a = 300 + rand() % 340;
		if(lives <= 0) triggerGameOver(); 
	}

	//v3  (v3.bmp: 40 wide x 76 tall)
	if (pR >= x3 + 4 && pL <= x3 + 36 && pT >= y3 + 4 && pB <= y3 + 72){
		lives--;
		y3 = 601;
		x3 = 300 + rand() % 340;
		v3TargetX = x3;
		if(lives <= 0) triggerGameOver(); 
	}
}


//bgrendering  (Easy / Hard page)

int totalimagecount = 30;
int increment = 20;
char backgroundimagepath[30][250] = {
	"Images//01.bmp", "Images//02.bmp", "Images//03.bmp", "Images//04.bmp", "Images//05.bmp",
	"Images//06.bmp", "Images//07.bmp", "Images//08.bmp", "Images//09.bmp", "Images//10.bmp",
	"Images//11.bmp", "Images//12.bmp", "Images//13.bmp", "Images//14.bmp", "Images//15.bmp",
	"Images//16.bmp", "Images//17.bmp", "Images//18.bmp", "Images//19.bmp", "Images//20.bmp",
	"Images//21.bmp", "Images//22.bmp", "Images//23.bmp", "Images//24.bmp", "Images//25.bmp",
	"Images//26.bmp", "Images//27.bmp", "Images//28.bmp", "Images//29.bmp", "Images//30.bmp"
};

int imgposition[100];
int backgroundY = 0;
int backgroundimagepathindex = 0;

void initializeimageposition(){
	int i, j;
	for (i = 0, j = 0; i < totalimagecount; i++){
		imgposition[i] = j;
		j = j + increment;
	}
}

void movebackground(){
	for (int i = 0; i < totalimagecount; i++){
		imgposition[i] = imgposition[i] - increment;
	}
	for (int i = 0; i < totalimagecount; i++){
		if (imgposition[i] < 0){
			imgposition[i] = 600 - increment;
		}
	}
}
void rendering(){
	for (int i = 0; i < totalimagecount; i++){
		iShowBMP2(0, imgposition[i], backgroundimagepath[i], 0);
	}
}
    

int bg2totalimagecount = 30;
int bg2increment = 20;
char bg2backgroundimagepath[30][250] = {
	"Images//bg2-01.bmp", "Images//bg2-02.bmp", "Images//bg2-03.bmp", "Images//bg2-04.bmp", "Images//bg2-05.bmp",
	"Images//bg2-06.bmp", "Images//bg2-07.bmp", "Images//bg2-08.bmp", "Images//bg2-09.bmp", "Images//bg2-10.bmp",
	"Images//bg2-11.bmp", "Images//bg2-12.bmp", "Images//bg2-13.bmp", "Images//bg2-14.bmp", "Images//bg2-15.bmp",
	"Images//bg2-16.bmp", "Images//bg2-17.bmp", "Images//bg2-18.bmp", "Images//bg2-19.bmp", "Images//bg2-20.bmp",
	"Images//bg2-21.bmp", "Images//bg2-22.bmp", "Images//bg2-23.bmp", "Images//bg2-24.bmp", "Images//bg2-25.bmp",
	"Images//bg2-26.bmp", "Images//bg2-27.bmp", "Images//bg2-28.bmp", "Images//bg2-29.bmp", "Images//bg2-30.bmp"
};

int bg2imgposition[100];

void initializebg2imageposition(){
	int i, j;
	for (i = 0, j = 0; i < bg2totalimagecount; i++){
		bg2imgposition[i] = j;
		j = j + bg2increment;
	}
}

void movebg2background(){
	for (int i = 0; i < bg2totalimagecount; i++){
		bg2imgposition[i] = bg2imgposition[i] - bg2increment;
	}
	for (int i = 0; i < bg2totalimagecount; i++){
		if (bg2imgposition[i] < 0){
			bg2imgposition[i] = 600 - bg2increment;
		}
	}
}

void rendering2(){
	for (int i = 0; i < bg2totalimagecount; i++){
		iShowBMP2(0, bg2imgposition[i], bg2backgroundimagepath[i], 0);
	}
}

int bg3totalimagecount = 30;
int bg3increment = 20;
char bg3backgroundimagepath[30][250] = {
	"Images//bg3slice//bg3-01.bmp", "Images//bg3slice//bg3-02.bmp", "Images//bg3slice//bg3-03.bmp", "Images//bg3slice//bg3-04.bmp", "Images//bg3slice//bg3-05.bmp",
	"Images//bg3slice//bg3-06.bmp", "Images//bg3slice//bg3-07.bmp", "Images//bg3slice//bg3-08.bmp", "Images//bg3slice//bg3-09.bmp", "Images//bg3slice//bg3-10.bmp",
	"Images//bg3slice//bg3-11.bmp", "Images//bg3slice//bg3-12.bmp", "Images//bg3slice//bg3-13.bmp", "Images//bg3slice//bg3-14.bmp", "Images//bg3slice//bg3-15.bmp",
	"Images//bg3slice//bg3-16.bmp", "Images//bg3slice//bg3-17.bmp", "Images//bg3slice//bg3-18.bmp", "Images//bg3slice//bg3-19.bmp", "Images//bg3slice//bg3-20.bmp",
	"Images//bg3slice//bg3-21.bmp", "Images//bg3slice//bg3-22.bmp", "Images//bg3slice//bg3-23.bmp", "Images//bg3slice//bg3-24.bmp", "Images//bg3slice//bg3-25.bmp",
	"Images//bg3slice//bg3-26.bmp", "Images//bg3slice//bg3-27.bmp", "Images//bg3slice//bg3-28.bmp", "Images//bg3slice//bg3-29.bmp", "Images//bg3slice//bg3-30.bmp"
};

int bg3imgposition[100];

void initializebg3imageposition(){
	int i, j;
	for (i = 0, j = 0; i < bg3totalimagecount; i++){
		bg3imgposition[i] = j;
		j = j + bg3increment;
	}
}

void movebg3background(){
	for (int i = 0; i < bg3totalimagecount; i++){
		bg3imgposition[i] = bg3imgposition[i] - bg3increment;
	}
	for (int i = 0; i < bg3totalimagecount; i++){
		if (bg3imgposition[i] < 0){
			bg3imgposition[i] = 600 - bg3increment;
		}
	}
}

void rendering3(){
	for (int i = 0; i < bg3totalimagecount; i++){
		iShowBMP2(0, bg3imgposition[i], bg3backgroundimagepath[i], 0);
	}
}

// Medium page: gradual speed ramp every 250 score points
float mediumSpeedMultiplier = 1.0f;
int   mediumScoreMilestone  = 250;   // next score threshold that triggers a speed bump

// Hard page: gradual speed ramp every 200 score points
float hardSpeedMultiplier = 2.0f;
int   hardScoreMilestone  = 200;   // next score threshold that triggers a speed bump

// MAX_BULLETS, struct Bullet defined in structs.h (included via globals.h)
Bullet bullets[MAX_BULLETS];
int missileCount = 0;
int v1Health = 2;
int v2Health = 2;
int v3Health = 2;
int botKillScore = 0;

float diamondX = 1000;
float diamondY = -100;
int diamondActive = 0;
int nextDiamondScore = 350;

float lifeX = 1000;
float lifeY = -100;
int lifeActive = 0;
int nextLifeScore = -1;

float blockadeX = 1000;
float blockadeY = -100;
int blockadeActive = 0;
int blockadeHealth = 5;

float obs2X = 1000;
float obs2Y = -100;
int obs2Active = 0;
int obs2Health = 5;

float policeX = 470;
float policeY = -200;
int policeActive = 0;
int policeTimer = 0;
float policeCurrentSpeed = 0.3f; // starts slow, ramps up each frame



void coincollision()
{
	// c1
	if ((playerx + 60 >= c1X && playerx <= c1X + 50) &&
		(playery + 60 >= c1Y && playery <= c1Y + 60))
	{
		coinscore += 5;

		// respawn coin to right side
		c1X = 1000;
		c1Y = rand() % 400 + 50;
	}

	// c2
	if ((playerx + 50 >= c2X && playerx <= c2X + 50) &&
		(playery + 50 >= c2Y && playery <= c2Y + 50))
	{
		coinscore += 10;

		c2X = 1000;
		c2Y = rand() % 400 + 50;
	}

	// c3
	if ((playerx + 50 >= c3X && playerx <= c3X + 50) &&
		(playery + 54 >= c3Y && playery <= c3Y + 54))
	{
		coinscore += 15;

		c3X = 1000;
		c3Y = rand() % 400 + 50;
           	}

	score = second + coinscore + botKillScore;
}



char sec[10000];
int minute = 0;
int hour = 0;
char hr[10000];
int totalsecond = 0;
char point[10000];


void changetimer(){
	if (easypage == 1 || mediumpage == 1 || hardpage == 1){
		totalsecond = (second++) + (60 * minute) + (60 * 60 * hour);
		score = score + second;
	}
}


void drawtimer(){
	if (easypage == 1 || mediumpage == 1 || hardpage == 1){
		
		iSetColor(20, 30, 70); 
		iFilledRectangle(820, 480, 140, 60);
		
		// Score Border
		iSetColor(100, 110, 255);
		iRectangle(820, 480, 140, 60);

		// Score Text
		iSetColor(255, 255, 255);
		iText(855, 515, "Score", GLUT_BITMAP_TIMES_ROMAN_24);

		sprintf_s(point, "%d", score);
		iText(870, 490, point, GLUT_BITMAP_TIMES_ROMAN_24);

		// Lives Box
		iSetColor(20, 30, 70); 
		iFilledRectangle(820, 410, 140, 50);
		iSetColor(100, 110, 255);
		iRectangle(820, 410, 140, 50);

		iSetColor(255, 255, 255);
		iText(855, 435, "Lives", GLUT_BITMAP_TIMES_ROMAN_24);
		
		char lStr[20];
		sprintf_s(lStr, "%d", lives);
		iText(880, 415, lStr, GLUT_BITMAP_TIMES_ROMAN_24);
	}
}



void iDraw()
{
	iClear();
	if (homepage == 1){
		drawHomepage();
	}
	else if (startpage == 1){
		drawstartpage();
	}
	else if (aboutpage == 1){
		drawAboutPage();
	}
	else if (instructionpage == 1){
		drawInstructionPage();
	}
	else if (scorepage == 1){
		drawScorePage();
	}
	else if (easypage == 1){
		draweasypage();
	}
	else if (mediumpage == 1){
		drawmediumpage();
	}
	else if (hardpage == 1){
		drawhardpage();
	}
	else if (pausepage == 1){
		drawpausepage();
	}
	else if (gameoverpage == 1){
		drawgameoverpage();
	}
	else if (nameentrypage == 1){
		drawNameEntryPage();
	}

}

void iKeyboard(unsigned char key)
{
	// Debug: You can remove these checks once verified
	if (nameentrypage == 1 && nameFocus == 1) {
		if (key == '\r') {
			// Enter key: Start game if name is not empty
			if (nameIndex > 0) {
				nameentrypage = 0;
				nameFocus = 0;
				if (easybuttonclick == 1) easybuttonclickhandler();
				else if (mediumbuttonclick == 1) mediumbuttonclickhandler();
				else if (hardbuttonclick == 1) hardbuttonclickhandler();
			}
		}
		else if (key == '\b') {
			// Backspace
			if (nameIndex > 0) {
				nameIndex--;
				playerName[nameIndex] = '\0';
			}
		}
		else if (nameIndex < 49 && key >= 32 && key <= 126) {
			// Capture all printable characters
			playerName[nameIndex] = key;
			nameIndex++;
			playerName[nameIndex] = '\0';
		}
	}
}

void iSpecialKeyboard(unsigned char key)
{
	if(key == GLUT_KEY_END)
	{
		exit(0);
	}
}
void iMouseMove(int mx, int my)
{
	
}

void iPassiveMouseMove(int mx, int my)
{
	
}

void iMouse(int button, int state, int mx, int my)
{
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		printf("x=%d  y=%d  ",mx, my);
		if (homepage == 1 && (mx > 363 && mx<636) && (my>171 && my < 210)){
			aboutbuttonclickhandler();
		}
		else if (homepage == 1 && (mx> 362 && mx< 635) && (my>310 && my <355)){
			startbuttonclickhandler();
		}

		else if (homepage == 1 && (mx > 360 && mx<640) && (my>240 && my < 282)){
			instructionbuttonclickhandler();
		}

		else if (homepage == 1 && (mx>360 && mx<640) && (my >90 && my <140 )){
			loadScores();
			scorebuttonclickhandler();
		}
		else if ((startpage == 1 || aboutpage == 1 || instructionpage == 1 || scorepage == 1)
			&& (mx > 6 && mx < 71) && (my > 10 && my < 78))
		{
			backbuttonclickhandler();
		}

		// Pause button during gameplay -> open pause page
		else if ((easypage == 1 || mediumpage == 1 || hardpage == 1) 
			&& (mx > 6 && mx < 71) && (my > 10 && my < 75))
		{
			pausebuttonclickhandler();
		}

		// Pause page: Start (Resume) button
		else if (pausepage == 1 && (mx >= 300 && mx <= 725) && (my >= 230 && my <= 296))
		{
			pausebuttonclickhandler2(); // resume game
		}

		// Pause page: Back button (return to homepage)
		else if (pausepage == 1 && (mx >= 300 && mx <= 725) && (my >= 140 && my <= 200))
		{
			homepage = 1;
			startpage = 0;
			aboutpage = 0;
			instructionpage = 0;
			scorepage = 0;
			easypage = 0;
			pausepage = 0;
		}

		else if (gameoverpage == 1) {
			// Retry button
			if (mx >= 240 && mx <= 430 && my >= 200 && my <= 280) {
				gameoverpage = 0;
				if (easybuttonclick == 1) easybuttonclickhandler();
				else if (mediumbuttonclick == 1) mediumbuttonclickhandler();
				else if (hardbuttonclick == 1) hardbuttonclickhandler();
				else easybuttonclickhandler();
			}
			// Home button
			else if (mx >= 560 && mx <= 750 && my >= 200 && my <= 280) {
				gameoverpage = 0;
				homepage = 1;
			}
		}

		else if (startpage == 1 && (mx>55 && mx<325) && (my>200 && my < 440)){
			easybuttonclick = 1; mediumbuttonclick = 0; hardbuttonclick = 0;
			nameentrypage = 1; startpage = 0;
			nameIndex = 0; playerName[0] = '\0'; nameFocus = 1; // Auto-focus when entering
		}
		else if (startpage == 1 && (mx>360 && mx<635) && (my>200 && my < 440)){
			easybuttonclick = 0; mediumbuttonclick = 1; hardbuttonclick = 0;
			nameentrypage = 1; startpage = 0;
			nameIndex = 0; playerName[0] = '\0'; nameFocus = 1;
		}
		else if (startpage == 1 && (mx>673 && mx<946) && (my>200 && my < 440)){
			easybuttonclick = 0; mediumbuttonclick = 0; hardbuttonclick = 1;
			nameentrypage = 1; startpage = 0;
			nameIndex = 0; playerName[0] = '\0'; nameFocus = 1;
		}

		else if (nameentrypage == 1 && (mx > 300 && mx < 700) && (my > 350 && my < 400)) {
			nameFocus = 1;
		}
		else if (nameentrypage == 1 && (mx > 400 && mx < 600) && (my > 150 && my < 220)) {
			nameFocus = 0;
			nameentrypage = 0;
			if (easybuttonclick == 1) easybuttonclickhandler();
			else if (mediumbuttonclick == 1) mediumbuttonclickhandler();
			else if (hardbuttonclick == 1) hardbuttonclickhandler();
		}
		else if (nameentrypage == 1) {
			nameFocus = 0;
		}
		
	}
	
	
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		
	}
}

// Special Keys:
// GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6, GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12, 
// GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP, GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT

void fixedUpdate()
{
	if (isKeyPressed('w') || isSpecialKeyPressed(GLUT_KEY_UP))
	{
		playery = playery + 10;
		if (playery > 510){
			playery = 510;
		}
	}
	if (isKeyPressed('a') || isSpecialKeyPressed(GLUT_KEY_LEFT))
	{
		playerx = playerx - 15;
		if (playerx < 260){
			playerx = 260;
		}
	}
	if (isKeyPressed('s') || isSpecialKeyPressed(GLUT_KEY_DOWN))
	{
		playery = playery - 10;
		if (playery < 15){
			playery = 15;
		}
	}
	if (isKeyPressed('d') || isSpecialKeyPressed(GLUT_KEY_RIGHT))
	{
		playerx = playerx + 15;
		if (playerx > 670){
			playerx = 670;
		}
	}

	if (isKeyPressed(' ') || isKeyPressed('\r')) {
		// Spawn a bullet or missile
		for (int i = 0; i < MAX_BULLETS; i++) {
			if (!bullets[i].active) {
				bullets[i].active = 1;
				bullets[i].x = playerx + 22; // approx center of user car
				bullets[i].y = playery + 60; // just above the user car
				
				if (missileCount > 0) {
					bullets[i].type = 1; // missile
					missileCount--;
				} else {
					bullets[i].type = 0; // normal bullet
				}
				break;
			}
		}
		// Reset keys so they don't hold-fire too fast (optional semi-auto feel)
		keyPressed[' '] = 0; 
		keyPressed['\r'] = 0; 
	}



	
}

void drawHomepage(){
	iSetColor(0, 0, 0);
	iFilledRectangle(0, 0, 1000, 600);
	iShowBMP2(0, 0, "Images\\mainmenu.bmp", 0);

	

	
}

void drawstartpage(){
	iSetColor(255, 255, 255);
	iShowBMP2(0, 0, "Images\\startpage.bmp", 0);

	iShowBMP2(06, 10, "Images\\backbutton2.bmp", 0);

	/*iText(440, 500, "Select Level : ", GLUT_BITMAP_TIMES_ROMAN_24);
	// Draw Easy button
	iShowBMP2(350, 400, "Images\\easybutton.bmp", 0);
	// Draw Medium button with spacing
	iShowBMP2(350, 320, "Images\\mediumbutton.bmp", 0);
	// Draw Hard button with spacing
	iShowBMP2(350, 240, "Images\\hardbutton.bmp", 0);*/
}
void drawAboutPage(){
	iSetColor(0, 0, 0);
	iFilledRectangle(0, 0, 1000, 600);
	iShowBMP2(0, 0, "Images\\aboutpage2.bmp", 0);

	iShowBMP2(06, 10, "Images\\backbutton2.bmp", 0);

}

void drawInstructionPage(){
	iSetColor(0,0,0);
	iFilledRectangle(0, 0, 1000, 600);
	iShowBMP2(0, 0, "Images\\instructionpage.bmp", 0);

	iShowBMP2(06, 10, "Images\\backbutton2.bmp", 0);
}

void drawScorePage(){
	iSetColor(20, 30, 70);
	iFilledRectangle(0, 0, 1000, 600);

	iSetColor(255, 255, 255);
	iText(420, 520, "HIGH SCORES", GLUT_BITMAP_TIMES_ROMAN_24);

	iSetColor(100, 110, 255);
	iRectangle(250, 100, 500, 400);

	iSetColor(255, 255, 255);
	for (int i = 0; i < 5; i++) {
		char scoreStr[100];
		sprintf_s(scoreStr, "%d. %-20s %d", i + 1, topScores[i].name, topScores[i].score);
		iText(300, 430 - (i * 60), scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
	}

	iShowBMP2(06, 10, "Images\\backbutton2.bmp", 0);
}

void drawgamepage(float speedMultiplier){
	// Use appropriate bg based on level
	if (mediumpage == 1)
		rendering2();
	else if (hardpage == 1)
		rendering3();
	else
		rendering();

	// Pause button during gameplay
	iShowBMP2(6, 10, "Images\\pausebutton.bmp", 0);

	iShowBMP2(c1X, c1Y, "Images\\c1.bmp", 0);
	iShowBMP2(c2X, c2Y, "Images\\c2.bmp", 0);
	iShowBMP2(c3X, c3Y, "Images\\c3.bmp", 0);

	iShowBMP2(playerx, playery, "Images\\usercar.bmp", 0);
	iShowBMP2(x, y, "Images\\v1.bmp", 0);
	iShowBMP2(a, b, "Images\\v2.bmp", 0);
	iShowBMP2(x3, y3, "Images\\v3.bmp", 0);

	y = y - (1.5 * speedMultiplier);
	if (y <= 0){
		y = 601;
		x = 300 + rand() % 340; // Road boundary 300-640
		while (blockadeActive && x + 50 > blockadeX && x < blockadeX + 100) {
			x = 300 + rand() % 340; 
		}
		v1Health = (mediumpage == 1 || hardpage == 1) ? 5 : 2;
	}
	collision();

	b = b - (2 * speedMultiplier);
	if (b <= 0){
		b = 601;
		a = 300 + rand() % 340; // Road boundary 300-640
		while (blockadeActive && a + 50 > blockadeX && a < blockadeX + 100) {
			a = 300 + rand() % 340;
		}
		v2Health = (mediumpage == 1 || hardpage == 1) ? 5 : 2;
	}
	collision();

	y3 = y3 - (1.8 * speedMultiplier);
	if (y3 <= 0){
		y3 = 601;
		x3 = 300 + rand() % 340;
		v3TargetX = x3;
		while (blockadeActive && x3 + 50 > blockadeX && x3 < blockadeX + 100) {
			x3 = 300 + rand() % 340;
			v3TargetX = x3;
		}
		v3Health = (mediumpage == 1 || hardpage == 1) ? 5 : 2;
	}
	
	// Lane switching logic for V3 in Hard mode
	if (hardpage == 1) {
		v3LaneSwitchTimer++;
		if (v3LaneSwitchTimer > 150) { // Every ~1.5 seconds at 10ms timer
			v3TargetX = 300 + rand() % 340;
			v3LaneSwitchTimer = 0;
		}
		// Gradually move towards target X
		if (x3 < v3TargetX) x3 += 0.5f;
		else if (x3 > v3TargetX) x3 -= 0.5f;
	}

	collision();

	c1Y = c1Y - (7 * speedMultiplier);
	if (c1Y <= 0){
		c1Y = 600;
		c1X = 300 + rand() % 350; // Road boundary 300-650
	}

	c2Y = c2Y - (8 * speedMultiplier);
	if (c2Y <= 0){
		c2Y = 600;
		c2X = 300 + rand() % 350; // Road boundary 300-650
	}

	c3Y = c3Y - (7.5 * speedMultiplier);
	if (c3Y <= 0){
		c3Y = 600;
		c3X = 300 + rand() % 350; // Road boundary 300-650
	}
	
	// Diamond Logic
	if (mediumpage == 1 || hardpage == 1) {
		if (diamondActive == 0) {
			// Spawn chance
			if (score >= nextDiamondScore) {
				diamondActive = 1;
				diamondY = 600;
				diamondX = 300 + rand() % 350;
				
				if (hardpage == 1) {
					nextLifeScore = score + 250; // Comes much later in hard mode
					nextDiamondScore += 700; // Drops less frequently in hard mode
				} else {
					nextLifeScore = score + 120; // Comes sooner in medium mode
					nextDiamondScore += 350;
				}
			}
		} else {
			iShowBMP2(diamondX, diamondY, "Images\\diamond.bmp", 0);
			diamondY -= (5 * speedMultiplier); // Slightly slower than coins

			if (diamondY < -50) {
				diamondActive = 0;
			}

			// Collision with player (diamond: 50x50, player: 60x60 shrunk 5px)
			if ((playerx + 55 >= diamondX && playerx + 5 <= diamondX + 50) &&
				(playery + 55 >= diamondY && playery + 5 <= diamondY + 50))
			{
				missileCount += 6;
				diamondActive = 0;
			}
		}

		// Life Logic
		if (lifeActive == 0) {
			if (nextLifeScore != -1 && score >= nextLifeScore) {
				lifeActive = 1;
				lifeY = 600;
				lifeX = 300 + rand() % 350;
				nextLifeScore = -1; // Reset until next diamond sets it
			}
		} else {
			iShowBMP2(lifeX, lifeY, "Images\\life.bmp", 0);
			lifeY -= (5 * speedMultiplier); // Same speed as diamond

			if (lifeY < -50) {
				lifeActive = 0;
			}

			// Collision with player (assuming life image is roughly 50x50)
			if ((playerx + 55 >= lifeX && playerx + 5 <= lifeX + 50) &&
				(playery + 55 >= lifeY && playery + 5 <= lifeY + 50))
			{
				lives++;
				lifeActive = 0;
			}
		}
	}
	
	coincollision();

	// Update and draw bullets
	for(int i = 0; i < MAX_BULLETS; i++) {
		if(bullets[i].active) {
			if (bullets[i].type == 1) {
				iShowBMP2(bullets[i].x, bullets[i].y, "Images\\missile.bmp", 0);
			} else {
				iShowBMP2(bullets[i].x, bullets[i].y, "Images\\bullet.bmp", 0);
			}
			bullets[i].y += (15 * speedMultiplier);
			
			if(bullets[i].y > 600) {
				bullets[i].active = 0;
			}

			// check collision with v1 (v1.bmp: 40x76) missile: 60x60, bullet: 8x50
			int bulletW = bullets[i].type == 1 ? 60 : 8;
			int bulletH = bullets[i].type == 1 ? 60 : 50;

			if(bullets[i].active &&
			   bullets[i].x + bulletW >= x + 4 && bullets[i].x <= x + 36 &&
			   bullets[i].y + bulletH >= y + 4 && bullets[i].y <= y + 72) 
			{
				bullets[i].active = 0;
				v1Health = bullets[i].type == 1 ? 0 : v1Health - 1;
				if(v1Health <= 0) {
					botKillScore += 50;
					y = 601;
					x = 300 + rand() % 340;
					v1Health = (mediumpage == 1 || hardpage == 1) ? 5 : 2;
				}
			}

			// check collision with v2 (v2.bmp: 40x76)
			if(bullets[i].active &&
			   bullets[i].x + bulletW >= a + 4 && bullets[i].x <= a + 36 &&
			   bullets[i].y + bulletH >= b + 4 && bullets[i].y <= b + 72) 
			{
				bullets[i].active = 0;
				v2Health = bullets[i].type == 1 ? 0 : v2Health - 1;
				if(v2Health <= 0) {
					botKillScore += 50;
					b = 601;
					a = 300 + rand() % 340;
					v2Health = (mediumpage == 1 || hardpage == 1) ? 5 : 2;
				}
			}

			// check collision with v3 (v2.bmp: 40x76)
			if((mediumpage == 1 || hardpage == 1) && bullets[i].active &&
			   bullets[i].x + bulletW >= x3 + 4 && bullets[i].x <= x3 + 36 &&
			   bullets[i].y + bulletH >= y3 + 4 && bullets[i].y <= y3 + 72) 
			{
				bullets[i].active = 0;
				v3Health = bullets[i].type == 1 ? 0 : v3Health - 1;
				if(v3Health <= 0) {
					botKillScore += 50;
					y3 = 601;
					x3 = 300 + rand() % 340;
					v3Health = 5;
				}
			}

			// check collision with blockade
			if ((mediumpage == 1 || hardpage == 1) && bullets[i].active && blockadeActive &&
				bullets[i].x + bulletW >= blockadeX && bullets[i].x <= blockadeX + 80 &&
				bullets[i].y + bulletH >= blockadeY && bullets[i].y <= blockadeY + 17) 
			{
				bullets[i].active = 0;
				blockadeHealth = bullets[i].type == 1 ? 0 : blockadeHealth - 1;
				if(blockadeHealth <= 0) {
					botKillScore += 50; 
					blockadeActive = 0;
				}
			}

			// check collision with obs2 (obs2.bmp: 39x40)
			if (hardpage == 1 && bullets[i].active && obs2Active &&
				bullets[i].x + bulletW >= obs2X && bullets[i].x <= obs2X + 39 &&
				bullets[i].y + bulletH >= obs2Y && bullets[i].y <= obs2Y + 40) 
			{
				bullets[i].active = 0;
				obs2Health = bullets[i].type == 1 ? 0 : obs2Health - 1;
				if(obs2Health <= 0) {
					botKillScore += 50;
					obs2Active = 0;
				}
			}
		}
	}

	// Blockade Logic (Only for Medium and Hard)
	if (mediumpage == 1 || hardpage == 1) {
		if (blockadeActive == 0) {
			// Random chance to spawn a blockade
			if (rand() % 100 < 2) {
				blockadeActive = 1;
				blockadeHealth = 5;
				blockadeX = 300 + rand() % 340; // Road boundary
				blockadeY = 600;
			}
		} else {
			iShowBMP2(blockadeX, blockadeY, "Images\\blockade.bmp", 0);
			blockadeY -= (1.5 * speedMultiplier);

			if (blockadeY < -100) {
				blockadeActive = 0;
			}

			// Collision with user car (blockade.bmp: 80x17, player: 60x60 shrunk 5px)
			if ((playerx + 55 >= blockadeX + 5 && playerx + 5 <= blockadeX + 75) &&
				(playery + 55 >= blockadeY && playery + 5 <= blockadeY + 17))
			{
				lives--;
				blockadeActive = 0; // Destroy blockade so it doesn't multi-hit
				if (lives <= 0) triggerGameOver(); 
			}
		}
		
		// obs2 Logic (Only for Hard)
		if (hardpage == 1) {
			if (obs2Active == 0) {
				if (rand() % 100 < 2) {
					obs2Active = 1;
					obs2Health = 5;
					obs2X = 300 + rand() % 340;
					obs2Y = 600;
				}
			} else {
				iShowBMP2(obs2X, obs2Y, "Images\\obs2.bmp", 0);
				obs2Y -= (1.6 * speedMultiplier);

				if (obs2Y < -100) {
					obs2Active = 0;
				}

				// Collision with user car (obs2.bmp: 39x40, player: 60x60 shrunk 5px)
				if ((playerx + 55 >= obs2X + 5 && playerx + 5 <= obs2X + 34) &&
					(playery + 55 >= obs2Y && playery + 5 <= obs2Y + 40))
				{
					lives--;
					obs2Active = 0; // Destroy obs2
					if (lives <= 0) triggerGameOver(); 
				}
			}
		}

		// Police Car Chase Logic
		if (policeActive == 0) {
			policeTimer++;
			if (policeTimer > 200) { 
				policeActive = 1;
				policeY = -200; // start farther below user
				policeX = playerx; // start horizontally aligned
				policeTimer = 0;
				policeCurrentSpeed = 0.3f; // reset to slow each new chase
			}
		} else {
			iShowBMP2(policeX, policeY, "Images\\policecar.bmp", 0);
			// Gradually ramp up police speed (starts slow, accelerates to max 3.0)
			if (policeCurrentSpeed < 3.0f) policeCurrentSpeed += 0.015f;
			policeY += policeCurrentSpeed;
			
			// Chase the player horizontally while it's behind the player
			if (policeY < playery + 100) {
				if (policeX < playerx - 5) policeX += 2;
				else if (policeX > playerx + 5) policeX -= 2;
			}
			
			if (policeY > 650) {
				policeActive = 0; // successfully dodged
			}
			
			// Collision with player (policecar.bmp: 50x50, player: 60x60 shrunk 5px)
			if ((playerx + 55 >= policeX + 5 && playerx + 5 <= policeX + 45) &&
				(playery + 55 >= policeY + 5 && playery + 5 <= policeY + 45))
			{
				lives--;
				policeActive = 0;
				if (lives <= 0) triggerGameOver(); 
			}
		}
	}

	drawtimer();
}

void draweasypage(){
	drawgamepage(1.0);
}

void drawmediumpage(){
	// Increase speed every 250 score points (cap at 2.5)
	if (score >= mediumScoreMilestone && mediumSpeedMultiplier < 2.5f) {
		mediumSpeedMultiplier += 0.15f;
		mediumScoreMilestone  += 250;
	}
	drawgamepage(mediumSpeedMultiplier);
}

void drawhardpage(){
	// Increase speed every 200 score points (cap at 3.5)
	if (score >= hardScoreMilestone && hardSpeedMultiplier < 3.5f) {
		hardSpeedMultiplier += 0.20f;
		hardScoreMilestone  += 200;
	}
	drawgamepage(hardSpeedMultiplier);
}

void drawgameoverpage(){
	iClear();
	iShowBMP2(0, 0, "Images\\gameover.bmp", 0);
	
	// Draw Score Box similar to timer page
	iSetColor(20, 30, 70); 
	iFilledRectangle(430, 300, 140, 60);
	
	iSetColor(100, 110, 255);
	iRectangle(430, 300, 140, 60);

	iSetColor(255, 255, 255);
	iText(465, 335, "Score", GLUT_BITMAP_TIMES_ROMAN_24);

	char scoreStr[30];
	sprintf_s(scoreStr, "%d", score);
	iText(480, 310, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
}

void drawNameEntryPage() {
	iClear();
	iSetColor(20, 30, 70);
	iFilledRectangle(0, 0, 1000, 600);

	iSetColor(255, 255, 255);
	iText(380, 450, "ENTER YOUR NAME", GLUT_BITMAP_TIMES_ROMAN_24);

	if (nameFocus == 1) {
		iSetColor(150, 200, 255); // Highlight color when focused
	} else {
		iSetColor(100, 110, 255);
	}
	iRectangle(300, 350, 400, 50);

	iSetColor(255, 255, 255);
	char displayName[60];
	if (nameFocus == 1) {
		sprintf_s(displayName, "%s|", playerName);
	} else {
		strcpy_s(displayName, sizeof(displayName), playerName);
	}
	iText(320, 365, displayName, GLUT_BITMAP_TIMES_ROMAN_24);

	// Start Button
	iSetColor(0, 200, 0);
	iFilledRectangle(400, 150, 200, 70);
	iSetColor(255, 255, 255);
	iText(460, 175, "START", GLUT_BITMAP_TIMES_ROMAN_24);


}

void drawpausepage(){
	iClear();
	iShowBMP2(0, 0, "Images\\pause.bmp", 0);
}

void startbuttonclickhandler(){
	homepage = 0;
	startpage = 1;
}

void aboutbuttonclickhandler(){
	homepage = 0;
	startpage = 0;
	aboutpage = 1;
}

void instructionbuttonclickhandler(){
	homepage = 0;
	startpage = 0;
	aboutpage = 0;
	instructionpage = 1;
}

void scorebuttonclickhandler(){
	homepage = 0;
	startpage = 0;
	aboutpage = 0;
	instructionpage = 0;
	scorepage = 1;
}

void backbuttonclickhandler(){
	homepage = 1;
	startpage = 0;
	aboutpage = 0;
	instructionpage = 0;
	scorepage = 0;
}

void easybuttonclickhandler(){
	homepage = 0; startpage = 0; aboutpage = 0; instructionpage = 0; scorepage = 0; 
	easypage = 1; mediumpage = 0; hardpage = 0;
	easybuttonclick = 1; mediumbuttonclick = 0; hardbuttonclick = 0;
	score = 0; coinscore = 0; second = 0; botKillScore = 0; nextDiamondScore = 350; diamondActive = 0; lifeActive = 0; nextLifeScore = -1;
	v1Health = 2; v2Health = 2; v3Health = 2;
	x = 400; y = 601; a = 600; b = 800; x3 = 500; y3 = 1000;
	// Do not reset playerName here so user doesn't have to retype if retrying
	lives = 4;
}

void mediumbuttonclickhandler(){
	homepage = 0; startpage = 0; aboutpage = 0; instructionpage = 0; scorepage = 0;
	easypage = 0; mediumpage = 1; hardpage = 0;
	easybuttonclick = 0; mediumbuttonclick = 1; hardbuttonclick = 0;
	score = 0; coinscore = 0; second = 0; botKillScore = 0; nextDiamondScore = 350; diamondActive = 0; lifeActive = 0; nextLifeScore = -1;
	v1Health = 5; v2Health = 5; v3Health = 5;
	x = 400; y = 601; a = 600; b = 800; x3 = 500; y3 = 1000;
	// Reset speed ramp for a fresh game
	mediumSpeedMultiplier = 1.0f;
	mediumScoreMilestone  = 250;
	lives = 3;
}

void hardbuttonclickhandler(){
	homepage = 0; startpage = 0; aboutpage = 0; instructionpage = 0; scorepage = 0;
	easypage = 0; mediumpage = 0; hardpage = 1;
	easybuttonclick = 0; mediumbuttonclick = 0; hardbuttonclick = 1;
	score = 0; coinscore = 0; second = 0; botKillScore = 0; nextDiamondScore = 700; diamondActive = 0; lifeActive = 0; nextLifeScore = -1;
	v1Health = 5; v2Health = 5; v3Health = 5;
	x = 400; y = 601; a = 600; b = 800; x3 = 500; y3 = 1000;
	v3TargetX = 500; v3LaneSwitchTimer = 0;
	// Reset speed ramp for a fresh game
	hardSpeedMultiplier = 2.0f;
	hardScoreMilestone  = 200;
	lives = 2;
}

void pausebuttonclickhandler(){
	homepage = 0; startpage = 0; aboutpage = 0; instructionpage = 0; scorepage = 0;
	easypage = 0; mediumpage = 0; hardpage = 0;
	pausepage = 1;
}

void pausebuttonclickhandler2(){
	homepage = 0; startpage = 0; aboutpage = 0; instructionpage = 0; scorepage = 0;
	pausepage = 0;
	if (easybuttonclick == 1) easypage = 1;
	else if (mediumbuttonclick == 1) mediumpage = 1;
	else if (hardbuttonclick == 1) hardpage = 1;
	else easypage = 1; 
}



int main()
{
	// Opening/Loading the audio files
	mciSendString("open \"Audios//background.mp3\" alias bgsong", NULL, 0, NULL);
	mciSendString("open \"Audios//gameover.mp3\" alias ggsong", NULL, 0, NULL);

	// Playing the background audio on repeat
	mciSendString(" play bgsong repeat", NULL, 0, NULL);

	// If the use of an audio is finished, close it to free memory
	// mciSendString("close bgsong", NULL, 0, NULL);
	// mciSendString("close ggsong", NULL, 0, NULL);

	initializeimageposition();
	initializebg2imageposition();
	initializebg3imageposition();
	iSetTimer(10, movebackground);
	iSetTimer(10, movebg2background);  // same scroll speed as easy page
	iSetTimer(10, movebg3background);
	iSetTimer(1000, changetimer);

	loadScores();


	iInitialize(1000, 600, "The Highway Pursuit");
	iStart();
	return 0;
}