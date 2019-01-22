#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include "Doge_SDL.h"

#define SQUARESIZE 32
#define GRIDXLEN (WINDOWXLEN / SQUARESIZE)
#define GRIDYLEN (WINDOWYLEN / SQUARESIZE)

enum Color{BLACK, WHITE, RED, GREEN, BLUE};

enum Color grid[GRIDXLEN][GRIDYLEN] = {};

enum Direction{dirU, dirD, dirL, dirR};

typedef struct{
	uint xind, yind;
	enum Direction dir;
}Ant;

Ant ant = {GRIDXLEN/2, GRIDYLEN/2, dirU};

uint wrapIndX(int ind)
{
	if(ind<0)
	{
		return GRIDXLEN+ind;
	}
	return ind >= GRIDXLEN? ind%GRIDXLEN : ind;
}

uint wrapIndY(int ind)
{
	if(ind<0)
	{
		return GRIDYLEN+ind;
	}
	return ind >= GRIDYLEN? ind%GRIDYLEN : ind;
}

uint indPos(uint ind)
{
	return ind * SQUARESIZE;
}

void drawGridIndex(uint xindex, uint yindex)
{
	uint xpos = indPos(xindex);
	uint ypos = indPos(yindex);
	setColor(50,50,50);
	fillRect(xpos, ypos, SQUARESIZE, SQUARESIZE);
	switch (grid[xindex][yindex]) {
		case BLACK:
			setColor(0,0,0);
			break;
		case WHITE:
			setColor(255,255,255);
			break;
		case RED:
			setColor(255,0,0);
			break;
		case GREEN:
			setColor(0,255,0);
			break;
		case BLUE:
			setColor(0,0,255);
			break;
		default:
			setColor(255,55,25);
			break;
	}
	fillRect(xpos+4, ypos+4, SQUARESIZE-8, SQUARESIZE-8);
}

void drawGrid()
{
	for(uint y = 0; y < GRIDYLEN; y++){
		for(uint x = 0; x < GRIDXLEN; x++){
			printf("(%d,%d)", x, y);
			drawGridIndex(x, y);
		}
	}
	drawFrame();
}

void drawAntBody(){
	setColor(255,55,25);
	if(ant.dir==dirU){
		drawVLine(indPos(ant.xind)+SQUARESIZE/2, indPos(ant.yind),
		SQUARESIZE/2);
	}
	else if(ant.dir==dirD){
		drawVLine(indPos(ant.xind)+SQUARESIZE/2, indPos(ant.yind)+SQUARESIZE/2-1,
		SQUARESIZE/2);
	}
	else if(ant.dir==dirL){
		drawHLine(indPos(ant.xind), indPos(ant.yind)+SQUARESIZE/2,
		SQUARESIZE/2);
	}
	else if(ant.dir==dirR){
		drawHLine(indPos(ant.xind)+SQUARESIZE/2-1, indPos(ant.yind)+SQUARESIZE/2,
		SQUARESIZE/2);
	}
}

void drawAnt()
{
	drawAntBody();
}

void rotate()
{
	enum Color c = grid[ant.xind][ant.yind];

	if(c==WHITE || c==RED){
		switch (ant.dir){
			case dirU:
				ant.dir=dirL;
				break;
			case dirL:
				ant.dir=dirD;
				break;
			case dirD:
				ant.dir=dirR;
				break;
			case dirR:
				ant.dir = dirU;
				break;
			default:
				break;
		}
	}
	else{// if(c==BLACK || c==BLUE || c==GREEN)
		switch (ant.dir){
			case dirU:
				ant.dir=dirR;
				break;
			case dirL:
				ant.dir=dirU;
				break;
			case dirD:
				ant.dir=dirL;
				break;
			case dirR:
				ant.dir = dirD;
				break;
			default:
				break;
		}
	}
}

void move()
{
	switch (ant.dir) {
		case dirU:
			ant.yind = wrapIndY(ant.yind+1);
			break;
		case dirD:
			ant.yind = wrapIndY(ant.yind-1);;
			break;
		case dirR:
			ant.xind = wrapIndX(ant.xind+1);
			break;
		case dirL:
			ant.xind = wrapIndX(ant.xind-1);;
			break;
	}
}

void changeColor()
{
	switch(grid[ant.xind][ant.yind]){
		case WHITE:
			grid[ant.xind][ant.yind]=BLACK;
			break;
		case BLACK:
			grid[ant.xind][ant.yind]=RED;
			break;
		case RED:
			grid[ant.xind][ant.yind]=GREEN;
			break;
		case GREEN:
			grid[ant.xind][ant.yind]=BLUE;
			break;
		case BLUE:
			grid[ant.xind][ant.yind]=WHITE;
			break;
	}
}

void step()
{
	rotate();
	drawAnt();
	drawFrame();
	//delay(100);
	changeColor();
	drawGridIndex(ant.xind,ant.yind);
	move();
	drawFrame();
}

int main()
{
	init();
	while(1){
		step();
		events();
	}
	quit(0);
	return 0;
}
