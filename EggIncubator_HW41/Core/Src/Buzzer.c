#include "Buzzer.h"
Buzzer_t Buzzer;
void BuzzerOn(uint16_t duration)
{
	Buzzer.duration=duration/10;
	Buzzer.counter_on=0;
	Buzzer.active=1;
}
void BuzzerRepeatStart(uint16_t duration,uint16_t delaybetweenrepeat)
{
	if(!Buzzer.repeatstate)
	{
		Buzzer.duration=duration/10;
		Buzzer.delaybetweenrepeat=delaybetweenrepeat/10;
		Buzzer.counter_repeat=0;
		Buzzer.repeatstate=1;
		Buzzer.counter_on=0;
		Buzzer.active=1;
	}
}
void BuzzerRepeatStop()
{
	Buzzer.repeatstate=0;

}
void BuzzerMute(uint8_t mute)
{
	Buzzer.mute=mute;
}
void BuzzerInit()
{
	Buzzer.mute=0;
	Buzzer.counter_on=0;
	Buzzer.counter_repeat=0;
	Buzzer.active=0;
	Buzzer.repeatstate=0;
	Buzzer.duration=10; //*10ms
	Buzzer.delaybetweenrepeat=400;//*10ms
}