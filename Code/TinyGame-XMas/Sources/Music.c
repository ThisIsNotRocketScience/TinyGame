/*
 * Music.c
 *
 *  Created on: 26 sep. 2014
 *      Author: Stijn
 */
#include "Music.h"

#include <math.h>
#include "PPG1.h"

//#include "PPG2.h"
#include  "AudioSwitch.h"
int Notes[16] = {100,0,0,0,100,0,0,0,100,0,0,0,100,102,105,106};
int msecpertick = 100;
int T = 100;
int Tick =-1;


//#include "Track.h"

float msecspertick = 60000 / (143.1 * 96.0);

int TrackByte = 0;
int nexttick = 0;
int Tmult = 0;;
int ActiveNotes = 0;
int ActiveNote[128];
int NoteList[10];
int LastArpNote = 0;
float msecelapsed = 0;

unsigned char track[100];
int TrackBytes = 320;

void BuildTrack(int Seed)
{
	TrackBytes =0 ;
	for(int i =0 ;i<128;i++) ActiveNote[i] = 0;
	ActiveNotes = 0;
	TrackByte = 0;
	Tmult = 0;
					nexttick = 0;

};

unsigned char *currenttrack = track;
//int TrackBytes = 3000;
extern unsigned long timerticks ;
void UpdateMusic(int delta)
{
	timerticks++;
	msecelapsed += delta;
	while (msecelapsed >= msecspertick)
	{
		msecelapsed -=msecspertick;
		Tmult++;
		int AN = 0;
		if (ActiveNotes > 0)
		{
			for (int i =0 ;i<128;i++)
			{
				if (ActiveNote[i] >0 )
				{
					NoteList[AN++] = i;
				}
			}
			if (Tmult %3 == 0) LastArpNote  = (LastArpNote + 1) % ActiveNotes;
			int note = NoteList[LastArpNote];
			int Hz = (int)(440.0f * powf(2.0f, (note-69.0f)/12.0));
			//		printf("%d %d\n", note, Hz);
			PPG1_SetFrequencyHz (PPG1_DeviceData,  Hz);
	//		PPG2_SetFreqHz(Hz);
		}
		else
		{
			PPG1_Disable(PPG1_DeviceData);
		//	PPG2_Disable();
		}
	}

	while(Tmult >= nexttick)
	{
		//T = 0;
		if (TrackBytes > 0)
		{
			nexttick  = currenttrack[TrackByte++]  + (currenttrack[TrackByte++] * 255) + (currenttrack[TrackByte++]*65536);
			TrackByte++;;
			byte note = currenttrack[TrackByte++];
			if (TrackByte >= TrackBytes )
				{
				TrackByte = 0;
				Tmult = 0;
				nexttick = 0;

				}
			if (note &0x80  || note == 0 )
			{
				if (note > 0)
				{
					if (ActiveNote[note&127] > 0)
					{
						ActiveNote[note&127] = 0;
						ActiveNotes--;
					}
				}

				if (ActiveNotes == 0)			PPG1_Disable(PPG1_DeviceData);
			}
			else
			{
				if (note > 0)
				{
					if (ActiveNote[note] == 0)
					{
						ActiveNote[note] = 1;
						ActiveNotes++;
					}

					if (ActiveNotes == 1)
						{
							if (AudioSwitch_GetVal(AudioSwitch_DeviceData) == TRUE)
							{


								 PORTA_PCR5 = (uint32_t)((PORTA_PCR5 & (uint32_t)~(uint32_t)(
								                PORT_PCR_ISF_MASK |
								                PORT_PCR_MUX(0x05)
								               )) | (uint32_t)(
								                PORT_PCR_MUX(0x02)
								               ));
								 PORTA_PCR6 = (uint32_t)((PORTA_PCR6 & (uint32_t)~(uint32_t)(
															                PORT_PCR_ISF_MASK |
															                PORT_PCR_MUX(0x05)
															               )) | (uint32_t)(
															            		   PORT_PCR_MUX(0x01)

															               ));
								 PPG1_Enable(PPG1_DeviceData);
								//	PPG2_Disable();

							}
							else
							{

								 PORTA_PCR5 = (uint32_t)((PORTA_PCR5 & (uint32_t)~(uint32_t)(
								                PORT_PCR_ISF_MASK |
								                PORT_PCR_MUX(0x05)
								               )) | (uint32_t)(
								            		   PORT_PCR_MUX(0x01)

								               ));

								 PORTA_PCR6 = (uint32_t)((PORTA_PCR6 & (uint32_t)~(uint32_t)(
																                PORT_PCR_ISF_MASK |
																                PORT_PCR_MUX(0x05)
																               )) | (uint32_t)(
																                PORT_PCR_MUX(0x02)
																               ));
								 PPG1_Enable(PPG1_DeviceData);

								 // PPG1_Disable(PPG1_DeviceData);
							//		PPG2_Enable();
							}
						//	PPG1_Enable(PPG1_DeviceData);
						}
				}
			}
		}
		else
		{
			nexttick = Tmult +1;
			PPG1_Disable(PPG1_DeviceData);
			//PPG2_Disable();

		}
	}
}

unsigned long GetTick()
{
	return timerticks ;
};

extern void TDelay(int ticks);

void SetTrack(unsigned char *track, int bytes)
{
	TrackBytes= bytes;
	currenttrack = track;
}

