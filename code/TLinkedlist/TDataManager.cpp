#include "TDataManager.h"

void TDataManager::ShowFL(TIterator<TCharacter>* a, TIterator<TCharacter>* b)
{
    if (a == nullptr) a = m_tList.begin();
    if (b == nullptr) b = a;

    printf("\n");
    TIterator<TCharacter>* pBegin = a;
    TIterator<TCharacter>* pEnd = b;
    do {
          if (pBegin != m_tList.head() && pBegin != m_tList.end())
        {
            pBegin->data->show();
        }
        pBegin = pBegin->pNext;
    } while (pBegin != b);
}
void TDataManager::ShowLF(TIterator<TCharacter>* a, TIterator<TCharacter>* b)
{
    if (a == nullptr) a = m_tList.end()->pPrev;
    if (b == nullptr) b = a;

    printf("\n");
    TIterator<TCharacter>* pBegin = a;
    TIterator<TCharacter>* pEnd = b;
    do {
        if (pBegin != m_tList.head() && pBegin != m_tList.end())
        {
            printf("%d ", pBegin->data);
        }
        pBegin = pBegin->pPrev;
    } while (pBegin != b);
}
void TDataManager::print(bool bforward)
{
    printf("\n");
    if (bforward)
    {
        ShowFL(m_tList.begin(), m_tList.end());
    }
    else
    {
        ShowLF(m_tList.end()->pPrev, m_tList.head());
    }
}
//error C4996 : 'fopen' : This function or variable may be unsafe.
//Consider using fopen_s instead.To disable deprecation, 
//use _CRT_SECURE_NO_WARNINGS.See online help for details.
void   TDataManager::Save()
{
    FILE* fp = NULL;
    errno_t errCode = fopen_s(&fp, "demo.txt", "w");
    if (errCode == 0)
    {
        TIterator<TCharacter>* pBegin = m_tList.begin();
        fprintf(fp, "%s = %d\n", "Total", m_tList.size());
        // 파일에 출력작업..
        do {
            fprintf(fp, "%d ", pBegin->data);
            pBegin = pBegin->pNext;
        } while (pBegin != m_tList.end());
        fclose(fp);
    }
}
int    TDataManager::Load()
{
    FILE* fp = NULL;
    errno_t errCode = fopen_s(&fp, "demo.txt", "r");
    if (errCode == 0)
    {
        int iLoadValue;
        int iTotalCounter = 0;
        char  strBuffer[256] = { 0, };
        fscanf(fp, "%s = %d\n", strBuffer, &iTotalCounter);
        for (int iCnt = 0; iCnt < iTotalCounter; iCnt++)
        {
            fscanf(fp, "%d", &iLoadValue);
            //TIterator<TCharacter>* newNode = new TIterator<TCharacter>();
            TCharacter* newItem = new TCharacter(iLoadValue);
            m_tList.push_front(m_tList.head(), 
                newItem);
        }
        fclose(fp);
        return true;
    }
    return false;
}
void TDataManager::NewData()
{
    TCharacter* pItem = new TCharacter(rand() % 100);
    m_tList.push_back(m_tList.end(), pItem);
}
void TDataManager::FileSave()
{
    Save();
}
void TDataManager::FileLoad()
{
    m_tList.AllDeleteData();
    Load();
}
void TDataManager::Update()
{
}
void TDataManager::DelData(int iData)
{

    m_tList.Delete(iData);
}
void TDataManager::sort()
{

}
void TDataManager::Sample()
{
    m_tList.AllDeleteData();
    for (int iData = 0; iData < 5; iData++)
    {
        TCharacter* pNewData = new TCharacter(rand() % 100);
        m_tList.push_back(m_tList.end(), pNewData);
    }
}
void TDataManager::AllDeleteData()
{
    m_tList.AllDeleteData();
}

TCharacter* TDataManager::NewData(int data)
{
    //Heap Memory
    TCharacter* pNewData = new TCharacter(data);
    if (pNewData == nullptr)
    {
        return nullptr;
    }
    return pNewData;
}