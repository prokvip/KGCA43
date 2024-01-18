#pragma once
#include "TComponent.h"
class TComposite :public TComponent
{
    std::vector< TComponent*> m_list;
public:
    virtual void   Operation();
    virtual void   Add(TComponent* com);
    virtual void   Remove(TComponent* com);
    virtual TComponent* GetChild(int index);
};

