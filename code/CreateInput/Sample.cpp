#include "TDevice.h"
#include "TInput.h"

class Sample : public TDevice
{
public:
	TInput m_Input;
public:
	virtual void   GameRun() override
	{
		m_Input.Frame();
		m_Input.KeyTest();
		TDevice::GameRun();		
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
