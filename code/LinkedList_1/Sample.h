#pragma once
#include <stdio.h>
#include <iostream>
struct TDATA
{
    int    iValue;
    TDATA* pNext; // �ڷ�
    TDATA* pPrev; // ������
};

TDATA* g_pHead = nullptr;
TDATA* g_pTail = nullptr;
TDATA* g_pEndData = nullptr;

void    Init(); // �������ʱ�ȭ
void    Reset();
TDATA*  NewData(int data);
void    Release();
void    AllDeleteData();
bool    push_back(int data);
bool    push_front(int data);
void    print(bool bForward);
void    Show(TDATA* a, TDATA* b);