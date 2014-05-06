/*==========================================
HW9 mylib.c
Hamilton Greene
============================================*/

#include "mylib.h"

//Character sprites
#include "jonsnow1.h"

u16 *videoBuffer = ((u16 *)0x6000000);
u16 __key_curr = 0, __key_prev=0;

//Sets a pixel
void setPixel(int y, int x, u16 color)
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	videoBuffer[OFFSET(y, x, 240)] = color;
}

//Draws a filled-in rectangle
void drawRect(int y, int x, int height, int width, u16 color)
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	int r,c;
	for(r=0; r<height; r++)
	{
		for(c=0; c<width; c++)
		{
			setPixel(y+r, x+c, color);
		}
	}
}	

//Draws rectangle using DMA and standard setPixel - Practice for CS 2110 final
void drawBorderedRect(int row, int col, int height, int width, u16 borderColor, u16 fillColor)
{
	int i;
	for(i = 1; i<height -1; i++)
	{
		DMA[3].src = &fillColor;
		DMA[3].dst = &videoBuffer[OFFSET(row+i, col+1, 240)];
		DMA[3].cnt = (width - 1) | DMA_ON | DMA_SOURCE_FIXED;

	}

	//Draw top border
	DMA[3].src = &borderColor;
	DMA[3].dst = &videoBuffer[OFFSET(row, col, 240)];
	DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;

	//Draw bottom border
	DMA[3].dst = &videoBuffer[OFFSET(row+height-1, col, 240)];
	DMA[3].cnt = width |DMA_ON | DMA_SOURCE_FIXED;

	//Draw side borders
	int x, y;
	for(int x = 0; x < width; x++)
	{
		for(int y = 1; y<height - 1; y++)
		{
				if(x == 0)
				{
					setPixel(row+y,col+x,borderColor);
				}
				else if(x==(width - 1))
				{
					setPixel(row+y,col+x,borderColor);
				}
		}
	}
}

//Draw rectangle using DMA
void drawRectDMA(int y, int x, int height, int width, volatile unsigned short color)
{
	int i;
	for(i = 0; i< height; i++)
	{
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(y+i, x, 240)];
		DMA[3].cnt = width | DMA_ON |DMA_SOURCE_FIXED;
	}
}

//Fills the screen with the given color
void fillScreen(volatile unsigned short color)
{
	drawRectDMA(0,0,160,240, color);
}

//Draws a hollow rectangle
void drawHollowRect(int y, int x, int width, int height, u16 color)
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	int r,c;
	for(r=0; r<height; r++)
	{
		for(c=0; c<width; c++)
		{
			if(r==0 || r == (height - 1))
			{
				setPixel(y+r,x+c, color);
			} 
			else if (c==0 || c == (width - 1))
			{
				setPixel(y+r,x+c, color);
			}
		}
	}
}

/* drawimage3
A function that will draw an arbitrary sized image
onto the screen (with DMA).
@param r row to draw the image 
@param c column to draw the image
@param width width of the image
@param height height of the image
@param image Pointer to the first element of the image.
*/
void drawImage3(int y, int x, int width, int height, const u16* image)
{
	for(int i = 0; i< height; i++)
	{
		DMA[3].src = &image[OFFSET(i,0,width)];
		DMA[3].dst = &videoBuffer[OFFSET(y+i, x, 240)];
		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_INCREMENT;
	}
}

//=========================Text============================
void drawChar(int row, int col, unsigned char ch, unsigned short color)
{
	int r,c;
	for(r=0; r<8; r++)
	{
		for(c=0; c<6; c++)
		{
			if(fontdata_6x8[OFFSET(r, c, 6) + ch*48])
			{
				setPixel(r+row, c+col, color);
			}
		}
	}
}

void drawString(int row, int col, char *str, unsigned short color)
{
	while(*str)
	{
		drawChar(row, col, *str++, color);
		col += 6;
	}
}

//Graphics
void waitForVblank()
{
	while(SCANLINECOUNTER >160);
	while(SCANLINECOUNTER < 160);
}