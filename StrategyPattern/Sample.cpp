// StrategyPattern.cpp 
#include "TContext.h"
#include "TCircleLeaf.h"
#include "TLineLeaf.h"
#include "TRectangleLeaf.h"
#include "TComposite.h"
int main()
{
    TCircleLeaf a;
    TLineLeaf b;
    TRectangleLeaf c;
    

    TContext con1(&b);
    con1.operation();

    TContext con2;
    con2.operation(&c);

    TContext con3;
    con3.Set(&a);
    con3.operation();


    TComposite d;
    d.Add(&a);
    d.Add(&b);
    d.Add(&c);    
    TContext con4;
    con4.Set(&d);
    con4.operation();


    TComposite e;
    e.Add(&d);
    e.Add(&c);    
    TContext con5;
    con5.Set(&e);
    con5.operation();
    std::cout << "Hello World!\n";
}
