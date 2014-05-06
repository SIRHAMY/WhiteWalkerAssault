/*====================================================
HW 9 Main.c
Hamilton Greene
========================================================*/
#include <stdlib.h>
#include <stdio.h>
#include "mylib.h"

//Character sprites
#include "jonsnow1.h"
#include "jonsnow2.h"
#include "ghost.h"
#include "walker.h"
#include "walker1.h"
#include "icewall.h"

#define OBJNUM 6

int start();
void instr();
int play();
void win();
void lose();
void refreshOptions(volatile unsigned short);
void gameInit();
void move();
void clear();
void clearPlayer();
void drawPlayer();
void summonGhost();
void summonWalker();
void drawWall();

enum{START, INSTR, PLAY, WIN, LOSE};

int phase;
int counter;
int kills;


JON jon;
FIRE fire[OBJNUM];
ENEMY enemy[OBJNUM];

int main()
{
	phase = START;
	while(1)
	{
		switch(phase)
		{
		case START:
			phase = start();
			break;
		case INSTR:
			instr();
			phase = START;
			break;
		case PLAY:
			phase = play();
			break;
		case WIN:
			win();
			phase = START;
			break;
		case LOSE:
			lose();
			phase = START;
			break;
		}
	}
	return(0);
}

/*
waitForVblank();
REG_DISPCTL = MODE3 | BG2_ENABLE;
drawString(50,50,"Back b Start!",CLR_YELLOW);
*/

int start()
{
	waitForVblank();
	//drawRect(0,0,160,240,CLR_WHITE);
	drawRectDMA(0,0,160,240,CLR_WHITE);
	REG_DISPCTL = MODE3 | BG2_ENABLE;

	//Draw Jon Snow on the screen
	drawImage3(80, 20, 60, 60, jonsnow2);

	//Draw text on the screen
	drawString(20,60, "Welcome to ", CLR_BLACK);
	drawString(20, 125, "The Wall!", CLR_RED);
	drawString(40, 55, "Take the Black (START)", CLR_BLACK);
	drawString(60, 62, "Learn the Oaths (B)", CLR_BLUE);
	drawString(100, 140, "Created by", CLR_BLACK);
	drawString(120, 140, "Hamilton Greene", CLR_BLACK);
	
	//Practice for final
	//drawBorderedRect(50, 50, 45, 100, CLR_RED, CLR_BLACK);


	//Decide which screen to switch to
	while(!KEY_DOWN_NOW(KEY_START) && !KEY_DOWN_NOW(KEY_B));
	if(KEY_DOWN_NOW(KEY_B))
	{
		while(KEY_DOWN_NOW(KEY_B));
		return INSTR;
	}
	else
	{
	while(KEY_DOWN_NOW(KEY_START));
	return PLAY;
	}
}

void instr()
{
	waitForVblank();
	fillScreen(CLR_WHITE);

	//Draw Walker on screen
	drawImage3(100, 160, 60, 60, walker1);
	
	//Write text
	drawString(20, 20, "Keep the ", CLR_BLACK);
	drawString(20, 80, "White Walkers", CLR_RED);
	drawString(40,20,"from reaching the", CLR_BLACK);
	drawString(40, 125, "Wall", CLR_BLUE);
	drawString(60,20,"and assaulting the", CLR_BLACK);
	drawString(80, 20, "Seven Kingdoms!", CLR_YELLOW);
	drawString(100, 20, "Up/Down to move", CLR_BLACK);
	drawString(120, 20, "A/(z) to shoot", CLR_BLACK);
	drawString(140,25, "(Press Start)", CLR_MAGENTA);



	while(!KEY_DOWN_NOW(KEY_START));
	while(KEY_DOWN_NOW(KEY_START));
}

int play()
{
	fillScreen(CLR_WHITE);
	gameInit();
	counter = 0;
	drawPlayer();

	while(1)
	{
		//waitForVblank();
		//Handles randoms
		counter++;
		if(counter >= 150)
		{
			counter = 0;
		}

		if(jon.lives <= 0)
		{
			return LOSE;
		}

		if(jon.score >= 50)
		{
			return WIN;
		}

		//jon.score = kills / 5;

		if(KEY_DOWN_NOW(KEY_UP))
		{
			clearPlayer();
			jon.y = jon.y - 2;
			drawPlayer();
			//while(KEY_DOWN_NOW(KEY_UP));
		}
		else if (KEY_DOWN_NOW(KEY_DOWN))
		{
			clearPlayer();
			jon.y = jon.y + 2;
			drawPlayer();
			//while(KEY_DOWN_NOW(KEY_DOWN));
		}
		else if(KEY_DOWN_NOW(KEY_A))
		{
			summonGhost();
			//while(KEY_DOWN_NOW(KEY_A));
		}

		if(KEY_DOWN_NOW(KEY_SELECT))
		{
			return START;
		}
		
		summonWalker();
		drawWall();
		
		move();
	}
	return 0;
}

void win()
{
	fillScreen(CLR_WHITE);
	//gameInit();
	drawString(50,20,"You've successfully defended", CLR_BLACK);
	drawString(70,25, "the realm!", CLR_BLACK);
	drawString(100, 25, "(Press Start)", CLR_MAGENTA);
	while(!KEY_DOWN_NOW(KEY_START));
	while(KEY_DOWN_NOW(KEY_START));
}

void lose()
{
	fillScreen(CLR_WHITE);
	//gameInit();
	drawString(50,20,"Oh no, the ", CLR_BLACK);
	drawString(50, 85, "White Walkers", CLR_RED);
	drawString(70, 20, "have breached the wall!", CLR_BLACK);
	drawString(100, 25, "(Press Start)", CLR_MAGENTA);
	while(!KEY_DOWN_NOW(KEY_START));
	while(KEY_DOWN_NOW(KEY_START));

}

//Refresh option pane
void refreshOptions(volatile unsigned short color)
{
	//drawRectDMA(145, 0, 15, 240, color);
	//drawString(145, 5, score, CLR_WHITE);
}

//Initialize all game pieces
void gameInit()
{
	jon.score = 0;
	jon.lives = 5;
	jon.strength = 1;
	jon.x = 4;
	jon.y = 80;

	int f;
	for(f = 0; f<OBJNUM; f++)
	{
		fire[f].state = 0;
		enemy[f].state = 0;
	}
}

//Move all characters, check for collisions, draw people
void move()
{
	waitForVblank();
	int i;
	for(i=0; i< OBJNUM; i++)
	{
		if(enemy[i].state == 1)
		{		
			//waitForVblank();
			drawRectDMA(enemy[i].y,enemy[i].x, 10,10, CLR_WHITE);

			enemy[i].x-=enemy[i].speed;

			drawImage3(enemy[i].y, enemy[i].x, 10, 10, walker);

			if(enemy[i].x < 30)
			{
				jon.lives--;
				enemy[i].state = 0;
				drawRectDMA(enemy[i].y,enemy[i].x, 10, 10, CLR_WHITE);
				summonWalker();
			}
		}
	}

	//Move ghost, check for enemy collisions
	int f;
	for(f = 0; f < OBJNUM; f++)
	{
		if(fire[f].state == 1)
		{
			drawRectDMA(fire[f].y, fire[f].x, 10, 10, CLR_WHITE);
			fire[f].x+=fire[f].speed;
			int w;
			for(w = 0;w<OBJNUM;w++)
			{
				if(enemy[w].state == 1)
				{
					if(((fire[f].y) > (enemy[w].y - 10)) && (fire[f].y< (enemy[w].y + 10)))
					{
						if((fire[f].x > (enemy[w].x - 10)) && (fire[f].x < (enemy[w].x + 10)) )
						{
							enemy[w].state = 0;
							drawRectDMA(enemy[w].y, enemy[w].x, 10, 10, CLR_WHITE);
							fire[f].state = 0;
							drawRectDMA(fire[f].y, fire[f].x, 10, 10, CLR_WHITE);
							kills++;
							jon.score++;
						}
					}
				}
			}
			if(fire[f].x >= 230)
			{
				fire[f].state = 0;
				//waitForVblank();
				drawRectDMA(fire[f].y, fire[f].x, 10, 10, CLR_WHITE);
			}
		}
		if(fire[f].state == 1)
		{
			drawImage3(fire[f].y, fire[f].x, 10, 10, ghost);
		}
	}

}

//Clears sprites at old positions
void clear()
{

}

//Clears player's sprite
void clearPlayer()
{
	drawRectDMA(jon.y, jon.x, 20, 20, CLR_WHITE);
}

void drawPlayer()
{
	if(jon.y<0)
	{
		jon.y = 0;
	}
	else if(jon.y > 140)
	{
		jon.y = 140;
	}
	drawImage3(jon.y, jon.x, 20, 20, jonsnow1);
}

//Summon ghost to attack the White Walkers
void summonGhost()
{
	int f;
	for(f = 0; f<OBJNUM;f++)
	{
		if(fire[f].state == 0)
		{
			fire[f].state = 1;
			fire[f].x = 20;
			fire[f].y = jon.y + 5;
			fire[f].strength = jon.strength;
			fire[f].speed = 2;

			//Delete this line in final
			drawImage3(fire[f].y, fire[f].x,10,10,ghost);
			break;
		}
	}
}

//Summon a walker to assault the Seven Kingdoms
void summonWalker()
{
	int w;
	int enemyCount;
	enemyCount = 3;

	//Determines number of enemies at one time
	int count;
	count = 0;
	for(w = 0; w < enemyCount; w++)
	{
		if(enemy[w].state == 0)
		{
			enemy[w].state = 1;
			enemy[w].health = 1;
			/*if(jon.score < 3)
			{
				enemy[w].health = 1;
			}*/

			enemy[w].speed = 1;
			//enemy[w].strength = jon.score / 5;
			enemy[w].strength = 1;
			enemy[w].x = 230;
			if((jon.score % 2) == 0)
			{
				enemy[w].y = (w + 1) *50; //%count;  //% 150;
				counter += 13;
			}
			else
			{
				counter += 50;
				enemy[w].y = jon.y % counter;
			}
		}
		count++;
		//if(count >= (jon.score/2 + 1)) break;
	}

}

//Draw the Wall - the only thing seperating humanity from the White Walkers
void drawWall()
{
	
	drawImage3(0,20,10, 160, icewall);
	if(jon.lives == 5)
	{
		//drawImage3(0,20,10, 160, icewall);
		drawRectDMA(150,230,10,10, CLR_WHITE);
		drawString(150,230, "5", CLR_BLACK);
	}
	else if (jon.lives == 4)
	{
		//drawImage3(0,20,8, 160, icewall);
		drawRectDMA(150,230,10,10, CLR_WHITE);
		drawString(150,230, "4", CLR_BLACK);
	}
	else if(jon.lives == 3)
	{
		//drawImage3(0,20,6, 160, icewall);
		drawRectDMA(150,230,10,10, CLR_WHITE);
		drawString(150,230, "3", CLR_BLACK);
	}
	else if (jon.lives == 2)
	{
		//drawImage3(0,20,4, 160, icewall);
		drawRectDMA(150,230,10,10, CLR_WHITE);
		drawString(150,230, "2", CLR_BLACK);
	}
	else if(jon.lives == 1)
	{
		//drawImage3(0,20,2, 160, icewall);
		drawRectDMA(150,230,10,10, CLR_WHITE);
		drawString(150,230, "1", CLR_BLACK);
	}
}