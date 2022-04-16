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
void IncTime(Time_t *t,Time_t inc)
{
	t->sec+=inc.sec;
	t->min+=inc.min;
	t->hr+=inc.hr;
	t->day+=inc.day;
	if(t->sec>59)
	{
		t->sec=0;
		t->min+=1;
		if(t->min>59)
		{
			t->min=0;
			t->hr+=1;
			if(t->hr>23)
			{
				t->hr=0;
				t->day+=1;
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
	EEWriteByte((uint8_t *)&t,sizeof(Time_t),add);
}
void TimeInit(Time_t *t)
{
		EEReadByte((uint8_t *)t,sizeof(Time_t),EE_ADD_CURTIME);

}