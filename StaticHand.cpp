// StaticHand.cpp : implementation file
//

#include "stdafx.h"
#include "camera.h"
#include "StaticHand.h"
#include "Hand.h"
#include "cameraDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HBITMAP hm;//用于 处理结果的存储
/////////////////////////////////////////////////////////////////////////////
// CStaticHand dialog


CStaticHand::CStaticHand(CWnd* pParent /*=NULL*/)
	: CDialog(CStaticHand::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStaticHand)
	//}}AFX_DATA_INIT


	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);

}

BOOL CStaticHand::OnInitDialog()   //初始化对话框
{
	CDialog::OnInitDialog();
	CBitmap bmp; 
	bmp.LoadBitmap(IDB_BG); 
	m_brBk.CreatePatternBrush(&bmp); 
	bmp.DeleteObject(); 
	return TRUE; // return TRUE unless you set the focus to a control 
}

void CStaticHand::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStaticHand)
	DDX_Control(pDX, IDC_STATICRESULT, m_staticresult);
	DDX_Control(pDX, IDC_LIST1, m_result);
	DDX_Control(pDX, IDC_STATICHAND, m_statichand);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStaticHand, CDialog)
	//{{AFX_MSG_MAP(CStaticHand)
	ON_BN_CLICKED(IDC_OPEN, OnOpen)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnSaveResult)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticHand message handlers

void CStaticHand::OnOpen() //打开待识别图片
{
	// TODO: Add your control notification handler code here
	TCHAR szCurDirBackup[_MAX_PATH];   
	GetCurrentDirectory(sizeof(szCurDirBackup),szCurDirBackup); //得到当前路径值    
	CFileDialog* lpszOpenFile; 
	lpszOpenFile = new CFileDialog(TRUE," "," ",OFN_FILEMUSTEXIST |OFN_HIDEREADONLY ,"待识别文件(*.bmp)|*.bmp||");
	lpszOpenFile->m_ofn.lpstrInitialDir = szCurDirBackup;//当前目录

	if( lpszOpenFile->DoModal() == IDOK)//确定按钮
	{
		CString szGetName;
		szGetName = lpszOpenFile->GetPathName();//得到打开文件的路径
		n = lpszOpenFile->GetFileName();
		SetWindowText(szGetName); //在窗口标题上显示路径
		//显示到控件中
		hm = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),szGetName,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_DEFAULTSIZE|LR_LOADFROMFILE);
		m_statichand.SetBitmap(hm); 
	}

	delete lpszOpenFile;//释放分配的对话框
	SetCurrentDirectory(szCurDirBackup);//设置回当前路径因为CFileDlg返回时会将程序的当前路径改成选择文件的路径  
}

void CStaticHand::OnButton2() //识别原始图像
{
	// TODO: Add your control notification handler code here
	BITMAP Bitmap;
	GetObject(hm,sizeof(Bitmap),&Bitmap);//得到bitmap对象
	//char a[10]="";ultoa(bitmap.bmBitsPixel,a,10);
	//-----------------------------------------------------------------------------------------------------------------------------------------------
	LPBITMAPINFOHEADER lpbi;   
	HANDLE       hDib, hPal;
	BITMAPINFOHEADER   bi;
	HDC     hDC;  
	HPALETTE  hOldPal=NULL;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes  = 1;
	bi.biBitCount  = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	hDib = GlobalAlloc(GHND,Bitmap.bmWidthBytes*Bitmap.bmHeight+sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;   
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal=SelectPalette(hDC,(HPALETTE)hPal,FALSE);
		RealizePalette(hDC);
	}
	int p = GetDIBits(hDC,hm,0,(UINT)Bitmap.bmHeight,(LPSTR)lpbi+sizeof(BITMAPINFOHEADER), (BITMAPINFO *)lpbi,DIB_RGB_COLORS);		
	BYTE * data = (BYTE*)lpbi + sizeof(BITMAPINFOHEADER);//得到数据区指针 
	//-------------------------------------------------------------------------------------------------------------------------------------------------
	Hand hand;
	//形态学滤波      从硬盘读取二值化bmp文件  (已经过 中值滤波)
	
	hand.findHand_4ToBinaryimage(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	hand.middleSmooth(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);

	hand.First_border2B(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes,2);//
	for (int i = 0; i<2;i++)//腐蚀次数      ?????????  待调整
	{
		hand.Picture_FuShi(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	}
	for (i = 0;i<4;i++)//膨胀次数        ???????????  待调整
	{
		hand.Picture_PengZhang(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	}
	
	hand.boxOutHand(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//框出手部
	hand.get_broad_line(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//边界点


	char re[2];
	memset(re,0,sizeof(char)*2);
	//检查分割质量
	bool b = true;
	//b = hand.check_FG(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	if (b == true)
	{
		hand.get_OutLine(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//凸包
		hand.get_QueXian_vector(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//计算缺陷大小
		re[0] = hand.check();//识别手势
	}else
	{
		re[0] = '!';
	}

	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------
	/*
	if (re[0] == '!')//五个手指时
	{
		WinExec(_T("F:\\TTPlayer\\TTPlayer.exe"),SW_SHOWMAXIMIZED);
	}
	*/
	//::ExitProcess(0); //退出程序

	//显示结果                 '#'表示无法识别
	m_result.InsertString(-1,re);
	// 发音
	CCameraDlg c = new CCameraDlg();
	c.readRESULT(re);

	//更新数据
	SetDIBits(hDC,hm,0,(UINT)Bitmap.bmHeight,data, (BITMAPINFO *)lpbi,DIB_RGB_COLORS);
	m_staticresult.SetBitmap(hm);

	delete c;
	//-----------------------------------------------------------------------------------------------------------------------------------------------------

	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}
	//清除   
	GlobalUnlock(hDib);
	GlobalFree(hDib);	
}

void CStaticHand::OnSaveResult() //保存识别结果
{
	// TODO: Add your control notification handler code here
	CCameraDlg c = new CCameraDlg();
	CTime tm=CTime::GetCurrentTime();
	CString name=tm.Format(".\\pic\\%Y年%m月%d日%H时%M分%S秒.bmp");
	char* str=name.GetBuffer(name.GetLength()); 
	c.SaveBitmapToFile(hm,str);
	free(c);
}

void CStaticHand::OnButton3() //训练手势    写入数据文件
{
	// TODO: Add your control notification handler code here
	BITMAP Bitmap;
	GetObject(hm,sizeof(Bitmap),&Bitmap);//得到bitmap对象
	//char a[10]="";ultoa(bitmap.bmBitsPixel,a,10);
	//-----------------------------------------------------------------------------------------------------------------------------------------------
	LPBITMAPINFOHEADER lpbi;   
	HANDLE       hDib, hPal;
	BITMAPINFOHEADER   bi;
	HDC     hDC;  
	HPALETTE  hOldPal=NULL;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes  = 1;
	bi.biBitCount  = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	hDib = GlobalAlloc(GHND,Bitmap.bmWidthBytes*Bitmap.bmHeight+sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;   
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal=SelectPalette(hDC,(HPALETTE)hPal,FALSE);
		RealizePalette(hDC);
	}
	int p = GetDIBits(hDC,hm,0,(UINT)Bitmap.bmHeight,(LPSTR)lpbi+sizeof(BITMAPINFOHEADER), (BITMAPINFO *)lpbi,DIB_RGB_COLORS);		
	BYTE * data = (BYTE*)lpbi + sizeof(BITMAPINFOHEADER);//得到数据区指针 
	//-------------------------------------------------------------------------------------------------------------------------------------------------
	
	//形态学滤波      从硬盘读取二值化bmp文件
	Hand hand;
	hand.First_border2B(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes,2);//
	for (int i = 0; i<2;i++)//腐蚀次数
	{
		hand.Picture_FuShi(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	}
	for (i = 0;i<3;i++)//膨胀次数
	{
		hand.Picture_PengZhang(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	}
	hand.boxOutHand(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//框出手部
	hand.get_broad_line(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//边界点
	hand.get_OutLine(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//凸包
	hand.get_QueXian_vector(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//计算缺陷向量
	hand.Save_qx(n);//存储计算后的结果
	
	//更新数据
	SetDIBits(hDC,hm,0,(UINT)Bitmap.bmHeight,data, (BITMAPINFO *)lpbi,DIB_RGB_COLORS);
	m_staticresult.SetBitmap(hm);
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}
	//清除   
	GlobalUnlock(hDib);
	GlobalFree(hDib);	
}



BOOL CStaticHand::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DestroyWindow();
}

HBRUSH CStaticHand::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (pWnd == this) 
	{ 
		return m_brBk; 
	} 
	
	//设置控件透明
	if   (nCtlColor   ==   CTLCOLOR_STATIC)     
	{     

	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CStaticHand::OnButton4() //识别二值图
{
	// TODO: Add your control notification handler code here
	BITMAP Bitmap;
	GetObject(hm,sizeof(Bitmap),&Bitmap);//得到bitmap对象
	//-----------------------------------------------------------------------------------------------------------------------------------------------
	LPBITMAPINFOHEADER lpbi;   
	HANDLE       hDib, hPal;
	BITMAPINFOHEADER   bi;
	HDC     hDC;  
	HPALETTE  hOldPal=NULL;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes  = 1;
	bi.biBitCount  = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	hDib = GlobalAlloc(GHND,Bitmap.bmWidthBytes*Bitmap.bmHeight+sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;   
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal=SelectPalette(hDC,(HPALETTE)hPal,FALSE);
		RealizePalette(hDC);
	}
	int p = GetDIBits(hDC,hm,0,(UINT)Bitmap.bmHeight,(LPSTR)lpbi+sizeof(BITMAPINFOHEADER), (BITMAPINFO *)lpbi,DIB_RGB_COLORS);		
	BYTE * data = (BYTE*)lpbi + sizeof(BITMAPINFOHEADER);//得到数据区指针 
	//-------------------------------------------------------------------------------------------------------------------------------------------------
	Hand hand;
	//形态学滤波      从硬盘读取二值化bmp文件  (已经过 中值滤波)
	for (int i = 0; i<2;i++)//腐蚀次数      ?????????  待调整
	{
		hand.Picture_FuShi(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	}
	for (i = 0;i<4;i++)//膨胀次数        ???????????  待调整
	{
		hand.Picture_PengZhang(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	}
	hand.boxOutHand(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//框出手部
	hand.get_broad_line(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//边界点

	char re[2];
	memset(re,0,sizeof(char)*2);
	//检查分割质量
	bool b = true;
	b = hand.check_FG(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	if (b == true)
	{
		hand.get_OutLine(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//凸包
		hand.get_QueXian_vector(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//计算缺陷大小
		re[0] = hand.check();//识别手势
	}else
	{
		re[0] = '!';
	}

	//显示结果                 '#'表示无法识别
	m_result.InsertString(-1,re);

	// 发音
	CCameraDlg c = new CCameraDlg();
	c.readRESULT(re);

	//更新数据
	SetDIBits(hDC,hm,0,(UINT)Bitmap.bmHeight,data, (BITMAPINFO *)lpbi,DIB_RGB_COLORS);
	m_staticresult.SetBitmap(hm);

	delete c;
	//-----------------------------------------------------------------------------------------------------------------------------------------------------
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}
	//清除   
	GlobalUnlock(hDib);
	GlobalFree(hDib);	
}
