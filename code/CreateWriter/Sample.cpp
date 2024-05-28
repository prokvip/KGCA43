#include "TCore.h"
#include "TWriterFont.h"
// 기능을 구현 및 테스트한다.
class Sample : public TCore
{
	TWriterFont  m_font;
public:
	// 초기화 작업
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
	// 소멸 작업
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
		// 그래픽 처리를 위한 초기화 작업
		if (win.CreateDevice(win.m_hWnd))
		{
			// 블럭상태
			win.WindowRun();			
		}
		win.DeleteDevice();
	}	
	return 0;
}
