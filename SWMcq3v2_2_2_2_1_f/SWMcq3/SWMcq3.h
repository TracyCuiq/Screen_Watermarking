
// SWMcq3.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSWMcq3App:
// �йش����ʵ�֣������ SWMcq3.cpp
//

class CSWMcq3App : public CWinAppEx
{
public:
	CSWMcq3App();
	~CSWMcq3App();


// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSWMcq3App theApp;

