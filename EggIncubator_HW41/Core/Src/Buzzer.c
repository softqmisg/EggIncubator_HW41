#include "Buzzer.h"
Buzzer_t Buzzer;
void BuzzerOn(uint16_t duration)
{
	Buzzer.duration=duration;
	Buzzer.counter_on=0;
	Buzzer.active=1;
}
void BuzzerRepeatStart(uint16_t duration,uint16_t delaybetweenrepeat)
{
	if(!Buzzer.repeatstate)
	{
		Buzzer.duration=duration;
		Buzzer.delaybetweenrepeat=delaybetweenrepeat;
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