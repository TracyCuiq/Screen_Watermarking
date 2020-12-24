
// SWMcq3Dlg.cpp : ʵ���ļ�
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
CString ENCRYPTION = L"admin";//����ID��Կ
static unsigned long ulThreadId = 0;
bool  glb_mode = true;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CSWMcq3Dlg �Ի���
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




// CSWMcq3Dlg ��Ϣ�������

BOOL CSWMcq3Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_trans.SetRange(10,100);//���÷�Χ
    m_trans.SetTicFreq(10);//������ʾ�̶ȵļ��
	m_trans.SetPos(15);//��ǰͣ����λ��
	m_trans.SetLineSize(10);//һ�еĴ�С����Ӧ���̵ķ����
	//UpdateData(TRUE);
	//m_trans_va=m_trans.GetPos();//��û����λ��
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
	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BOOL CSWMcq3Dlg::PreTranslateMessage(MSG* pMsg)
{
    //����ESC�رմ���/
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE ) return TRUE;
    //���λس��رմ���,���ᵼ�»س��ڴ�����ʧЧ.
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSWMcq3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSWMcq3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSWMcq3Dlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CSWMcq3Dlg::OnBnClickedButtonGen()
{
	
	int nPos = m_trans.GetPos();
	glb_m_Trans = nPos;

	int secret = GetDlgItemInt(IDC_EDIT1, NULL, 0);
	glb_m_Secret = secret;


	//�����߳�
    //static unsigned long ulThreadId = 0;
    if (0 != ulThreadId)    //��ֹ��ε��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//UpdateData(TRUE);
	int nPos = m_trans.GetPos();   //��û���ĵ�ǰλ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	//�ҵ�ϵͳ��������
	CString lpRun = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	//��������
	long lRet = RegOpenKeyEx(HKEY_CURRENT_USER, lpRun, 0, KEY_ALL_ACCESS, &hKey);
	if (lRet != ERROR_SUCCESS)
		MessageBox(_T("��������ʧ��"));
	//�ҵ���������·��
	TCHAR pFileName[MAX_PATH] = {};
	GetModuleFileName(NULL, pFileName, MAX_PATH);
	//�ж��Ƿ��Ѿ����ÿ�������
	TCHAR PowerBoot[MAX_PATH] = {};
	DWORD nLongth = MAX_PATH;
	long result = RegGetValue(hKey, NULL, _T("PowerBoot"), RRF_RT_REG_SZ, 0, PowerBoot, &nLongth);
	if (result == ERROR_SUCCESS)
	{
		//����״̬;
		//ȡ���Զ�����
		lRet=RegDeleteValue(hKey, _T("PowerBoot"));
		if (lRet == ERROR_SUCCESS)
		{
			MessageBox(_T("�ر������ɹ�"));
			//PowerBootButton.SetWindowText(_T("δ����״̬"));
		}
	}
	else
	{
		//δ����״̬
		//��������
		lRet = RegSetValueEx(hKey, _T("PowerBoot"), 0, REG_SZ, (LPBYTE)pFileName, (lstrlen(pFileName) + 1)*sizeof(TCHAR));
		if (lRet == ERROR_SUCCESS)
		{
			MessageBox(_T("���������ɹ�������رմ˳��������������"));
			//PowerBootButton.SetWindowText(_T("����״̬"));
		}
	}
	//�ر�ע���
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
		//MessageBox("������ȷ��",NULL,MB_OK);
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
		//MessageBox("�������",NULL,MB_OK);
		MessageBox(_T("�������"));
		return;
	}
}
void CSWMcq3Dlg::OnBnClickedButton6()
{
	glb_mode=!glb_mode;
	OnBnClickedButtonGen();
}
