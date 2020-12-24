
// SWMcq3Dlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSWMcq3Dlg 对话框
class CSWMcq3Dlg : public CDialog
{
// 构造
public:
	CSWMcq3Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SWMCQ3_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);//
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonGen();
	
	CSliderCtrl m_trans;
	int m_secret;//ID
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton2();
	//afx_msg void OnBnClickedButton3();
	//CButton PowerBootButton;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
void ShowWaterMark();
void UpdateWaterMark();