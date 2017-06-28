/*
 * Music.c
 *
 *  Created on: 26 sep. 2014
 *      Author: Stijn
 */
#include "Music.h"

//#include <math.h>
#include "PPG1.h"

//#include "PPG2.h"
#include  "AudioSwitch.h"
int Notes[16] = {100,0,0,0,100,0,0,0,100,0,0,0,100,102,105,106};
int T = 100;
int Tick =-1;


int msecspertick = 60000/(140*96);

int TrackByte = 0;
int nexttick = 0;
int Tmult = 0;;
int ActiveNotes = 0;
int ActiveNote[128];
int NoteList[10];
int LastArpNote = 0;
int msecelapsed = 0;

unsigned char track[120];
int TrackBytes = 0;
uint32_t now =0 ;

void ResetTrack()
{
	TrackBytes =0 ;
	for(int i =0 ;i<128;i++) ActiveNote[i] = 0;
	ActiveNotes = 0;
	TrackByte = 0;
	Tmult = 0;
	nexttick = 0;
	now =0 ;

};

void WriteByte(unsigned char b)
{
	track[TrackBytes++] = b;
}

void WriteTime(uint32_t time)
{
	int low = time &0xff;
	int mid = (time>>8)&0xff;
	int high = (time>>16)&0xff;

	WriteByte(low);
	WriteByte(mid);
	WriteByte(high);
	WriteByte(0);
}

void AddNote(int note, int length, int gap)
{
	WriteTime(now);
	WriteByte(note);
	now = now + length;
	WriteTime(now);
	WriteByte(0x80 + note);
	now = now + gap;

}

void PlayPackageSound()
{
	ResetTrack();
		for(int i =0 ;i<5;i++)
		{
			AddNote(70+i*2,4,3 );
		}
		for(int i =0 ;i<5;i++)
		{
			AddNote(70 + (10-i*2),4,3 );
		}
}

void PlayJumpSound()
{
	ResetTrack();
	for(int i =0 ;i<10;i++)
	{
		AddNote(70+i,0,6 );
	}
}


void PlayFallSound()
{
	ResetTrack();
	for(int i =0 ;i<8;i++)
	{
		AddNote(65-i,6,10 );
	}
}

void PlayAscentSound()
{
	ResetTrack();
	for(int i =0 ;i<8;i++)
	{
		AddNote(65+i*2,8-i,3+i*2 );
	}
}

void PlayLandSound()
{
	//	ResetTrack();
	//	AddNote(60,2,2);
}
int ontime;
int offtime;
int deltatime;
void PlayNextLevel()
{
	ResetTrack();
	for(int i =0 ;i<5;i++)
	{
		AddNote(-20+60 + i * 6,10,1000);
		AddNote(-20+64+ i * 6,10,1000);
		AddNote(-20+67+ i * 6,10,1000);
	}
}

const uint16_t freqtable[128]={8,8,9,9,10,10,11,12,12,13,14,15,16,17,18,19,20,21,23,24,25,27,29,30,32,34,36,38,41,43,46,48,51,55,58,61,65,69,73,77,82,87,92,97,103,110,116,123,130,138,146,155,164,174,184,195,207,220,233,246,261,277,293,311,329,349,369,391,415,440,466,493,523,554,587,622,659,698,739,783,830,880,932,987,1046,1108,1174,1244,1318,1396,1479,1567,1661,1760,1864,1975,2093,2217,2349,2489,2637,2793,2959,3135,3322,3520,3729,3951,4186,4434,4698,4978,5274,5587,5919,6271,6644,7040,7458,7902,8372,8869,9397,9956,10548,11175,11839,12543};
const uint16_t dutytable[128]= {61156,57723,54483,51425,48539,45815,43243,40816,38525,36363,34322,32396,30578,28861,27241,25712,24269,22907,21621,20408,19262,18181,17161,16198,15289,14430,13620,12856,12134,11453,10810,10204,9631,9090,8580,8099,7644,7215,6810,6428,6067,5726,5405,5102,4815,4545,4290,4049,3822,3607,3405,3214,3033,2863,2702,2551,2407,2272,2145,2024,1911,1803,1702,1607,1516,1431,1351,1275,1203,1136,1072,1012,955,901,851,803,758,715,675,637,601,568,536,506,477,450,425,401,379,357,337,318,300,284,268,253,238,225,212,200,189,178,168,159,150,142,134,126,119,112,106,100,94,89,84,79,75,71,67,63,59,56,53,50,47,44,42};
unsigned char *currenttrack = track;
int hz = 0;
byte decodednote;
//int TrackBytes = 3000;
extern unsigned long timerticks ;
void UpdateMusic(int delta)
{
	timerticks++;
	msecelapsed += delta;
	//PPG1_Disable(PPG1_DeviceData);

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
			int note = NoteList[LastArpNote% ActiveNotes];
			hz = freqtable[note];
			//		printf("%d %d\n", note, Hz);
			PPG1_SetFrequencyHz (PPG1_DeviceData,  hz);
			PPG1_SetDutyUS(PPG1_DeviceData, 5000);//dutytable[note]);
			PPG1_Enable(PPG1_DeviceData);
			//		PPG2_SetFreqHz(Hz);
		}
		else
		{
			PPG1_Disable(PPG1_DeviceData);
			//	PPG2_Disable();
		}


		while(Tmult >= nexttick)
		{
			//T = 0;
			if (TrackBytes > 0)
			{
				nexttick  = currenttrack[TrackByte++]  + (currenttrack[TrackByte++] * 255) + (currenttrack[TrackByte++]*65536);
				TrackByte++;;
				decodednote = currenttrack[TrackByte++];
				if (TrackByte >= TrackBytes )
				{
					TrackBytes = 0;
					TrackByte = 0;
					Tmult = 0;
					nexttick = 0;

				}
				if (decodednote &0x80  || decodednote == 0 )
				{
					if (decodednote > 0)
					{
						if (ActiveNote[decodednote&127] > 0)
						{
							ActiveNote[decodednote&127] = 0;
							ActiveNotes--;
							offtime = Tmult;
							deltatime = offtime - ontime;
						}
					}

					if (ActiveNotes == 0)			PPG1_Disable(PPG1_DeviceData);
				}
				else
				{
					if (decodednote > 0)
					{
						if (ActiveNote[decodednote] == 0)
						{
							ActiveNote[decodednote] = 1;
							ActiveNotes++;
							ontime = Tmult;
						}

						if (ActiveNotes > 0)
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
				//PPG1_Disable(PPG1_DeviceData);
				//PPG2_Disable();

			}
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

