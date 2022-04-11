#include "user_time.h"
#include "user_eeprom.h"
#include "eeprom.h"
int8_t compareTime(Time_t t1,Time_t t2)
{
	int8_t r=0;
	if(t1.day>t2.day)
	{
		r=1;
	}
	else if(t1.day<t2.day)
	{
		r=-1;
	}
	else{
		if(t1.hr>t2.hr)
		{
			r=1;
		}
		else if(t1.hr<t2.hr)
		{
			r=-1;
		}
		else
		{
			if(t1.min>t2.min)
			{
				r=1;
			}
			else if(t1.min<t2.min)
			{
				r=-1;
			}
			else
			{
				if(t1.sec>t2.sec)
				{
					r=1;
				}
				else if(t1.sec<t2.sec)
				{
					r=-1;
				}
				else
				{
					r=0;
				}
			}
		}
	}
	return r;
}
/*
*/
void IncTime(Time_t *time,Time_t inc)
{
	time->sec+=inc.sec;
	if(time->sec>59)
	{
		time->sec=0;
		time->min+=1;
		time->min+=inc.min;
		if(time->min>59)
		{
			time->min=0;
			time->hr+=1;
			time->hr+=inc.hr;
			if(time->hr>23)
			{
				time->hr=0;
				time->day+=1;
				time->day=inc.day;
			}
		}
	}
}
void IncTimesec(Time_t *t)
{
	Time_t tmp={.sec=1,.min=0,.hr=0,.day=0};
	IncTime(t,tmp);
}
/*
*/
void TimeSave(Time_t t,uint16_t add)
{
	EEWriteByte((uint8_t *)&t,sizeof(t),add);
}
void TimeInit(Time_t *t)
{
		EEReadByte((uint8_t *)&t,sizeof(Time_t),EE_ADD_CURTIME);

}