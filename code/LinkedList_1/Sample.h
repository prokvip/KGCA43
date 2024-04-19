#pragma once
#include <stdio.h>
#include <iostream>
struct TDATA
{
    int    iValue;
    TDATA* pNext; // 뒤로
    TDATA* pPrev; // 앞으로
};

TDATA* g_pHead = nullptr;
TDATA* g_pTail = nullptr;
TDATA* g_pEndData = nullptr;

void    Init(); // 데이터초기화
void    Reset();
TDATA*  NewData(int data);
void    Release();
void    AllDeleteData();
bool    push_back(int data);
bool    push_front(int data);
void    print(bool bForward);
void    Show(TDATA* a, TDATA* b);