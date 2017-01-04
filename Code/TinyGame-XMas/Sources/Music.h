#ifndef MUSIC
#define MUSIC


void UpdateMusic(int delta);
unsigned long GetTick();
void TDelay(int ticks);
void SetTrack(unsigned char *track, int bytes);
extern void  BuildTrack(int Seed);
#endif
