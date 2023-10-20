/// Stack
#include <iostream>
#define MAX 4
// 구조체 변수 : 서로 다른 데이터형의 묶음으로 변수를 지정.
struct  TNode  // 구조체 변수
{
    char    m_csName;
    int     m_iAge;
    float   m_fData;
};
TNode*  g_NodeArray[MAX];// 구조체 포인터 배열 선언
int   g_iTop = -1;
void  Print()
{
    for (int i = 0; i < MAX; i++)
    {
        TNode* node = g_NodeArray[i];
        std::cout << "Name=" << node->m_csName << " "
            << "Age=" << node->m_iAge << " "
            << "float=" << node->m_fData << std::endl;
    }
}
void  DeleteAll()
{
    for (int i = 0; i < MAX; i++)
    {
        TNode* node = g_NodeArray[i];
        delete node;// 동적할당된 메모리 소거
        node = nullptr;
    }
}
bool  push(TNode* t)
{
    if (g_iTop >= MAX - 1)
    {
        return false;
    }
    g_NodeArray[++g_iTop] = t;
    return true;
}
TNode* pop()
{
    if (g_iTop < 0)
    {
        return NULL;
    }
    return g_NodeArray[g_iTop--];
}


/// <summary>
/// /
/// </summary>
/// <returns></returns>
int main()
{   
    for (int i = 0; i < MAX + 10; i++)
    {
        TNode* node =  new TNode;// 동적할당
        node->m_csName = 65 + i; // A=65, a=97
        node->m_iAge = i;
        node->m_fData = 3.14f * i;
        if (push(node) != true)
        {
            break;
        }
    }
    Print();

    TNode node;
    node.m_iAge = 0;
    node.m_fData = 9.9f;
    if (push(&node) == false) //"Stack Overflow!
    {
        std::cout << "Stack Overflow!" << std::endl;
    }

    for (int i = 0; i < MAX + 10; i++)
    {
        TNode* pNode = pop();
        if (pNode == NULL)
        {
            std::cout << "Stack Underflow!" << std::endl;
            break;
        }
        std::cout << "Name=" << (*pNode).m_csName << " "
            << "Age=" << pNode->m_iAge << " "
            << "float=" << (*pNode).m_fData << std::endl;
    }
    TNode* nodeReturn = pop();//"Stack Underflow!
    if (nodeReturn == NULL)
    {
        std::cout << "Stack Underflow!\n";
    }

    DeleteAll();
    std::cout << "Hello World!\n";
}
