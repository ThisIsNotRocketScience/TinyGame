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
#include "AD1.h"
#include "TI1.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
#include "TI2.h"
#include "TU2.h"
#include "OLED_RESET.h"
#include "SDA.h"
#include "SCL.h"
#include "Button1.h"
#include "PTB.h"
#include "Button2.h"
#include "Button3.h"
#include "Button4.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"


/* User includes (#include below this line is not maintained by Processor Expert) */
#include "OLED049.h"
typedef struct
{
	int x;
	int y;
	int width;
} platform;

platform level[6] = {{10,31-10,10}, {30,31-14,10}, {55,31-5,5},{65,31-10,5}, {75,31-15,5}, {85,31-20,10}};
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
byte pic[64*4];

byte minipic[64*4] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,254,254,2,250,250,26,26,26,250,250,26,26,26,250,226,2,218,218,2,242,242,226,130,2,242,242,2,218,218,2,254,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63,63,48,55,55,48,48,48,55,55,48,48,48,55,55,48,55,55,48,55,55,48,49,51,55,55,48,55,55,48,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

byte qrpic[64*4] = {255,255,1,125,69,69,69,125,1,255,25,219,67,149,113,219,65,229,125,161,77,239,123,255,1,125,69,69,69,125,1,255,255,255,255,255,255,255,255,255,255,127,127,63,63,159,159,143,143,143,143,159,159,63,63,127,127,255,255,255,255,255,255,255,255,255,157,233,73,67,171,77,85,171,184,9,120,214,30,73,120,34,192,119,174,16,163,117,153,33,245,119,183,185,17,255,255,255,255,255,127,15,7,193,224,240,224,195,135,15,31,7,49,49,3,255,255,255,255,252,248,248,225,7,31,127,255,255,255,255,86,84,93,114,90,76,85,214,150,33,141,214,30,73,120,1,24,255,99,226,0,194,65,204,30,153,187,219,49,255,255,255,255,255,240,192,0,15,127,127,255,255,255,239,2,192,0,128,192,3,135,143,31,31,63,127,31,128,224,248,255,255,255,255,192,223,209,209,209,223,192,255,198,208,253,214,222,201,248,210,200,243,230,224,192,241,205,233,192,234,255,247,209,255,255,255,255,255,255,255,255,254,252,248,240,243,227,231,231,199,230,231,199,199,227,227,225,240,248,252,254,255,255,255,255,255};

unsigned char minitiles[64*4] = {0,112,136,136,136,80,136,132,132,132,132,72,48,0,0,0,0,0,0,96,144,144,96,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,64,64,112,88,72,76,76,120,64,128,0,0,0,128,64,224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,142,81,49,49,78,128,0,0,0,0,0,0,0,0,0,0,252,3,0,60,91,66,66,60,91,66,66,60,0,1,254,1,36,252,3,0,0,0,0,0,0,0,0,0,0,0,2,4,24,96,128,64,32,16,8,8,4,4,4,0,0,0,0,0,1,2,4,4,2,1,0,0,0,0,0,0,0,0,0,0,3,4,8,16,35,38,46,46,46,38,35,32,16,8,7,12,25,17,62,0,0,0,0,0,0,0,0,0,0,80,224,112,160,160,127,224,80,80,128,16,160,80,128,16,160,252,132,180,180,132,252,168,84,168,84,168,84,0,120,72,72,120,0,84,168,84,168,84,168,0,0,0,0,112,190,10,190,112,0,0,24,40,168,78,74,78,168,40,24,0,0,0,0};

void Menu()
{
	bool menu = TRUE;
	int t = 0;
	while (menu == TRUE)
	{
		t++;
		ClearScr(pic);
		OLED_Blit(pic, minitiles, ((t+0)%4)*4,28,4,4,10,10);
		OLED_Blit(pic, minitiles, ((t+1)%4)*4,28,4,4,20,10);

		OLED_Blit(pic, minitiles, ((t+2)%4)*4,28,4,4,30,10);
		OLED_Blit(pic, minitiles, ((t+3)%4)*4,28,4,4,40,10);

		OLED_Blit(pic, minitiles, 35,2, 19,20,10,10);

		OLED_Display(pic);
		if (Button3_GetVal(0) == FALSE)
		{
			menu = FALSE;
		}
	}
}

void Game()
{
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


	while (playing == TRUE)
	{
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
			x = (x + 1);
			if (x>199)x = 199;
		}

		if (Button3_GetVal(0) == FALSE)
			{
				x = (x - 1);
				if (x<3)x = 3;
			}

		if (x - cx > 50) cx = x-50;
		if (x - cx < 10) cx = x-10;
		if (cx < 0) cx = 0;
		if (cx > 200-64) cx = 200-64;
		bool below = FALSE;
		bool above = FALSE;

		for (int i = cx%2 ;i<64;i+=2) SetPixel(pic,i,31);
		for (int i  =0 ; i < 6;i++)
		{
			for (int j = 0;j<level[i].width;j++)
			{
				int xx = j + level[i].x ;
				if (xx == x)
				{
					if (level[i].y == y-2) above = TRUE;
					if (level[i].y == y+1) below = TRUE;
				}
				xx -= cx;
				if (xx >= 0 &&  xx <=63) SetPixel(pic, xx, level[i].y);
			}
		}

		if (y == 30) below = TRUE;

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


		if (jumped == TRUE)
		{
			OLED_Blit(pic, minitiles, 51,25,9,7,x-4-cx,y-6);

		}
		else
		{
			OLED_Blit(pic, minitiles, 44,25,5,7,x-2-cx,y-6);
		}

		//SetPixel(pic, x-cx,y);
		//SetPixel(pic, x-cx,y-1);
		OLED_Display(pic);
		//OLED_Contrast(n++);
		//	OLED_PictureTest();
	}
}

int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	OLED_Init();

	/* For example: for(;;) { } */
	for (;;)
	{

		Menu();
		Game();

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
