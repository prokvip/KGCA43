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

void    Init(); // 데이터초기화
void    Reset();
TDATA*  NewData(int data);
void    Release();
void    AllDeleteData();
bool    push_back(TDATA* pPos, int data);
bool    push_front(TDATA* pPos, int data);
void    print(bool bForward);
void    ShowFL(TDATA* a, TDATA* b);
void    ShowLF(TDATA* a, TDATA* b);
TDATA*  Find(int iValue);
bool    Delete(int iValue);
bool    Delete(TDATA* pDel);
