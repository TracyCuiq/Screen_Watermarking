
// SWMcq3Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SWMcq3.h"
#include "SWMcq3Dlg.h"
#include "MScreenInfoCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int   glb_m_Trans = 15;
int   glb_m_Secret = 0;
CString ENCRYPTION = L"admin";//解锁ID密钥
static unsigned long ulThreadId = 0;
bool  glb_mode = true;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSWMcq3Dlg 对话框
CSWMcq3Dlg::CSWMcq3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSWMcq3Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_secret = 0;
	m_secret = AfxGetApp()->GetProfileInt(_T("SWMSetting"), _T("m_ID"), 0);
	glb_m_Secret = m_secret;

}



void CSWMcq3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER2, m_trans);
	//DDX_Control(pDX, IDC_BUTTON3, PowerBootButton);
	DDX_Text(pDX, IDC_EDIT1, m_secret);
}

BEGIN_MESSAGE_MAP(CSWMcq3Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	
	ON_BN_CLICKED(IDOK, &CSWMcq3Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_GEN, &CSWMcq3Dlg::OnBnClickedButtonGen)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CSWMcq3Dlg::OnNMCustomdrawSlider2)
	//ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, &CSWMcq3Dlg::OnIpnFieldchangedIpaddress1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSWMcq3Dlg::OnBnClickedButton2)
	//ON_BN_CLICKED(IDC_BUTTON3, &CSWMcq3Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSWMcq3Dlg::OnBnClickedButton4)
	ON_EN_CHANGE(IDC_EDIT1, &CSWMcq3Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON3, &CSWMcq3Dlg::OnBnClickedButton3)
	ON_EN_CHANGE(IDC_EDIT2, &CSWMcq3Dlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON5, &CSWMcq3Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CSWMcq3Dlg::OnBnClickedButton6)
END_MESSAGE_MAP()




// CSWMcq3Dlg 消息处理程序

BOOL CSWMcq3Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_trans.SetRange(10,100);//设置范围
    m_trans.SetTicFreq(10);//设置显示刻度的间隔
	m_trans.SetPos(15);//当前停留的位置
	m_trans.SetLineSize(10);//一行的大小，对应键盘的方向键
	//UpdateData(TRUE);
	//m_trans_va=m_trans.GetPos();//获得滑块的位置
	//UpdateData(FALSE);

	if (0 == ulThreadId)    
    {
		 HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ShowWaterMark, NULL, 0, &ulThreadId);
         CloseHandle(hThread);
    }
	else
	{}

	CWnd *EDSECRET = GetDlgItem(IDC_EDIT1);		
    EDSECRET->EnableWindow(false);
	CWnd *BTLOCK = GetDlgItem(IDC_BUTTON3);		
    BTLOCK->EnableWindow(false);
	CWnd *BTLOCK2 = GetDlgItem(IDC_BUTTON4);		
    BTLOCK2->EnableWindow(false);
	EDSECRET=NULL;
	BTLOCK=NULL;
	BTLOCK2=NULL;


	int secret_reg;
    secret_reg = AfxGetApp()->GetProfileInt(_T("SWMSetting"), _T("m_ID"), 0);
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BOOL CSWMcq3Dlg::PreTranslateMessage(MSG* pMsg)
{
    //屏蔽ESC关闭窗体/
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE ) return TRUE;
    //屏蔽回车关闭窗体,但会导致回车在窗体上失效.
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN && pMsg->wParam) return TRUE;
    else
        return CDialog::PreTranslateMessage(pMsg);
}



void CSWMcq3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSWMcq3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSWMcq3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSWMcq3Dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CSWMcq3Dlg::OnBnClickedButtonGen()
{
	
	int nPos = m_trans.GetPos();
	glb_m_Trans = nPos;

	int secret = GetDlgItemInt(IDC_EDIT1, NULL, 0);
	glb_m_Secret = secret;


	//创建线程
    //static unsigned long ulThreadId = 0;
    if (0 != ulThreadId)    //防止多次点击
    {
		//::PostThreadMessage(ulThreadId,WM_QUIT,0,0);
		//HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ShowWaterMark, NULL, 0, &ulThreadId);
        //CloseHandle(hThread);
		int ulThreadIdP = ulThreadId;
		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ShowWaterMark, NULL, 0, &ulThreadId);
		::Sleep(300);
		::PostThreadMessage(ulThreadIdP, WM_QUIT, 0, 0);
		CloseHandle(hThread);
    }
    else
    {
        HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ShowWaterMark, NULL, 0, &ulThreadId);
        CloseHandle(hThread);
    }
	
}

void ShowWaterMark()
{
    CMScreenInfoCtrl *ctrl = new CMScreenInfoCtrl();
    ::SetThreadDesktop(NULL);
    ctrl->CreateSCWindow();

    //The message loop
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}



void CSWMcq3Dlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	int nPos = m_trans.GetPos();   //获得滑块的当前位置
	//glb_m_Trans = nPos;
	SetDlgItemInt(IDC_value,nPos,0);
	*pResult = 0;
}


BOOL HideWnd(CWnd* pWnd)
{
	return pWnd->ShowWindow(SW_HIDE);
}

void CSWMcq3Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	HideWnd(this);

	NOTIFYICONDATA m_tNotifyIconData; 
	//IconDataPtr *m_pIconData;
	//int m_nSelectedIconIndex;

	m_tNotifyIconData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    BOOL res = Shell_NotifyIcon(NIM_ADD, &m_tNotifyIconData); 
	//if (m_pIconData[m_nSelectedIconIndex]->GetIconHandler()) 
      //  DestroyIcon(m_pIconData[m_nSelectedIconIndex]->GetIconHandler());
}

void CSWMcq3Dlg::OnBnClickedButton4()
{
	HKEY hKey;
	//找到系统的启动项
	CString lpRun = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	//打开启动项
	long lRet = RegOpenKeyEx(HKEY_CURRENT_USER, lpRun, 0, KEY_ALL_ACCESS, &hKey);
	if (lRet != ERROR_SUCCESS)
		MessageBox(_T("打开启动项失败"));
	//找到程序自身路径
	TCHAR pFileName[MAX_PATH] = {};
	GetModuleFileName(NULL, pFileName, MAX_PATH);
	//判断是否已经设置开机启动
	TCHAR PowerBoot[MAX_PATH] = {};
	DWORD nLongth = MAX_PATH;
	long result = RegGetValue(hKey, NULL, _T("PowerBoot"), RRF_RT_REG_SZ, 0, PowerBoot, &nLongth);
	if (result == ERROR_SUCCESS)
	{
		//自启状态;
		//取消自动启动
		lRet=RegDeleteValue(hKey, _T("PowerBoot"));
		if (lRet == ERROR_SUCCESS)
		{
			MessageBox(_T("关闭自启成功"));
			//PowerBootButton.SetWindowText(_T("未自启状态"));
		}
	}
	else
	{
		//未自启状态
		//设置自启
		lRet = RegSetValueEx(hKey, _T("PowerBoot"), 0, REG_SZ, (LPBYTE)pFileName, (lstrlen(pFileName) + 1)*sizeof(TCHAR));
		if (lRet == ERROR_SUCCESS)
		{
			MessageBox(_T("设置自启成功，请勿关闭此程序以免电脑重启"));
			//PowerBootButton.SetWindowText(_T("自启状态"));
		}
	}
	//关闭注册表
	RegCloseKey(hKey);
}

void CSWMcq3Dlg::OnEnChangeEdit1()
{
	UpdateData();
	if (m_secret < 0) m_secret = 0;
	if (m_secret > 65535) m_secret = 65535;
	UpdateData(false);
	Invalidate();
}

void CSWMcq3Dlg::OnBnClickedButton3()
{
	//int lock =1;
	//AfxGetApp()->WriteProfileInt(_T("Setting"), _T("m_lock"), lock);
	CWnd *EDSECRET = GetDlgItem(IDC_EDIT1);		
    EDSECRET->EnableWindow(false);
	CWnd *BTLOCK = GetDlgItem(IDC_BUTTON3);		
    BTLOCK->EnableWindow(false);
	CWnd *BTLOCK2 = GetDlgItem(IDC_BUTTON4);		
    BTLOCK2->EnableWindow(false);
	EDSECRET=NULL;
	BTLOCK=NULL;
	BTLOCK2=NULL;

	int secret = GetDlgItemInt(IDC_EDIT1, NULL, 0);
	AfxGetApp()->WriteProfileInt(_T("SWMSetting"), _T("m_ID"), secret);
}

void CSWMcq3Dlg::OnEnChangeEdit2()
{
	UpdateData();
	UpdateData(false);
	Invalidate();
}

void CSWMcq3Dlg::OnBnClickedButton5()
{
	CString tmp;
	GetDlgItemText(IDC_EDIT2, tmp);
	if(tmp==ENCRYPTION)
	{
		//MessageBox("密码正确！",NULL,MB_OK);
		//return;
		CWnd *EDSECRET = GetDlgItem(IDC_EDIT1);		
		EDSECRET->EnableWindow(true);
		EDSECRET=NULL;
		CWnd *BTLOCK = GetDlgItem(IDC_BUTTON3);		
		BTLOCK->EnableWindow(true);
		BTLOCK=NULL;
		CWnd *BTLOCK2 = GetDlgItem(IDC_BUTTON4);		
		BTLOCK2->EnableWindow(true);
		BTLOCK2=NULL;
		SetDlgItemText(IDC_EDIT2, L"");//clear IDC_EDIT2
	}
	else
	{
		//MessageBox("密码错误！",NULL,MB_OK);
		MessageBox(_T("密码错误"));
		return;
	}
}
void CSWMcq3Dlg::OnBnClickedButton6()
{
	glb_mode=!glb_mode;
	OnBnClickedButtonGen();
}
