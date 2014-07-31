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

HBITMAP hm;//���� �������Ĵ洢
/////////////////////////////////////////////////////////////////////////////
// CStaticHand dialog


CStaticHand::CStaticHand(CWnd* pParent /*=NULL*/)
	: CDialog(CStaticHand::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStaticHand)
	//}}AFX_DATA_INIT


	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);

}

BOOL CStaticHand::OnInitDialog()   //��ʼ���Ի���
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

void CStaticHand::OnOpen() //�򿪴�ʶ��ͼƬ
{
	// TODO: Add your control notification handler code here
	TCHAR szCurDirBackup[_MAX_PATH];   
	GetCurrentDirectory(sizeof(szCurDirBackup),szCurDirBackup); //�õ���ǰ·��ֵ    
	CFileDialog* lpszOpenFile; 
	lpszOpenFile = new CFileDialog(TRUE," "," ",OFN_FILEMUSTEXIST |OFN_HIDEREADONLY ,"��ʶ���ļ�(*.bmp)|*.bmp||");
	lpszOpenFile->m_ofn.lpstrInitialDir = szCurDirBackup;//��ǰĿ¼

	if( lpszOpenFile->DoModal() == IDOK)//ȷ����ť
	{
		CString szGetName;
		szGetName = lpszOpenFile->GetPathName();//�õ����ļ���·��
		n = lpszOpenFile->GetFileName();
		SetWindowText(szGetName); //�ڴ��ڱ�������ʾ·��
		//��ʾ���ؼ���
		hm = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),szGetName,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_DEFAULTSIZE|LR_LOADFROMFILE);
		m_statichand.SetBitmap(hm); 
	}

	delete lpszOpenFile;//�ͷŷ���ĶԻ���
	SetCurrentDirectory(szCurDirBackup);//���ûص�ǰ·����ΪCFileDlg����ʱ�Ὣ����ĵ�ǰ·���ĳ�ѡ���ļ���·��  
}

void CStaticHand::OnButton2() //ʶ��ԭʼͼ��
{
	// TODO: Add your control notification handler code here
	BITMAP Bitmap;
	GetObject(hm,sizeof(Bitmap),&Bitmap);//�õ�bitmap����
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
	BYTE * data = (BYTE*)lpbi + sizeof(BITMAPINFOHEADER);//�õ�������ָ�� 
	//-------------------------------------------------------------------------------------------------------------------------------------------------
	Hand hand;
	//��̬ѧ�˲�      ��Ӳ�̶�ȡ��ֵ��bmp�ļ�  (�Ѿ��� ��ֵ�˲�)
	
	hand.findHand_4ToBinaryimage(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	hand.middleSmooth(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);

	hand.First_border2B(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes,2);//
	for (int i = 0; i<2;i++)//��ʴ����      ?????????  ������
	{
		hand.Picture_FuShi(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	}
	for (i = 0;i<4;i++)//���ʹ���        ???????????  ������
	{
		hand.Picture_PengZhang(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	}
	
	hand.boxOutHand(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//����ֲ�
	hand.get_broad_line(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//�߽��


	char re[2];
	memset(re,0,sizeof(char)*2);
	//���ָ�����
	bool b = true;
	//b = hand.check_FG(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	if (b == true)
	{
		hand.get_OutLine(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//͹��
		hand.get_QueXian_vector(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//����ȱ�ݴ�С
		re[0] = hand.check();//ʶ������
	}else
	{
		re[0] = '!';
	}

	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------
	/*
	if (re[0] == '!')//�����ָʱ
	{
		WinExec(_T("F:\\TTPlayer\\TTPlayer.exe"),SW_SHOWMAXIMIZED);
	}
	*/
	//::ExitProcess(0); //�˳�����

	//��ʾ���                 '#'��ʾ�޷�ʶ��
	m_result.InsertString(-1,re);
	// ����
	CCameraDlg c = new CCameraDlg();
	c.readRESULT(re);

	//��������
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
	//���   
	GlobalUnlock(hDib);
	GlobalFree(hDib);	
}

void CStaticHand::OnSaveResult() //����ʶ����
{
	// TODO: Add your control notification handler code here
	CCameraDlg c = new CCameraDlg();
	CTime tm=CTime::GetCurrentTime();
	CString name=tm.Format(".\\pic\\%Y��%m��%d��%Hʱ%M��%S��.bmp");
	char* str=name.GetBuffer(name.GetLength()); 
	c.SaveBitmapToFile(hm,str);
	free(c);
}

void CStaticHand::OnButton3() //ѵ������    д�������ļ�
{
	// TODO: Add your control notification handler code here
	BITMAP Bitmap;
	GetObject(hm,sizeof(Bitmap),&Bitmap);//�õ�bitmap����
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
	BYTE * data = (BYTE*)lpbi + sizeof(BITMAPINFOHEADER);//�õ�������ָ�� 
	//-------------------------------------------------------------------------------------------------------------------------------------------------
	
	//��̬ѧ�˲�      ��Ӳ�̶�ȡ��ֵ��bmp�ļ�
	Hand hand;
	hand.First_border2B(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes,2);//
	for (int i = 0; i<2;i++)//��ʴ����
	{
		hand.Picture_FuShi(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	}
	for (i = 0;i<3;i++)//���ʹ���
	{
		hand.Picture_PengZhang(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	}
	hand.boxOutHand(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//����ֲ�
	hand.get_broad_line(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//�߽��
	hand.get_OutLine(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//͹��
	hand.get_QueXian_vector(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//����ȱ������
	hand.Save_qx(n);//�洢�����Ľ��
	
	//��������
	SetDIBits(hDC,hm,0,(UINT)Bitmap.bmHeight,data, (BITMAPINFO *)lpbi,DIB_RGB_COLORS);
	m_staticresult.SetBitmap(hm);
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}
	//���   
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
	
	//���ÿؼ�͸��
	if   (nCtlColor   ==   CTLCOLOR_STATIC)     
	{     

	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CStaticHand::OnButton4() //ʶ���ֵͼ
{
	// TODO: Add your control notification handler code here
	BITMAP Bitmap;
	GetObject(hm,sizeof(Bitmap),&Bitmap);//�õ�bitmap����
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
	BYTE * data = (BYTE*)lpbi + sizeof(BITMAPINFOHEADER);//�õ�������ָ�� 
	//-------------------------------------------------------------------------------------------------------------------------------------------------
	Hand hand;
	//��̬ѧ�˲�      ��Ӳ�̶�ȡ��ֵ��bmp�ļ�  (�Ѿ��� ��ֵ�˲�)
	for (int i = 0; i<2;i++)//��ʴ����      ?????????  ������
	{
		hand.Picture_FuShi(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	}
	for (i = 0;i<4;i++)//���ʹ���        ???????????  ������
	{
		hand.Picture_PengZhang(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	}
	hand.boxOutHand(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//����ֲ�
	hand.get_broad_line(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//�߽��

	char re[2];
	memset(re,0,sizeof(char)*2);
	//���ָ�����
	bool b = true;
	b = hand.check_FG(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	if (b == true)
	{
		hand.get_OutLine(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//͹��
		hand.get_QueXian_vector(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//����ȱ�ݴ�С
		re[0] = hand.check();//ʶ������
	}else
	{
		re[0] = '!';
	}

	//��ʾ���                 '#'��ʾ�޷�ʶ��
	m_result.InsertString(-1,re);

	// ����
	CCameraDlg c = new CCameraDlg();
	c.readRESULT(re);

	//��������
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
	//���   
	GlobalUnlock(hDib);
	GlobalFree(hDib);	
}
