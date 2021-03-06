#pragma once
#include <stdint.h>

#define PATTERNGEN_MAXTICK 32

struct PatternGen_Tick
{
	unsigned char vel;
	signed char note;
	unsigned char accent;
};

struct PatternGen_Target
{
	struct PatternGen_Tick Ticks[PATTERNGEN_MAXTICK];
	unsigned char TPB;
	unsigned char Length;
};

struct PatternGen_Random
{
	long RandomMemory;
};

struct PatternGen_PatternFuncSpecific
{
	int16_t b1;
	int16_t b2;
	int16_t b3;
	int16_t b4;
	uint8_t matrix[8][8][2];
	struct PatternGen_Random ExtraRandom;
};

extern void PatternGen_RandomSeed(struct PatternGen_Random *R, unsigned int seed);

extern void PatternGen_Goa(struct PatternGen_Target *T, struct PatternGen_Random *R, int Length);
extern void PatternGen_Flat(struct PatternGen_Target *T, struct PatternGen_Random *R, int Length);
extern void PatternGen_Psych(struct PatternGen_Target *T, struct PatternGen_Random *R, int Length);
extern void PatternGen_Zeph(struct PatternGen_Target *T, struct PatternGen_Random *R, int stepsperbeat, int beats, int fullcycles);

#define PATTERNGEN_MAXALGO 4
#define PATTERNGEN_MAXSCALE 4
#define PATTERNGEN_MAXBEAT 4
#define PATTERNGEN_MAXTPB 4

struct PatternGen_Settings
{
	uint8_t tpboptions[PATTERNGEN_MAXTPB];
	uint8_t beatoptions[PATTERNGEN_MAXBEAT];
	uint8_t scale[PATTERNGEN_MAXSCALE][12];
	uint8_t scalecount[PATTERNGEN_MAXSCALE];
	uint8_t algooptions[PATTERNGEN_MAXALGO];
};


struct PatternGen_Params
{
	uint8_t tpbopt;
	uint8_t beatopt;
	uint8_t scale;
	uint8_t algo;

	uint8_t seed1;
	uint8_t seed2;
};

extern void PatternGen_ValidateSettings(struct PatternGen_Settings *S);
extern void PatternGen_ValidateParams(struct PatternGen_Params *P);

extern void PatternGen_LoadSettings(struct PatternGen_Settings *S, struct PatternGen_Params *P);
extern void PatternGen_Generate(struct PatternGen_Target *T, struct PatternGen_Params *P, struct PatternGen_Settings *S);
