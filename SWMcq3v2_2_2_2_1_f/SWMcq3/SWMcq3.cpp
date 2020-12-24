
// SWMcq3.cpp : 定义应用程序的类行为。
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


// CSWMcq3App 构造

CSWMcq3App::CSWMcq3App()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
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



// 唯一的一个 CSWMcq3App 对象

CSWMcq3App theApp;



// CSWMcq3App 初始化

BOOL CSWMcq3App::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("ScreenWatermarkingCQ"));

	CSWMcq3Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
		//HideWnd(this);

	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
