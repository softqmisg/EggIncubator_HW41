#include "Buzzer.h"

//#define SilentRythm	0	//0ms ON,1000ms OFF
//#define Rythm1			1 //100ms ON,2000ms OFF
//#define	Rythm2			2	//50ms ON,30ms OFF,50ms ON,50ms OFF,200ms ON,500ms OFF
//#define	Rythm3			3	//100ms ON,500ms OFF
Rhythm_t SilentRhythmProg[1]={
	{.onduration=0,.offduration=100}
};
Rhythm_t Rhythm1Prog[1]={
	{.onduration=10,.offduration=400}
};

Rhythm_t Rhythm2Prog[3]={
	{.onduration=5,.offduration=3},
	{.onduration=5,.offduration=10},
	{.onduration=20,.offduration=50}
};
Rhythm_t Rhythm3Prog[1]={
	{.onduration=10,.offduration=50}
};
Buzzer_t Buzzer;


void BuzzerMute(uint8_t mute)
{
	Buzzer.mute=mute;
}
void BuzzerSetRhythm(RhythmTypes_t type,uint8_t repeat)
{
	if(Buzzer.type!=type)
	{
		Buzzer.type=type;
		Buzzer.repeatactive=repeat;
		Buzzer.indexrhythm=0;
		Buzzer.counterTime=1;
		Buzzer.stateoffon=1;
		Buzzer.active=1;
		switch(type)
		{
			case SilentRhythm:
				Buzzer.rhythmProg=SilentRhythmProg;
				Buzzer.numofrhythms=1;
				break;
			case Rhythm1:
				Buzzer.rhythmProg=Rhythm1Prog;
				Buzzer.numofrhythms=1;
				break;
			case Rhythm2:
				Buzzer.rhythmProg=Rhythm2Prog;
				Buzzer.numofrhythms=3;
				break;
			case Rhythm3:
				Buzzer.rhythmProg=Rhythm3Prog;
				Buzzer.numofrhythms=1;
				break;
		}
	}
}
void BuzzerInit()
{
	Buzzer.rhythmProg=SilentRhythmProg;
	Buzzer.type=SilentRhythm;
	Buzzer.numofrhythms=1;
	Buzzer.indexrhythm=0;
	Buzzer.counterTime=0;
	Buzzer.stateoffon=1;
	Buzzer.repeatactive=1;
	Buzzer.mute=0;
	Buzzer.active=1;

}