// CrashReporter.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CCrashReporterApp:
// �йش����ʵ�֣������ CrashReporter.cpp
//

class CCrashReporterApp : public CWinApp
{
public:
	CCrashReporterApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCrashReporterApp theApp;
