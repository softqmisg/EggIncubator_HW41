#include "Bird.h"
#include "user_eeprom.h"
#include "eeprom.h"
#include <string.h>
Prog_t Manual_prog[2]={
		{.humidity=600,.temperature=377,.durationDays=21},//setter
		{.humidity=600,.temperature=377,.durationDays=0}//hatcher

};
Prog_t Morgh_prog[2]={
		{.humidity=550,.temperature=377,.durationDays=18},//setter
		{.humidity=700,.temperature=372,.durationDays=3}//hatcher
};
Prog_t MorghShakhdar_prog[2]={
		{.humidity=550,.temperature=377,.durationDays=23},//setter
		{.humidity=700,.temperature=372,.durationDays=3}//hatcher
};
//Prog_t Boghalamoon_prog[4]={
//		{.humidity=550,.temperature=380,.durationDays=7},//setter1
//		{.humidity=520,.temperature=377,.durationDays=7},//setter2
//		{.humidity=600,.temperature=375,.durationDays=11},//setter3
//		{.humidity=700,.temperature=372,.durationDays=3}//hatcher
//};
Prog_t Boghalamoon_prog[2]={
		{.humidity=550,.temperature=370,.durationDays=25},//setter
		{.humidity=700,.temperature=372,.durationDays=3}//hatcher
};

Prog_t Belderchin_prog[2]={
		{.humidity=550,.temperature=377,.durationDays=14},//setter
		{.humidity=700,.temperature=372,.durationDays=3}//hatcher
};
Prog_t Gharghavol_prog[2]={
		{.humidity=550,.temperature=377,.durationDays=22},//setter
		{.humidity=700,.temperature=372,.durationDays=3}//hatcher
};
Prog_t Kabk_prog[2]={
		{.humidity=550,.temperature=377,.durationDays=21},//setter
		{.humidity=700,.temperature=372,.durationDays=3}//hatcher
};
Prog_t Ordak_prog[2]={
		{.humidity=600,.temperature=377,.durationDays=25},//setter
		{.humidity=700,.temperature=372,.durationDays=3}//hatcher
};
//Prog_t Ghaz_prog[4]={
//		{.humidity=600,.temperature=380,.durationDays=7},//setter1
//		{.humidity=580,.temperature=378,.durationDays=7},//setter2
//		{.humidity=620,.temperature=375,.durationDays=11},//setter3
//		{.humidity=750,.temperature=372,.durationDays=3}//hatcher
//};
Prog_t Ghaz_prog[2]={
		{.humidity=600,.temperature=377,.durationDays=29},//setter
		{.humidity=700,.temperature=372,.durationDays=3}//hatcher
};
Prog_t ShotorMorgh_prog[2]={
		{.humidity=250,.temperature=363,.durationDays=39},//setter
		{.humidity=300,.temperature=366,.durationDays=3}//hatcher
};
//Bird_t	defaultBirds[NumOfBirds]={
//	{.Type=Manual,				.TotalDurationDays=21,.HatchTotalDays=0,.NumofProg=2,.pProgs=Manual_prog},//Manual
//	{.Type=Morgh,					.TotalDurationDays=21,.HatchTotalDays=3,.NumofProg=2,.pProgs=Morgh_prog},//Morgh
//	{.Type=MorghShakhdar,	.TotalDurationDays=28,.HatchTotalDays=3,.NumofProg=2,.pProgs=MorghShakhdar_prog},//MorghShakhdar
//	{.Type=Boghalamoon,		.TotalDurationDays=28,.HatchTotalDays=3,.NumofProg=4,.pProgs=Boghalamoon_prog},//Boghalamoon
//	{.Type=Belderchin,		.TotalDurationDays=17,.HatchTotalDays=3,.NumofProg=2,.pProgs=Belderchin_prog},//Belderchin
//	{.Type=Gharghavol,		.TotalDurationDays=26,.HatchTotalDays=3,.NumofProg=2,.pProgs=Gharghavol_prog},//Gharghavol
//	{.Type=Kabk,					.TotalDurationDays=24,.HatchTotalDays=3,.NumofProg=2,.pProgs=Kabk_prog},//Kabk
//	{.Type=Ordak,					.TotalDurationDays=28,.HatchTotalDays=3,.NumofProg=2,.pProgs=Ordak_prog},//Ordak
//	{.Type=Ghaz,					.TotalDurationDays=28,.HatchTotalDays=3,.NumofProg=4,.pProgs=Ghaz_prog},//Ghaz
//	{.Type=ShotorMorgh,		.TotalDurationDays=42,.HatchTotalDays=3,.NumofProg=2,.pProgs=ShotorMorgh_prog}//ShotorMorgh
//};
Bird_t	defaultBirds[NumOfBirds]={
	{.Type=Manual,				.TotalDurationDays=21,.HatchTotalDays=0,.NumofProg=2 },//Manual
	{.Type=Morgh,					.TotalDurationDays=21,.HatchTotalDays=3,.NumofProg=2},//Morgh
	{.Type=MorghShakhdar,	.TotalDurationDays=28,.HatchTotalDays=3,.NumofProg=2},//MorghShakhdar
	{.Type=Boghalamoon,		.TotalDurationDays=28,.HatchTotalDays=3,.NumofProg=4},//Boghalamoon
	{.Type=Belderchin,		.TotalDurationDays=17,.HatchTotalDays=3,.NumofProg=2},//Belderchin
	{.Type=Gharghavol,		.TotalDurationDays=26,.HatchTotalDays=3,.NumofProg=2},//Gharghavol
	{.Type=Kabk,					.TotalDurationDays=24,.HatchTotalDays=3,.NumofProg=2},//Kabk
	{.Type=Ordak,					.TotalDurationDays=28,.HatchTotalDays=3,.NumofProg=2},//Ordak
	{.Type=Ghaz,					.TotalDurationDays=28,.HatchTotalDays=3,.NumofProg=4},//Ghaz
	{.Type=ShotorMorgh,		.TotalDurationDays=42,.HatchTotalDays=3,.NumofProg=2}//ShotorMorgh
};
/*
*/
void BirdLoadDefault(void )
{
	memcpy((void *)defaultBirds[(uint8_t)Manual].pProgs,(void *)Manual_prog,sizeof(Prog_t)*defaultBirds[(uint8_t)Manual].NumofProg);
	memcpy((void *)defaultBirds[(uint8_t)Morgh].pProgs,(void *)Morgh_prog,sizeof(Prog_t)*defaultBirds[(uint8_t)Morgh].NumofProg);
	memcpy((void *)defaultBirds[(uint8_t)MorghShakhdar].pProgs,(void *)MorghShakhdar_prog,sizeof(Prog_t)*defaultBirds[(uint8_t)MorghShakhdar].NumofProg);
	memcpy((void *)defaultBirds[(uint8_t)Boghalamoon].pProgs,(void *)Boghalamoon_prog,sizeof(Prog_t)*defaultBirds[(uint8_t)Boghalamoon].NumofProg);
	memcpy((void *)defaultBirds[(uint8_t)Belderchin].pProgs,(void *)Belderchin_prog,sizeof(Prog_t)*defaultBirds[(uint8_t)Belderchin].NumofProg);
	memcpy((void *)defaultBirds[(uint8_t)Gharghavol].pProgs,(void *)Gharghavol_prog,sizeof(Prog_t)*defaultBirds[(uint8_t)Gharghavol].NumofProg);
	memcpy((void *)defaultBirds[(uint8_t)Kabk].pProgs,(void *)Kabk_prog,sizeof(Prog_t)*defaultBirds[(uint8_t)Kabk].NumofProg);
	memcpy((void *)defaultBirds[(uint8_t)Ordak].pProgs,(void *)Ordak_prog,sizeof(Prog_t)*defaultBirds[(uint8_t)Ordak].NumofProg);
	memcpy((void *)defaultBirds[(uint8_t)Ghaz].pProgs,(void *)Ghaz_prog,sizeof(Prog_t)*defaultBirds[(uint8_t)Ghaz].NumofProg);
	memcpy((void *)defaultBirds[(uint8_t)ShotorMorgh].pProgs,(void *)ShotorMorgh_prog,sizeof(Prog_t)*defaultBirds[(uint8_t)ShotorMorgh].NumofProg);
}
/*
*/
void BirdReadManual(Bird_t *bird)
{
	uint16_t add=EE_ADD_MANBIRD;
	bird->Type=Manual;
	bird->NumofProg=2;
	EEReadByte((uint8_t *)&(bird->TotalDurationDays),2,add);add+=2;
	EEReadByte((uint8_t *)&(bird->HatchTotalDays),2,add);add+=2;
	EEReadByte((uint8_t *)&(bird->pProgs[0].humidity),2,add);add+=2;
	EEReadByte((uint8_t *)&(bird->pProgs[0].temperature),2,add);add+=2;
	EEReadByte((uint8_t *)&(bird->pProgs[0].durationDays),2,add);add+=2;	
	EEReadByte((uint8_t *)&(bird->pProgs[1].humidity),2,add);add+=2;
	EEReadByte((uint8_t *)&(bird->pProgs[1].temperature),2,add);add+=2;
	EEReadByte((uint8_t *)&(bird->pProgs[1].durationDays),2,add);add+=2;	

}
void BirdSaveManual(Bird_t bird)
{
	uint16_t add=EE_ADD_MANBIRD;
	EEWriteByte((uint8_t *)&(bird.TotalDurationDays),2,add);add+=2;
	EEWriteByte((uint8_t *)&(bird.HatchTotalDays),2,add);add+=2;
	EEWriteByte((uint8_t *)&(bird.pProgs[0].humidity),2,add);add+=2;
	EEWriteByte((uint8_t *)&(bird.pProgs[0].temperature),2,add);add+=2;
	EEWriteByte((uint8_t *)&(bird.pProgs[0].durationDays),2,add);add+=2;
	EEWriteByte((uint8_t *)&(bird.pProgs[1].humidity),2,add);add+=2;
	EEWriteByte((uint8_t *)&(bird.pProgs[1].temperature),2,add);add+=2;
	EEWriteByte((uint8_t *)&(bird.pProgs[1].durationDays),2,add);add+=2;

}
void BirdInit(Bird_t *bird)
{
	uint8_t type;
	EEReadByte((uint8_t *) &type,1,EE_ADD_CURBIRDTYPE);
	bird->Type=(BirdType_t)type;
	if(bird->Type==Manual)
	{		
		BirdReadManual(&defaultBirds[Manual]);
	}
	//memcpy((void  *)bird,(void  *)&defaultBirds[type],sizeof(defaultBirds[type]));
	
//	
	bird->TotalDurationDays=defaultBirds[type].TotalDurationDays;
	bird->HatchTotalDays=defaultBirds[type].HatchTotalDays;
	bird->NumofProg=defaultBirds[type].NumofProg;
//	memcpy((void *)bird->pProgs,(void *)defaultBirds[type].pProgs,sizeof(Prog_t)*(bird->NumofProg));
	for(uint8_t i=0;i<bird->NumofProg;i++)
	{
		bird->pProgs[i].durationDays=defaultBirds[type].pProgs[i].durationDays;
		bird->pProgs[i].humidity=defaultBirds[type].pProgs[i].humidity;
		bird->pProgs[i].temperature=defaultBirds[type].pProgs[i].temperature;
	}
	
}