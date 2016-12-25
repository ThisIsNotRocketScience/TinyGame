#ifndef OLED
#define OLED

void OLED_Init();
void OLED_Contrast(unsigned char byte);
void OLED_SendByte(unsigned char byte);
unsigned char OLED_RecvByte();
void OLED_PictureTest();
void OLED_Display(unsigned char *pic);

void ClearScr(unsigned char *pic);
void SetPixel(unsigned char *pic, int x, int y);
void SetPixelSafe(unsigned char *pic, int x, int y);
void ClrPixel(unsigned char *pic, int x, int y);

unsigned char GetPixel(unsigned char *pic, int x, int y);

void OLED_Blit(unsigned char *pic, unsigned char *minitiles, int sx, int sy, int sw, int sh, int x,int y);

extern unsigned char i2c_end;
#endif
