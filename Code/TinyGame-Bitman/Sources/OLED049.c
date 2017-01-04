/*
 * OLED049.c
 *
 *  Created on: 16 sep. 2014
 *      Author: Stijn
 */

#include "Cpu.h"
#include "OLED049.h"


//B3 scl
//B4 sda
//B5 reset

#define SCL 3
#define SDA 4
#define RESET 5

void OLED_InitHardware()
{
	SIM_SCGC5   |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK ;
	GPIOB_PDDR |= (1<<SCL)| (1<<SDA) | (1<<RESET);
	PORTB_PCR3 = PORT_PCR_MUX(0x01) ;
	PORTB_PCR4 = PORT_PCR_MUX(0x01) ;
	PORTB_PCR5 = PORT_PCR_MUX(0x01) ;
}

void SDA_SetVal()
{
	GPIOB_PSOR = (1<<SDA);
}

void SDA_ClrVal()
{
	GPIOB_PCOR= (1<<SDA);
}

void SCL_SetVal()
{
	GPIOB_PSOR = (1<<SCL);
}

void SCL_ClrVal()
{
	GPIOB_PCOR= (1<<SCL);
}
void OLED_RESET_SetVal()
{
	GPIOB_PSOR = (1<<RESET);
}

void OLED_RESET_ClrVal()
{
	GPIOB_PCOR= (1<<RESET);
}



unsigned char Contrast_level= 0x40;//0x8F;

unsigned char i2c_end = 0;

void Delay(uint32_t N)
{
	for (uint32_t i =0 ;i<N;i++)
	{
		__asm("nop");
	}
}

void write_w(unsigned char dat)
{
	unsigned char m;
	unsigned char da;
	unsigned char j;
	da = dat;
	for (j= 0;j<8;j++)
	{
		m = da;
		SCL_ClrVal(0);
		m = m&0x80;
		if (m == 0x80)
		{
			SDA_SetVal(0);

		}
		else
		{
			SDA_ClrVal(0);
		}
		da = da<<1;
		SCL_SetVal(0);

	}
	SCL_ClrVal(0);
	SCL_SetVal(0);
}

void i2cstart()
{
	SCL_SetVal(0);
	SDA_SetVal(0);
	SDA_ClrVal(0);
	SCL_ClrVal(0);
}

void i2cstop()
{
	SCL_ClrVal(0);
	SDA_ClrVal(0);
	SDA_SetVal(0);
	SCL_SetVal(0);
}

void write_i(unsigned char ins)
{
	i2cstart();
	write_w(0x78);
	write_w(0x80);
	write_w(ins);
	i2cstop();
}

// Set page address 0~15
void Set_Page_Address(unsigned char add)
{
	add+=32;

	add=0xb0|add;
	write_i(add);
}

void Set_Column_Address(unsigned char add)
{
	add+=32;

	write_i((0x10|(add>>4)));
	write_i((0x0f&add));


}

void Display_Picture(unsigned char pic[])
{
	unsigned char i,j,num=0;
	for(i=0;i<0x04;i++)
	{
		Set_Page_Address(i);
		Set_Column_Address(0x00);

		i2cstart();
		write_w(0x78);

		write_w(0x40);
		for(j=0;j<0x40;j++)
		{
			write_w(pic[i*0x40+j]);
		}
		i2cstop();
	}
	return;
}

void OLED_Init()
{
	OLED_RESET_SetVal(0);
	Delay(10000);
	OLED_RESET_ClrVal(0);
	Delay(10000);
	OLED_RESET_SetVal(0);
	Delay(10000);
	write_i(0xAE);
	write_i(0x00);
	write_i(0x12);
	write_i(0x00);
	write_i(0xB0);
	write_i(0x81);
	write_i(0x4f);
	write_i(0xa1);

	write_i(0xa6);
	write_i(0xa8);
	write_i(0x1f);
	write_i(0xc8);
	write_i(0xd3);
	write_i(0x00);
	write_i(0xd5);
	write_i(0x80);
	write_i(0xd9);
	write_i(0x1f);
	write_i(0xda);
	write_i(0x12);
	write_i(0xdb);
	write_i(0x40);

	write_i(0x8d);
	write_i(0x14); // 0x10 = on,

	write_i(0xaf);



	Delay(10000);


}

byte pic2[64*4];
int t = 0;
void OLED_PictureTest()
{
	t++;
	for(int i = 0;i<64*4;i++)
	{
		pic2[i] = i + t ;
	}
	Display_Picture(pic2);
}

void OLED_Display(unsigned char *pic)
{
	Display_Picture(pic);
}

void ClearScr(unsigned char *pic)
{
	for (int i =0 ;i<64*4;i++) pic[i] = 0;
}

void SetPixel(unsigned char *pic, int x, int y)
{
	int idx = x  + ((y/8)*64);
	pic[idx] |= 1<<(y%8);
}

void SetPixelSafe(unsigned char *pic, int x, int y)
{
	if (x>=0 && x<64 && y>=0 && y<32 ) SetPixel(pic,x,y);
}


void ClrPixel(unsigned char *pic, int x, int y)
{
	int idx = x  + ((y/8)*64);
	pic[idx] &=  ~(1<<(y%8));
}

unsigned char GetPixel(unsigned char *pic, int x, int y)
{
	int idx = x  + ((y/8)*64);
	return (pic[idx] & (1<<(y%8)))>0?1:0;

}

void OLED_Blit(byte *pic, byte *minitiles, int sx, int sy, int sw, int sh, int xx,int yy)
{
	if (xx > 63) return;
	if (yy > 31) return;
	if (xx + sw < 0) return;
	if (yy + sh < 0) return;
	for(int y =0;y<sh;y++)
	{
		int yyy = yy + y;
		if (yyy >=0 && yyy<= 31)
		{
			for (int x = 0;x<sw;x++)
			{
				int xxx = xx+x;
				if (xxx >=0 && xxx<= 63)
				{
					if (GetPixel(minitiles, sx + x, sy + y))
					{
						SetPixel(pic, xxx, yyy);
					}
					else
					{
						ClrPixel(pic, xxx, yyy);
					}
				}
			}

		}
	}
}

