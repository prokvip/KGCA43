#include <stdio.h>
#include <iostream>
struct TDATA
{
    int    iValue;
    TDATA* pNext;
};

TDATA* g_pHead     = nullptr;
TDATA* g_pBeginData= nullptr;
TDATA* g_pEndData  = nullptr;



int top;
void Init(); // 데이터초기화
void Reset();
void NewData();
void Release();

void AllDeleteData();
bool push_back(int data);
bool push_front(int data);
bool pushback(int data);
bool pushfront(int data);
int  pop();
void print();

// link(&g_pEndData, &pNewData);
void  link(TDATA** a, TDATA** b)
{
    /*if (g_pBeginData == nullptr)
    {
        g_pBeginData = *b;
        g_pEndData = *b;
    }
    else*/
    {
        // 후위
        //(*a)->pNext = *b;
        //*a = *b;

        //전위
        (*a)->pNext = *b;
        *b = *a;
    }
}
int main()
{
    //int    id = 3;
    //int    pa;
    //int    pb;
    //pa = id;
    //pb = id;

    //TDATA  d;
    //TDATA* pA = nullptr;
    //TDATA* pB = nullptr;
    //pA = &d;
    //pB = &d;
   
    Init();

    // 후위연결리스트
    // TDATA(1,2* address) ->  TDATA(2, 3* add) -> TDATA(3, nullptr) 
    for (int iData = 0; iData < 3; iData++)
    {
        //push_back(iData);
        pushback(iData);
    }
    print();
    AllDeleteData();

    // 전위연결리스트
    // TDATA(2, 1 address) ->TDATA(1, 0 address) -> TDATA(0, nullptr) 
    for (int iData = 0; iData < 3; iData++)
    {
        //push_front(iData);
        pushfront(iData);
    }
    print();
    AllDeleteData();

    Release();
}

void Release()
{
    free(g_pHead);
    g_pHead = nullptr;
}
TDATA* NewData(int data)
{
    //Heap Memory
    TDATA* pNewData = (TDATA*)malloc(sizeof(TDATA));
    if (pNewData == nullptr)
    {
        return nullptr;
    }
    pNewData->iValue = data;
    pNewData->pNext = nullptr;
    return pNewData;
}
// 메모리 단편화
// [ 0 0 0 0 0 0 ] -> 6 -> 3byte
// [ 0 1 0 1 1 0 ] -> [ 1 1 1 0 0 0] 
void Init()
{    
     g_pHead = (TDATA*)malloc(sizeof(TDATA));
     memset(g_pHead, 0, sizeof(TDATA));

     Reset();
}
void Reset()
{
    g_pHead->pNext = nullptr;

    g_pBeginData = g_pHead;
    g_pEndData = g_pHead;
}
//bool Init()
//{    
//    g_pBeginData = (TDATA*)malloc(sizeof(TDATA));
//    if (g_pBeginData == nullptr)
//    {
//        return false;
//    }
//    return true;
//}
void AllDeleteData()
{
    //TDATA* pDelData  = g_pBeginData;
    //TDATA* pNextData = g_pBeginData;
    //while (pNextData)
    //{
    //    pDelData = pNextData;
    //    pNextData = pNextData->pNext;
    //    free(pDelData);
    //    pDelData = nullptr;
    //}
   
   TDATA* pNextData = g_pBeginData->pNext;
   for (TDATA* pBegin = g_pBeginData->pNext;
        pBegin != nullptr;
        pBegin = pNextData)
    {
        pNextData = pBegin->pNext;
        free(pBegin);
        pBegin = nullptr;
    }    
    
   

   Reset();
}
bool pushback(int data)
{
    TDATA* pNewData = NewData(data);

  /*  if (g_pBeginData == nullptr)
    {
        g_pBeginData = pNewData;
        g_pEndData = pNewData;
    }
    else*/
    {
        //link(&g_pEndData, &pNewData);
        g_pEndData->pNext = pNewData;
        g_pEndData = pNewData;
    }
    return true;
}
bool push_back(int data)
{
    TDATA* pNewData = NewData(data);  
    
    if (g_pBeginData == nullptr)
    {
        g_pBeginData = pNewData;    
        g_pEndData = pNewData;
    }
    else
    {
        //link(&g_pEndData, &pNewData);
        g_pEndData->pNext = pNewData; 
        g_pEndData = pNewData;
    }
    return true;  
}
bool push_front(int data)
{
    TDATA* pNewData = NewData(data);
    
    if (g_pBeginData == nullptr)
    {
        g_pBeginData = pNewData;
        g_pEndData   = pNewData;
    }
    else
    {
        //link(&pNewData, &g_pBeginData );
        pNewData->pNext = g_pBeginData;
        g_pBeginData = pNewData;        
    }   
    return true;   
}
bool pushfront(int data)
{
    TDATA* pNewData = NewData(data);
    {
        pNewData->pNext = g_pBeginData->pNext;
        g_pBeginData->pNext = pNewData;
    }
    return true;
}
int  pop()
{
    //_ASSERT(top > 0);
   /* if (top <= 0)
    {
        printf("\nStack Underflow!");
        printf(" %s:%d", __FUNCTION__, __LINE__);
        return -99999;
    }
    printf("\npop");
    return g_pBeginData[--top];*/
    return 1;
}
void print()
{
    for (TDATA* pBegin=  g_pBeginData->pNext;
         pBegin != nullptr;
         pBegin = pBegin->pNext)
    {
        printf("\n%d ", pBegin->iValue);
    }
}