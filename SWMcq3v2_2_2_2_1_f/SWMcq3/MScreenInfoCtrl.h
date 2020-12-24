#pragma once

#include<vector>
#include <GdiPlus.h>
#include <sstream>
using namespace  std;
using namespace Gdiplus;

#define STR_CLASSNAME		L"ScW"
#define STR_CLASSPOINTER	L"CLASSPOINTER"

static HINSTANCE m_s_hInstance;

class CMScreenInfoCtrl
{
public:
    CMScreenInfoCtrl(void);
    ~CMScreenInfoCtrl(void);

    int				 m_nTop;
    int				 m_nLeft;
    int				 m_nBottom;
    int				 m_nRight; 
    int				 m_nWidth;
    int				 m_nHeight;
	int              m_trans_va;

    HWND			 m_hWnd;
    //wstring			 m_wstrText;
	//DWORD            m_nWaterDepth;
	//double	         m_dAngle;
	//BOOL             m_bShowMode;//屏幕或者中心单行
	//BYTE			 m_nColorRed;
	//BYTE			 m_nColorGreen;
	//BYTE			 m_nColorBlue;

    BLENDFUNCTION	 m_Blend;
    wstring			 m_FontName;
    REAL			 m_fontsize;
    StringFormat	 m_format;
    //wstring     m_wsColor;
    ULONG_PTR		 m_gdiplusToken;
	/////////
    Image*		     m_pImage;
    //int              m_nDiagonalLen;
	//int              m_nMarkWinDiagonalLen;
	//int				 m_nLineSpace; //行间距
	//int				 m_nColumnSpace;//列间距


    void RefreshSCWinSize();
    static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    ATOM RegisterWindowClass();
    WORD CreateSCWindow();
    LRESULT OnWndProc( UINT message, WPARAM wParam, LPARAM lParam);
    void     OnPaint(HDC hdc);
    size_t   GenerateVecPt(vector<Point>& vecPt, const wchar_t* lpwstr);
	//void UpdateWaterMarkWin(HDC hdcMemory);
	//SizeF GetTextBounds(const Gdiplus::Font& font, const StringFormat& strFormat, const wstring& szText);
	
};

