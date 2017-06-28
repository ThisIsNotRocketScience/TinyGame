#ifndef MUSIC
#define MUSIC


void UpdateMusic(int delta);
unsigned long GetTick();
void TDelay(int ticks);
void SetTrack(unsigned char *track, int bytes);
extern void  BuildTrack(int Seed);

extern void PlayPackageSound();
extern void PlayJumpSound();
extern void PlayLandSound();
extern void PlayNextLevel();
extern void PlayFallSound();
extern void PlayAscentSound();

#endif
