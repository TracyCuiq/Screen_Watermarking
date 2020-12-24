#include "StdAfx.h"
#include "MScreenInfoCtrl.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include<bitset> 

#define PI 3.1415927
//extern UINT glb_m_Rotate;
//extern UINT glb_m_Secret;
//extern double glb_m_ScaleX;
//extern double glb_m_ScaleY;
//extern double glb_m_Focus;
//UINT glb_m_Secret =27;
extern int glb_m_Trans;
extern int glb_m_Secret;
extern bool glb_mode;

int m_monitorNum = 0;
bool m_Flag = false;

std::string chToHex(unsigned char ch)
{
    const std::string hex = "0123456789ABCDEF";
    std::stringstream ss1;
    ss1 << hex[ch >> 4] << hex[ch & 0xf];
    return ss1.str();
}

BYTE *LToBin(unsigned int num)
{
	unsigned char *bb;
	bb = (BYTE*)malloc(16);
	for(int j=15; j >= 0; j--)
    {
		int i = 15-j;
		if ( num & ( 1 << j) ){bb[i]='1';} 
		else {bb[i]='0';}
    }
	bb[16]='\0';
	return bb;
}
char *LToBin2(unsigned int num)
{
	char *bb;
	bb = (char*)malloc(8);
	for(int j=7; j >= 0; j--)
     {
		int i = 7-j;
		if ( num & ( 1 << j) ){bb[i]='1';} 
		else {bb[i]='0';}
     }
	bb[8]='\0';
	return bb;
}
char *LToBin2_(unsigned int num)
{
	char *bb;
	bb = (char*)malloc(16);
	for(int j=15; j >= 0; j--)
     {
		int i = 15-j;
		if ( num & ( 1 << j) ){bb[i]='1';} 
		else {bb[i]='0';}
     }
	bb[16]='\0';
	return bb;
}
WORD BinStr2Dec(char const* const binaryString)
{ 
	WORD size = strlen(binaryString); 
	WORD parseBinary = 0; 
	for (int i = 0; i < size; ++i) 
	{ 
		if (binaryString[i] == '1') 
		{
			parseBinary += pow(2.0, size - i - 1); 
		} 
	} 
	return parseBinary; 
} 


CMScreenInfoCtrl::CMScreenInfoCtrl(void)
{
    GdiplusStartupInput gdiplusStartupInput;
    m_nTop      = 0;
    m_nLeft     = 0;
    m_nBottom   = 0;
    m_nRight    = 0;
    m_nWidth    = 0;
    m_nHeight   = 0;

	//m_nDiagonalLen = 0; //桌面对角线长度
	//m_nMarkWinDiagonalLen = 0;//水印对话框对角线长度
	//m_nLineSpace = 100;    //行间距
	//m_nColumnSpace = 100;//列间距
	//m_bShowMode = true;

    m_gdiplusToken = 0;
    //m_wsColor   = L"black";
    //m_wstrText  = L"屏幕水印显示";
	//m_wstrText  = L"";
    //m_FontName  = L"宋体";
    //m_fontsize  = 40;
	//m_nWaterDepth = 200; //水印深度
	//m_dAngle = 45.0f;
	//m_nColorRed = 200;
	//m_nColorGreen = 200;
	//m_nColorBlue = 200;
	
    // Initialize GDI+.
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	///////////////////////
	m_pImage = Image::FromFile(L"qz.png");
	//Image m_pImage(L"logo_qz.bmp");
}

CMScreenInfoCtrl::~CMScreenInfoCtrl(void)
{
    GdiplusShutdown(m_gdiplusToken);
}



ATOM CMScreenInfoCtrl::RegisterWindowClass()
{
    WNDCLASSEX wcex;

    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= m_s_hInstance;
    wcex.hIcon			= NULL;
    wcex.hCursor		= NULL;
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= STR_CLASSNAME;
    wcex.hIconSm		= NULL;

    return RegisterClassEx(&wcex);
}

WORD CMScreenInfoCtrl::CreateSCWindow()
{
    RegisterWindowClass();
    m_Blend.BlendOp				= 0; 
    m_Blend.BlendFlags			= 0; 
    m_Blend.AlphaFormat			= 1; 
	m_Blend.SourceConstantAlpha = glb_m_Trans;//this->m_trans_va;////1*255; //透明度

    m_format.SetAlignment(StringAlignmentNear);

    //刷新屏幕位置坐标和尺寸
    RefreshSCWinSize();

    m_hWnd = ::CreateWindowEx((WS_EX_TOOLWINDOW|
        WS_EX_TRANSPARENT|WS_EX_TOPMOST)&~WS_EX_APPWINDOW,
        STR_CLASSNAME,
		L"",
        WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP,
        m_nLeft, m_nTop, m_nWidth, m_nHeight, NULL, NULL, m_s_hInstance, NULL);

    ::SetProp(m_hWnd, STR_CLASSPOINTER, this);
	//::SetProp(m_hWnd, NULL, this);
    ::PostMessage(m_hWnd, WM_PAINT, 0, 0);

    return 0;
}

LRESULT CALLBACK CMScreenInfoCtrl::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HANDLE h = ::GetProp(hWnd, STR_CLASSPOINTER);
	//HANDLE h = ::GetProp(hWnd, NULL);
    CMScreenInfoCtrl * p = (CMScreenInfoCtrl*)h;
    if (NULL == h)
    {
        return ::DefWindowProc(hWnd, message, wParam, lParam);
    }
    return p->OnWndProc(message, wParam, lParam);
}

LRESULT CMScreenInfoCtrl::OnWndProc( UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;
    BringWindowToTop(m_hWnd);

    switch(message)
    {
        //屏幕水印 响应屏幕显示变化事件，包括 分辨率变化、增加/减少屏幕
    case WM_DISPLAYCHANGE:
        RefreshSCWinSize(); //刷新水印位置和大小
        //不使用 break，继续执行 Paint 处理
    case WM_PAINT:
        hdc = ::BeginPaint(m_hWnd, &ps);
        OnPaint(hdc);
        EndPaint(m_hWnd, &ps);
        BringWindowToTop(m_hWnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(m_hWnd, message, wParam, lParam);
    }
    return DefWindowProc(m_hWnd, message, wParam, lParam);
}

void CMScreenInfoCtrl::OnPaint(HDC hdc)
{

	if(glb_mode){
	//黑色
	WORD HatchBits1[14] = 
	{ 
		0x0F, 0x6F, 0xFF, 0xFF, 
		0xFF, 
		0xD7, 0xEF, 0xFF, 0xFF, 
		0xFF, 
		0xFF, 0xFB, 0xF3, 0xF3,
	};
	WORD HatchBits2[14] = 
	{ 
		0xF0, 0xF6, 0xFF, 0xFF, 
		0xFF, 
		0xD7, 0xEF, 0xFF, 0xFF, 
		0xFF, 
		0xFF, 0xBF, 0x3F, 0x3F,
	};

	WORD HatchBits3[8] = 
	{
		0xFF0F, 0xFF6F, 0xFFFA, 0xFFFD,
		0xFFFF, 0xEFFF, 0xCFFF, 0xCFFF,
	};
	WORD HatchBits4[8] = 
	{ 
		0xC3FF, 0xDBFF, 0xFFFA, 0xFFFD,
		0xFFFF, 0xFFBF, 0xFF3F, 0xFF3F,
	};
	
	BYTE *bins = LToBin(glb_m_Secret);//密文二进制

	///////////////////////更新HatchBits
	/////////////HatchBits1
	char *bin2 =LToBin2(HatchBits1[2]);//转为二进制字符串
	bin2[1]=bins[0]; bin2[2]=bins[1];//修改bit位
	WORD re = BinStr2Dec(bin2);//转为十进制short
	HatchBits1[2]=re;//更新HatchBits

	bin2 =LToBin2(HatchBits1[3]);
	bin2[0]=bins[2]; bin2[1]=bins[3]; bin2[2]=bins[4]; bin2[3]=bins[5];//修改bit位
    re = BinStr2Dec(bin2);
	HatchBits1[3]=re;
    
	bin2 =LToBin2(HatchBits1[7]);
    bin2[3]=bins[6]; bin2[5]=bins[7];
	re = BinStr2Dec(bin2);
	HatchBits1[7]=re;

	bin2 =LToBin2(HatchBits1[8]);
    bin2[2]=bins[8]; bin2[3]=bins[9]; bin2[5]=bins[10];
	re = BinStr2Dec(bin2);
	HatchBits1[8]=re;

	bin2 =LToBin2(HatchBits1[11]);
    bin2[6]=bins[11]; bin2[7]=bins[12]; 
	re = BinStr2Dec(bin2);
	HatchBits1[11]=re;

	bin2 =LToBin2(HatchBits1[12]);
    bin2[6]=bins[13]; bin2[7]=bins[14]; 
	re = BinStr2Dec(bin2);
	HatchBits1[12]=re;

	bin2 =LToBin2(HatchBits1[13]);
    bin2[7]=bins[15]; 
	re = BinStr2Dec(bin2);
	HatchBits1[13]=re;
	/////////////HatchBits1

	/////////////HatchBits2
	bin2 =LToBin2(HatchBits2[2]);
	bin2[5]=bins[0]; bin2[6]=bins[1];
	re = BinStr2Dec(bin2);
	HatchBits2[2]=re;
	
	bin2 =LToBin2(HatchBits2[3]);
	bin2[4]=bins[2]; bin2[5]=bins[3]; bin2[6]=bins[4]; bin2[7]=bins[5];
	re = BinStr2Dec(bin2);
	HatchBits2[3]=re;

	bin2 =LToBin2(HatchBits2[7]);
	bin2[3]=bins[6]; bin2[5]=bins[7]; 
	re = BinStr2Dec(bin2);
	HatchBits2[7]=re;

	bin2 =LToBin2(HatchBits2[8]);
	bin2[2]=bins[8]; bin2[3]=bins[9]; bin2[5]=bins[10];
	re = BinStr2Dec(bin2);
	HatchBits2[8]=re;

	bin2 =LToBin2(HatchBits2[11]);
	bin2[2]=bins[11]; bin2[3]=bins[12];
	re = BinStr2Dec(bin2);
	HatchBits2[11]=re;
	
	bin2 =LToBin2(HatchBits2[12]);
	bin2[2]=bins[13]; bin2[3]=bins[14];
	re = BinStr2Dec(bin2);
	HatchBits2[12]=re;

	bin2 =LToBin2(HatchBits2[13]);
	bin2[3]=bins[15];
	re = BinStr2Dec(bin2);
	HatchBits2[13]=re;
	/////////////HatchBits2

	/////////////HatchBits3
	char *bin2_ =LToBin2_(HatchBits3[2]);//转为二进制字符串
	bin2_[9]=bins[0]; bin2_[10]=bins[1];//修改bit位
	WORD re_ = BinStr2Dec(bin2_);//转为十进制short
	HatchBits3[2]=re_;//更新HatchBits

	bin2_ =LToBin2_(HatchBits3[3]);
	bin2_[8]=bins[2]; bin2_[9]=bins[3]; bin2_[10]=bins[4]; bin2_[11]=bins[5];
	re_ = BinStr2Dec(bin2_);
	HatchBits3[3]=re_;

	bin2_ =LToBin2_(HatchBits3[4]);
	bin2_[14]=bins[6]; bin2_[0]=bins[7];
	re_ = BinStr2Dec(bin2_);
	HatchBits3[4]=re_;

	bin2_ =LToBin2_(HatchBits3[5]);
	bin2_[13]=bins[8]; bin2_[14]=bins[9]; bin2_[0]=bins[10]; bin2_[4]=bins[11]; bin2_[5]=bins[12]; 
	re_ = BinStr2Dec(bin2_);
	HatchBits3[5]=re_;

	bin2_ =LToBin2_(HatchBits3[6]);
	bin2_[4]=bins[13]; bin2_[5]=bins[14];
	re_ = BinStr2Dec(bin2_);
	HatchBits3[6]=re_;

	bin2_ =LToBin2_(HatchBits3[7]);
	bin2_[5]=bins[15];
	re_ = BinStr2Dec(bin2_);
	HatchBits3[7]=re_;
	/////////////HatchBits3

	/////////////HatchBits4
	bin2_ =LToBin2_(HatchBits4[2]);
	bin2_[3]=bins[0]; bin2_[4]=bins[1]; 
	re_ = BinStr2Dec(bin2_);
	HatchBits4[2]=re_;

	bin2_ =LToBin2_(HatchBits4[3]);
	bin2_[2]=bins[2]; bin2_[3]=bins[3]; bin2_[4]=bins[4]; bin2_[5]=bins[5];
	re_ = BinStr2Dec(bin2_);
	HatchBits4[3]=re_;

	bin2_ =LToBin2_(HatchBits4[4]);
	bin2_[14]=bins[6]; bin2_[0]=bins[7]; 
	re_ = BinStr2Dec(bin2_);
	HatchBits4[4]=re_;

	bin2_ =LToBin2_(HatchBits4[5]);
	bin2_[10]=bins[8]; bin2_[11]=bins[9]; bin2_[13]=bins[10]; bin2_[14]=bins[11]; bin2_[0]=bins[12];
	re_ = BinStr2Dec(bin2_);
	HatchBits4[5]=re_;

	bin2_ =LToBin2_(HatchBits4[6]);
	bin2_[10]=bins[13]; bin2_[11]=bins[14]; 
	re_ = BinStr2Dec(bin2_);
	HatchBits4[6]=re_;

	bin2_ =LToBin2_(HatchBits4[7]);
	bin2_[11]=bins[15]; 
	re_ = BinStr2Dec(bin2_);
	HatchBits4[7]=re_;
	/////////////HatchBits4


//////////////////

    HDC      hdcMemory = CreateCompatibleDC(hdc); //HDC是DC的句柄
    HBITMAP  hBitMap   = CreateCompatibleBitmap(hdc, m_nWidth, m_nHeight);//用于创建与指定的设备环境相关的设备兼容的位图
    GraphicsPath path;

	CBitmap bm;
	bm.CreateBitmap(14, 8, 1, 1, HatchBits4);
	HBITMAP  hBmp = (HBITMAP)bm.GetSafeHandle();
	Image* img_g = new Bitmap(hBmp, NULL);

	CBitmap bm2;
	bm2.CreateBitmap(14, 8, 1, 1, HatchBits3);
	HBITMAP  hBmp2 = (HBITMAP)bm2.GetSafeHandle();
	Image* img_g2 = new Bitmap(hBmp2, NULL);

	CBitmap bm3;
	bm3.CreateBitmap(8, 14, 1, 1, HatchBits2);
	HBITMAP  hBmp3 = (HBITMAP)bm3.GetSafeHandle();
	Image* img_g3 = new Bitmap(hBmp3, NULL);

	CBitmap bm4;
	bm4.CreateBitmap(8, 14, 1, 1, HatchBits1);
	HBITMAP  hBmp4 = (HBITMAP)bm4.GetSafeHandle();
	Image* img_g4 = new Bitmap(hBmp4, NULL);


    SelectObject(hdcMemory, hBitMap);
    //
    // Graphics 定义不能放在SelectObject函数之前，否则无法显示水印
    Graphics  graphics(hdcMemory);
    //graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    //graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	//graphics.SetInterpolationMode(InterpolationModeInvalid);
    /////////////////////////////////////
	
	//Rect rct(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
	Rect rct(0, 0, img_g->GetWidth(), img_g->GetHeight());
	Rect rct2(0, 0, img_g3->GetWidth(), img_g3->GetHeight());
	//Rect rc(m_Left, m_Top, m_Width, m_Height);
	int nScreenWidth, nScreenHeight;
    nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	Rect rc(0, 0, nScreenWidth, nScreenHeight);
				
	TextureBrush brush(img_g, rct);
    brush.SetWrapMode(WrapModeClamp);

	TextureBrush brush2(img_g2, rct);
    brush2.SetWrapMode(WrapModeClamp);

	TextureBrush brush3(img_g3, rct2);
    brush3.SetWrapMode(WrapModeClamp);

	TextureBrush brush4(img_g4, rct2);
    brush4.SetWrapMode(WrapModeClamp);
	//brush.TranslateTransform(0, 0, MatrixOrderPrepend);//偏移




	ImageAttributes imAtt;
	imAtt.SetColorKey(
       Color(255, 255, 255),//colorLow
       Color(255, 255, 255),//colorHigh
       ColorAdjustTypeBitmap);
	//SetBkColor(hdcMemory, RGB(50, 50, 50));
	//graphics.Clear(Color.Black);
	//double alpha =1;
	




	///////////////////////////////////
    int w=img_g->GetWidth();int h=img_g->GetHeight();
	int w2=img_g3->GetWidth();int h2=img_g3->GetHeight();
	int tab_w =0; 
	int tab_h =0;
	int dens=5;//dens: how many lines per Group
	int split=6;//横纵分组格数
	int W_group = (nScreenWidth+split-1)/split; int H_group = (nScreenHeight+split-1)/split;
	int num_2 = (H_group + tab_h)/6;//TODO 6
	//int num_1 = (H_group + tab_h)/h;
	int W_2 = (W_group+dens-1)/dens;//TODO convert to float
	//int W_1 = (W_group+dens-1)/dens;
	int groups=split*split;//how many Groups
	int indi_flag = 0;

//////////////////////////////计算纹理方向
	unsigned int* indi = new unsigned int[groups]();//方向指示标识
    int j=0;
	for (int i=0; i<=14; i+=2)
	{
		if (bins[i]=='0')
		{
			if (bins[i+1]=='0'){indi[j]=1; indi[groups-1-j]=1; j++;}
			else{indi[j]=2; indi[groups-1-j]=2; j++;}
		}
		else
		{
			if (bins[i+1]=='0'){indi[j]=3;  indi[groups-1-j]=3; j++;}
			else{indi[j]=4; indi[groups-1-j]=4; j++;}
		}	
	}
	j=9;
	for (int i=0; i<=14; i+=2)
	{
		if (bins[i]=='0')
		{
			if (bins[i+1]=='0'){indi[j]=1; indi[groups-1-j]=1; j++;}
			else{indi[j]=2; indi[groups-1-j]=2; j++;}
		}
		else
		{
			if (bins[i+1]=='0'){indi[j]=3;  indi[groups-1-j]=3; j++;}
			else{indi[j]=4; indi[groups-1-j]=4; j++;}
		}	
	}
////////////////////////////

	for(int g=0; g<groups; g++)
	{
		if(indi[g]==0)
		{
			tab_w=-8; tab_h=1; dens=9;
			W_2 = (W_group+dens-1)/dens;
			//num_2 = (H_group+tab_h)/(h+1);
			num_2 = (H_group + tab_h)/(h2+1);
		}
		else if (indi[g]==1)
		{
			//if(g<=14){tab_w=-29; tab_h=-2;}
			//else{tab_w=1; tab_h=-2;}
			tab_w=-29; tab_h=-2;dens=5;
			W_2 = (W_group+dens-1)/dens;
			//num_2 = (H_group + tab_h)/6;
			num_2 = (H_group+tab_h)/(h-2);
		}
		else if (indi[g]==2)
		{
			//if(g<=14){tab_w=1; tab_h=-2;}
			//else{tab_w=1; tab_h=-2;}
			tab_w=1; tab_h=-2;dens=5;
			W_2 = (W_group+dens-1)/dens;
			//num_2 = (H_group + tab_h)/6;
			num_2 = (H_group+tab_h)/(h-2);
		}
		else if (indi[g]==3)
		{
			tab_w=-14; tab_h=1; dens=11;
			W_2 = (W_group+dens-1)/dens;
			//num_2 = (H_group + tab_h)/h2;
			num_2 = (H_group+tab_h)/(h2+1);
		}
	    else if (indi[g]==4)
		{
			tab_w=-2; tab_h=1; dens=11;
			W_2 = (W_group+dens-1)/dens;
			//num_2 = (H_group + tab_h)/h2;
			num_2 = (H_group+tab_h)/(h2+1);
		}

		for(int i = 0; i< dens; i++)
	    {
			for (int j=0; j<num_2; j++)
			{
				if(indi[g]==0)
				{
					graphics.DrawImage(
					img_g4, 
					Rect( 
					(j*(w2+tab_w)+i*W_2)%W_group//可能会小于0 
					+ (g%split)*W_group//group小组偏移
					+ ((W_group-(j*(w2+tab_w)+i*W_2)%W_group)/W_group)*W_group, //设(j*(w+tab_w)+i*W_)%W_group为X --> ((W_group-X)/W_group)*W_group,作用为判断X小于0时, 将其偏移
					j*(h2+tab_h) +(g/split)*H_group, 
					w2, h2),
					0, 0, w2, h2,
					UnitPixel, 
					&imAtt);
					brush4.TranslateTransform(j*(w2+tab_w)+ (g%split)*W_group, j*(h2+tab_h)+(g/split)*H_group, MatrixOrderPrepend);
					rc.X+=w2; rc.Y+=h2;
					
				}
				else if (indi[g]==1)
				{
					graphics.DrawImage(
					img_g, 
					Rect( 
					(j*(w+tab_w)+i*W_2)%W_group//可能会小于0 
					+ (g%split)*W_group//group小组偏移
					+ ((W_group-(j*(w+tab_w)+i*W_2)%W_group)/W_group)*W_group, //设(j*(w+tab_w)+i*W_)%W_group为X --> ((W_group-X)/W_group)*W_group,作用为判断X小于0时, 将其偏移
					j*(h+tab_h) +(g/split)*H_group, 
					w, h),
					0, 0, w, h,
					UnitPixel, 
					&imAtt);
					brush.TranslateTransform(j*(w+tab_w)+ (g%split)*W_group, j*(h+tab_h)+(g/split)*H_group, MatrixOrderPrepend);
					rc.X+=w; rc.Y+=h;

				}
				else if (indi[g]==2)
				{
					graphics.DrawImage(
					img_g2, 
					Rect( 
					(j*(w+tab_w)+i*W_2)%W_group//可能会小于0 
					+ (g%split)*W_group//group小组偏移
					+ ((W_group-(j*(w+tab_w)+i*W_2)%W_group)/W_group)*W_group, //设(j*(w+tab_w)+i*W_)%W_group为X --> ((W_group-X)/W_group)*W_group,作用为判断X小于0时, 将其偏移
					j*(h+tab_h) +(g/split)*H_group, 
					w, h),
					0, 0, w, h,
					UnitPixel, 
					&imAtt);
					brush2.TranslateTransform(j*(w+tab_w)+ (g%split)*W_group, j*(h+tab_h)+(g/split)*H_group, MatrixOrderPrepend);
					rc.X+=w; rc.Y+=h;
				}
				else if (indi[g]==3)
				{
					graphics.DrawImage(
					img_g3, 
					Rect( 
					(j*(w2+tab_w)+i*W_2)%W_group//可能会小于0 
					+ (g%split)*W_group//group小组偏移
					+ ((W_group-(j*(w2+tab_w)+i*W_2)%W_group)/W_group)*W_group, //设(j*(w+tab_w)+i*W_)%W_group为X --> ((W_group-X)/W_group)*W_group,作用为判断X小于0时, 将其偏移
					j*(h2+tab_h) +(g/split)*H_group, 
					w2, h2),
					0, 0, w2, h2,
					UnitPixel, 
					&imAtt);
					brush3.TranslateTransform(j*(w2+tab_w)+ (g%split)*W_group, j*(h2+tab_h)+(g/split)*H_group, MatrixOrderPrepend);
					rc.X+=w2; rc.Y+=h2;
				}
				else if (indi[g]==4)
				{
					graphics.DrawImage(
					img_g4, 
					Rect( 
					(j*(w2+tab_w)+i*W_2)%W_group//可能会小于0 
					+ (g%split)*W_group//group小组偏移
					+ ((W_group-(j*(w2+tab_w)+i*W_2)%W_group)/W_group)*W_group, //设(j*(w+tab_w)+i*W_)%W_group为X --> ((W_group-X)/W_group)*W_group,作用为判断X小于0时, 将其偏移
					j*(h2+tab_h) +(g/split)*H_group, 
					w2, h2),
					0, 0, w2, h2,
					UnitPixel, 
					&imAtt);
					brush4.TranslateTransform(j*(w2+tab_w)+ (g%split)*W_group, j*(h2+tab_h)+(g/split)*H_group, MatrixOrderPrepend);
					rc.X+=w2; rc.Y+=h2;
				}
			}
	    }
	}

 
    //窗口扩展属性，设置窗口透明
    LONG lWindowLong = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
    if ((lWindowLong&WS_EX_LAYERED) != WS_EX_LAYERED)
    {
        lWindowLong = lWindowLong| WS_EX_LAYERED;
        ::SetWindowLong(m_hWnd, GWL_EXSTYLE, lWindowLong);
    }

    m_Blend.SourceConstantAlpha = glb_m_Trans;//////////////////

    POINT ptSrc      = {0, 0};
    POINT ptWinPos   = {m_nLeft, m_nTop};
    HDC   hdcScreen  = ::GetDC(m_hWnd);
    SIZE  sizeWindow = {m_nWidth, m_nHeight};
    BOOL  bRet       = UpdateLayeredWindow(m_hWnd, hdcScreen, &ptWinPos,
        &sizeWindow, hdcMemory, &ptSrc,
        0, &m_Blend, ULW_ALPHA);//UpdateLayeredWindow更新一个分层窗口的位置，大小，形状，内容和半透明度

    ::SetWindowPos(m_hWnd, HWND_TOPMOST, m_nLeft, m_nTop, m_nWidth, m_nHeight, 
        SWP_SHOWWINDOW);

    ::ReleaseDC(m_hWnd, hdcScreen);
    ::ReleaseDC(m_hWnd, hdc);

    hdcScreen = NULL;
    hdc = NULL;

    DeleteObject(hBitMap);
    DeleteDC(hdcMemory);
    hdcMemory = NULL;
}else{

    HDC          hdcMemory = CreateCompatibleDC(hdc);
    HBITMAP      hBitMap   = CreateCompatibleBitmap(hdc,m_nWidth,m_nHeight);
    GraphicsPath path;

    SelectObject(hdcMemory, hBitMap);
    // Graphics 定义不能放在SelectObject函数之前，否则无法显示水印
    Graphics  graphics(hdcMemory);
    ////////////////////

	//graphics.DrawImage(m_pImage, 0,0, m_pImage->GetWidth(), m_pImage->GetWidth());
    Rect rct(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
	int nScreenWidth, nScreenHeight;
    nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	Rect rc(0, 0, nScreenWidth, nScreenHeight);
	TextureBrush brush(m_pImage, rct);
    brush.SetWrapMode(WrapModeTile);
	brush.RotateTransform(30, TRUE ? MatrixOrderPrepend : MatrixOrderAppend);
	graphics.FillRectangle(&brush, rc);
    

    //窗口扩展属性，设置窗口透明
    LONG lWindowLong = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
    if ((lWindowLong&WS_EX_LAYERED) != WS_EX_LAYERED)
    {
        lWindowLong = lWindowLong| WS_EX_LAYERED;
        ::SetWindowLong(m_hWnd, GWL_EXSTYLE, lWindowLong);
    }

    POINT ptSrc      = {0, 0};
    POINT ptWinPos   = {m_nLeft, m_nTop};
    HDC   hdcScreen  = ::GetDC(m_hWnd);
    SIZE  sizeWindow = {m_nWidth, m_nHeight};
    BOOL  bRet       = UpdateLayeredWindow(m_hWnd, hdcScreen, &ptWinPos,
        &sizeWindow, hdcMemory, &ptSrc,
        0, &m_Blend, ULW_ALPHA);

    ::SetWindowPos(m_hWnd, HWND_TOPMOST, m_nLeft, m_nTop, m_nWidth, m_nHeight, 
        SWP_SHOWWINDOW);

    ::ReleaseDC(m_hWnd, hdcScreen);
    ::ReleaseDC(m_hWnd, hdc);

    hdcScreen = NULL;
    hdc = NULL;

    DeleteObject(hBitMap);
    DeleteDC(hdcMemory);
    hdcMemory = NULL;
    }

}

size_t CMScreenInfoCtrl::GenerateVecPt( vector<Point>& vecPt, const wchar_t* lpwstr)
{
    Point        pt;
    RectF        boundRect;
    GraphicsPath pathTemp;
    FontFamily   fontFamily(m_FontName.c_str());
    pathTemp.AddString(lpwstr, -1, &fontFamily,
        FontStyleRegular, m_fontsize, pt, &m_format);
    pathTemp.GetBounds(&boundRect);
    const INT    offset   = 20;

    int num = 0;        //屏幕索引，主屏幕为0
    Point rpt(0,0);     //当前屏幕左上角与水印屏幕左上角的相对位置
    BOOL flag = TRUE;   //显示器有效标志
    BOOL rflag = TRUE;  //真显示器有效标志
    DISPLAY_DEVICE dd;  //显示器属性
    DEVMODE devMode;    //显示器配置
    //初始化变量
    ZeroMemory(&devMode,sizeof(devMode));
    devMode.dmSize=sizeof(devMode);
    ZeroMemory(&dd, sizeof(dd));
    dd.cb = sizeof(dd);

    //循环遍历多个屏幕，计算水印窗口所需大小------------------------------------------
    do
    {
        flag = EnumDisplayDevices(NULL, num, &dd, 0);
        rflag = flag && EnumDisplaySettings(dd.DeviceName,ENUM_CURRENT_SETTINGS,&devMode);

        if (rflag)
        {
            //屏幕中间
            rpt.X = devMode.dmPosition.x- m_nLeft;
            rpt.Y = devMode.dmPosition.y- m_nTop;

            pt.X = rpt.X + INT(devMode.dmPelsWidth - boundRect.Width - offset*2)/2;
            pt.Y = rpt.Y + (devMode.dmPelsHeight - boundRect.Height - offset*2)/2;
            vecPt.push_back(pt);

        }

        num++; //增加屏幕索引

    }while(flag);
    //----------------------------------------------------------------------------------------

    return vecPt.size();
}

void CMScreenInfoCtrl::RefreshSCWinSize()
{
    int num = 0;
    int iTop = 0;
    int iBottom = 0;
    int iLeft = 0;
    int iRight = 0;
    BOOL flag = TRUE;
    BOOL rflag = TRUE;
    DISPLAY_DEVICE dd;
    DEVMODE devMode;
	//int t = GetDlgItemInt(IDC_value, NULL, 0);
	//int t = glb_m_Trans;

    //初始化变量
    ZeroMemory(&devMode,sizeof(devMode));
    devMode.dmSize=sizeof(devMode);
    ZeroMemory(&dd, sizeof(dd));
    dd.cb = sizeof(dd);

    //循环遍历多个屏幕，计算水印窗口所需大小
    do
    {
        flag = EnumDisplayDevices(NULL, num, &dd, 0);
        rflag = flag && EnumDisplaySettings(dd.DeviceName,ENUM_CURRENT_SETTINGS,&devMode);

        if (rflag)
        {
            if (iTop > devMode.dmPosition.y)
            {
                iTop = devMode.dmPosition.y;
            }
            if (iLeft > devMode.dmPosition.x)
            {
                iLeft = devMode.dmPosition.x;
            }
            if (iBottom < devMode.dmPosition.y + devMode.dmPelsHeight)
            {
                iBottom = devMode.dmPosition.y + devMode.dmPelsHeight;
            }
            if (iRight < devMode.dmPosition.x + devMode.dmPelsWidth)
            {
                iRight = devMode.dmPosition.x + devMode.dmPelsWidth;
            }
        }

        num++;

    } while (flag);

    //设置成员变量
    m_nLeft = iLeft;
    m_nTop = iTop;
    m_nWidth = iRight - iLeft;
    m_nHeight = iBottom - iTop;

	//m_nDiagonalLen = (int)sqrt(float(m_nWidth*m_nWidth + m_nHeight*m_nHeight));
	//m_nMarkWinDiagonalLen = (int)sqrt(float(m_nDiagonalLen*m_nDiagonalLen*2));

    return;
}