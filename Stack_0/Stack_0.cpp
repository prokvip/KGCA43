/// Stack
#include <iostream>
#define MAX 3
// 구조체 변수 : 서로 다른 데이터형의 묶음으로 변수를 지정.
struct  TNode  // 구조체 변수
{
    char    m_csName;
    int     m_iAge;
    float   m_fData;
};
TNode  g_NodeArray[MAX];// 구조체 배열 선언
int   g_iTop = -1;

bool  push(TNode t)
{
    if (g_iTop >= MAX - 1)
    {        
        return false;
    }
    g_NodeArray[++g_iTop] = t;
    return true;
}
TNode*   pop()
{
    if (g_iTop < 0)
    {
        return NULL;
    }
    return &g_NodeArray[g_iTop--];
}

/// <summary>
/// 값이 복사되어 반환된다.
/// 새로운 반환 변수가 생성되서 값이 복사되어 반환된다.
/// </summary>
int g_iTemp = 1000;
int Fun()
{
    return g_iTemp;
}

/// <summary>
/// /
/// </summary>
/// <returns></returns>
int main()
{
    int  iRet=Fun();

    for (int i = 0; i < MAX+10; i++)
    {
        TNode node;
        node.m_csName = 65+i; // A=65, a=97
        node.m_iAge = i;
        node.m_fData = 3.14f * i;
        if (push(node) != true)
        {
            break;
        }
        std::cout << "Name=" << node.m_csName << " "
            << "Age=" << node.m_iAge << " "
                    << "float=" << node.m_fData << std::endl;        
    }
    TNode node;
    node.m_iAge = 0;
    node.m_fData = 9.9f;
    if (push(node)==false) //"Stack Overflow!
    {
        std::cout << "Stack Overflow!" << std::endl;
    }

    for (int i = 0; i < MAX+10; i++)
    {
        TNode* pNode = pop();
        if (pNode == NULL)
        {
            std::cout << "Stack Underflow!" << std::endl;
            break;
        }
        std::cout   << "Name=" << (*pNode).m_csName << " "
                    << "Age="  << pNode->m_iAge << " "
                    << "float=" << (*pNode).m_fData << std::endl;
    }
    TNode* nodeReturn = pop();//"Stack Underflow!
    if (nodeReturn == NULL)
    {
        std::cout << "Stack Underflow!\n";
    }
    std::cout << "Hello World!\n";
}
