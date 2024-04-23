#include "Sample.h"
TDATA* g_pTemp = nullptr;
TDATA* g_pHead;
TDATA* g_pTail;

int    g_iDataCounter = 0;
void TitlePrint()
{
    printf("\n<<--------(점수)관리프로그램-------->>\n");
}
void MenuPrint()
{
    printf("\n출력(0),신규(1),저장(2),로드(3),수정(4),삭제(5),정렬(6),샘플용(9),종료(99)");
}
void main()
{
    int iSelect = 0;
    Init();
    Load();
    TitlePrint();
    while (1)
    {        
        MenuPrint();
        scanf("%d", &iSelect);
        if (iSelect == EXIT) break;
        system("cls");
        // 작업리스트
        switch (iSelect)
        {
        case SCREEN_PRINT:
        {        
            TitlePrint();
            print(true);
        }break;
        case NEW_DATA:
        {           
            push_back(g_pTail, rand()%100);
        }break;

        case FILE_SAVE:
        {          
            Save();
        }break;
        case FILE_LOAD:
        {        
            AllDeleteData();
            Load();
        }break;
        case DATA_UPDATE:
        {          
        }break;
        case DEL_DATA:
        {          
            printf("값을 입력하시오: ");
            int iValue;
            scanf_s("%d", &iValue);
            Delete(iValue);
        }break;
        case SORT_DATA:
        {           
        }break;
        case CREATE_SAMPLE_DATA:
        {         
            AllDeleteData();
            for (int iData = 0; iData < rand()%10; iData++)
            {
               push_back(g_pTail, rand() % 100);
            }
        }break;
        }
    }

    AllDeleteData();
    Release();
}
// 메모리 단편화
// [ 0 0 0 0 0 0 ] -> 6 -> 3byte
// [ 0 1 0 1 1 0 ] -> [ 1 1 1 0 0 0] 
//int main()
//{
//#ifdef _DEBUG
//   g_iValue = 9;
//#endif
//   Test();
//
//   Init();
//   if (Load())
//   {
//       print(false);
//       return 1;
//   }
//    // 후위연결리스트
//    // TDATA(1,2* address) ->  TDATA(2, 3* add) -> TDATA(3, nullptr) 
//    for (int iData = 0; iData < 3; iData++)
//    {
//        push_back(g_pTail,iData);
//    }
//    g_pTemp = Find(2);
//    push_back(g_pTemp, 4);
//    Delete(g_pTemp);
//    Delete(4);
//
//    /*ShowFL(nullptr, nullptr);
//    ShowFL(nullptr, g_pTail);
//    ShowFL(g_pTail, nullptr);
//    ShowFL(g_pHead->pNext, nullptr);
//    ShowFL(nullptr,g_pHead->pNext);
//    ShowFL(g_pHead->pNext, g_pTail );
//    ShowFL(g_pTail, g_pHead->pNext);
//    ShowFL(g_pHead, g_pHead->pNext);
//    ShowFL(g_pHead, g_pHead);*/
//
//    print(true);
//
//    for (int iData = 3; iData < 6; iData++)
//    {
//        push_front(g_pHead, iData);
//    }
//
//    Save();
//
//    AllDeleteData();
//
//
//    // file load
//    system("cls");
//    Load();
//    AllDeleteData();
//    Release();
//}

void ShowFL(TDATA* a, TDATA* b)
{
    if (a == nullptr ) a = g_pHead->pNext;
    if (b == nullptr ) b = a;

    printf("\n");
    TDATA* pBegin = a;
    TDATA* pEnd = b;
    do {
        if (pBegin != g_pHead && pBegin != g_pTail)
        {
            printf("%d ", pBegin->iValue);
        }
        pBegin = pBegin->pNext;
    } while (pBegin != b);
}
void ShowLF(TDATA* a, TDATA* b)
{
    if (a == nullptr) a = g_pTail->pPrev;
    if (b == nullptr) b = a;

    printf("\n");
    TDATA* pBegin = a;
    TDATA* pEnd = b;
    do {
        if (pBegin != g_pHead && pBegin != g_pTail)
        {
            printf("%d ", pBegin->iValue);
        }
        pBegin = pBegin->pPrev;
    } while (pBegin != b);
}
void print(bool bforward)
{
    printf("\n");
    if (bforward)
    {
        ShowFL(g_pHead->pNext, g_pTail);
    }
    else    
    {
        ShowLF(g_pTail->pPrev, g_pHead);
    }
}
//error C4996 : 'fopen' : This function or variable may be unsafe.
//Consider using fopen_s instead.To disable deprecation, 
//use _CRT_SECURE_NO_WARNINGS.See online help for details.
void    Save()
{
    FILE* fp = NULL;
    errno_t errCode = fopen_s( &fp, "demo.txt", "w");
    if (errCode == 0)
    {
        TDATA* pBegin = g_pHead->pNext;
        fprintf(fp, "%s = %d\n", "Total", g_iDataCounter);
        // 파일에 출력작업..
        do {            
                fprintf(fp, "%d ", pBegin->iValue);
                pBegin = pBegin->pNext;
        } while (pBegin != g_pTail);
        fclose(fp);
    } 
    //FILE* fp = fopen("demo.txt", "w");
    //if (fp != NULL)
    //{
    //    // 파일에 출력작업..
    //    fclose(fp);
    //}   
}
int    Load()
{
    FILE* fp = NULL;
    errno_t errCode = fopen_s(&fp, "demo.txt", "r");
    if (errCode == 0)
    {
        int iLoadValue;
        int iTotalCounter=0;
        char  strBuffer[256] = { 0, };
        fscanf(fp, "%s = %d\n", strBuffer, &iTotalCounter);
        for( int iCnt=0; iCnt < iTotalCounter; iCnt++)
        {
            fscanf(fp, "%d", &iLoadValue );
            push_front(g_pHead, iLoadValue);
        }
        fclose(fp);
        return true;
    }
    return false;
}
