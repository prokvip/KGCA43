#include "TCore.h"
class Sample : public TCore
{
public:
	// �ʱ�ȭ �۾�
	virtual void Init() override
	{
		int k = 0;
	}
	virtual void Frame()override
	{
		int k = 0;
	}
	virtual void Render()override
	{
		int k = 0;
	}
	// �Ҹ� �۾�
	virtual void Release()override
	{
		int k = 0;
	}
};

int WINAPI wWinMain( HINSTANCE hInstance,
					HINSTANCE hPrevInstance, 
					PWSTR pCmdLine, 
					int nCmdShow)
{

	Sample win;
	win.CreateRegisterClass(hInstance);
	if (win.CreateWin(1024, 768))
	{
		// �׷��� ó���� ���� �ʱ�ȭ �۾�
		if (win.CreateDevice(win.m_hWnd))
		{
			// ������
			win.WindowRun();			
		}
		// �׷��� ó���� ���� �Ҹ� �۾�
		win.DeleteDevice();
	}	
	return 0;
}
