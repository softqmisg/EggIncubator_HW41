#include "Bird.h"
#include "user_eeprom.h"
#include "eeprom.h"
#include <string.h>
//{Manual=0,Morgh,ShotorMorgh,Ghaz,Ordak,Kabk,Gharghavol,Belderchin,Boghalamoon,MorghShakhdar} BirdType_t;
Prog_t Manual_prog[2]={
		{.humidty=450,.temperature=370,.durationDays=18},//setter
		{.humidty=450,.temperature=370,.durationDays=3},//hatcher
};
Prog_t Morgh_prog[2]={
		{.humidty=450,.temperature=370,.durationDays=18},//setter
		{.humidty=450,.temperature=370,.durationDays=3},//hatcher
};
Prog_t ShotorMorgh_prog[2]={
		{.humidty=450,.temperature=370,.durationDays=18},//setter
		{.humidty=450,.temperature=370,.durationDays=3},//hatcher
};
Prog_t Ghaz_prog[2]={
		{.humidty=450,.temperature=370,.durationDays=18},//setter
		{.humidty=450,.temperature=370,.durationDays=3},//hatcher
};
Prog_t Ordak_prog[2]={
		{.humidty=450,.temperature=370,.durationDays=18},//setter
		{.humidty=450,.temperature=370,.durationDays=3},//hatcher
};
Prog_t Kabk_prog[2]={
		{.humidty=450,.temperature=370,.durationDays=18},//setter
		{.humidty=450,.temperature=370,.durationDays=3},//hatcher
};
Prog_t Gharghavol_prog[2]={
		{.humidty=450,.temperature=370,.durationDays=18},//setter
		{.humidty=450,.temperature=370,.durationDays=3},//hatcher
};
Prog_t Belderchin_prog[2]={
		{.humidty=450,.temperature=370,.durationDays=18},//setter
		{.humidty=450,.temperature=370,.durationDays=3},//hatcher
};
Prog_t Boghalamoon_prog[2]={
		{.humidty=450,.temperature=370,.durationDays=18},//setter
		{.humidty=450,.temperature=370,.durationDays=3},//hatcher
};
Prog_t MorghShakhdar_prog[2]={
		{.humidty=450,.temperature=370,.durationDays=18},//setter
		{.humidty=450,.temperature=370,.durationDays=3},//hatcher
};

Bird_t	defaultBirds[NumOfBirds]={
	{.Type=Manual,				.TotalDurationDays=21,.HatchTotalDays=3,.NumofProg=2,.pProgs=Manual_prog,				.curDateTime={0,0,0}},//Manual
	{.Type=Morgh,					.TotalDurationDays=21,.HatchTotalDays=3,.NumofProg=2,.pProgs=Morgh_prog,				.curDateTime={0,0,0}},//Morgh
	{.Type=ShotorMorgh,		.TotalDurationDays=21,.HatchTotalDays=3,.NumofProg=2,.pProgs=ShotorMorgh_prog,	.curDateTime={0,0,0}},//ShotorMorgh
	{.Type=Ghaz,					.TotalDurationDays=21,.HatchTotalDays=3,.NumofProg=2,.pProgs=Ghaz_prog,					.curDateTime={0,0,0}},//Ghaz
	{.Type=Ordak,					.TotalDurationDays=21,.HatchTotalDays=3,.NumofProg=2,.pProgs=Ordak_prog,				.curDateTime={0,0,0}},//Ordak
	{.Type=Kabk,					.TotalDurationDays=21,.HatchTotalDays=3,.NumofProg=2,.pProgs=Kabk_prog,					.curDateTime={0,0,0}},//Kabk
	{.Type=Gharghavol,		.TotalDurationDays=21,.HatchTotalDays=3,.NumofProg=2,.pProgs=Gharghavol_prog,		.curDateTime={0,0,0}},//Gharghavol
	{.Type=Belderchin,		.TotalDurationDays=21,.HatchTotalDays=3,.NumofProg=2,.pProgs=Belderchin_prog,		.curDateTime={0,0,0}},//Belderchin
	{.Type=Boghalamoon,		.TotalDurationDays=21,.HatchTotalDays=3,.NumofProg=2,.pProgs=Boghalamoon_prog,	.curDateTime={0,0,0}},//Boghalamoon
	{.Type=MorghShakhdar,	.TotalDurationDays=21,.HatchTotalDays=3,.NumofProg=2,.pProgs=MorghShakhdar_prog,.curDateTime={0,0,0}},//MorghShakhdar
};

/*
*/
void BirdReadManual(Bird_t *bird)
{
	uint16_t add=EE_ADD_MANBIRD;
	bird->Type=Manual;
	bird->NumofProg=2;
	EEReadByte((uint8_t *)&(bird->TotalDurationDays),2,add);add+=2;
	EEReadByte((uint8_t *)&(bird->HatchTotalDays),2,add);add+=2;
	EEReadByte((uint8_t *)&(bird->pProgs[0].humidty),2,add);add+=2;
	EEReadByte((uint8_t *)&(bird->pProgs[0].temperature),2,add);add+=2;
	EEReadByte((uint8_t *)&(bird->pProgs[0].durationDays),2,add);add+=2;	
	EEReadByte((uint8_t *)&(bird->pProgs[1].humidty),2,add);add+=2;		
	EEReadByte((uint8_t *)&(bird->pProgs[1].temperature),2,add);add+=2;		
	EEReadByte((uint8_t *)&(bird->pProgs[1].durationDays),2,add);	

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
	EEWriteByte((uint8_t *)&(bird.pProgs[1].durationDays),2,add);		
	
}
void BirdInit(Bird_t *bird)
{
	memcpy((void  *)bird,(void  *)&defaultBirds[(uint8_t)(bird->Type)],sizeof(defaultBirds[(uint8_t)(bird->Type)]));
}