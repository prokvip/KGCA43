#pragma once
#include <iostream>
#include <vector>
#include <list>
// 1)관찰자 - 이벤트를 감시하고 있다가 변화가 생기면 알려주는 역활.
// 2)       - 이벤트를 통지 받았을 때 동일한 데이터를 여러가지 형태로 표현하는 기능.
class TObserver
{
public:
	virtual void update() = 0;
};

