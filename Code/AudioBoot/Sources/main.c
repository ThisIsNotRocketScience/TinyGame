/* ###################################################################
 **     Filename    : main.c
 **     Project     : AudioBoot
 **     Processor   : MKL02Z32VFM4
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2016-11-12, 18:36, # CodeGen: 0
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
#include "AD1.h"
#include "AdcLdd1.h"
#include "TI1.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
#include "IFsh1.h"
#include "IntFlashLdd1.h"
#include "PTA.h"
#include "PTB.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "Boot.h"
#include "Reader.h"
#include "OLED049.h"
extern AudioReaderStruct Reader;
byte theprogress =0 ;
byte error = 0;
byte started = 0;
void  GUIProgress(uint8_t progress)
{
	theprogress = progress;
} // 255 = 99.999%
void  GUIReset()
{
	theprogress =0;
	error =0 ;
	started = 1;
} // 255 = 99.999%
void GUIErrorState()
{
	error++;
}
extern void DecoderInit();


__attribute__ ((section (".resdat")))
const unsigned char const Image[64*4] =
{3,3,3,255,255,255,3,3,3,0,0,251,251,251,0,0,255,255,255,6,3,3,255,254,254,0,0,31,63,63,240,240,240,63,63,31,0,0,0,95,80,80,64,95,81,95,64,95,73,95,64,95,81,94,64,95,85,81,64,95,66,65,0,0,126,255,255,195,219,219,219,251,123,0,0,252,254,255,51,51,51,255,254,252,0,0,255,255,255,6,3,3,255,255,255,6,3,3,255,255,254,0,0,126,255,255,219,219,219,195,195,195,0,0,0,0,0,128,128,128,128,128,128,128,128,128,128,128,196,190,64,58,234,110,98,42,254,78,200,62,238,106,186,62,234,56,194,2,62,62,234,126,78,42,254,156,98,28,192,178,72,38,192,64,192,0,192,128,64,0,192,0,192,0,0,0,0,0,0,160,0,255,128,190,138,142,128,188,136,132,128,255,255,40,56,0,255,21,12,0,255,138,255,0,255,140,207,0,249,23,9,0,248,168,143,5,188,168,239,0,184,168,239,0,80,0,7,4,143,168,255,112,248,248,221,248,117,32,0,0,0,0,245,170,245,255,128,174,170,186,128,190,168,160,128,255};
int History[64];
int historypos = 0;

unsigned char buffer[64*4];

void InitSequence()
{
	ClearScr(buffer,0);
	OLED_Blit(buffer, Image, 53, 15, 11,17,27,7);
	for(int i =0 ;i<120;i++)
		{
			OLED_Display(buffer);
		}

	for(int i =0 ;i<32;i++)
	{
	ClearScr(buffer,0);
	OLED_Blit(buffer, Image, 0, 0, 37,8,4,40-i);
	OLED_Blit(buffer, Image, 0, 8, 48,8,10,40-i+9);
	OLED_Display(buffer);
	OLED_Display(buffer);
	}
	for(int i =0 ;i<120;i++)
	{
		OLED_Display(buffer);
	}


	for(int i =0 ;i<60;i++)
	{
	ClearScr(buffer,0);
	OLED_Blit(buffer, Image, 0, 0, 37,8,4+i,8);
	OLED_Blit(buffer, Image, 0, 8, 48,8,10-i,8+9);
	OLED_Display(buffer);
	OLED_Display(buffer);
	}
}
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
  {
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	Boot_Check();

	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	/* For example: for(;;) { } */

	AD1_Calibrate(TRUE);
	OLED_Init();
	ClearScr(buffer,0);
	OLED_Display(buffer);

	InitSequence();

	DecoderInit();
	int T = 0;
	for(;;) {
		T++;
		ClearScr(buffer,0);
		//OLED_Blit(buffer, Image, 0, 0, 64,32,0,0);

		//for (int x = 0;x<64;x++)
		//{
		//		buffer[Reader.Sync * 64 + x] = 1;
		//	}
		OLED_Blit(buffer, Image, 39, 0, 23,7,21,2);
		OLED_Blit(buffer, Image, 50, 20 + Reader.Sync*3, 3,3,17,4);
		if (started==1)
		{
			{

				OLED_Blit(buffer, Image, 0, 27, 33,5,1,12);
				OLED_Blit(buffer, Image, 36, 27, 10,5,theprogress/4+2,22);
				for (int i =0;i<theprogress;i++)
				{
					SetPixel(buffer, i/4, 23 + i%3);
				}

			}

			if (error>0)
			{
				OLED_Blit(buffer, Image, 22, 22, 25,5,1,26);
			}
		}
		//else
		{
			for(int i =0 ;i<64;i++)
			{
				int y =  24 + (((History[(i+historypos)&63]-32768) * 15)/22600);
				if (y>= 0 && y< 32) SetPixel(buffer, i, y);
			}
		}
		//OLED_Blit(buffer, buffer, 10,10,10,10,10,10);
		OLED_Display(buffer);

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
 **     This file was created by Processor Expert 10.5 [05.21]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */
