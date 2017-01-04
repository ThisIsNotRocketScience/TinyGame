#include "Cpu.h"
#include "OLED049.h"


//B3 scl
//B4 sda
//B5 reset

#define OLEDSCL 3
#define OLEDSDA 4
#define OLEDRESET 5
__attribute__ ((section (".resdat")))
void OLED_InitHardware()
{
	PORTB_PCR3 = PORT_PCR_MUX(0x01);
	PORTB_PCR4 = PORT_PCR_MUX(0x01);
	PORTB_PCR5 = PORT_PCR_MUX(0x01);

	GPIOB_PDDR |= (1 << OLEDSCL) | (1 << OLEDSDA) | (1 << OLEDRESET);
}
__attribute__ ((section (".resdat")))
void Delay(uint32_t N)
{
	for (volatile uint32_t i =0 ;i<N;i++)
	{
		__asm volatile("nop");
	}

}
__attribute__ ((section (".resdat")))
void Delay10000()
{
	Delay(10000);


}

#define SDA_SET() GPIOB_PDOR |= (1<<OLEDSDA);
#define SDA_CLR() GPIOB_PDOR &= ~(1<<OLEDSDA);
#define SCL_SET() {GPIOB_PDOR |= (1<<OLEDSCL);};
#define SCL_CLR() {GPIOB_PDOR &= ~(1<<OLEDSCL);Delay(1);};
#define  OLED_RESET_SET() GPIOB_PDOR |= (1<<OLEDRESET);
#define  OLED_RESET_CLR() GPIOB_PDOR &= ~(1<<OLEDRESET);


unsigned char Contrast_level = 0x40;//0x8F;

unsigned char i2c_end  = 0;


__attribute__ ((section (".resdat")))

void write_w(unsigned char dat)
{
	unsigned char m;
	unsigned char da;
	unsigned char j;
	da = dat;
	for (j= 0;j<8;j++)
	{
		m = da;
		SCL_CLR();
		m = m&0x80;
		if (m == 0x80)
		{
			SDA_SET();

		}
		else
		{
			SDA_CLR();
		}
		da = da<<1;
		SCL_SET();

	}
	SCL_CLR();
	SCL_SET();
}
__attribute__ ((section (".resdat")))
void i2cstart()
{
	SCL_SET();
	SDA_SET();
	SDA_CLR();
	SCL_CLR();
}
__attribute__ ((section (".resdat")))
void i2cstop()
{
	SCL_CLR();
	SDA_CLR();
	SDA_SET();
	SCL_SET();
}
__attribute__ ((section (".resdat")))
void write_i(unsigned char ins)
{
	i2cstart();
	write_w(0x78);
	write_w(0x80);
	write_w(ins);
	i2cstop();
}

// Set page address 0~15
//__attribute__ ((section (".resdat")))
void Set_Page_Address(unsigned char add)
{
	add+=32;

	add=0xb0|add;
	write_i(add);
}
//__attribute__ ((section (".resdat")))
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


const unsigned char const initseq[25] = {0xAE,0x00,0x12,0x00,0xB0,0x81,0x4f,0xa1,0xa6,0xa8,0x1f,0xc8,0xd3,0x00,0xd5,0x80,0xd9,0x1f,0xda,0x12,0xdb,0x40,0x8d,0x14,0xaf};

__attribute__ ((section (".resdat")))
void OLED_Init()
{
	OLED_InitHardware();
	OLED_RESET_SET();
	Delay10000();
	OLED_RESET_CLR();
	Delay10000();
	OLED_RESET_SET();
	Delay10000();


	for(int i =0 ;i< 25;i++) write_i(initseq[i]);
	Delay10000();
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
__attribute__ ((section (".resdat")))
void OLED_Display(unsigned char *pic)
{
	Display_Picture(pic);
}

const unsigned char charset[]   = {
		255,255,255,255,255,255,255,255,255,255,
		255,239,239,239,239,239,255,239,255,255,
		215,215,215,255,255,255,255,255,255,255,
		255,255,215,131,215,215,131,215,255,255,
		255,239,135,235,199,175,195,239,255,255,
		255,255,119,171,215,175,87,187,255,255,
		255,231,219,219,231,91,187,71,255,255,
		239,239,239,255,255,255,255,255,255,255,
		223,239,247,247,247,247,247,239,223,255,
		247,239,223,223,223,223,223,239,247,255,
		255,255,239,171,199,171,239,255,255,255,
		255,255,239,239,131,239,239,255,255,255,
		255,255,255,255,255,255,255,239,247,255,
		255,255,255,255,131,255,255,255,255,255,
		255,255,255,255,255,255,255,239,255,255,
		191,191,223,223,239,239,247,247,251,255,
		255,199,187,155,171,179,187,199,255,255,
		255,239,231,239,239,239,239,199,255,255,
		255,199,187,191,207,247,251,131,255,255,
		255,199,187,191,207,191,187,199,255,255,
		255,223,207,215,219,131,223,223,255,255,
		255,131,251,251,195,191,187,199,255,255,
		255,207,247,251,195,187,187,199,255,255,
		255,131,191,223,223,239,239,247,255,255,
		255,199,187,187,199,187,187,199,255,255,
		255,199,187,187,135,191,223,231,255,255,
		255,255,255,239,255,255,239,255,255,255,
		255,255,255,239,255,255,239,247,255,255,
		255,255,191,207,243,207,191,255,255,255,
		255,255,255,131,255,131,255,255,255,255,
		255,255,251,231,159,231,251,255,255,255,
		255,199,187,191,223,239,255,239,255,255,
		255,135,123,75,75,11,251,7,255,255,
		255,239,239,215,215,199,187,187,255,255,
		255,195,187,187,195,187,187,195,255,255,
		255,199,187,251,251,251,187,199,255,255,
		255,227,219,187,187,187,219,227,255,255,
		255,131,251,251,195,251,251,131,255,255,
		255,131,251,251,195,251,251,251,255,255,
		255,199,187,251,155,187,187,135,255,255,
		255,187,187,187,131,187,187,187,255,255,
		255,199,239,239,239,239,239,199,255,255,
		255,143,191,191,191,191,191,195,255,255,
		255,187,219,235,243,235,219,187,255,255,
		255,251,251,251,251,251,251,131,255,255,
		255,187,187,147,147,171,171,187,255,255,
		255,179,179,171,171,171,155,155,255,255,
		255,199,187,187,187,187,187,199,255,255,
		255,195,187,187,195,251,251,251,255,255,
		255,199,187,187,187,187,155,135,255,255,
		255,195,187,187,195,187,187,187,255,255,
		255,199,187,251,199,191,187,199,255,255,
		255,131,239,239,239,239,239,239,255,255,
		255,187,187,187,187,187,187,199,255,255,
		255,187,187,187,215,215,239,239,255,255,
		255,187,171,171,171,215,215,215,255,255,
		255,187,187,215,239,215,187,187,255,255,
		255,187,187,187,199,239,239,239,255,255,
		255,131,191,223,239,247,251,131,255,255,
		199,247,247,247,247,247,247,247,199,255,
		251,251,247,247,239,239,223,223,191,255,
		199,223,223,223,223,223,223,223,199,255,
		255,239,215,187,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,3,255,
		255,239,223,255,255,255,255,255,255,255,
		255,255,255,199,191,135,187,135,255,255,
		251,251,251,195,187,187,187,195,255,255,
		255,255,255,135,251,251,251,135,255,255,
		191,191,191,135,187,187,187,135,255,255,
		255,255,255,199,187,131,251,135,255,255,
		143,247,247,195,247,247,247,247,255,255,
		255,255,255,199,187,187,187,135,191,199,
		251,251,251,195,187,187,187,187,255,255,
		255,239,255,231,239,239,239,239,255,255,
		255,223,255,207,223,223,223,223,223,227,
		247,247,247,183,215,231,215,183,255,255,
		231,239,239,239,239,239,239,239,255,255,
		255,255,255,211,171,171,171,171,255,255,
		255,255,255,195,187,187,187,187,255,255,
		255,255,255,199,187,187,187,199,255,255,
		255,255,255,195,187,187,187,195,251,251,
		255,255,255,135,187,187,187,135,191,191,
		255,255,255,203,179,251,251,251,255,255,
		255,255,255,135,251,199,191,195,255,255,
		255,247,247,199,247,247,247,143,255,255,
		255,255,255,187,187,187,187,135,255,255,
		255,255,255,187,187,215,215,239,255,255,
		255,255,255,187,171,171,215,215,255,255,
		255,255,255,187,215,239,215,187,255,255,
		255,255,255,187,187,187,187,135,191,199,
		255,255,255,131,223,239,247,131,255,255,
		223,239,239,239,247,239,239,239,223,255,
		239,239,239,239,239,239,239,239,239,255,
		247,239,239,239,223,239,239,239,247,255,
		255,255,255,255,103,155,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,223,239,255,255,
		239,239,199,239,239,239,239,239,247,255,
		255,255,255,255,255,255,215,215,255,255,
		255,255,255,255,255,255,171,255,255,255,
		239,131,239,239,239,239,239,239,255,255,
		239,131,239,131,239,239,239,239,255,255,
		215,255,255,255,255,255,255,255,255,255,
		255,183,215,239,87,91,255,255,255,255,
		239,255,135,251,131,191,195,255,255,255,
		255,255,223,239,247,239,223,255,255,255,
		39,219,219,155,219,219,39,255,255,255,
		255,255,255,255,255,255,255,255,255,255,
		239,255,131,223,239,247,131,255,255,255,
		255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,
		239,223,255,255,255,255,255,255,255,255,
		223,239,255,255,255,255,255,255,255,255,
		215,175,255,255,255,255,255,255,255,255,
		175,215,255,255,255,255,255,255,255,255,
		255,255,255,207,207,255,255,255,255,255,
		255,255,255,255,131,255,255,255,255,255,
		255,255,255,255,3,255,255,255,255,255,
		255,255,255,175,215,255,255,255,255,255,
		255,255,3,23,87,87,255,255,255,255,
		239,255,135,251,199,191,195,255,255,255,
		255,255,239,223,191,223,239,255,255,255,
		255,255,39,219,155,219,39,255,255,255,
		255,255,255,255,255,255,255,255,255,255,
		239,255,131,223,239,247,131,255,255,255,
		255,187,187,187,199,239,239,255,255,255,
		255,255,255,255,255,255,255,255,255,255,
		255,239,255,239,239,239,239,239,255,255,
		255,255,223,143,215,215,143,223,255,255,
		159,239,239,135,239,247,251,131,255,255,
		255,255,123,135,183,183,135,123,255,255,
		255,187,187,199,239,131,239,239,255,255,
		239,239,239,239,255,239,239,239,239,255,
		159,239,239,199,187,187,199,239,239,243,
		255,215,255,255,255,255,255,255,255,255,
		255,135,123,75,107,75,123,135,255,255,
		239,223,207,215,207,255,255,255,255,255,
		255,255,255,111,183,219,183,111,255,255,
		255,255,255,255,135,191,191,191,255,255,
		255,255,255,255,255,255,255,255,255,255,
		255,135,123,91,107,107,123,135,255,255,
		255,131,255,255,255,255,255,255,255,255,
		255,239,215,239,255,255,255,255,255,255,
		255,255,239,239,131,239,239,131,255,255,
		231,223,239,247,199,255,255,255,255,255,
		231,223,231,223,231,255,255,255,255,255,
		223,239,255,255,255,255,255,255,255,255,
		255,255,255,183,183,183,183,71,247,251,
		255,7,163,163,167,175,175,175,175,175,
		255,255,255,255,239,255,255,255,255,255,
		255,255,255,255,255,255,255,207,223,239,
		239,231,239,239,199,255,255,255,255,255,
		239,215,215,239,255,255,255,255,255,255,
		255,255,255,219,183,111,183,219,255,255,
		191,187,219,219,111,47,23,119,251,255,
		191,187,219,219,47,111,183,55,251,255,
		187,183,211,215,107,47,23,119,251,255,
		255,239,255,239,223,191,187,199,255,255,
		247,239,255,239,215,199,187,187,255,255,
		239,247,255,239,215,199,187,187,255,255,
		239,215,255,239,215,199,187,187,255,255,
		175,215,255,239,215,199,187,187,255,255,
		255,215,255,239,215,199,187,187,255,255,
		239,215,239,239,215,199,187,187,255,255,
		255,15,215,215,135,219,219,27,255,255,
		255,199,187,251,251,251,187,199,239,247,
		247,239,255,131,251,195,251,131,255,255,
		239,247,255,131,251,195,251,131,255,255,
		239,215,255,131,251,195,251,131,255,255,
		255,215,255,131,251,195,251,131,255,255,
		247,239,255,199,239,239,239,199,255,255,
		239,247,255,199,239,239,239,199,255,255,
		239,215,255,199,239,239,239,199,255,255,
		255,215,255,199,239,239,239,199,255,255,
		255,199,183,119,67,119,183,199,255,255,
		175,215,255,179,171,171,155,155,255,255,
		247,239,255,199,187,187,187,199,255,255,
		239,247,255,199,187,187,187,199,255,255,
		239,215,255,199,187,187,187,199,255,255,
		175,215,255,199,187,187,187,199,255,255,
		255,215,255,199,187,187,187,199,255,255,
		255,255,187,215,239,215,187,255,255,255,
		255,255,79,183,151,167,183,203,255,255,
		247,239,255,187,187,187,187,199,255,255,
		239,247,255,187,187,187,187,199,255,255,
		239,215,255,187,187,187,187,199,255,255,
		255,215,255,187,187,187,187,199,255,255,
		223,239,187,187,187,199,239,239,255,255,
		255,247,247,199,183,183,199,247,247,255,
		255,199,183,183,199,183,183,199,251,255,
		247,239,255,199,191,135,187,135,255,255,
		239,247,255,199,191,135,187,135,255,255,
		239,215,255,199,191,135,187,135,255,255,
		175,215,255,199,191,135,187,135,255,255,
		255,215,255,199,191,135,187,135,255,255,
		239,215,239,199,191,135,187,135,255,255,
		255,255,255,167,95,135,219,39,255,255,
		255,255,255,135,251,251,251,135,239,247,
		247,239,255,199,187,131,251,135,255,255,
		239,247,255,199,187,131,251,135,255,255,
		239,215,255,199,187,131,251,135,255,255,
		255,215,255,199,187,131,251,135,255,255,
		247,239,255,231,239,239,239,239,255,255,
		239,247,255,231,239,239,239,239,255,255,
		239,215,255,231,239,239,239,239,255,255,
		255,215,255,231,239,239,239,239,255,255,
		255,167,223,191,135,187,187,199,255,255,
		175,215,255,195,187,187,187,187,255,255,
		247,239,255,199,187,187,187,199,255,255,
		239,247,255,199,187,187,187,199,255,255,
		239,215,255,199,187,187,187,199,255,255,
		215,235,255,199,187,187,187,199,255,255,
		255,215,255,199,187,187,187,199,255,255,
		255,255,239,255,131,255,239,255,255,255,
		255,255,79,183,151,167,183,203,255,255,
		247,239,255,187,187,187,187,135,255,255,
		239,247,255,187,187,187,187,135,255,255,
		239,215,255,187,187,187,187,135,255,255,
		255,215,255,187,187,187,187,135,255,255,
		239,247,255,187,187,187,187,135,191,199,
		255,251,251,195,187,187,187,195,251,251,
		255,215,255,187,187,187,187,135,191,199,
		0};


#define __min(a,b) (((a)<(b))?(a):(b))
void DrawStringHoriz(unsigned char *pic, int x, int y, char *t)
{
	int c = 0;
	while (*t && x<64)
	{
		byte mask = (4<<c);
		for(int yy = y; yy < __min(32, y+10); yy++)
		{
			int ry = yy-y;

			byte b = charset[(*t - 32) * 10 + ry];
			if ((b & mask) == 0)
			{
				SetPixel(pic, x, yy);
			}
			else
			{
				ClrPixel(pic, x, yy);
			}

		}

		c++;
		if (c == 6)
		{
			c = 0;
			//x++;
			t++;
		}
		x++;

	}
}

void ClearScr(unsigned char *pic, byte pattern)
{
	for (int i =0 ;i<64*4;i++) pic[i] = pattern;
}
__attribute__ ((section (".resdat")))
void SetPixel(unsigned char *pic, int x, int y)
{
	int idx = x  + ((y/8)*64);
	pic[idx] |= 1<<(y%8);
}

void ClrPixel(unsigned char *pic, int x, int y)
{
	int idx = x  + ((y/8)*64);
	pic[idx] &=  ~(1<<(y%8));
}
__attribute__ ((section (".resdat")))
unsigned char GetPixel(unsigned char *pic, int x, int y)
{
	int idx = x  + ((y/8)*64);
	return (pic[idx] & (1<<(y%8)))>0?1:0;

}


__attribute__ ((section (".resdat")))
void OLED_Blit(byte *pic, byte *minitiles, int sx, int sy, int sw, int sh, int xx,int yy)
{
	//if (xx > 63) return;
	//if (yy > 31) return;
	//if (xx + sw < 0) return;
	//if (yy + sh < 0) return;
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

