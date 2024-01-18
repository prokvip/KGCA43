#include "TComposite.h"
void   TComposite::Operation()
{
	std::cout << "TComposite Begin!\n";
	for (auto data : m_list)
	{
		data->Operation();
	}
	std::cout << "TComposite End!\n";
};
void   TComposite::Add(TComponent* com)
{
	m_list.push_back(com);
};
void   TComposite::Remove(TComponent* com)
{
	std::vector<TComponent*>::iterator iter =
		std::find(std::begin(m_list),
			std::end(m_list), com);
	if( iter != m_list.end())
	{ 
		m_list.erase(iter);
	}	
};
TComponent* TComposite::GetChild(int index)
{
	return m_list[index];
};