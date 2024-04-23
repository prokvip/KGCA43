#pragma once
#include <stdio.h>
#include <iostream>
// (학생)관리프로그램
// 기능요소
// 초기화, 소멸(해제), 생성, 삭제, 추가(ADD,INSERT), 출력, 
// 검색, 수정, 정렬, 저장, 로드, 리셋, 복사...
struct TDATA
{
    int    iValue;
    TDATA* pNext; // 뒤로
    TDATA* pPrev; // 앞으로
};

//Internal Linkage:개별 "C++파일"에서만 전역으로 사용되는 전역변수이다.
static  int  g_iValue = 3; 
extern  TDATA* g_pHead;
extern  TDATA* g_pTail;
extern  int    g_iDataCounter;
void    Test();
//// 데이터초기화
void    Init(); // 데이터초기화
void    Reset();
TDATA*  NewData(int data);
void    Release();
void    AllDeleteData();
bool    push_back(TDATA* pPos, int data);
bool    push_front(TDATA* pPos, int data);
TDATA*  Find(int iValue);
bool    Delete(int iValue);
bool    Delete(TDATA* pDel);
