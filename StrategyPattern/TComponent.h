#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
class TComponent
{
public:
    virtual void   Operation() {};
    virtual void   Add(TComponent* com) {};
    virtual void   Remove(TComponent* com) {};
    virtual TComponent* GetChild(int index) { return nullptr; };
};

