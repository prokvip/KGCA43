#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "LinkedList.h"

enum WORK {
	SCREEN_PRINT = 0,
	NEW_DATA,
	FILE_SAVE,
	FILE_LOAD,
	DATA_UPDATE,
	DEL_DATA,
	SORT_DATA,
	CREATE_SAMPLE_DATA = 9,
	EXIT = 99
};

void    print(bool bForward);
void    ShowFL(TDATA* a, TDATA* b);
void    ShowLF(TDATA* a, TDATA* b);
void    Save();
int     Load();
//1 > Sample.obj : error LNK2005 : "struct TDATA * g_pHead" (? g_pHead@@3PEAUTDATA@@EA)이(가) LinkedList.obj에 이미 정의되어 있습니다.
//1 > Sample.obj : error LNK2005 : "struct TDATA * g_pTail" (? g_pTail@@3PEAUTDATA@@EA)이(가) LinkedList.obj에 이미 정의되어 있습니다.
