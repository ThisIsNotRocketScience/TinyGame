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
#include "OLED_RESET.h"
#include "SDA.h"
#include "SCL.h"
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



#include <math.h>
unsigned char pic[64*4]  = {0};

const unsigned char pic1[64*4] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,252,252,4,0,0,240,248,72,72,248,240,0,0,248,248,48,96,248,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,96,240,145,241,96,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,64,192,192,0,0,64,67,7,4,135,135,5,0,128,128,0,0,128,128,0,0,128,128,0,0,128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,24,16,16,31,31,0,0,31,31,0,0,31,31,3,6,31,31,0,0,31,31,4,4,31,31,0,0,31,31,20,20,16,16,0,0,0,0,0,0,0,0,0,0,0,0,0};
const unsigned char pic2[64*4] = {0,0,0,0,0,0,0,0,0,0,0,128,128,0,0,0,128,128,0,0,128,128,128,128,128,128,0,0,128,128,128,128,128,0,0,0,128,128,128,128,128,0,0,0,128,128,0,0,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63,63,7,14,7,63,63,0,0,63,63,36,36,36,32,0,0,63,63,4,4,63,59,0,0,63,63,4,4,63,59,0,0,3,7,60,60,7,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,198,238,56,16,56,238,198,0,0,254,254,28,56,28,254,254,0,0,248,252,38,38,252,248,0,0,76,222,146,146,246,100,0,0,222,222,14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const unsigned char pic3[64*4] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,112,252,140,140,252,112,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,31,25,25,27,27,12,31,27,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const unsigned char pic4[64*4] = {0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,192,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63,63,0,0,0,0,63,63,8,8,63,63,0,0,62,63,9,9,63,62,0,0,63,63,6,12,63,63,0,0,63,63,12,30,51,33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,28,60,224,224,60,28,0,0,120,252,132,132,252,120,0,0,124,252,128,128,252,252,0,0,188,188,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const unsigned char pic5[64*4] = {0,0,248,248,72,72,8,8,0,192,224,32,32,224,192,0,0,224,224,32,32,224,192,0,0,0,0,0,0,192,224,32,32,224,192,0,0,224,224,192,128,224,224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,135,199,64,64,192,128,0,3,135,196,68,199,67,192,128,7,7,129,193,71,70,192,128,0,0,64,64,71,199,193,1,7,71,64,0,7,199,192,129,7,199,192,0,0,128,192,64,64,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,143,130,130,143,15,0,0,15,143,128,143,128,15,15,0,0,143,143,2,2,15,15,128,128,140,142,11,9,8,128,128,143,15,0,0,15,15,1,3,15,15,0,0,7,15,8,10,14,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,33,49,56,44,39,35,0,0,31,63,32,32,63,31,0,0,33,63,63,32,0,0,31,63,36,36,61,25,0,0,47,47,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

unsigned char sprites[64*4] = {17,10,31,10,17,4,21,14,14,21,4,0,255,1,125,21,29,1,121,17,9,1,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,3,31,15,127,31,63,31,255,15,255,63,255,0,0,0,0,0,0,0,0,0,0,0,0,255,1,93,85,117,1,125,81,65,1,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,63,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


#define MAXPARTICLE 130
typedef struct TParticle
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
	if (P->a>29) P->a = 0;

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
	case 0:P->x-=8;break;
	case 1:P->x+=8;break;
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


void AddSprites()
{
	OLED_Blit_Add(pic, sprites, 46,0,18,15,46,0);
	if (zrand()%100 <10)
	{
		SpawnParticle(1, rand()%50, -4,0, 0);
	}
	for(int i =0 ;i<MAXPARTICLE;i++)
	{
		if(Particles[i].a)
		{
			OLED_Blit_Add(pic, sprites,5,0,6,5,Particles[i].x>>3,Particles[i].y>>3);

			UpdateParticle(&Particles[i]);
		}


	}
}

void Show(const unsigned char *ipic)
{
	for(int i =0 ;i<100;i++)
	{
		OLED_Blit(pic, ipic, 0,0,64,32,0,0);

		AddSprites();

		UpdateTimeForMusic();
		OLED_Display(pic);
		TDelay(20);
	}

}

void Fade(const unsigned char *pic1,const unsigned char *pic2)
{
	for(int i = 0;i<32;i++)
	{
		ClearScr(pic);
		if (pic1 >0)OLED_Blit( pic, pic1,0,0,64,32,0,-i );
		if (pic2 >0)OLED_Blit( pic, pic2,0,0,64,32,0,32-i );

		AddSprites();

		UpdateTimeForMusic();
		OLED_Display(pic);
		TDelay(20);

	}

}

void XMAS()
{
	while (1)
	{
		Fade(0, pic1);
		Show(pic1);
		Fade(pic1, pic2);
		Show(pic2);
		Fade(pic2, pic3);
		Show(pic3);
		Fade(pic3, pic4);
		Show(pic4);
		Fade(pic4, pic5);
		Show(pic5);
		Fade(pic5, 0);
		for(int i = 0;i<200;i++)
		{
			ClearScr(pic);
			AddSprites();
			OLED_Blit( pic, sprites,12,0,11,17,26,8 );

			AddSprites();

			UpdateTimeForMusic();
			OLED_Display(pic);
			TDelay(20);

		}
		for(int i = 0;i<150;i++)
		{
			ClearScr(pic);
			AddSprites();
			OLED_Display(pic);
			TDelay(20);
		}


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


uint16_t lastt = 0;
extern void UpdateMusic(int delta);

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
	BuildTrack(0);
	/* For example: for(;;) { } */
	for (;;)
	{
		XMAS();
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
