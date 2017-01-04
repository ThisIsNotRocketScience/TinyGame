/* ###################################################################
 **     Filename    : main.c
 **     Project     : KL02MiniGameBoy1
 **     Processor   : MKL02Z32VFM4
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2014-09-04, 15:41, # CodeGen: 0
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file main.c
 ** @version 01.01
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Rood.h"
#include "Groen.h"
#include "TU2.h"
#include "Button1.h"
#include "PTB.h"
#include "Button2.h"
#include "Button3.h"
#include "Button4.h"
#include "PPG1.h"
#include "RT1.h"
#include "WAIT1.h"
#include "AudioSwitch.h"
#include "PTA.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"


/* User includes (#include below this line is not maintained by Processor Expert) */
#include "OLED049.h"


typedef struct __attribute__ ((__packed__))
{
	byte type;
	short x;
	signed short y;
	short param1;
	byte param2;
} entity;

#include <math.h>
entity *activelevel[30];
int activeentities = 0;
bool HasGround = FALSE;

unsigned char levelset[889] = {0x08
		,0x14,0x00
		,0x00,0x0F,0x00,0x1B,0x00,0x05,0x00,0x00
		,0x00,0x00,0x00,0x1C,0x00,0x11,0x00,0x00
		,0x05,0xB0,0x00,0x1E,0x00,0x00,0x00,0x01
		,0x00,0x38,0x00,0x15,0x00,0x05,0x00,0x00
		,0x00,0x4F,0x00,0x0F,0x00,0x05,0x00,0x00
		,0x00,0x12,0x00,0x1A,0x00,0x05,0x00,0x00
		,0x00,0x15,0x00,0x19,0x00,0x1F,0x00,0x00
		,0x00,0x31,0x00,0x1A,0x00,0x03,0x00,0x00
		,0x00,0x31,0x00,0x1B,0x00,0x03,0x00,0x00
		,0x00,0x31,0x00,0x1C,0x00,0x13,0x00,0x00
		,0x00,0x41,0x00,0x1B,0x00,0x03,0x00,0x00
		,0x00,0x41,0x00,0x1A,0x00,0x03,0x00,0x02
		,0x00,0x41,0x00,0x19,0x00,0x21,0x00,0x00
		,0x00,0x67,0x00,0x19,0x00,0x15,0x00,0x00
		,0x04,0x77,0x00,0x15,0x00,0x05,0x00,0x00
		,0x03,0x3A,0x00,0x11,0x00,0x05,0x00,0x00
		,0x00,0x08,0x00,0x16,0x00,0x05,0x00,0x00
		,0x03,0x0A,0x00,0x12,0x00,0x05,0x00,0x00
		,0x00,0x62,0x00,0x1C,0x00,0x05,0x00,0x00
		,0x00,0x5C,0x00,0x1E,0x00,0x11,0x00,0x01
		,0x0D,0x01
		,0x00,0x0A,0x00,0x1A,0x00,0x05,0x00,0x00
		,0x00,0x14,0x00,0x1A,0x00,0x05,0x00,0x01
		,0x05,0x5D,0x00,0x0E,0x00,0xF6,0xFF,0x01
		,0x00,0x6D,0x00,0x15,0x00,0x05,0x00,0x00
		,0x04,0xAA,0x00,0x06,0x00,0x05,0x00,0x00
		,0x03,0x0C,0x00,0x16,0x00,0x05,0x00,0x00
		,0x00,0xA1,0x00,0x0A,0x00,0x05,0x00,0x00
		,0x00,0x9B,0x00,0x0D,0x00,0x05,0x00,0x00
		,0x00,0x95,0x00,0x11,0x00,0x05,0x00,0x00
		,0x05,0xBF,0x00,0x18,0x00,0x05,0x00,0x00
		,0x00,0x25,0x00,0x1B,0x00,0x05,0x00,0x00
		,0x03,0x6F,0x00,0x11,0x00,0x05,0x00,0x00
		,0x00,0x8A,0x00,0x1A,0x00,0x16,0x00,0x01
		,0x07,0x01
		,0x00,0x14,0x00,0x1A,0x00,0x05,0x00,0x01
		,0x05,0x64,0x00,0x1E,0x00,0x00,0x00,0x01
		,0x00,0x9F,0x00,0x11,0x00,0x05,0x00,0x00
		,0x04,0xA6,0x00,0x1B,0x00,0x05,0x00,0x00
		,0x03,0x45,0x00,0x1B,0x00,0x05,0x00,0x00
		,0x03,0x7D,0x00,0x1B,0x00,0x05,0x00,0x00
		,0x03,0x64,0x00,0x1B,0x00,0x05,0x00,0x00
		,0x0A,0x00
		,0x00,0x95,0x00,0x08,0x00,0x0E,0x00,0x01
		,0x05,0x64,0x00,0x1E,0x00,0x00,0x00,0x01
		,0x00,0x9E,0x00,0x08,0x00,0x05,0x00,0x00
		,0x04,0xA0,0x00,0x04,0x00,0x05,0x00,0x00
		,0x00,0x00,0x00,0x1F,0x00,0x21,0x00,0x00
		,0x00,0x28,0x00,0x1C,0x00,0x1F,0x00,0x00
		,0x00,0x4E,0x00,0x19,0x00,0x15,0x00,0x03
		,0x00,0x6A,0x00,0x14,0x00,0x0E,0x00,0x00
		,0x00,0x7D,0x00,0x10,0x00,0x0A,0x00,0x00
		,0x00,0x8C,0x00,0x0C,0x00,0x05,0x00,0x00
		,0x0B,0x00
		,0x00,0x13,0x00,0x17,0x00,0x0C,0x00,0x01
		,0x05,0x64,0x00,0x1E,0x00,0x00,0x00,0x01
		,0x00,0x9E,0x00,0x11,0x00,0x05,0x00,0x00
		,0x04,0xA0,0x00,0x0D,0x00,0x05,0x00,0x00
		,0x02,0x00,0x00,0x20,0x00,0x18,0x00,0x1C
		,0x02,0x1A,0x00,0x1E,0x00,0x16,0x00,0x1A
		,0x02,0x31,0x00,0x1E,0x00,0x0F,0x00,0x17
		,0x00,0x40,0x00,0x12,0x00,0x24,0x00,0x00
		,0x03,0x52,0x00,0x0F,0x00,0x05,0x00,0x00
		,0x01,0x64,0x00,0x13,0x00,0x12,0x00,0x1D
		,0x02,0x91,0x00,0x1B,0x00,0x0D,0x00,0x10
		,0x0A,0x00
		,0x00,0x25,0x00,0x1A,0x00,0x05,0x00,0x01
		,0x05,0x64,0x00,0x1E,0x00,0x00,0x00,0x01
		,0x00,0x42,0x00,0x15,0x00,0x05,0x00,0x00
		,0x04,0x3C,0x00,0x0A,0x00,0x05,0x00,0x00
		,0x00,0x00,0x00,0x1E,0x00,0x3C,0x00,0x00
		,0x00,0x2C,0x00,0x17,0x00,0x05,0x00,0x00
		,0x00,0x25,0x00,0x11,0x00,0x05,0x00,0x00
		,0x00,0x2D,0x00,0x0D,0x00,0x05,0x00,0x00
		,0x00,0x46,0x00,0x10,0x00,0x05,0x00,0x00
		,0x00,0x3D,0x00,0x1A,0x00,0x05,0x00,0x00
		,0x0C,0x00
		,0x00,0x0A,0x00,0x1A,0x00,0x05,0x00,0x00
		,0x00,0x16,0x00,0x1A,0x00,0x05,0x00,0x01
		,0x05,0x64,0x00,0x1E,0x00,0x00,0x00,0x01
		,0x00,0x9E,0x00,0x11,0x00,0x05,0x00,0x00
		,0x01,0x6E,0x00,0x19,0x00,0x2A,0x00,0x18
		,0x00,0x05,0x00,0x17,0x00,0x14,0x00,0x00
		,0x03,0x0E,0x00,0x12,0x00,0x05,0x00,0x03
		,0x04,0x76,0x00,0x08,0x00,0x05,0x00,0x00
		,0x03,0xA2,0x00,0x0E,0x00,0x05,0x00,0x00
		,0x01,0x66,0x00,0x11,0x00,0x2B,0x00,0x32
		,0x00,0x1B,0x00,0x1C,0x00,0x15,0x00,0xFF
		,0x00,0x38,0x00,0x1C,0x00,0x36,0x00,0x00
		,0x1A,0x00
		,0x00,0x00,0x00,0x16,0x00,0x08,0x00,0x00
		,0x00,0x08,0x00,0x19,0x00,0x08,0x00,0x00
		,0x05,0xB0,0x00,0x1E,0x00,0x00,0x00,0x01
		,0x00,0x10,0x00,0x1C,0x00,0x08,0x00,0x00
		,0x00,0x18,0x00,0x1F,0x00,0x28,0x00,0x00
		,0x00,0x46,0x00,0x1F,0x00,0x18,0x00,0x00
		,0x02,0x40,0x00,0x21,0x00,0x06,0x00,0x11
		,0x00,0x46,0x00,0x10,0x00,0x12,0x00,0x00
		,0x00,0x9D,0x00,0x1E,0x00,0x0E,0x00,0x02
		,0x02,0xAB,0x00,0x1E,0x00,0x0B,0x00,0x0C
		,0x01,0xB4,0x00,0x10,0x00,0x0F,0x00,0x16
		,0x00,0xD5,0x00,0x17,0x00,0x17,0x00,0x01
		,0x03,0x4E,0x00,0x0D,0x00,0x00,0x00,0x00
		,0x03,0xE3,0x00,0x14,0x00,0x00,0x00,0x00
		,0x00,0x63,0x00,0x1F,0x00,0x10,0x00,0x00
		,0x00,0x73,0x00,0x1A,0x00,0x0D,0x00,0x00
		,0x01,0x7F,0x00,0x1A,0x00,0x11,0x00,0x0F
		,0x00,0x5D,0x00,0x10,0x00,0x05,0x00,0x01
		,0x02,0x63,0x00,0x10,0x00,0x05,0x00,0x07
		,0x00,0x68,0x00,0x09,0x00,0x13,0x00,0x01
		,0x01,0x71,0x00,0x0A,0x00,0x16,0x00,0x25
		,0x03,0xA0,0x00,0x07,0x00,0x00,0x00,0x00
		,0x04,0xE3,0x00,0x08,0x00,0x05,0x00,0x00
		,0x00,0xEC,0x00,0x13,0x00,0x09,0x00,0x00
		,0x01,0xC6,0x00,0x0B,0x00,0x0A,0x00,0x1E
		,0x02,0xE6,0x00,0x1F,0x00,0x10,0x00,0x15
};


void LoadLevel(int i)
{
	activeentities = 0;
	if (i > levelset[0]) return;
	int L = 0;
	int idx = 1;
	while (L < i)
	{
		byte entitiestoskip = levelset[idx];
		idx += 2 + entitiestoskip  * 8;
		L++;
	}
	activeentities = levelset[idx++];
	if (levelset[idx++] > 0 ) HasGround = TRUE;else HasGround = FALSE;
	for (int i = 0;i< activeentities;i++)
	{
		activelevel[i] = (entity*)(levelset + idx);
		if (activelevel[i]->type == 3)
		{
			activelevel[i]->param1 = 0;
		}
		idx += 8;
	}
}

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
byte pic[64*4];
byte pic2[64*4];

byte titlepic[64*4] = {0,0,220,20,220,64,220,0,220,64,220,20,220,0,220,0,220,64,220,0,220,20,220,64,220,20,220,64,220,0,220,20,220,0,220,64,220,20,220,0,85,170,0,8,248,8,0,248,32,248,0,232,0,184,168,232,0,0,232,0,184,168,232,0,0,0,253,4,245,149,149,148,101,5,21,244,21,4,21,20,245,21,21,4,245,196,5,197,245,4,5,197,53,196,5,4,245,52,197,5,245,4,253,0,85,170,0,124,8,124,0,124,68,124,0,4,124,4,16,40,0,84,124,56,124,108,56,16,0,0,223,16,215,84,212,20,211,80,212,23,212,16,208,16,215,80,208,16,215,16,211,80,215,16,214,81,209,17,214,16,215,16,209,86,215,16,223,0,85,170,0,62,10,54,0,62,34,62,0,62,34,0,62,8,54,0,62,42,0,2,62,2,0,0,29,20,29,1,29,0,29,1,29,20,29,0,29,0,29,1,29,0,29,20,29,1,29,20,29,1,29,0,29,20,29,0,29,1,29,20,29,0,85,170,0,23,21,29,0,31,17,0,29,0,31,21,0,31,2,31,0,31,17,0,31,21};

unsigned char tiles[64*4] =// {223,17,223,0,223,64,221,21,215,17,213,31,199,4,223,151,213,29,223,213,221,1,193,159,95,21,95,215,85,31,0,192,0,0,0,192,192,192,0,192,0,192,0,192,192,192,0,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,122,41,120,0,59,65,51,64,58,1,88,0,58,64,51,64,59,0,123,90,90,0,122,89,91,0,88,3,0,0,0,3,2,2,0,3,2,2,0,1,2,1,0,3,2,2,0,3,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,124,85,85,126,85,85,124,40,93,33,86,9,117,40,84,33,93,42,117,9,84,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
{223,17,223,0,223,64,221,21,215,17,213,31,199,4,223,151,213,29,223,213,221,1,193,159,95,21,95,215,85,31,0,192,0,0,0,192,192,192,0,192,0,192,0,192,192,192,0,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,122,41,120,0,59,65,51,64,58,1,88,0,58,64,51,64,59,0,123,90,90,0,122,89,91,0,88,3,0,0,0,3,2,2,0,3,2,2,0,1,2,1,0,3,2,2,0,3,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,124,85,85,126,213,85,124,40,221,33,86,9,245,40,84,33,221,42,117,9,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,20,34,65,255,0,28,99,255,0,0,0,255,0,0,0,255,99,28,0,255,65,34,20,8,224,160,96,184,96,160,224,224,96,160,120,160,96,224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

unsigned char spiffing[64*4] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,144,144,144,32,0,240,144,144,144,96,0,16,240,16,0,240,144,144,144,16,0,240,144,144,144,16,0,16,240,16,0,240,48,192,0,240,0,224,16,16,144,160,0,240,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,4,4,3,0,7,0,0,0,0,0,4,7,4,0,7,0,0,0,0,0,7,0,0,0,0,0,4,7,4,0,7,0,1,6,7,0,3,4,4,4,7,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

bool AnyKey()
{
	bool B = TRUE;

	if (Button1_GetVal(0) == FALSE) B = FALSE;
	if (Button2_GetVal(0) == FALSE) B = FALSE;
	if (Button3_GetVal(0) == FALSE) B = FALSE;
	if (Button4_GetVal(0) == FALSE) B = FALSE;

	return B;
}
void ShowWin()
{

}

void ShowGameOver()
{

}
void SpriteDemo()
{
	bool spritedemo = TRUE;
	int t = 0;
	while (spritedemo == TRUE)
	{
		t++;
		ClearScr(pic);

		OLED_Blit(pic, tiles, 0 * 3, 0 ,3, 5, 1+4*0,1);

		OLED_Blit(pic, tiles, 1 * 3, 0 ,3, 5, 1+4*1,1);
		OLED_Blit(pic, tiles, 2 * 3, 0 ,3, 5, 1+4*2,1);
		OLED_Blit(pic, tiles, 3 * 3, 0 ,3, 5, 1+4*3,1);
		OLED_Blit(pic, tiles, 4 * 3, 0 ,3, 5, 1+4*4,1);
		OLED_Blit(pic, tiles, 5 * 3, 0 ,3, 5, 1+4*5,1);
		OLED_Blit(pic, tiles, 6 * 3, 0 ,3, 5, 1+4*6,1);
		OLED_Blit(pic, tiles, 7 * 3, 0 ,3, 5, 1+4*7,1);
		OLED_Blit(pic, tiles, 8 * 3, 0 ,3, 5, 1+4*8,1);
		OLED_Blit(pic, tiles, 9 * 3, 0 ,3, 5, 1+4*9,1);


		OLED_Blit(pic, tiles, 0 + 7 * (t%3),16,7,7, 10,10);


		OLED_Display(pic);
		if (AnyKey() == FALSE)
		{
			spritedemo = FALSE;
		}
	}
	while (AnyKey() == TRUE)
	{

	}
}

extern void TDelay(int ticks);


void Succes()
{
	bool menu = TRUE;
	int t = 0;
	for (int i = 0;i<32;i++)
	{
		ClearScr(pic);
		OLED_Blit( pic, spiffing,0,0,64,32,0,32-i );

		OLED_Display(pic);
		TDelay(20);
	}
	for(int i =0 ;i<8;i++)
	{
		ClearScr(pic);
		OLED_Display(pic);
		TDelay(100);
		OLED_Display(spiffing);
		TDelay(100);
	}
	TDelay(100);

	while (menu == TRUE)
	{
		t++;
		OLED_Display(spiffing);
		if (AnyKey() == FALSE)
		{
			menu = FALSE;
		}

	}
	while (AnyKey() == TRUE)
	{

	}

	for (int i = 0;i<32;i++)
		{
			ClearScr(pic);
//			OLED_Blit( pic, spiffing,0,0,64,32,0,-i );

			OLED_Display(pic);
			TDelay(20);
		}
	TDelay(200);

}

void UpdateTimeForMusic();

void Menu()
{
	//BuildTrack(0);
	//Succes();
	bool menu = TRUE;
	int t = 0;
	for (int i = 0;i<32;i++)
	{
		ClearScr(pic);
		UpdateTimeForMusic();

		OLED_Blit( pic, titlepic,0,0,64,32,0,32-i );
		UpdateTimeForMusic();

		OLED_Display(pic);
		UpdateTimeForMusic();

	}
	while (menu == TRUE)
	{
		UpdateTimeForMusic();

		t++;
		OLED_Display(titlepic);
		if (AnyKey() == FALSE)
		{
			menu = FALSE;
		}
	}
	while (AnyKey() == TRUE)
	{

	}
}

int TriWave(int T, int Limit)
{
	if (Limit == 0) return 0;
	int T2 = T % (Limit * 2);
	if (T2> Limit)
	{
		T2 = Limit - (T2 - Limit);
	}
	return T2;
}

void DoPlatform(byte *PP,int cx, int *playerx, int *playery, int platformx, int platformy, int platformwidth, bool *above, bool *below, bool dotted)
{
	if (platformx - cx > 63) return;
	if (platformx + platformwidth - cx < 0) return;
	if (platformy > 31) return;
	if (platformy < 0) return;
	for (int j = 0;j<platformwidth;j++)
	{
		int xx = j + platformx ;
		if (xx == *playerx)
		{
			if (platformy == *playery-2) *above = TRUE;
			if (platformy == *playery+1) *below = TRUE;
			if (platformy == *playery) *playery = platformy - 2 ;

		}
		xx -= cx;
		if (xx >= 0 &&  xx <=63)
		{
			if (dotted == FALSE)
			{
				SetPixel(PP, xx, platformy);
			}
			else
			{
				if ((j%2 + cx)%2 == 0) SetPixel(PP, xx, platformy); else ClrPixel(PP, xx, platformy);
			}
			if (platformy + 1 < 32)
			{
				if ((xx%2)%2 == 1) SetPixel(PP, xx, platformy+1); else ClrPixel(PP, xx, platformy+1);
			}
		}
	}
}

void drawdigit(byte *PP,byte dig, int xx, int yy)
{
	OLED_Blit(PP, tiles, dig * 3, 0 ,3, 5, xx,yy);
}
#define MAXPARTICLE 30

struct TParticle
{
	signed char dx, dy;
	unsigned short x,y,a;
};

int32_t seed = 123456789;

int zrand()
{
	return (((seed = seed * 214013L + 2531011L) >> 16) & 0x7fff);
}

struct TParticle Particles[MAXPARTICLE] = {0};
void UpdateParticle(struct TParticle *P)
{
	P->a++;
	if (P->a>20) P->a = 0;

	if (P->dy != 0x70) {
		P->dy++;
		P->y+=P->dy;
	}
	else
	{
		P->y-=3;
	}
	P->x+=P->dx;

	switch((zrand()%30)/10)
	{
	case 0:P->x--;break;
	case 1:P->x++;break;
	}
}

void SpawnParticle(int G, int x,int y, signed char dx, signed char dy)
{
	for(int i =0 ;i<MAXPARTICLE;i++)
	{
		if (Particles[i].a == 0)
		{
			Particles[i].a =1 ;
			Particles[i].x = x<<3;
			Particles[i].y = y<<3;
			Particles[i].dx = dx;
			Particles[i].dy = dy;
			if (G == 0) Particles[i].dy=0x70;

			return;
		}
	}
}

uint8_t iabs(uint32_t inp)
{
	if (inp > 0) return inp;
	return -inp;
}
bool RenderLevel(byte *PP, int cx, int t, bool *above, bool *below, int *x, int *y, byte *score)
{
	//drawdigit(PP, *score, 60,1);
	//drawdigit(PP, *lives, 2,1);

	if (HasGround == TRUE)
	{
		for (int i = cx%2 ;i<64;i+=2) SetPixel(pic,i,31);
	}

	OLED_Blit(PP, titlepic, 57,10,7,5,0,0-cx);

	for (int i  =0 ; i < activeentities;i++)
	{
		switch(activelevel[i]->type)
		{
		case 3:
			if (activelevel[i]->param1 == 0)
			{
				OLED_Blit(PP, tiles, 0 + 7 * (t%3),16,7,7, activelevel[i]->x-3 - cx,activelevel[i]->y-3);

				if (iabs(activelevel[i]->x - *x) <3 && iabs(activelevel[i]->y - *y)<4)
				{
					*score = *score + 1;
					activelevel[i]->param1 = 1;
					for(int j = 0;j<13;j++) SpawnParticle(1, *x,*y+1,(zrand()%15)-7, -(zrand()%15));
				}
			}
			break;
		case 4:
			//if (activelevel[i]->param1 == 0)
		{
			int t2 = ((t/5)%10);
			if (t2 > 4) t2 = 9 - t2;
			int off[5] = {4,3,2,1,0};
			OLED_Blit(PP, tiles, 0 + 5 * t2,23,5,9, activelevel[i]->x-3 - cx  - off[t2] + 3,activelevel[i]->y - 6-3);
			OLED_Blit(PP, tiles, 25 + 7 * (t%2),27,7,5, activelevel[i]->x-3 - cx,activelevel[i]->y+5 - 6);

			if (iabs(activelevel[i]->x - *x) <3 && iabs(activelevel[i]->y - *y-4)<5)
			{
				*score = *score + 1;
				return TRUE;
			}
		}
		break;
		case 2: // vplatform
		{
			int py = activelevel[i]->y - TriWave(t/4, activelevel[i]->param2);
			DoPlatform(PP,cx, x,y,activelevel[i]->x, py, activelevel[i]->param1, above, below, FALSE);
		}
		break;
		case 1:
		{
			int px = activelevel[i]->x + TriWave(t/4, activelevel[i]->param2);
			DoPlatform(PP,cx, x,y,px, activelevel[i]->y, activelevel[i]->param1, above, below, FALSE);
		}
		break;
		case 0:
			DoPlatform(PP,cx, x,y,activelevel[i]->x, activelevel[i]->y, activelevel[i]->param1, above, below, (activelevel[i]->param2 == 0? FALSE: TRUE));
			break;
		}
	};
	return FALSE;
};

uint16_t lastt = 0;

void UpdateTimeForMusic()
{
	uint16_t t = 0;
	RT1_GetTimeMS(RT1_DeviceData, &t);

	int dt = t - lastt;
	if (t>0)
	{
		UpdateMusic(dt);
		lastt = t;
	}

}


bool Game()
{
	byte level = 0;
	bool first = TRUE;

	while (level < levelset[0])
	{
		LoadLevel(level);
		//BuildTrack(level * 10 + 1);
		unsigned char n = 0;
		int x =  3;
		int y =  10;
		int cx = 0;
		int cy = 0;
		int dy = 1;
		int dyc = 0;
		int roodtimer = 0;
		int groentimer = 0;
		bool jumped = 0;
		bool playing = TRUE;
		bool below = FALSE;
		bool above = FALSE;
		bool nextlevel = FALSE;
		int t = 0;
		byte score = 0;

		ClearScr(pic2);

		RenderLevel(pic2, cx,t,  &above, &below, &x, &y, &score);

		for (int i = 0;i<32;i+=2)
		{
			UpdateTimeForMusic();
			ClearScr(pic);
			if (first == TRUE) OLED_Blit( pic, titlepic,0,0,64,32,0,-i );
			OLED_Blit( pic, pic2,0,0,64,32,0,32-i );
			OLED_Display(pic);
		}

		first = FALSE;
		below = FALSE;
		while (playing == TRUE)
		{
			UpdateTimeForMusic();


			above = FALSE;

			t++;
			if (groentimer > 0)

			{
				Groen_SetVal(0);
				groentimer--;
			}
			else
			{
				Groen_ClrVal(0);
			}
			if (roodtimer > 0)
			{
				Rood_SetVal(0);
				roodtimer--;
			}
			else
			{
				Rood_ClrVal(0);
			}

			ClearScr(pic);

			if (Button2_GetVal(0) == FALSE)
			{
				if(zrand()%100<20 && below == TRUE) SpawnParticle(0,x,y+1,-(zrand()%10), -8);
				x = (x + 1);
				if (x>510)x = 510;
			}

			if (Button3_GetVal(0) == FALSE)
			{
				if(zrand()%100<20&& below == TRUE) SpawnParticle(0,x,y+1,(zrand()%10), -8);
				x = (x - 1);
				if (x<3)x = 3;
			}
			below = FALSE;


			if (x - cx > 40) cx = x-40;
			if (x - cx < 10) cx = x-10;
			if (cx < 0) cx = 0;
			if (cx > 512-64) cx = 512-64;



			if (RenderLevel(pic, cx,t,  &above, &below, &x, &y, &score) == TRUE)
			{
				level++;
				nextlevel = TRUE;
				playing = FALSE;
			}

			if (y == 30 && HasGround == TRUE) below = TRUE;
			if (y == 31){ playing = FALSE; }
			// die!

			if (above == TRUE && dy<0)
			{
				dy = 1;
				roodtimer=4;
			};

			if (below == TRUE)
			{
				if(dy > 0){ dy = 0; jumped = 0;groentimer = 4;};
				if ( (Button1_GetVal(0) == FALSE || Button4_GetVal(0) == FALSE) && jumped == FALSE && above == FALSE)
				{
					jumped = 1;

					for(int j = 0;j<6;j++) SpawnParticle(1, x,y+1,(zrand()%11)-5, -(zrand()%9));

					dyc = 13;
					dy = -1;
				}
				if (dy > 0) dy = 0;
			}
			else
			{
				if (dy == 0) dy = 1;
			}

			if (dyc > 0 && dy < 0)
			{
				dyc--;
				if (dyc == 0) dy = 1;
			}


			y = y + dy;

			SetPixel(pic, x-cx,y);
			SetPixel(pic, x-cx,y-1);
			for (int i =0 ;i<MAXPARTICLE;i++)
			{
				if (Particles[i].a>0)
				{
					SetPixelSafe(pic,((Particles[i].x)>>3) - cx, (Particles[i].y>>3) );
					UpdateParticle(&Particles[i]);

				}
			}

			if (x-cx < 32)
			{
				drawdigit(pic, level, 60, 0);
				OLED_Blit(pic, tiles, 31,6,19,4,60-21 ,0);
			}
			else
			{
				drawdigit(pic, level, 21, 0);
				OLED_Blit(pic, tiles, 31,6,19,4,0 ,0);

			}

			OLED_Display(pic);
			TDelay(10);
			//OLED_Contrast(n++);
			//	OLED_PictureTest();
		}

		ClrPixel(pic, x-cx,y);
		ClrPixel(pic, x-cx,y-1);
		for (int i = 0;i<32;i++)
		{
			UpdateTimeForMusic();
			ClearScr(pic2);
			UpdateTimeForMusic();
			if (nextlevel == TRUE)
			{
				OLED_Blit( pic2, pic,0,0,64,32,0, i/2 );
				UpdateTimeForMusic();
			}
			else
			{
				OLED_Blit( pic2, pic,0,0,64,32,0,-i );
				UpdateTimeForMusic();
			}
			SetPixel(pic2, x-cx,y);
			SetPixel(pic2, x-cx,y-1);
			OLED_Display(pic2);
			UpdateTimeForMusic();
		}
	}
	// return TRUE; //levelset[0]) return TRUE;
	return FALSE;
}

int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */



	//	TDelay(200/5);
	OLED_Init();
	//	TDelay(20/5);

	/* For example: for(;;) { } */
	for (;;)
	{
		Menu();
		bool GamePlaying = TRUE;

		while (GamePlaying == TRUE)
		{
			GamePlaying = Game();

		}
		Succes();

	}
	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.4 [05.10]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */
