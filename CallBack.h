#include "Hand.h"
//#include<dib.h>
#include "stdafx.h"

//����һ֡��Ļص�����

BOOL callBack(HWND hWndCap,LPVIDEOHDR videohead)//���ھ��    �������Ƶ����Ϣ
{
	DWORD time = videohead->dwTimeCaptured;
	DWORD size = videohead->dwBytesUsed;//���ṹ�� LPVIDEOHDR

/*  //�ļ���ȡ
	CStdioFile file;
	file.Open(TEXT("..\\b.txt"),CFile::modeCreate|CFile::modeWrite | CFile::typeText);
	file.WriteString(videohead->lpData);
	file.Close();
*/
/*
	CFile file;
	file.Open(TEXT(".\\a.txt"),CFile::modeCreate|CFile::modeWrite);
//	file.Write(videohead->lpData,videohead->dwBufferLength);
	file.Write(&lHeight,sizeof(LONG));
	file.Close();
*/


	CCameraDlg c = new CCameraDlg();
	
	CRect rect; 
	CWnd* pWnd = new CWnd(); 
	pWnd->Attach(hWndCap);
	pWnd->GetWindowRect(&rect);//�õ�  ����λ��
    HBITMAP hBitmap = c.CopyScreenToBitmap(rect); //�õ����

	BITMAP Bitmap;      
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);//�õ�ͼ��

	LPBYTE data = (LPBYTE)Bitmap.bmBits;//������ָ��
	LONG lHeight = Bitmap.bmHeight;//�߶�
	LONG lWidth = Bitmap.bmWidth;//���
	LONG  widthBytes =  Bitmap.bmWidthBytes;//ÿһ�� ��ռ���ֽ���
    WORD  planes =  Bitmap.bmPlanes;//��ɫƽ����
    WORD  bitsPixel  =  Bitmap.bmBitsPixel;//���ص�λ��    4  X8 R8 G8 B8     X������ͨ��

	BYTE x,r,g,b;//�ĸ�ͨ��
	//for
	
    char* ch = "";//�����ʼ��
	ltoa((LONG)planes,ch,10);  //ʮ����
	CStdioFile file;
	file.Open(TEXT(".\\a.txt"),CFile::modeCreate|CFile::modeWrite | CFile::typeText);
	file.WriteString(ch);
	file.Close();

	










	CTime tm;
	tm=CTime::GetCurrentTime();
	CString name;
	name=tm.Format(".\\pic\\%Y��%m��%d��%Hʱ%M��%S��.bmp");//ʱ���ʽ
	char * str;
	str=name.GetBuffer(name.GetLength()); //ת���ַ�������
	c.SaveBitmapToFile(hBitmap,str);//�浽��ǰĿ¼��




	
	return 0;
}