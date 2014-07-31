// cameraDlg.cpp : implementation file
//

#include "stdafx.h"
#include "camera.h"
#include "cameraDlg.h"
#include "HelpDlg.h"
#include "StaticHand.h"
#include<stdlib.h>
#include<afxwin.h>

#include <mmsystem.h>// ����


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
// CCameraDlg dialog   ʵ��

CListBox CCameraDlg::m_listboxresult;//��ʼ�� ��̬��Ա
CStatic	CCameraDlg::m_hand;
SYSTEMTIME CCameraDlg::ti;
CCameraDlg::CCameraDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCameraDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCameraDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);//ͼ���ʼ��
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

BEGIN_MESSAGE_MAP(CCameraDlg, CDialog)//��Ϣ������
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

BOOL CCameraDlg::OnInitDialog()   //��ʼ���Ի���
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
		strAboutMenu.LoadString(IDS_STRING102); //�Ҽ��˵���   �汾
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu); 

			strAboutMenu.LoadString(IDS_HELP);//�Ҽ��˵��� ����
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_HELP, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	//��������
	CString str;
	str.LoadString(IDC_STATIC01);
	CFont f;
	LOGFONT log;
	log.lfHeight = 20;
	log.lfWidth = 12;
	log.lfWeight  = 700;
	log.lfEscapement = 0;
	log.lfCharSet = GB2312_CHARSET;
	lstrcpy((LPSTR)log.lfFaceName,"����_GB2312");

	f.CreateFontIndirect(&log);
	this->GetDlgItem(IDC_LISTRESULT)->SetFont(&f,1);
	//bt.Create(

	//���´��� Ϊ������   ����ʶ����������� ���� listbox��  ���ڷ���
	CString s;
	s = "L";
	m_listboxresult.InsertString(-1,s);//-1 ��λ��
	s = "I";
	m_listboxresult.InsertString(-1,s);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCameraDlg::OnSysCommand(UINT nID, LPARAM lParam)//�˵�ѡ��
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)//�汾��Ϣ
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}else if((nID & 0xFFF0) == IDM_HELP)//��������
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
		bmpBackground.LoadBitmap(IDB_BG1);   //����ͼƬID
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


void CCameraDlg::OnButton1() //����
{
	// TODO: Add your control notification handler code here
	char   achDeviceVersion[80]   ; //   �豸�汾��Ϣ   
	char   achDeviceAndVersion[160];   //�豸�����汾��Ϣ   
	int   uIndex;   
	int DriverCount=0;//֧�ֵ��豸�����������   
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
        AfxMessageBox("�Ҳ�����Ƶ�豸����ȷ������ȷ����");   
	else   
	{   
		
      //CMainFrame   *pFrame=(CMainFrame   *)AfxGetApp()->m_pMainWnd;   
	  //CVideoView   *pView=(CVideoView   *)pFrame->GetActiveView();   
        HWND   hWnd;   
      //hWnd=pView->GetSafeHwnd();   
		hWnd=this->GetSafeHwnd();//��õ�ǰ���ڵľ��
		RECT   rc;   
		GetDlgItem(IDC_STATIC)->GetWindowRect(&rc); //IDC_STATIC λ��
		ScreenToClient(&rc);
		hWndCap=capCreateCaptureWindow(NULL, 
				WS_CHILD|WS_VISIBLE,
				(rc.left+2),
				(rc.top+2), 
				(rc.right-rc.left-4),
				(rc.bottom-rc.top-4),   
				hWnd,  //parent   window   
				0                     //���񴰿ڵ�  id   
			);//������Ƶ�ɼ����ڣ��൱��һ���ؼ�    ���÷��� picture �ؼ���λ����
		ASSERT(hWndCap);   
		if(capDriverConnect(hWndCap,--uIndex))//�жϲɼ������Ƿ���0�Ųɼ�������������   		
		{   
			CAPDRIVERCAPS CapDriverCaps;
			//��ȡ��Ƶ�豸�ɼ�����   
			capDriverGetCaps(hWnd,&CapDriverCaps,sizeof(CAPDRIVERCAPS));   
			if(CapDriverCaps.fCaptureInitialized)//��ʼ���ɹ�   			
			{   
				//���Ҫ������Ƶ������Ҫʹ�ú���ָ���������ļ������򽫻��Զ�����AVI�ļ� 
				//CapCaptureSequenceNoFile(hWndCap); 
				//����Previewģʽ��Ƶ֡   
				capPreviewRate(hWndCap,20);  //ÿ�� ���ٺ��� ����һ��   
				//����Previewģʽ   
				capPreview(hWndCap,   TRUE);   
				//����ÿ֡�����������õĻص�����   

				/*VIDEOHDR  *videohead;
				LRESULT CALLBACKCapture::OnCaptureVideo(){};
			    capSetCallbackOnVideoStream(hWndCap,OnCaptureVideo);
				*/
				capSetCallbackOnFrame(hWndCap,&(Hand::callBack));// callback() �ص����� 
				GetSystemTime(&ti);// �õ�ϵͳʱ��
				
				//���֧����ƵԴѡ���򵯳���ƵԴѡ��Ի���   
				if(CapDriverCaps.fHasDlgVideoSource)   
					capDlgVideoSource(hWnd);   
				/*
				//���֧����Ƶ��ʽѡ���򵯳���Ƶ��ʽѡ��Ի���   
				if(CapDriverCaps.fHasDlgVideoFormat)   
					capDlgVideoFormat(hWnd);   
				//���֧����Ƶ��ʾ��ʽѡ���򵯳���Ƶ��ʾ��ʽѡ��Ի���   
				if(CapDriverCaps.fHasDlgVideoDisplay)   
					capDlgVideoDisplay(hWnd);   
				*/
			}   
			else//��ʼ�����ɹ�   
			{
				//AfxMessageBox("���񿨳�ʼ��ʧ��");   
			}
        }   
		else   
		{
			//AfxMessageBox("��������ʧ��");   
		}
	}//if
}

HBITMAP CCameraDlg::CopyScreenToBitmap(LPRECT lpRect) //lpRect ����ѡ������
{
	HDC hScrDC, hMemDC;      
	// ��Ļ���ڴ��豸������
	HBITMAP hBitmap,hOldBitmap;   
	// λͼ���
	int       nX, nY, nX2, nY2;      
	// ѡ����������
	int       nWidth, nHeight;      
	// λͼ��Ⱥ͸߶�
	int       xScrn, yScrn;         
	// ��Ļ�ֱ���
	// ȷ��ѡ������Ϊ�վ���
	if (IsRectEmpty(lpRect))
		return NULL;
	//Ϊ��Ļ�����豸������
	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	//Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
	hMemDC = CreateCompatibleDC(hScrDC);
	// ���ѡ����������
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;
	// �����Ļ�ֱ���
	xScrn = GetDeviceCaps(hScrDC, HORZRES);
	yScrn = GetDeviceCaps(hScrDC, VERTRES);
	//ȷ��ѡ�������ǿɼ���
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

	// ����һ������Ļ�豸��������ݵ�λͼ
	hBitmap=CreateCompatibleBitmap(hScrDC,nWidth,nHeight);
	// ����λͼѡ���ڴ��豸��������
	hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBitmap);
	// ����Ļ�豸�����������ڴ��豸��������
	BitBlt(hMemDC,0,0, nWidth,nHeight,hScrDC, nX, nY, SRCCOPY);
	//�õ���Ļλͼ�ľ��
	hBitmap=(HBITMAP)SelectObject(hMemDC,hOldBitmap);
	//��� 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	// ����λͼ���
	return hBitmap;
}

int CCameraDlg::SaveBitmapToFile(HBITMAP hBitmap, LPSTR lpFileName) //hBitmap Ϊ�ղŵ���Ļλͼ���
{                                                                                                                        //lpFileName Ϊλͼ�ļ���
	HDC     hDC;         
	//�豸������
	int     iBits;      
	//��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���
	WORD    wBitCount;   
	//λͼ��ÿ��������ռ�ֽ���
	//�����ɫ���С�� λͼ�������ֽڴ�С �� λͼ�ļ���С �� д���ļ��ֽ���
	DWORD           dwPaletteSize=0,dwBmBitsSize,dwDIBSize, dwWritten;
	BITMAP          Bitmap;        
	//λͼ���Խṹ
	BITMAPFILEHEADER   bmfHdr;        
	//λͼ�ļ�ͷ�ṹ
	BITMAPINFOHEADER   bi;            
	//λͼ��Ϣͷ�ṹ 
	LPBITMAPINFOHEADER lpbi;          
	//ָ��λͼ��Ϣͷ�ṹ
	HANDLE          fh, hDib, hPal;
	HPALETTE     hOldPal=NULL;
	//�����ļ��������ڴ�������ɫ����

	//����λͼ�ļ�ÿ��������ռ�ֽ���
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
	//�����ɫ���С
	if (wBitCount <= 8)
		dwPaletteSize=(1<<wBitCount)*sizeof(RGBQUAD);

	//����λͼ��Ϣͷ�ṹ
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
	//Ϊλͼ���ݷ����ڴ�

	/*xxxxxxxx����λͼ��С�ֽ�һ��(����һ����������)xxxxxxxxxxxxxxxxxxxx 
	//ÿ��ɨ������ռ���ֽ���Ӧ��Ϊ4���������������㷨Ϊ:
	int biWidth = (Bitmap.bmWidth*wBitCount) / 32;
	if((Bitmap.bmWidth*wBitCount) % 32)
	biWidth++; //�����������ļ�1
	biWidth *= 4;//���������õ���Ϊÿ��ɨ���е��ֽ�����
	dwBmBitsSize = biWidth * Bitmap.bmHeight;//�õ���С
	xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/


	hDib = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;
	// �����ɫ��   
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal=SelectPalette(hDC,(HPALETTE)hPal,FALSE);
		RealizePalette(hDC);
	}
	// ��ȡ�õ�ɫ�����µ�����ֵ
	GetDIBits(hDC,hBitmap,0,(UINT)Bitmap.bmHeight,(LPSTR)lpbi+sizeof(BITMAPINFOHEADER)+dwPaletteSize, (BITMAPINFO *)lpbi,DIB_RGB_COLORS);
	//�ָ���ɫ��   
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}
	//����λͼ�ļ�    
	fh=CreateFile(lpFileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh==INVALID_HANDLE_VALUE)
	return FALSE;
	// ����λͼ�ļ�ͷ
	bmfHdr.bfType = 0x4D42; // "BM"
	dwDIBSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize; 
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER)+dwPaletteSize;
	// д��λͼ�ļ�ͷ
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	// д��λͼ�ļ���������
	WriteFile(fh, (LPSTR)lpbi, sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize , &dwWritten, NULL); 
	//���   
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	return TRUE;
}

void CCameraDlg::OnButton2() //ͼ���ȡ
{
	// TODO: Add your control notification handler code here
	CRect rect; 
	GetDlgItem(IDC_STATIC)->GetWindowRect(&rect);//�õ� picture�� ����λ��
	CTime tm;
	tm=CTime::GetCurrentTime();
	CString name;
	name=tm.Format(".\\pic\\%Y��%m��%d��%H%M%S.bmp");//ʱ���ʽ
	char * str;
	str=name.GetBuffer(name.GetLength());//ת���ַ�������
	
	HBITMAP hMap = CopyScreenToBitmap(rect); 
	SaveBitmapToFile(hMap,str);//�浽��ǰĿ¼��

	int select = MessageBox(TEXT("ͼ�񱣴���ϣ�   �鿴�ļ���"),TEXT("����ʶ�� 1.0"),MB_YESNO | MB_ICONASTERISK);//��ʾ��Ϣ
	if(select == IDYES)
	{
		//ShellExecute(HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd);
		ShellExecute(NULL,"open",".\\pic",NULL,NULL,SW_SHOWNORMAL);//�� Ŀ¼
	}
}


void CCameraDlg::OnRead() //����    
{
	// TODO: Add your control notification handler code here
	//SetDlgItemText(IDC_STATIC01,"");
	
	//SetDlgItemText(IDC_RESULT,m_result);//�ѽ����ʾ���ؼ���
	/*
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;//�ļ�ͷ��Ϣ
	CFile f;
	CString fileName;
	fileName = "a";
	f.Open(TEXT(".\\hand\\"+fileName+".bmp"),CFile::modeRead);
	f.Read(&bfh,sizeof(bfh));
	f.Read(&bih,sizeof(bih));//����bmp�ļ�
	long l = bih.biWidth;
	char *ch = "";
	ltoa(l,ch,10);
	m_result = ch;
	SetDlgItemText(IDC_TEST,m_result);
	f.Close();
	*/

	//m_listboxresult.InsertString(-1,str);//-1 ��λ��
	//str = "c";
	//m_listboxresult.InsertString(-1,str);
	//UpdateData(FALSE);//ˢ�±���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//���� listbox�е� ������ĸ  ���η���
	CString s;
	for( int i =0 ; i<m_listboxresult.GetCount();i++)
	{
		m_listboxresult.GetText(i,s);//�õ� ��ǰλ�� �� ��ĸ
		readRESULT(s);		
	}	
	//	

}
bool CCameraDlg::readRESULT(CString lpname) //�����ļ��� ��ȡ�����ļ�
{
	sndPlaySound(".//music//"+lpname+".wav",SND_SYNC);//SND_ASYNC�첽����    SND_SYNC ͬ������
	return true;
}

void CCameraDlg::OnButton3() // ֹͣͼ�񲶻�
{
	// TODO: Add your control notification handler code here
	//ֹͣ���� 
	capCaptureAbort(hWndCap);  
	//������ͬ�������Ͽ� 
    capDriverDisconnect(hWndCap);  
	
}

void CCameraDlg::showHand(CString name)//����ʶ������Ľ��  ��ʾ��Ӧ����      �ɻص���������
{	
	HBITMAP hm;
	hm = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),".\\hand\\"+name+".bmp",IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_DEFAULTSIZE|LR_LOADFROMFILE);
	m_hand.SetBitmap(hm);
}

void CCameraDlg::OnButton4() //ʶ��̬����
{
	// TODO: Add your control notification handler code here
	CStaticHand staticHand;
	staticHand.DoModal();
}
