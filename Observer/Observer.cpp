// Observer.cpp 
#include <iostream>
#include "TScoreCard.h"
#include "TSubject.h"
#include "TBarGraphObserver.h"
#include "TLineGraphObserver.h"

TSubject totalScore;

int main()
{   
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
