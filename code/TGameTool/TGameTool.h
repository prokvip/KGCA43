
// TGameTool.h: TGameTool 애플리케이션의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.
#include "Sample.h"
//0) Sample 만들거나, 아니면 CTGameToolApp 다중상속하거나.
//1)BOOL CTGameToolApp::InitInstance() 엔진 초기화
//2)BOOL CTGameToolApp::ExitInstance() 엔진 소멸
//3)엔진 프레임(랜더링) 
// BOOL CTGameToolApp::OnIdle(LONG lCount) 오버라이딩
// -ToolGameFrame, ToolGameRender, 반환값 true(매프레임호출된다.)
// 4)오버라이딩void CTGameToolView::OnSize(UINT nType, int cx, int cy)
// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//CTGameToolApp* pApp = (CTGameToolApp*)AfxGetApp();
//pApp->Resize(cx, cy);
// 
// CTGameToolApp:
// 이 클래스의 구현에 대해서는 TGameTool.cpp을(를) 참조하세요.
//

class CTGameToolApp : public CWinAppEx//, public TCore
{
public:
	CTGameToolApp() noexcept;
	Sample    m_Tool;

// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CTGameToolApp theApp;
