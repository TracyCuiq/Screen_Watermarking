
// SWMcq3.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "SWMcq3.h"
#include "SWMcq3Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSWMcq3App

BEGIN_MESSAGE_MAP(CSWMcq3App, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSWMcq3App ����

CSWMcq3App::CSWMcq3App()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}
CSWMcq3App::~CSWMcq3App()
{
	/*
	UINT FFlag = EWX_REBOOT | EWX_FORCE; 
	// Get version info to determine operation
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (GetVersionEx(&osvi))
	{
		// Determine the platform
		if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			HANDLE hToken;
			TOKEN_PRIVILEGES tkp;
	         
			// Windows NT 3.51, Windows NT 4.0, Windows 2000,
			// Windows XP, or Windows .NET Server
			if (OpenProcessToken(GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
			{
				LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	             
				tkp.PrivilegeCount = 1;  // one privilege to set
				tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	             
				AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
			}
		}
	}
	 
	ExitWindowsEx(FFlag, 0);
	*/
	
	wchar_t pBuf[MAX_PATH];
	GetModuleFileName(NULL, pBuf, MAX_PATH);
	STARTUPINFO startupinfo;
	PROCESS_INFORMATION proc_info;

	memset(&startupinfo,0,sizeof(STARTUPINFO));
	startupinfo.cb=sizeof(STARTUPINFO);
	::CreateProcess(pBuf,NULL,NULL,NULL,FALSE,
    NORMAL_PRIORITY_CLASS,NULL,NULL,&startupinfo,&proc_info);
	

}



// Ψһ��һ�� CSWMcq3App ����

CSWMcq3App theApp;



// CSWMcq3App ��ʼ��

BOOL CSWMcq3App::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("ScreenWatermarkingCQ"));

	CSWMcq3Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
		//HideWnd(this);

	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
