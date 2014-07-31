#include "Hand.h"
//#include<dib.h>
#include "stdafx.h"

//捕获一帧后的回调函数

BOOL callBack(HWND hWndCap,LPVIDEOHDR videohead)//窗口句柄    捕获的视频流信息
{
	DWORD time = videohead->dwTimeCaptured;
	DWORD size = videohead->dwBytesUsed;//见结构体 LPVIDEOHDR

/*  //文件存取
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
	pWnd->GetWindowRect(&rect);//得到  坐标位置
    HBITMAP hBitmap = c.CopyScreenToBitmap(rect); //得到句柄

	BITMAP Bitmap;      
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);//得到图像

	LPBYTE data = (LPBYTE)Bitmap.bmBits;//数据区指针
	LONG lHeight = Bitmap.bmHeight;//高度
	LONG lWidth = Bitmap.bmWidth;//宽度
	LONG  widthBytes =  Bitmap.bmWidthBytes;//每一行 所占的字节数
    WORD  planes =  Bitmap.bmPlanes;//颜色平面数
    WORD  bitsPixel  =  Bitmap.bmBitsPixel;//像素的位数    4  X8 R8 G8 B8     X是明度通道

	BYTE x,r,g,b;//四个通道
	//for
	
    char* ch = "";//必须初始化
	ltoa((LONG)planes,ch,10);  //十进制
	CStdioFile file;
	file.Open(TEXT(".\\a.txt"),CFile::modeCreate|CFile::modeWrite | CFile::typeText);
	file.WriteString(ch);
	file.Close();

	










	CTime tm;
	tm=CTime::GetCurrentTime();
	CString name;
	name=tm.Format(".\\pic\\%Y年%m月%d日%H时%M分%S秒.bmp");//时间格式
	char * str;
	str=name.GetBuffer(name.GetLength()); //转成字符串数组
	c.SaveBitmapToFile(hBitmap,str);//存到当前目录下




	
	return 0;
}