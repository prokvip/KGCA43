// Observer.cpp 
#include <iostream>
#include "TScoreCard.h"
#include "TSubject.h"
#include "TBarGraphObserver.h"
#include "TLineGraphObserver.h"


void LegacyDeleterNoneFun(TSubject* p)
{
    //delete p;
};
template<class T>
void LegacyDeleterNoneFunTemplate2(T* p)
{
    //delete p;
}
void LegacyDeleterFun(TSubject* p)
{    
    delete p;
};
struct LegacyDeleterTemplate
{
    template<typename T>
    void operator()(T* p) 
    {        
        delete p;
    }
};
struct LegacyDeleterNoneFunTemplate
{
    template<typename T>
    void operator()(T* p)
    {
        delete p;
    }
};
struct LegacyDeleter
{
    LegacyDeleter()
    {
        std::wcout << L"LegacyDeleter" << std::endl;
    }
    ~LegacyDeleter()
    {
        std::wcout << L"~LegacyDeleter" << std::endl;
    }
    void operator()(TSubject* p)
    {
        delete p;
    }
};
int main()
{   
    TSubject totalScore;
    //{
    //    //template< class T > struct default_delete;
    //    std::shared_ptr<TSubject> newData1
    //        = std::make_shared<TSubject>();
    //}
    //
    {
        TSubject* newData = new TSubject;
        std::shared_ptr<TSubject> newData1(newData, LegacyDeleterFun);
    }
    {        
        TSubject* newData = new TSubject;
        std::shared_ptr<TSubject> newData1(newData, LegacyDeleter());
    }
    {
        TSubject* newData = new TSubject;
        std::shared_ptr<TSubject> newData1(newData, LegacyDeleterTemplate());
    }
    {
        TSubject* newData = new TSubject;
        std::shared_ptr<TSubject> newData1(newData);
    }
    {
        TSubject* newData = new TSubject;
        // error C2661: 'std::shared_ptr<TSubject>::shared_ptr': 오버로드된 함수에서 2개의 인수를 사용하지 않습니다.
        std::shared_ptr<TSubject> newData1(newData, TSubject());
    }
    {
        TSubject* newData = new TSubject;
        std::shared_ptr<TSubject> newData1(newData, 
            [](TSubject* p) { 
                delete p; 
            });
    }
    {
        TSubject data;
        //error
        //std::shared_ptr<TSubject> newData(&data, LegacyDeleterNoneFunTemplate2);

        TSubject* totalScore1 = new TSubject;       
        std::shared_ptr<TSubject> newData1(totalScore1, LegacyDeleterNoneFun);

        TSubject* totalScore2 = new TSubject;
        std::shared_ptr<TSubject> newData2(totalScore2, LegacyDeleterNoneFunTemplate());

        std::shared_ptr<TSubject> newData3(&totalScore, LegacyDeleterNoneFun);
    }
    TBarGraphObserver  barGraph(&totalScore);
    TLineGraphObserver lineGraph(&totalScore);
    barGraph.print();
    lineGraph.print();

    TScoreCard s1, s2, s3;
    s1.name = L"aaa";
    s1.iKorScore = 50;
    s1.iEngScore = 60;
    s1.iMatScore = 70;
    totalScore.AddScore(&s1); 
    barGraph.print();
    lineGraph.print();


    s2.name = L"bbb";
    s2.iKorScore = 10;
    s2.iEngScore = 20;
    s2.iMatScore = 30;
    totalScore.AddScore(&s2);
   
    
    barGraph.print();
    lineGraph.print();
    std::cout << "Hello World!\n";
}
