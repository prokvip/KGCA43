#include "TCore.h"
#include "TWriterFont.h"
// ����� ���� �� �׽�Ʈ�Ѵ�.
class Sample : public TCore
{
	TWriterFont  m_font;
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
		win.DeleteDevice();
	}	
	return 0;
}
