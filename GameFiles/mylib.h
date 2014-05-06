/*==============================================
HW9 mylib.h
Hamilton Greene
================================================*/

//=================def and vars==================

typedef unsigned short u16;
typedef unsigned int u32;

extern const unsigned char fontdata_6x8[12288];
extern unsigned short *videoBuffer;

//GBA Colors
#define CLR_BLACK   0x0000
#define CLR_RED     0x001F
#define CLR_LIME    0x03E0
#define CLR_YELLOW  0x03FF
#define CLR_BLUE    0x7C00
#define CLR_MAGENTA 0x7C1F
#define CLR_CYAN    0x7FE0
#define CLR_WHITE   0x7FFF

//GBA Screen
#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)

//Helper Macros
#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006
#define OFFSET(r,c,numcols) ((r)*(numcols)+(c))
#define RGB(R, G, B) (((B)<<10) + ((G)<<5) + (R));


//GBA Input
#define KEY_A        0x0001
#define KEY_B        0x0002
#define KEY_SELECT   0x0004
#define KEY_START    0x0008
#define KEY_RIGHT    0x0010
#define KEY_LEFT     0x0020
#define KEY_UP       0x0040
#define KEY_DOWN     0x0080
#define KEY_R        0x0100
#define KEY_L        0x0200

//Input state checks
#define KEY_DOWN_NOW(key)  (~(REG_KEYINPUT) & key)
#define REG_KEYINPUT *(volatile u16*)0x4000130

//DMA
#define DMA ((volatile DMAREC *)0x040000B0)

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

#define START_ON_FIFO_EMPTY 0x30000000





//======================prototypes========================

//Function Prototypes
void setPixel(int, int, u16);
void drawRect(int, int, int, int, u16);
void drawHollowRect(int, int, int, int, u16);
void waitForVblank();
void drawChar(int row, int col, unsigned char ch, unsigned short color);
void drawString(int row, int col, char *str, unsigned short color);
void drawImage3(int y, int x, int width, int height, const u16* image);
void drawRectDMA(int, int, int, int, volatile unsigned short);
void fillScreen(volatile unsigned short);

//================================structs=================

//DMA
typedef struct
{					
	 const volatile void *src;	
	 volatile void *dst;		
	 volatile u32 cnt;		
} DMAREC;

typedef struct 
{
	int lives;
	int score;
	//How much damage attacks do
	int strength;
	//Determines position
	int x;
	int y;
} JON;

typedef struct
{
	int strength;
	int health;
	int speed;
	int x;
	int y;
	//1 if alive, 0 if dead
	int state;
} ENEMY;

typedef struct 
{
	int x;
	int y;
	int strength;
	int speed;
	int state;
} FIRE;