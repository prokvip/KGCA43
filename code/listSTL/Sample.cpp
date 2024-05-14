#include "TDataManager.h"
#include <vector> // 
#include <list>
#include <iterator> 
#include <algorithm>

class TData
{
public:
	int   m_iValue = 5;
	TData(int i) : m_iValue(i) {}
};

void PRFun(int iValue)
{
	iValue += 100;
	std::cout << iValue << " ";
}
bool IntCmp(int iValue, int jValue)
{
	if (iValue > jValue)
	{
		return true;
	}
	return false;
}
bool findCmp(const int& iValue)
{
	if (iValue == 41)
	{
		return true;
	}
	return false;
}
int main()
{   
	std::vector<int> cmpList;
	cmpList.push_back(62);
	cmpList.push_back(58);
 	auto PR = [](auto& i) { std::cout << i << " "; };

 
#pragma region std::vector&std::algorithm
	using TDataVector = std::vector<TData*>;
	using TDataVectorIter = TDataVector::iterator;
	TDataVector tDataVector;
	for (int i = 0; i < 3; i++)
	{
		tDataVector.push_back(new TData(i + 1));
	}
	for (int i = 0; i < tDataVector.size(); i++)
	{
		std::cout << tDataVector[i]->m_iValue << " ";
	}
	TDataVector::iterator iterPoint;
	for (iterPoint = tDataVector.begin();
		iterPoint != tDataVector.end();
		/*iterPoint++*/)
	{
		delete *iterPoint;
		iterPoint = tDataVector.erase(iterPoint);
	}
	// 위험하다.(추가로 인하여 메모리 재할당이 되면)
	/*for (iterPoint = tDataVector.begin();
		iterPoint != tDataVector.end();
		iterPoint++)
	{
		if (rand()== 2) tDataVector.push_back(new TData(22));
		std::cout << *iterPoint;
	}
	for (int i= 0; i< tDataVector.size(); i++)
	{
		if(i ==2) tDataVector.push_back(new TData(22));		
		std::cout << tDataVector[i]->m_iValue;
	}*/
	tDataVector.clear();

		std::vector<int> vectorInt;
		vectorInt.reserve(10);
		for (int i = 0; i < 10; i++)
		{
			vectorInt.emplace_back(rand() % 100);
		}
		for (int i = 0; i < vectorInt.size(); i++)
		{
			std::cout << vectorInt[i] << " ";
		}
		std::for_each(std::begin(vectorInt),
			std::end(vectorInt), PRFun);
		std::cout << std::endl;
		std::for_each(std::begin(vectorInt),
			std::end(vectorInt), PR);


		std::sort(vectorInt.begin(), vectorInt.end(),
			IntCmp);
		std::cout << std::endl;
		std::for_each(std::begin(vectorInt),
			std::end(vectorInt), PR);

		std::sort(vectorInt.begin(), vectorInt.end(),
			std::less<>());// std::greater<>()
		std::cout << std::endl;
		for (int data : vectorInt)
		{
			std::cout << data << " ";
		}

		auto iter = std::find(vectorInt.begin(), vectorInt.end(),
			100);
		if (iter != vectorInt.end())
		{
			std::cout << *iter << std::endl;
		}

		iter = std::find_if(vectorInt.begin(), vectorInt.end(),
			[](auto& iValue)
			{
				if (iValue == 41)
				{
					return true;
				}
				return false;
			});//	findCmp);
		std::cout << *iter << std::endl;
		//0 24 34 41 58 62 64 67 69 78



		iter = std::search(vectorInt.begin(), vectorInt.end(),
			cmpList.begin(), cmpList.end());
		if (iter != vectorInt.end())
		{
			std::cout << "find" << std::endl;
		}
		else
		{
			std::cout << "failed" << std::endl;
		}


		iter = std::search(vectorInt.begin(), vectorInt.end(),
			cmpList.begin(), cmpList.end());
		if (iter != vectorInt.end())
		{
			std::cout << "find" << std::endl;
		}
		else
		{
			std::cout << "failed" << std::endl;
		}
#pragma endregion	

#pragma region std::list
		std::list<int> listInt;
		//listInt.reserve(10);
		for (int i = 0; i < 10; i++)
		{
			listInt.emplace_back(rand() % 100);
		}

		for (auto& data : listInt)
		{
			std::cout << data << " ";
		}

		std::for_each(std::begin(listInt),
			std::end(listInt), PRFun);
		std::cout << std::endl;
		std::for_each(std::begin(listInt),
			std::end(listInt), PR);

		listInt.sort();
		std::cout << std::endl;
		std::for_each(std::begin(listInt),
			std::end(listInt), PR);
		listInt.sort(std::greater<>());
		
		std::cout << std::endl;
		for (int data : listInt)
		{
			std::cout << data << " ";
		}

		auto iterList = std::find(listInt.begin(), listInt.end(),
			58);
		if (iterList != listInt.end())
		{
			std::cout << *iterList << std::endl;
		}

		iterList = std::find_if(listInt.begin(), listInt.end(),
			[](auto& iValue)
			{
				if (iValue == 41)
				{
					return true;
				}
				return false;
			});//	findCmp);
		if (iterList != listInt.end())
		{
			std::cout << *iterList << std::endl;
		}
		//0 24 34 41 58 62 64 67 69 78

		iterList = std::search(listInt.begin(), listInt.end(),
			cmpList.begin(), cmpList.end());
		if (iterList != listInt.end())
		{
			std::cout << "find" << std::endl;
		}
		else
		{
			std::cout << "failed" << std::endl;
		}


		iterList = std::search(listInt.begin(), listInt.end(),
			cmpList.begin(), cmpList.end());
		if (iterList != listInt.end())
		{
			std::cout << "find" << std::endl;
		}
		else
		{
			std::cout << "failed" << std::endl;
		}
#pragma endregion

		vectorInt.clear();
		listInt.clear();
}
