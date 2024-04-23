#pragma once
#include <stdio.h>
#include <iostream>
// (�л�)�������α׷�
// ��ɿ��
// �ʱ�ȭ, �Ҹ�(����), ����, ����, �߰�(ADD,INSERT), ���, 
// �˻�, ����, ����, ����, �ε�, ����, ����...
struct TDATA
{
    int    iValue;
    TDATA* pNext; // �ڷ�
    TDATA* pPrev; // ������
};

//Internal Linkage:���� "C++����"������ �������� ���Ǵ� ���������̴�.
static  int  g_iValue = 3; 
extern  TDATA* g_pHead;
extern  TDATA* g_pTail;
extern  int    g_iDataCounter;
void    Test();
//// �������ʱ�ȭ
void    Init(); // �������ʱ�ȭ
void    Reset();
TDATA*  NewData(int data);
void    Release();
void    AllDeleteData();
bool    push_back(TDATA* pPos, int data);
bool    push_front(TDATA* pPos, int data);
TDATA*  Find(int iValue);
bool    Delete(int iValue);
bool    Delete(TDATA* pDel);
