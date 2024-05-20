#include <iostream>
#include <set> 
#include <stack> 
#include <queue> 
#include <array> 
#include <map> 
#include <string> 
#include <list> 
#include <vector> 
#include <algorithm> 
struct TData
{
    int iValue;
    TData(const TData& a)
    {
        this->iValue = a.iValue;
    }
   /* bool operator <(const TData b)
    {
        return (iValue == b.iValue);
    }*/
    //bool operator() (TData& a, TData& b)
    //{
    //    return a.iValue < b.iValue;
    //}

    friend bool operator <(const TData& a, const TData& b);
    TData(int i) : iValue(i) {}
};

using TD = std::shared_ptr<TData>;

struct cmpPoint {
    bool operator () (TD& a, TD& b)
    {
        return a->iValue < b->iValue;
    }
    bool operator () (TData& a, TData& b)
    {
        return a.iValue < b.iValue;
    }
    bool operator () (TData* a, TData* b)
    {
        return a->iValue < b->iValue;
    }
};
bool operator <(const TData& a, const TData& b)
{
    return (a.iValue < b.iValue);
}

void PR(std::priority_queue<int>& q)
{
    while (!q.empty())
    {
        std::cout << q.top() << " ";
        q.pop();
    }
}
void PR(std::priority_queue<TData>& q)
{
    while (!q.empty())
    {
        std::cout << q.top().iValue << " ";        
        q.pop();
    }
}
void PR(std::priority_queue<TData*>& q)
{
    while (!q.empty())
    {
        std::cout << q.top() << std::endl;
        delete q.top();
        q.pop();
    }
}
int main()
{   
    ////
    std::priority_queue<TData, std::vector<TData>, cmpPoint> qDataList2;
    for (int n : {3, 5, 2, 6, 2, 6, 2, 667})
    {
        qDataList2.push(n);
    }
    std::cout << qDataList2.top().iValue << " ";
    qDataList2.pop();
    qDataList2.push(100);
    std::cout << qDataList2.top().iValue << " ";
    qDataList2.pop();
    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>

    auto lamba = [](TData* a, TData* b)-> bool { 
        return  (a->iValue < b->iValue); 
    };
    
    //std::priority_queue<TData*, std::vector<TData*>, decltype(lamba) > qDataList(lamba);
    std::priority_queue<TD, std::vector<TD>, cmpPoint> qDataList;
     
    //std::priority_queue<TData, std::vector<TData>>  qDataList;
    for (int n : {3, 5, 2, 6, 2, 6, 2, 667})
    {
        qDataList.push(std::make_shared<TData>(n));
    }
   
    std::cout << qDataList.top()->iValue << " ";
    qDataList.pop();

    qDataList.push(std::make_shared<TData>(100));
    std::cout << qDataList.top()->iValue << " ";
    qDataList.pop();

    //auto lamba = [](int a, int b)-> bool { return  (a < b); };
    //std::priority_queue<int, std::vector<int>, decltype(lamba) > qList(lamba);
   // std::priority_queue<int, std::vector<int>, std::less<int>> qList;
    std::priority_queue<int> qList;
    for (int n : {3, 5, 2, 6, 2, 6, 2, 667})
    {
        qList.push(n);
    }
    //PR(qList);
    std::cout << qList.top() << " ";
    qList.pop();

    qList.push(100);
    std::cout << qList.top() << " ";
    qList.pop();
    //PR(qList);
    /// <summary>
    /// heap
    /// </summary>
    /// <returns></returns>
    std::vector<int> list{1,2,4,65,32,2,43,54};    
    std::make_heap(list.begin(), list.end(), std::greater<>());
    std::pop_heap(list.begin(), list.end());
    auto top = list.back();
    list.pop_back();

    list.push_back(70);
    std::push_heap(std::begin(list), std::end(list));

    for (int i = list.size(); i > 0; --i)
    {
        std::pop_heap(list.begin(), list.end());
        auto top = list.back();
        list.pop_back();
        std::cout << top << " ";
    }

    std::set<TData> setlist;
    TData a(2);
    //error C2676: 이항 '<': 'const _Ty'이(가) 이 연산자를 정의하지 않거나 미리 정의된 연산자에 허용되는 형식으로의 변환을 정의하지 않습니다.
    setlist.insert(a);
    setlist.emplace( 1 );
    setlist.emplace(3);
    setlist.emplace(8);
    setlist.emplace(8);
    setlist.emplace(4);
    setlist.emplace(1);
    setlist.emplace(1);
    setlist.emplace(9);
    setlist.emplace(2);

    std::multiset<TData> setmultilist;
    setmultilist.insert(3);
    setmultilist.insert(3);
    setmultilist.insert(8);
    setmultilist.insert(8);
    setmultilist.insert(4);
    setmultilist.insert(1);
    setmultilist.insert(1);
    setmultilist.insert(9);
    setmultilist.insert(2);

   /* std::set<int> setlist;
    setlist.insert(3);
    setlist.insert(3);
    setlist.insert(8);
    setlist.insert(8);
    setlist.insert(4);
    setlist.insert(1);
    setlist.insert(1);
    setlist.insert(9);
    setlist.insert(2);

    std::multiset<int> setmultilist;
    setmultilist.insert(3);
    setmultilist.insert(3);
    setmultilist.insert(8);
    setmultilist.insert(8);
    setmultilist.insert(4);
    setmultilist.insert(1);
    setmultilist.insert(1);
    setmultilist.insert(9);
    setmultilist.insert(2);*/

    std::cout << "Hello World!\n";
}
