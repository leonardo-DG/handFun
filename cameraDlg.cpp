// cameraDlg.cpp : implementation file
//

#include "stdafx.h"
#include "camera.h"
#include "cameraDlg.h"
#include "HelpDlg.h"
#include "StaticHand.h"
#include<stdlib.h>
#include<afxwin.h>

#include <mmsystem.h>// 发音


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCameraDlg dialog   实现

CListBox CCameraDlg::m_listboxresult;//初始化 静态成员
CStatic	CCameraDlg::m_hand;
SYSTEMTIME CCameraDlg::ti;
CCameraDlg::CCameraDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCameraDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCameraDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);//图标初始化
//	AfxGetApp()->LoadButton();
}

void CCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCameraDlg)
	DDX_Control(pDX, IDC_STATIC1, m_hand);
	DDX_Control(pDX, IDC_START, m_start);
	DDX_Control(pDX, IDC_READ, m_read);
	DDX_Control(pDX, IDC_LISTRESULT, m_listboxresult);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCameraDlg, CDialog)//消息处理函数
	//{{AFX_MSG_MAP(CCameraDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_READ, OnRead)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCameraDlg message handlers

BOOL CCameraDlg::OnInitDialog()   //初始化对话框
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	::CMenu* pSysMenu = GetSystemMenu(FALSE);//
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_STRING102); //右键菜单项   版本
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu); 

			strAboutMenu.LoadString(IDS_HELP);//右键菜单项 帮助
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_HELP, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	//设置字体
	CString str;
	str.LoadString(IDC_STATIC01);
	CFont f;
	LOGFONT log;
	log.lfHeight = 20;
	log.lfWidth = 12;
	log.lfWeight  = 700;
	log.lfEscapement = 0;
	log.lfCharSet = GB2312_CHARSET;
	lstrcpy((LPSTR)log.lfFaceName,"楷体_GB2312");

	f.CreateFontIndirect(&log);
	this->GetDlgItem(IDC_LISTRESULT)->SetFont(&f,1);
	//bt.Create(

	//以下代码 为测试用   假设识别出来的手势 已在 listbox中  用于发音
	CString s;
	s = "L";
	m_listboxresult.InsertString(-1,s);//-1 是位置
	s = "I";
	m_listboxresult.InsertString(-1,s);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCameraDlg::OnSysCommand(UINT nID, LPARAM lParam)//菜单选择
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)//版本信息
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}else if((nID & 0xFFF0) == IDM_HELP)//帮助窗口
	{
		CHelpDlg dlgHelp;
		dlgHelp.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCameraDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialog::OnPaint();
		CPaintDC   dc(this);   
		CRect   rect;   
		GetClientRect(&rect);   
		CDC   dcMem;   
		dcMem.CreateCompatibleDC(&dc);   
		CBitmap   bmpBackground;   
		bmpBackground.LoadBitmap(IDB_BG1);   //背景图片ID
		BITMAP   bitmap;   
		bmpBackground.GetBitmap(&bitmap);   
		CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);   
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,   
        bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCameraDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}  

/*
BOOL CCameraDlg::callBack(HWND hWndCap,LPVIDEOHDR videohead)
{
	return 0;
}*/


void CCameraDlg::OnButton1() //启动
{
	// TODO: Add your control notification handler code here
	char   achDeviceVersion[80]   ; //   设备版本信息   
	char   achDeviceAndVersion[160];   //设备名及版本信息   
	int   uIndex;   
	int DriverCount=0;//支持的设备驱动程序个数   
	for(uIndex=0;uIndex<5;uIndex++)   
	{   
        if(capGetDriverDescription(uIndex,(LPSTR)achDeviceAndVersion,
							sizeof(achDeviceAndVersion),(LPSTR)achDeviceVersion,sizeof(achDeviceVersion)))   
        {   
			strcat(achDeviceAndVersion,",");   
			strcat(achDeviceAndVersion,achDeviceVersion);   
			DriverCount++;   	
        }   
        else   
			break;   
	}   
    
	if(DriverCount==0)   
        AfxMessageBox("找不到视频设备，请确认已正确连接");   
	else   
	{   
		
      //CMainFrame   *pFrame=(CMainFrame   *)AfxGetApp()->m_pMainWnd;   
	  //CVideoView   *pView=(CVideoView   *)pFrame->GetActiveView();   
        HWND   hWnd;   
      //hWnd=pView->GetSafeHwnd();   
		hWnd=this->GetSafeHwnd();//获得当前窗口的句柄
		RECT   rc;   
		GetDlgItem(IDC_STATIC)->GetWindowRect(&rc); //IDC_STATIC 位置
		ScreenToClient(&rc);
		hWndCap=capCreateCaptureWindow(NULL, 
				WS_CHILD|WS_VISIBLE,
				(rc.left+2),
				(rc.top+2), 
				(rc.right-rc.left-4),
				(rc.bottom-rc.top-4),   
				hWnd,  //parent   window   
				0                     //捕获窗口的  id   
			);//建立视频采集窗口，相当于一个控件    正好放在 picture 控件的位置上
		ASSERT(hWndCap);   
		if(capDriverConnect(hWndCap,--uIndex))//判断采集窗口是否与0号采集卡驱动相连接   		
		{   
			CAPDRIVERCAPS CapDriverCaps;
			//获取视频设备采集能力   
			capDriverGetCaps(hWnd,&CapDriverCaps,sizeof(CAPDRIVERCAPS));   
			if(CapDriverCaps.fCaptureInitialized)//初始化成功   			
			{   
				//如果要捕获视频流，则要使用函数指定不生成文件。否则将会自动生成AVI文件 
				//CapCaptureSequenceNoFile(hWndCap); 
				//设置Preview模式的频帧   
				capPreviewRate(hWndCap,20);  //每隔 多少毫妙 捕获一次   
				//启动Preview模式   
				capPreview(hWndCap,   TRUE);   
				//设置每帧结束后所调用的回调函数   

				/*VIDEOHDR  *videohead;
				LRESULT CALLBACKCapture::OnCaptureVideo(){};
			    capSetCallbackOnVideoStream(hWndCap,OnCaptureVideo);
				*/
				capSetCallbackOnFrame(hWndCap,&(Hand::callBack));// callback() 回调函数 
				GetSystemTime(&ti);// 得到系统时间
				
				//如果支持视频源选择，则弹出视频源选择对话框   
				if(CapDriverCaps.fHasDlgVideoSource)   
					capDlgVideoSource(hWnd);   
				/*
				//如果支持视频格式选择，则弹出视频格式选择对话框   
				if(CapDriverCaps.fHasDlgVideoFormat)   
					capDlgVideoFormat(hWnd);   
				//如果支持视频显示格式选择，则弹出视频显示格式选择对话框   
				if(CapDriverCaps.fHasDlgVideoDisplay)   
					capDlgVideoDisplay(hWnd);   
				*/
			}   
			else//初始化不成功   
			{
				//AfxMessageBox("捕获卡初始化失败");   
			}
        }   
		else   
		{
			//AfxMessageBox("捕获卡连接失败");   
		}
	}//if
}

HBITMAP CCameraDlg::CopyScreenToBitmap(LPRECT lpRect) //lpRect 代表选定区域
{
	HDC hScrDC, hMemDC;      
	// 屏幕和内存设备描述表
	HBITMAP hBitmap,hOldBitmap;   
	// 位图句柄
	int       nX, nY, nX2, nY2;      
	// 选定区域坐标
	int       nWidth, nHeight;      
	// 位图宽度和高度
	int       xScrn, yScrn;         
	// 屏幕分辨率
	// 确保选定区域不为空矩形
	if (IsRectEmpty(lpRect))
		return NULL;
	//为屏幕创建设备描述表
	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	//为屏幕设备描述表创建兼容的内存设备描述表
	hMemDC = CreateCompatibleDC(hScrDC);
	// 获得选定区域坐标
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;
	// 获得屏幕分辨率
	xScrn = GetDeviceCaps(hScrDC, HORZRES);
	yScrn = GetDeviceCaps(hScrDC, VERTRES);
	//确保选定区域是可见的
	if (nX < 0)
		nX = 0;
	if (nY < 0)
		nY = 0;
	if (nX2 > xScrn)
		nX2 = xScrn;
	if (nY2 > yScrn)
		nY2 = yScrn;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;

	// 创建一个与屏幕设备描述表兼容的位图
	hBitmap=CreateCompatibleBitmap(hScrDC,nWidth,nHeight);
	// 把新位图选到内存设备描述表中
	hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBitmap);
	// 把屏幕设备描述表拷贝到内存设备描述表中
	BitBlt(hMemDC,0,0, nWidth,nHeight,hScrDC, nX, nY, SRCCOPY);
	//得到屏幕位图的句柄
	hBitmap=(HBITMAP)SelectObject(hMemDC,hOldBitmap);
	//清除 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	// 返回位图句柄
	return hBitmap;
}

int CCameraDlg::SaveBitmapToFile(HBITMAP hBitmap, LPSTR lpFileName) //hBitmap 为刚才的屏幕位图句柄
{                                                                                                                        //lpFileName 为位图文件名
	HDC     hDC;         
	//设备描述表
	int     iBits;      
	//当前显示分辨率下每个像素所占字节数
	WORD    wBitCount;   
	//位图中每个像素所占字节数
	//定义调色板大小， 位图中像素字节大小 ， 位图文件大小 ， 写入文件字节数
	DWORD           dwPaletteSize=0,dwBmBitsSize,dwDIBSize, dwWritten;
	BITMAP          Bitmap;        
	//位图属性结构
	BITMAPFILEHEADER   bmfHdr;        
	//位图文件头结构
	BITMAPINFOHEADER   bi;            
	//位图信息头结构 
	LPBITMAPINFOHEADER lpbi;          
	//指向位图信息头结构
	HANDLE          fh, hDib, hPal;
	HPALETTE     hOldPal=NULL;
	//定义文件，分配内存句柄，调色板句柄

	//计算位图文件每个像素所占字节数
	hDC = CreateDC("DISPLAY",NULL,NULL,NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else if (iBits <= 24)
		wBitCount = 24;
	else
		wBitCount = 32;
	//计算调色板大小
	if (wBitCount <= 8)
		dwPaletteSize=(1<<wBitCount)*sizeof(RGBQUAD);

	//设置位图信息头结构
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);

	bi.biSize            = sizeof(BITMAPINFOHEADER);
	bi.biWidth           = Bitmap.bmWidth;
	bi.biHeight          = Bitmap.bmHeight;
	bi.biPlanes          = 1;
	bi.biBitCount         = wBitCount;
	bi.biCompression      = BI_RGB;
	bi.biSizeImage        = 0;
	bi.biXPelsPerMeter     = 0;
	bi.biYPelsPerMeter     = 0;
	bi.biClrUsed         = 0;
	bi.biClrImportant      = 0;

	dwBmBitsSize = ((Bitmap.bmWidth*wBitCount+31)/32)*4*Bitmap.bmHeight;
	//为位图内容分配内存

	/*xxxxxxxx计算位图大小分解一下(解释一下上面的语句)xxxxxxxxxxxxxxxxxxxx 
	//每个扫描行所占的字节数应该为4的整数倍，具体算法为:
	int biWidth = (Bitmap.bmWidth*wBitCount) / 32;
	if((Bitmap.bmWidth*wBitCount) % 32)
	biWidth++; //不是整数倍的加1
	biWidth *= 4;//到这里，计算得到的为每个扫描行的字节数。
	dwBmBitsSize = biWidth * Bitmap.bmHeight;//得到大小
	xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/


	hDib = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;
	// 处理调色板   
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal=SelectPalette(hDC,(HPALETTE)hPal,FALSE);
		RealizePalette(hDC);
	}
	// 获取该调色板下新的像素值
	GetDIBits(hDC,hBitmap,0,(UINT)Bitmap.bmHeight,(LPSTR)lpbi+sizeof(BITMAPINFOHEADER)+dwPaletteSize, (BITMAPINFO *)lpbi,DIB_RGB_COLORS);
	//恢复调色板   
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}
	//创建位图文件    
	fh=CreateFile(lpFileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh==INVALID_HANDLE_VALUE)
	return FALSE;
	// 设置位图文件头
	bmfHdr.bfType = 0x4D42; // "BM"
	dwDIBSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize; 
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER)+dwPaletteSize;
	// 写入位图文件头
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	// 写入位图文件其余内容
	WriteFile(fh, (LPSTR)lpbi, sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize , &dwWritten, NULL); 
	//清除   
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	return TRUE;
}

void CCameraDlg::OnButton2() //图像截取
{
	// TODO: Add your control notification handler code here
	CRect rect; 
	GetDlgItem(IDC_STATIC)->GetWindowRect(&rect);//得到 picture的 坐标位置
	CTime tm;
	tm=CTime::GetCurrentTime();
	CString name;
	name=tm.Format(".\\pic\\%Y年%m月%d日%H%M%S.bmp");//时间格式
	char * str;
	str=name.GetBuffer(name.GetLength());//转成字符串数组
	
	HBITMAP hMap = CopyScreenToBitmap(rect); 
	SaveBitmapToFile(hMap,str);//存到当前目录下

	int select = MessageBox(TEXT("图像保存完毕！   查看文件？"),TEXT("手势识别 1.0"),MB_YESNO | MB_ICONASTERISK);//提示信息
	if(select == IDYES)
	{
		//ShellExecute(HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd);
		ShellExecute(NULL,"open",".\\pic",NULL,NULL,SW_SHOWNORMAL);//打开 目录
	}
}


void CCameraDlg::OnRead() //发音    
{
	// TODO: Add your control notification handler code here
	//SetDlgItemText(IDC_STATIC01,"");
	
	//SetDlgItemText(IDC_RESULT,m_result);//把结果显示到控件上
	/*
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;//文件头信息
	CFile f;
	CString fileName;
	fileName = "a";
	f.Open(TEXT(".\\hand\\"+fileName+".bmp"),CFile::modeRead);
	f.Read(&bfh,sizeof(bfh));
	f.Read(&bih,sizeof(bih));//读出bmp文件
	long l = bih.biWidth;
	char *ch = "";
	ltoa(l,ch,10);
	m_result = ch;
	SetDlgItemText(IDC_TEST,m_result);
	f.Close();
	*/

	//m_listboxresult.InsertString(-1,str);//-1 是位置
	//str = "c";
	//m_listboxresult.InsertString(-1,str);
	//UpdateData(FALSE);//刷新变量
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//读出 listbox中的 所有字母  依次发音
	CString s;
	for( int i =0 ; i<m_listboxresult.GetCount();i++)
	{
		m_listboxresult.GetText(i,s);//得到 当前位置 的 字母
		readRESULT(s);		
	}	
	//	

}
bool CCameraDlg::readRESULT(CString lpname) //根据文件名 读取声音文件
{
	sndPlaySound(".//music//"+lpname+".wav",SND_SYNC);//SND_ASYNC异步发音    SND_SYNC 同步发音
	return true;
}

void CCameraDlg::OnButton3() // 停止图像捕获
{
	// TODO: Add your control notification handler code here
	//停止捕获 
	capCaptureAbort(hWndCap);  
	//将捕获窗同驱动器断开 
    capDriverDisconnect(hWndCap);  
	
}

void CCameraDlg::showHand(CString name)//根据识别出来的结果  显示相应手势      由回调函数调用
{	
	HBITMAP hm;
	hm = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),".\\hand\\"+name+".bmp",IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_DEFAULTSIZE|LR_LOADFROMFILE);
	m_hand.SetBitmap(hm);
}

void CCameraDlg::OnButton4() //识别静态手势
{
	// TODO: Add your control notification handler code here
	CStaticHand staticHand;
	staticHand.DoModal();
}
