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
#include "SCL.h"
#include "SDA.h"
#include "OLED_RESET.h"
#include "PTA.h"
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
void  GUIProgress(byte progress)
{
	theprogress = progress;
} // 255 = 99.999%
extern void DecoderInit();

int History[64];
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
 	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	/* For example: for(;;) { } */

	Boot_Check();
	AD1_Calibrate(TRUE);
	OLED_Init();
	unsigned char buffer[64*4];
	ClearScr(buffer,0);
	OLED_Display(buffer);
	DecoderInit();
	int T = 0;
	for(;;) {
		T++;
		ClearScr(buffer,0);
		for (int x = 0;x<64;x++)
		{
			buffer[Reader.Sync * 64 + x] = 255;
		}

		if (Reader.Sync == AUDIOREADER_SYNCED)
		{
			for (int i =0;i<theprogress/8;i++)
			{
				buffer[i] = 0xff;
			}

		}
		//else
		{
			for(int i =0 ;i<64;i++)
			{
				int y =  16 + (((History[i]-32768) * 15)/400);
				if (y>= 0 && y< 32) SetPixel(buffer, i, y);
			}
		}
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
