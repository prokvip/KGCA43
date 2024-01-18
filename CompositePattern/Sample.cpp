// StrategyPattern.cpp 
#include "TContext.h"
#include "TStrategyA.h"
#include "TStrategyB.h"
#include "TStrategyC.h"
int main()
{
    TStrategyA a;
    TStrategyB b;
    TStrategyC c;
    TContext con1(&b);
    con1.operation();

    TContext con2;
    con2.operation(&c);

    TContext con3;
    con3.Set(&a);
    con3.operation();
    //TStrategyA sA;
    //sA.Operation();
    std::cout << "Hello World!\n";
}
