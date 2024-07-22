#include "TAsyncSelect.h"
#include "TNetwork.h"
bool TAsyncSelect::Run()
{
	return true;
}
TAsyncSelect::TAsyncSelect(TNetwork* net) : TSelectModel(net->m_hSock)
{
	m_pNet = net;
}
TAsyncSelect::~TAsyncSelect()
{

}