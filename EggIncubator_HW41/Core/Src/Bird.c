#include "Bird.h"
#include "user_eeprom.h"
#include "eeprom.h"
#include <string.h>
//{Manual=0,Morgh,ShotorMorgh,Ghaz,Ordak,Kabk,Gharghavol,Belderchin,Boghalamoon,MorghShakhdar} BirdType_t;
Prog_t Manual_prog[2]={
		{.humidty=600,.temperature=377,.durationDays=21},//setter
		{.humidty=600,.temperature=377,.durationDays=0},//hatcher

};
Prog_t Morgh_prog[2]={
		{.humidty=600,.temperature=377,.durationDays=18},//setter
		{.humidty=700,.temperature=372,.durationDays=3},//hatcher
};
Prog_t ShotorMorgh_prog[2]={
		{.humidty=220,.temperature=363,.durationDays=18},//setter
		{.humidty=270,.temperature=366,.durationDays=3},//hatcher
};
Prog_t Ghaz_prog[2]={
		{.humidty=750,.temperature=375,.durationDays=18},//setter
		{.humidty=850,.temperature=375,.durationDays=3},//hatcher
};
Prog_t Ordak_prog[2]={
		{.humidty=700,.temperature=375,.durationDays=18},//setter
		{.humidty=800,.temperature=375,.durationDays=3},//hatcher
};
Prog_t Kabk_prog[2]={
		{.humidty=570,.temperature=375,.durationDays=18},//setter
		{.humidty=750,.temperature=370,.durationDays=3},//hatcher
};
Prog_t Gharghavol_prog[2]={
		{.humidty=650,.temperature=366,.durationDays=18},//setter
		{.humidty=800,.temperature=372,.durationDays=3},//hatcher
};
Prog_t Belderchin_prog[2]={
		{.humidty=650,.temperature=377,.durationDays=18},//setter
		{.humidty=770,.temperature=372,.durationDays=3},//hatcher
};
Prog_t Boghalamoon_prog[2]={
		{.humidty=600,.temperature=375,.durationDays=18},//setter
		{.humidty=700,.temperature=375,.durationDays=3},//hatcher
};
Prog_t MorghShakhdar_prog[2]={
		{.humidty=600,.temperature=375,.durationDays=18},//setter
		{.humidty=700,.temperature=372,.durationDays=3},//hatcher
};

Bird_t	defaultBirds[NumOfBirds]={
	{.Type=Manual,				.TotalDurationDays=21,.HatchTotalDays=0,.NumofProg=2,.pProgs=Manual_prog,				},//Manual
	{.Type=Morgh,					.TotalDurationDays=21,.HatchTotalDays=3,.NumofProg=2,.pProgs=Morgh_prog,				},//Morgh
	{.Type=MorghShakhdar,	.TotalDurationDays=28,.HatchTotalDays=3,.NumofProg=2,.pProgs=MorghShakhdar_prog,},//MorghShakhdar
	{.Type=Boghalamoon,		.TotalDurationDays=28,.HatchTotalDays=3,.NumofProg=2,.pProgs=Boghalamoon_prog,	},//Boghalamoon
	{.Type=Belderchin,		.TotalDurationDays=17,.HatchTotalDays=3,.NumofProg=2,.pProgs=Belderchin_prog,		},//Belderchin
	{.Type=Gharghavol,		.TotalDurationDays=26,.HatchTotalDays=3,.NumofProg=2,.pProgs=Gharghavol_prog,		},//Gharghavol
	{.Type=Kabk,					.TotalDurationDays=24,.HatchTotalDays=3,.NumofProg=2,.pProgs=Kabk_prog,					},//Kabk
	{.Type=Ordak,					.TotalDurationDays=28,.HatchTotalDays=3,.NumofProg=2,.pProgs=Ordak_prog,				},//Ordak
	{.Type=Ghaz,					.TotalDurationDays=35,.HatchTotalDays=3,.NumofProg=2,.pProgs=Ghaz_prog,					},//Ghaz
	{.Type=ShotorMorgh,		.TotalDurationDays=42,.HatchTotalDays=3,.NumofProg=2,.pProgs=ShotorMorgh_prog,	},//ShotorMorgh
};

/*
*/
void BirdReadManual(Bird_t *bird)
{
	uint16_t add=EE_ADD_MANBIRD;
	bird->Type=Manual;
	bird->NumofProg=1;
	EEReadByte((uint8_t *)&(bird->TotalDurationDays),2,add);add+=2;
	EEReadByte((uint8_t *)&(bird->HatchTotalDays),2,add);add+=2;
	EEReadByte((uint8_t *)&(bird->pProgs[0].humidty),2,add);add+=2;
	EEReadByte((uint8_t *)&(bird->pProgs[0].temperature),2,add);add+=2;
	EEReadByte((uint8_t *)&(bird->pProgs[0].durationDays),2,add);add+=2;	
	EEReadByte((uint8_t *)&(bird->pProgs[1].humidty),2,add);add+=2;
	EEReadByte((uint8_t *)&(bird->pProgs[1].temperature),2,add);add+=2;
	EEReadByte((uint8_t *)&(bird->pProgs[1].durationDays),2,add);add+=2;	

}
void BirdSaveManual(Bird_t bird)
{
	uint16_t add=EE_ADD_MANBIRD;
	EEWriteByte((uint8_t *)&(bird.TotalDurationDays),2,add);add+=2;
	EEWriteByte((uint8_t *)&(bird.HatchTotalDays),2,add);add+=2;
	EEWriteByte((uint8_t *)&(bird.pProgs[0].humidty),2,add);add+=2;
	EEWriteByte((uint8_t *)&(bird.pProgs[0].temperature),2,add);add+=2;
	EEWriteByte((uint8_t *)&(bird.pProgs[0].durationDays),2,add);add+=2;	
	EEWriteByte((uint8_t *)&(bird.pProgs[1].humidty),2,add);add+=2;
	EEWriteByte((uint8_t *)&(bird.pProgs[1].temperature),2,add);add+=2;
	EEWriteByte((uint8_t *)&(bird.pProgs[1].durationDays),2,add);add+=2;	

}
void BirdInit(Bird_t *bird)
{
	EEReadByte((uint8_t *) &(bird->Type),1,EE_ADD_CURBIRDTYPE);
	if(bird->Type==Manual)
	{		
		BirdReadManual(&defaultBirds[Manual]);
	}
	memcpy((void  *)bird,(void  *)&defaultBirds[(uint8_t)(bird->Type)],sizeof(defaultBirds[(uint8_t)(bird->Type)]));
}