/// Queue
#include <iostream>
#define MAX 10
struct  TNode  // 구조체 변수
{
    char    m_csName;
    int     m_iAge;
    float   m_fData;
};
TNode  g_iQuqueArray[MAX];// 구조체 배열 선언
int   g_iIn = 0;
int   g_iOut = 0;

void  push(TNode t)
{
    // 항상 1개의 방은 나머지가 존재한다.
    if ((g_iIn + 1) % MAX == g_iOut)
    {
        std::cout << "Queue Overflow!\n";
        return;
    }
    g_iQuqueArray[g_iIn] = t;
    g_iIn = ++g_iIn % MAX;
}
TNode*   pop()
{
    if (g_iIn == g_iOut)
    {
        std::cout << "Stack Underflow!\n";
        return nullptr;
    }
    TNode* iData = &g_iQuqueArray[g_iOut];
    g_iOut = ++g_iOut % MAX;
    return iData;
}
int main()
{
    for (int i = 0; i < MAX - 1; i++)
    {
        TNode node;
        node.m_csName = 65 + i; // A=65, a=97
        node.m_iAge = i;
        node.m_fData = 3.14f * i;
        push(node);
    }

    TNode node;
    node.m_csName = 65 + 5; // A=65, a=97
    node.m_iAge = 5;
    node.m_fData = 3.14f * 5;
    push(node);//"Stack Overflow!

    for (int i = 0; i < MAX - 1; i++)
    {
        TNode* pNode = pop();
        if (pNode != nullptr)
        {
            std::cout << "Name=" << (*pNode).m_csName << " "
                << "Age=" << pNode->m_iAge << " "
                << "float=" << (*pNode).m_fData << std::endl;

        }       
    }
    TNode* pNode  = pop();//"Stack Underflow!

    std::cout << "Hello World!\n";
}
