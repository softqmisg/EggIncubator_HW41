#include "Keys.h"
Keys_t Keys[KEYSNUM];
void KeysInit()
{
	Keys[KEYUP].counter=0;
	Keys[KEYUP].LongPress=0;
	Keys[KEYUP].ShortPress=0;
	Keys[KEYUP].LongLongPress=0;
	Keys[KEYUP].RawState=Keys[KEYUP].Prev_RawState=Release;
	Keys[KEYUP].port=KEYUP_PORT;
	Keys[KEYUP].pin=KEYUP_PIN;
	
	Keys[KEYDOWN].counter=0;
	Keys[KEYDOWN].LongPress=0;
	Keys[KEYDOWN].ShortPress=0;
	Keys[KEYDOWN].LongLongPress=0;
	Keys[KEYDOWN].RawState=Keys[KEYDOWN].Prev_RawState=Release;
	Keys[KEYDOWN].port=KEYDOWN_PORT;
	Keys[KEYDOWN].pin=KEYDOWN_PIN;

	Keys[KEYSETTING].counter=0;
	Keys[KEYSETTING].LongPress=0;
	Keys[KEYSETTING].ShortPress=0;
	Keys[KEYSETTING].LongLongPress=0;
	Keys[KEYSETTING].RawState=Keys[KEYSETTING].Prev_RawState=Release;
	Keys[KEYSETTING].port=KEYSETTING_PORT;
	Keys[KEYSETTING].pin=KEYSETTING_PIN;

	Keys[KEYLEDBUZZER].counter=0;
	Keys[KEYLEDBUZZER].LongPress=0;
	Keys[KEYLEDBUZZER].ShortPress=0;
	Keys[KEYLEDBUZZER].LongLongPress=0;
	Keys[KEYLEDBUZZER].RawState=Keys[KEYLEDBUZZER].Prev_RawState=Release;
	Keys[KEYLEDBUZZER].port=KEYLEDBUZZER_PORT;
	Keys[KEYLEDBUZZER].pin=KEYLEDBUZZER_PIN;
	
}