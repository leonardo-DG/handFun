#include "stdafx.h"
#include "Hand.h"
#include "cameraDlg.h"
#include "resource.h"
#include "tools.h"
#include "math.h"
#include "afxwin.h"
#include <math.h>
#include<stdio.h>
#include <fstream.h>

int k;//红框宽度
int g;//红框高度

spoint* A=NULL;//边界点
int num;

spoint* B=NULL;//最大轮廓上的点
int num_r;

int* num_q = NULL;//缺陷向量

double area;//手势面积

void Hand::boxOutHand(BYTE *data,int Height,int Width,int widthBytes)//框出手部
{
	bool flag_1=FALSE,flag_2=FALSE,flag_3=FALSE,flag_4=FALSE;
	int a[4][2];//存储边界点
	for (int i =0; i<Height;i++)
	{
		for (int j = 0; j< Width;j++)
		{
			if (data[i*widthBytes + j*4] == 0xFF)
			{
				if (flag_1 == FALSE)//最低点
				{
					a[0][0] = i;
					a[0][1] = j;
					flag_1 = TRUE;
				}
			}
			if (data[(Height - i -1)*widthBytes + j*4] == 0xFF)
			{
				if (flag_2 == FALSE)//最高点
				{
					a[1][0] = Height - i -1;
					a[1][1] = j;
					flag_2 = TRUE;
				}
			}
		}
	}
	for (i =0; i<Width;i++)
	{
		for (int j = 0; j< Height;j++)
		{
			if (data[j*widthBytes + i*4] == 0xFF)
			{
				if (flag_3 == FALSE)//最左点
				{
					a[2][0] = j;
					a[2][1] = i;
					flag_3 = TRUE;
				}
			}
			if (data[j*widthBytes + (Width -i - 1)*4] == 0xFF)
			{
				if (flag_4 == FALSE)//最右点
				{
					a[3][0] = j;
					a[3][1] = Width - i -1;
					flag_4 = TRUE;
				}
			}
		}
	}
	g = a[3][1] - a[2][1];//矩形框 高度
	for(i = a[2][1];i<a[3][1];i++)//上下
	{
		data[a[1][0]*widthBytes + i*4] = 0x00;
		data[a[1][0]*widthBytes + i*4+1] = 0x00;
		data[a[1][0]*widthBytes + i*4+2] = 0xFF;

		data[a[0][0]*widthBytes + i*4] = 0x00;
		data[a[0][0]*widthBytes + i*4+1] = 0x00;
		data[a[0][0]*widthBytes + i*4+2] = 0xFF;
	}
	k = a[1][0] - a[0][0];//矩形框 宽度
	for (i = a[0][0];i<a[1][0];i++)
	{
		data[i*widthBytes + a[2][1]*4] = 0x00;
		data[i*widthBytes + a[2][1]*4+1] = 0x00;
		data[i*widthBytes + a[2][1]*4+2] = 0xFF;
		
		data[i*widthBytes + a[3][1]*4] = 0x00;
		data[i*widthBytes + a[3][1]*4+1] = 0x00;
		data[i*widthBytes + a[3][1]*4+2] = 0xFF;
	}	
}

//捕获一帧后的回调函数
BOOL  Hand::callBack(HWND hWndCap,LPVIDEOHDR videohead)//窗口句柄    捕获的视频流信息（没有直接使用）
{
	Hand hand;//在静态函数中访问非静态成员
	//设置时间  每隔5秒识别一次
	SYSTEMTIME cti;
	GetSystemTime(&cti);
	if ((CCameraDlg::ti.wSecond - cti.wSecond)%5 != 0)
	{
		return false;
	}
	//hand.setHWND(hWndCap);
	//DWORD time = videohead->dwTimeCaptured;
	//DWORD size = videohead->dwBytesUsed;//见结构体 LPVIDEOHDR
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
	    char ch[9] = "";//必须初始化
	itoa(R,ch,16);  //十进制
*/
	CCameraDlg c = new CCameraDlg();
	CRect rect; 
	CWnd* pWnd = new CWnd(); 
	pWnd->Attach(hWndCap);
	pWnd->GetWindowRect(&rect);//得到  坐标位置
    HBITMAP hBitmap = c.CopyScreenToBitmap(rect); //得到句柄

	BITMAP Bitmap;      
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);//得到bmp图像
	LONG  widthBytes =  Bitmap.bmWidthBytes;//每一行 所占的字节数
/*
	//LPBYTE data = (LPBYTE)Bitmap.bmBits;//数据区指针
	LONG lHeight = Bitmap.bmHeight;//高度
	LONG lWidth = Bitmap.bmWidth;//宽度
	LONG  widthBytes =  Bitmap.bmWidthBytes;//每一行 所占的字节数
    WORD  planes =  Bitmap.bmPlanes;//颜色平面数
    WORD  bitsPixel  =  Bitmap.bmBitsPixel;//像素的位数    4  X8 R8 G8 B8     X是明度通道
*/
	//得到 像素矩阵
	//CBitmap bmp;
	//bmp.GetBitmap(&Bitmap);
/*
A:	BYTE* data = (BYTE*)GlobalAlloc(GPTR,widthBytes*lHeight);//动态申请
	if (data==0)
		goto A;
	bmp.GetBitmapBits(widthBytes*lHeight,data);
*/
	
	LPBITMAPINFOHEADER lpbi;   
	HANDLE       hDib, hPal;
	BITMAPINFOHEADER   bi;
	HDC     hDC = ::GetDC(hWndCap);   //必须得到 DC
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
	// 处理调色板   
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal=SelectPalette(hDC,(HPALETTE)hPal,FALSE);
		RealizePalette(hDC);
	}
	// 获取该调色板下新的像素值
	int p = GetDIBits(hDC,hBitmap,0,(UINT)Bitmap.bmHeight,(LPSTR)lpbi+sizeof(BITMAPINFOHEADER), (BITMAPINFO *)lpbi,DIB_RGB_COLORS);
	
	//保存原始图像
	CTime tm;
	tm=CTime::GetCurrentTime();
	CString name;
	name=tm.Format(".\\pic\\%Y年%m月%d日%H时%M分%S秒.bmp");//时间格式
	char * str;
	str=name.GetBuffer(name.GetLength()); //转成字符串数组
	c.SaveBitmapToFile(hBitmap,str);//存到当前目录下
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	BYTE * data = (BYTE*)lpbi + sizeof(BITMAPINFOHEADER);//得到数据区指针 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	//1. 手势分割
	//hand.RgbToGrade(data,Bitmap.bmHeight,Bitmap.bmWidth,widthBytes);//转成灰度图
	hand.findHand_1ToBinaryimage(data,Bitmap.bmHeight,Bitmap.bmWidth,widthBytes);
	//hand.findHand_2ToBinaryimage(data,Bitmap.bmHeight,Bitmap.bmWidth,widthBytes);
	//hand.findHand_3ToBinaryimage(data,Bitmap.bmHeight,Bitmap.bmWidth,widthBytes);
//---------------------------------------------------------------------------------------------------------------------------------------------	
	//2. 滤波处理  采用中值滤波 
	hand.middleSmooth(data,Bitmap.bmHeight,Bitmap.bmWidth,widthBytes);
//---------------------------------------------------------------------------------------------------------------------------------------------	
	//3.形态学滤波   先腐蚀再膨胀（开运算）去除噪声点；   先膨胀再腐蚀（闭运算）填补孔洞     	
	hand.First_border2B(data,Bitmap.bmHeight,Bitmap.bmWidth,widthBytes,2);//把边界的两个像素图黑

	for (int i = 0; i<2;i++)//腐蚀次数
	{
		hand.Picture_FuShi(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	}
	for (i = 0;i<3;i++)//膨胀次数
	{
		hand.Picture_PengZhang(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	}
	//效果仍然不好，考虑自己的方案 完全处理掉孔洞?
//---------------------------------------------------------------------------------------------------------------------------------------------
	//4.最大轮廓提取               
	hand.boxOutHand(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//框出手部
//----------------------------------------------------------------------------------------------------------------------------------------------
	//如果图像中没有手势 则get_broad_line（）函数不会提取到任何边缘点 所以这里也要判断一次
	int b_broad = hand.get_broad_line(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//边界点
	if (b_broad == -1)
	{
		return false;
	}
//---------------------------------------------------------------------------------------------------------------------------------------------
	char re[2];
	memset(re,0,sizeof(char)*2);
	if (b_broad == 1)
	{	
		//要进行后面几步  必须保证图像已处理的非常好了    这里检查是否完全分割出手势（一个连通域）
		bool bl = hand.check_FG(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
		if (bl == true)//仅有手势
		{
			hand.get_OutLine(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//凸包
			//5.特征提取 
			hand.get_QueXian_vector(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//计算缺陷大小
			//6.手势识别
			re[0] = hand.check();
		}else//有很大的干扰
		{
			re[0] = '!';
		}
	}else
	{
		re[0] = '#';
	}
	hand.free_A();
//---------------------------------------------------------------------------------------------------------------------------------------------
	//7. 显示 识别结果                                                                                                                                                                                                                                                 
	c.m_listboxresult.InsertString(-1,re);
	c.showHand(re);
//---------------------------------------------------------------------------------------------------------------------------------------------
	//8.发音
	c.readRESULT(re);
	delete c;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//更新 像素数据
	int nam = SetDIBits(hDC,hBitmap,0,(UINT)Bitmap.bmHeight,data, (BITMAPINFO *)lpbi,DIB_RGB_COLORS);
	//bmp.SetBitmapBits(Bitmap.bmWidthBytes*Bitmap.bmHeight,data);//更新bmp数据区
	//DWORD d = GetLastError();
	tm=CTime::GetCurrentTime();
	name=tm.Format(".\\pic\\%Y年%m月%d日%H时%M分%S秒.bmp");//时间格式
	str=name.GetBuffer(name.GetLength()); //转成字符串数组
	c.SaveBitmapToFile(hBitmap,str);//存到当前目录下
	//恢复调色板   
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}
	//清除   
	GlobalUnlock(hDib);
	GlobalFree(hDib);		
	return true;
}

double Hand::RGBToHSV(int r, int g, int b)//得到 色度信息
{
	double h;
	int max = maxRGB(r,g,b);
	int min = minRGB(r,g,b);
	double d = max - min;
	if (max = min)
	{
		h = 0;
	} 
	else if (max == r && g>=b)
	{
		h= 60*(g-b)/d;
	}else if (max == r && g<b)
	{
		h = 60*(g-b)/d+360;
	} 
	else if (max == g)
	{
		h = 60*(b-r)/d+120;
	} 
	else if(max == b)
	{
		h = 60*(r-g)/d+240;
	}
	return h;
}

void Hand::RgbToGrade(BYTE *data,int Height,int Width,int widthBytes)//灰度图
{
	BYTE r,g,b;//       BYTE 实际上是 unsigned char
	//颜色顺序 BGR
	for(int i =0; i<Height; i++)
	{
		for(int j = 0; j<Width; j++)
		{
			b = data[i*widthBytes + 4*j];
			g = data[i*widthBytes + 4*j + 2];
			r = data[i*widthBytes + 4*j + 3];		
			data[i*widthBytes + 4*j] = ((r*299 + g*587 + b*114)<<3);
			data[i*widthBytes + 4*j + 1] = ((r*299 + g*587 + b*114)<<3);
			data[i*widthBytes + 4*j + 2] = ((r*299 + g*587 + b*114)<<3);
		}	
	}
}

//找到手势在 图像中的位置    图像分割     基于HSV空间
/*
	去掉脸部方法：    头发是黑的，头发以下的全部去掉？？？
*/
void Hand::findHand_1ToBinaryimage(BYTE * data,int Height,int Width,int widthBytes)    //不稳定
{
	BYTE r,g,b;//       BYTE 实际上是 unsigned char
	//颜色顺序 BGR
//	int v,s;
	double d,h;
	for(int i =0; i<Height; i++)
	{
		for(int j = 0; j<Width; j++)
		{
			b = data[i*widthBytes + 4*j];
			g = data[i*widthBytes + 4*j + 1];
			r = data[i*widthBytes + 4*j + 2];	
			d = acos((double)(2*r-g-b)/(2*sqrt((r-g)*(r-g)+(r-b)*(g-b))));
			d = d/3.1416*180;//弧度转成度
			if (g>=b)
				h = d;
			else h= 2*180 - d;
			if( h>1.9 && h<20.5 && r>g && g>b)  //聚类  得到 肤色 范围
			{
				data[i*widthBytes + 4*j]=0xFF;
				data[i*widthBytes + 4*j +1 ]=0xFF;
				data[i*widthBytes + 4*j + 2 ]=0xFF;
			}
			else
			{
				data[i*widthBytes + 4*j]=0x00;
				data[i*widthBytes + 4*j +1 ]=0x00;
				data[i*widthBytes + 4*j + 2 ]=0x00;
			}
		}	
	}
}

//统计发现： R:69-100  G:60-80  B:54-73    r-g>10      基于统计方法
void Hand::findHand_2ToBinaryimage(BYTE * data,int Height,int Width,int widthBytes)
{
	BYTE r,g,b;
	for(int i =0; i<Height; i++)
	{
		for(int j = 0; j<Width; j++) 
		{
			b = data[i*widthBytes + 4*j];
			g = data[i*widthBytes + 4*j + 1];
			r = data[i*widthBytes + 4*j + 2];	
			if( r>(g+10) && r>60 && r<=110 && g>=50 && g<=88 && b>=50 && b<=79)  
			{
				data[i*widthBytes + 4*j]=0xFF;//白
				data[i*widthBytes + 4*j +1 ]=0xFF;
				data[i*widthBytes + 4*j + 2 ]=0xFF;
			}
			else
			{
				data[i*widthBytes + 4*j]=0x00;//黑
				data[i*widthBytes + 4*j +1 ]=0x00;
				data[i*widthBytes + 4*j + 2 ]=0x00;
			}
		}
	}
}

//基于YUV空间
void Hand::findHand_3ToBinaryimage(BYTE * data,int Height,int Width,int widthBytes)
{
	double u,v;
	BYTE r,g,b;
	for(int i =0; i<Height; i++)
	{
		for(int j = 0; j<Width; j++)
		{
			b = data[i*widthBytes + 4*j];
			g = data[i*widthBytes + 4*j + 2];
			r = data[i*widthBytes + 4*j + 3];
			u = -((1687*r +3313*g - 5000*b)<<4) + 128;
			v = (5000*g - 4184*g - 816*b)<<4 + 128;
			//计算概率信息 贝叶斯定理
			double yu = 0.65;
			if ((u > 163*yu || u < 163* (1+yu)) && (v >218*yu || v<218*(1+yu)))
			{
				data[j*widthBytes + 4*i] = 0x00;
				data[j*widthBytes + 4*i+1] = 0x00;
				data[j*widthBytes + 4*i+2] = 0x00;
			} 
			else
			{
				data[j*widthBytes + 4*i] = 0xFF;
				data[j*widthBytes + 4*i+1] = 0xFF;
				data[j*widthBytes + 4*i+2] = 0xFF;
			}
		}	
	}
}

void Hand::findHand_4ToBinaryimage(BYTE * data,int Height,int Width,int widthBytes)//简单背景下
{
	BYTE r,g,b;
	for(int i =0; i<Height; i++)
	{
		for(int j = 0; j<Width; j++) 
		{
			b = data[i*widthBytes + 4*j];
			g = data[i*widthBytes + 4*j + 1];
			r = data[i*widthBytes + 4*j + 2];	
			if(r>100 || g>100 || b>100)  
			{
				data[i*widthBytes + 4*j]=0xFF;//白
				data[i*widthBytes + 4*j +1 ]=0xFF;
				data[i*widthBytes + 4*j + 2 ]=0xFF;
			}
			else
			{
				data[i*widthBytes + 4*j]=0x00;//黑
				data[i*widthBytes + 4*j +1 ]=0x00;
				data[i*widthBytes + 4*j + 2 ]=0x00;
			}
		}
	}
}







void Hand::middleSmooth(BYTE *data,int Height,int Width,int widthBytes)//中值滤波
{
	int j,i,y,x;
	//BYTE p[9];//3 * 3 滤波窗口
	for (j=1; j<Height-1; j++)
	{
		for (i=1; i<Width-1; i++)
		{
			int num_0 = 0;
			for (y=j-1; y<=j+1; y++)
			{
				for (x=i-1; x<=i+1; x++)
				{
					if(data[y*widthBytes+ x*4] == 0x00)
						num_0++;
				}
			}
			//比较 0x00 0xFF 数目		
			if (num_0 > 4)
			{
				data[j*widthBytes + 4*i] = 0x00;
				data[j*widthBytes + 4*i+1] = 0x00;
				data[j*widthBytes + 4*i+2] = 0x00;
			} 
			else
			{
				data[j*widthBytes + 4*i] = 0xFF;
				data[j*widthBytes + 4*i+1] = 0xFF;
				data[j*widthBytes + 4*i+2] = 0xFF;
			}
		}
	}
}


void Hand::Picture_FuShi(BYTE *data,int Height,int Width,int widthBytes)//图像腐蚀   去噪
{
	int i, j, flag;
	//申请临时空间
	int sum = Height * Width * 4;
	BYTE *tmpdata = (BYTE*)malloc(sum);
	if (tmpdata == NULL)
	{
		::MessageBox(NULL,"内存申请失败!重试",NULL,MB_OK);
		return;
	}
	memcpy((BYTE*)tmpdata, (BYTE*)data, sum);

	for(i = 1;i < Height - 1;i++)
	{	for(j = 1;j < Width - 1;j++)
			{		
						flag = 1;
						for(int m = i - 1;m < i + 2;m++)
						{
							for(int n = j - 1; n < j + 2;n++)
							{
								if(tmpdata[i * widthBytes + j*4] == 0x00 || tmpdata[m * widthBytes + n*4] == 0x00)
								{	flag = 0;
									break;
								}
							}
							if(flag == 0)//只要有一个就退出
								break;
						}
						if(flag == 0)
						{
							data[i * widthBytes + j*4] = 0x00;
							data[i * widthBytes + j*4+1] = 0x00;
							data[i * widthBytes + j*4+2] = 0x00;
						}	
						else
						{
							data[i * widthBytes + j*4] = 0xFF;
							data[i * widthBytes + j*4+1] = 0xFF;
							data[i * widthBytes + j*4+2] = 0xFF;
						}
					}
			
		}
	free(tmpdata);
}

void Hand::Picture_PengZhang(BYTE *data,int Height,int Width,int widthBytes)//图像膨胀 填孔   
{
	int i, j, flag;
	//申请临时空间
	int sum = Height * Width * 4;
	BYTE *tmpdata = (BYTE*)malloc(sum);
	if (tmpdata == NULL)
	{
		::MessageBox(NULL,"内存申请失败!重试！",NULL,MB_OK);
		return;
	}
	memcpy((BYTE*)tmpdata, (BYTE*)data, sum);
	for(i = 1;i < Height - 1;i++)
	{
	    	for(j = 1;j < Width - 1;j++)
			{
				flag = 1;
				for(int m = i - 1;m < i + 2;m++)
				{
						for(int n = j - 1; n < j + 2;n++)
						{
						//自身及领域中若有一个为255
						//则将该点设为255
							if ( tmpdata[i * widthBytes + j*4] == 0xFF || tmpdata[m * widthBytes + n*4] == 0xFF)
							{
								flag = 0;
								break;
							}
						}
						if(flag == 0)//只要有一个就退出
						{
							break;
						}
				}	
				if( flag == 0)
				{
					data[i * widthBytes + j*4] = 0xFF;
					data[i * widthBytes + j*4+1] = 0xFF;
					data[i * widthBytes + j*4+2] = 0xFF;
				}
				else
				{
					data[i * widthBytes + j*4] = 0x00;
					data[i * widthBytes + j*4+1] = 0x00;
					data[i * widthBytes + j*4+2] = 0x00;
				}
			}	
	}
	free(tmpdata);
}

/*
因为就一个连通域，所以假如一个黑点上下左右都有白点，则可以判断该点是手势点  ？？？？？？？？
*/
void Hand::Picture_Clearhole(BYTE *data,int Height,int Width,int widthBytes)
{
	for(int i = 1; i<Height-1;i++)
	{
		for (int j = 1;j<Width-1;j++)
		{
			if (data[i*widthBytes+j*4] == 0x00)
			{
				
			}
		}
	}

}

void Hand::First_border2B(BYTE *data,int Height,int Width,int widthBytes,int num)
{
	for(int i = 0; i<num ;i++)
	{
		for (int j = 0;j<Width;j++)
		{
			if (data[i*widthBytes+j*4] == 0xFF)//下边界
			{
				data[i*widthBytes+j*4] =0x00;
				data[i*widthBytes+j*4+1] =0x00;
				data[i*widthBytes+j*4+2] =0x00;
			}
			if (data[(Height - i-1)*widthBytes + j*4] == 0xFF)//上边界
			{
				data[(Height - i-1)*widthBytes + j*4] = 0x00;
				data[(Height - i-1)*widthBytes + j*4+1] = 0x00;
				data[(Height - i-1)*widthBytes + j*4+2] = 0x00;
			}
		}
	}
	for (i =0; i<Height;i++)
	{
		for (int j = 0;j<num;j++)
		{		
			if (data[i*widthBytes + j*4] == 0xFF)//右边界
			{
				data[i*widthBytes + j*4] = 0x00;
				data[i*widthBytes + j*4+1] = 0x00;
				data[i*widthBytes + j*4+2] = 0x00;
			}
			if (data[i*widthBytes+(Width - 1 - j)*4] == 0xFF)//左边界
			{
				data[i*widthBytes+(Width - 1 - j)*4] = 0x00;
				data[i*widthBytes+(Width - 1 - j)*4+1] = 0x00;
				data[i*widthBytes+(Width - 1 - j)*4+2] = 0x00;
			}
		}
	}
}


void Hand::get_handku(BYTE *data,int Height,int Width,int widthBytes)
{
	for (int i =0;i<Height;i++)
	{
		for (int j=0;j<Width;j++)
		{
			if (data[i*widthBytes + j*4] != 0x00)
			{
				data[i*widthBytes + j*4] = 0xFF;
				data[i*widthBytes + j*4+1] = 0xFF;
				data[i*widthBytes + j*4+2] = 0xFF;
			}
		}
	}
}

int Hand::get_broad_line(BYTE *data,int Height,int Width,int widthBytes)//
{
	A = (spoint*)malloc((g+k)*4*sizeof(spoint));//申请空间 存轮廓点
	memset(A,-1,(g+k)*3*sizeof(spoint));
	if (A == NULL)
		return -1;
	int t=0;
	for (int i =1;i<Height-1;i++)
	{
		for (int j=1;j<Width-1;j++)
		{
			if (data[i*widthBytes + (j-1)*4] == 0x00 && data[i*widthBytes + j*4] == 0xFF
				|| data[i*widthBytes + j*4] == 0xFF && data[i*widthBytes + (j+1)*4] == 0x00 
				|| data[(i-1)*widthBytes + j*4] == 0x00 && data[i*widthBytes + j*4] == 0xFF
				|| data[(i+1)*widthBytes + j*4] == 0x00 && data[i*widthBytes + j*4] == 0xFF)
			{
				data[i*widthBytes + j*4] = 0x01;//判定条件
				data[i*widthBytes + j*4+1] = 0xFF;//绿色------------------------------------------------------------------------------------
				data[i*widthBytes + j*4+2] = 0x00;
				//存储轮廓点
				A[t].x = j;//列   x
				A[t].y = i;//行   y
				t++;
			}
		}
	}
	num = t;//边界点个数
	if (t==0)
	{
		return 0;
	}
	return 1;
}

void Hand::get_OutLine(BYTE *data,int Height,int Width,int widthBytes)//最大轮廓
{
	int start_i=A[0].x;
	int start_j=A[0].y;//起始点
	//坐标变换   以起始点为原点
	for (int i=1;i<=num;i++)
	{
		A[i-1].x = A[i].x-start_i;
		A[i-1].y = A[i].y-start_j;
 		A[i-1].ang = atan2(A[i-1].y,A[i-1].x);//俯角
	}
	num--;
	sort(A,num);//把边界点 按照夹角排序
	B = new spoint[g];// 凸包点数  估计不会超过高度----------------------------------------------------------------------------------------
	B[0].x = A[num].x; B[0].y = A[num].y;//最后一个点放在 头上
	B[1].x = 0; B[1].y = 0;//栈顶
	num_r = 1;//已有起点
	i = 1;
	double D=0;
	while (i<num)
	{
		D = B[num_r-1].x * B[num_r].y + B[num_r-1].y * A[i].x + B[num_r].x * A[i].y
				- B[num_r].y * A[i].x - B[num_r-1].y * B[num_r].x - B[num_r-1].x*A[i].y;
		if (D>0)
		{
			B[++num_r] = A[i++];
		}
		else // 包括 三点共线  只留一个边界
			num_r--;		
	}
	//num_r  多边形顶点数
	//坐标还原
	for (i=1;i<=num_r;i++)
	{
		B[i-1].x = B[i-1].x + start_i;
		B[i-1].y = B[i-1].y + start_j;
		data[B[i-1].y*widthBytes + (B[i-1].x-2)*4] = 0x00; //标识出 多边形顶点
		data[B[i-1].y*widthBytes + (B[i-1].x-2)*4+1] = 0xFF;
		data[B[i-1].y*widthBytes + (B[i-1].x-2)*4+2] = 0xFF;
		
		data[B[i-1].y*widthBytes + (B[i-1].x+2)*4] = 0x00;
		data[B[i-1].y*widthBytes + (B[i-1].x+2)*4+1] = 0xFF;
		data[B[i-1].y*widthBytes + (B[i-1].x+2)*4+2] = 0xFF;
		
	}
	//画出轮廓线  	
	double x,y;
	float dx,dy,k;
	for (i=0;i<num_r;i++) 
	{	
		dx=(float)(B[i+1].x-B[i].x);
		dy=(float)(B[i+1].y-B[i].y);
		x=B[i].x; y=B[i].y;
		if (i == num_r-1)//首尾              也方便了后面求 缺陷大小
		{
			dx=(float)(B[0].x-B[i].x);
			dy=(float)(B[0].y-B[i].y);
			B[i+1].x = B[0].x;
			B[i+1].y = B[0].y;
		}
		if (dx == 0)//竖线  斜率不存在时
		{
			for(;dy>0 && y<=B[i+1].y;y++)
			{
				data[(int)y*widthBytes + (int)x*4] = 0xFF;
				data[(int)y*widthBytes + (int)x*4+1] = 0x00;
				data[(int)y*widthBytes + (int)x*4+2] = 0x00;
			}
			for(;dy<0 && y>=B[i+1].y;y--)
			{
				data[(int)y*widthBytes + (int)x*4] = 0xFF;
				data[(int)y*widthBytes + (int)x*4+1] = 0x00;
				data[(int)y*widthBytes + (int)x*4+2] = 0x00;
			}
			continue;
		}
		k=dy / dx;//计算斜率
		//按斜率讨论
		if (k==0)// 水平线
		{
			if (dx > 0)
			{
				for (;x<=B[i+1].x;x++)
				{
					data[(int)(y)*widthBytes + (int)x*4] = 0xFF;
					data[(int)(y)*widthBytes + (int)x*4+1] = 0x00;
					data[(int)(y)*widthBytes + (int)x*4+2] = 0x00;
				}
			}else
			{
				for (;x>=B[i+1].x;x--)
				{
					data[(int)(y)*widthBytes + (int)x*4] = 0xFF;
					data[(int)(y)*widthBytes + (int)x*4+1] = 0x00;
					data[(int)(y)*widthBytes + (int)x*4+2] = 0x00;
				}
			}
			continue;
		}
		if (k<0 && k>-1)
		{	
			if (dx > 0)
			{
				for (;x<=B[i+1].x || y>=B[i+1].y;x++)
				{
					data[(int)(y-0.5)*widthBytes + (int)x*4] = 0xFF;
					data[(int)(y-0.5)*widthBytes + (int)x*4+1] = 0x00;
					data[(int)(y-0.5)*widthBytes + (int)x*4+2] = 0x00;
					y = y - fabs(k);
				}
			}
			else
			{
				for (;x>=B[i+1].x || y<=B[i+1].y;x--)
				{
					data[(int)(y+0.5)*widthBytes + (int)x*4] = 0xFF;
					data[(int)(y+0.5)*widthBytes + (int)x*4+1] = 0x00;
					data[(int)(y+0.5)*widthBytes + (int)x*4+2] = 0x00;
					y = y + fabs(k);
				}
			}
			continue;
		}
		if (k<=-1)
		{
			if (dy < 0)
			{
				for (;y>=B[i+1].y || x <= B[i+1].x;y--)
				{
					data[(int)y*widthBytes + (int)(x+0.5)*4] = 0xFF;
					data[(int)y*widthBytes + (int)(x+0.5)*4+1] = 0x00;
					data[(int)y*widthBytes + (int)(x+0.5)*4+2] = 0x00;
					x = x + fabs(1/k);
				}
			}
			else
			{
				for (;y<=B[i+1].y || x >= B[i+1].x;y++)
				{
					data[(int)y*widthBytes + (int)(x-0.5)*4] = 0xFF;
					data[(int)y*widthBytes + (int)(x-0.5)*4+1] = 0x00;
					data[(int)y*widthBytes + (int)(x-0.5)*4+2] = 0x00;
					x = x - fabs(1/k);
				}
			}
			continue;
		}
		if (k>0 && k<1)
		{
			if (dx > 0)
			{
				for (;y<=B[i+1].y || x<=B[i+1].x;x++)
				{
					data[(int)(y+0.5)*widthBytes + (int)x*4] = 0xFF;
					data[(int)(y+0.5)*widthBytes + (int)x*4+1] = 0x00;
					data[(int)(y+0.5)*widthBytes + (int)x*4+2] = 0x00;
					y = y + fabs(k);
				}
			} 
			else
			{
				for (;y>=B[i+1].y || x>=B[i+1].x;x--)
				{
					data[(int)(y-0.5)*widthBytes + (int)x*4] = 0xFF;
					data[(int)(y-0.5)*widthBytes + (int)x*4+1] = 0x00;
					data[(int)(y-0.5)*widthBytes + (int)x*4+2] = 0x00;
					y = y - fabs(k);
				}
			}
			continue;
		}
		if (k>=1)
		{
			if (dy > 0)
			{
				for (;y<=B[i+1].y || x <= B[i+1].x;y++)
				{
					data[(int)y*widthBytes + (int)(x+0.5)*4] = 0xFF;
					data[(int)y*widthBytes + (int)(x+0.5)*4+1] = 0x00;
					data[(int)y*widthBytes + (int)(x+0.5)*4+2] = 0x00;
					x = x + fabs(1/k);
				}
			} 
			else
			{
				for (;y>=B[i+1].y || x >= B[i+1].x;y--)
				{
					data[(int)y*widthBytes + (int)(x+0.5)*4] = 0xFF;
					data[(int)y*widthBytes + (int)(x+0.5)*4+1] = 0x00;
					data[(int)y*widthBytes + (int)(x+0.5)*4+2] = 0x00;
					x = x - fabs(1/k);
				}
			}	
			continue;
		} 
	}
	
}

void Hand::get_QueXian_vector(BYTE *data,int Height,int Width,int widthBytes)//计算缺陷大小
{
	int flag;
	area = 0;
	for (int i =0;i<Height;i++)//预处理
	{
		flag = 0;
		for (int j=0;j<Width;j++)
		{	
			if (data[i*widthBytes + 4*j]==0xFF && data[i*widthBytes + 4*j +1]==0x00 && data[i*widthBytes + 4*j +2]==0x00
				&& !(data[i*widthBytes + 4*j+4]==0xFF && data[i*widthBytes + 4*j +1+4]==0x00 && data[i*widthBytes + 4*j +2+4]==0x00)
				)
			{
				flag++;
			}
			if (data[i*widthBytes + 4*j]==0xFF && data[i*widthBytes + 4*j +1]==0xFF && data[i*widthBytes + 4*j +2]==0xFF)
			{
				area++;
			}
			if(flag == 2)
				break;
			if (flag == 1 && data[i*widthBytes + 4*j]==0x00 && data[i*widthBytes + 4*j +1]==0x00 && data[i*widthBytes + 4*j+2]==0x00)
			{
				for (int r=j;r<Width;r++)//必须判断后面还有蓝点   防止出现 只有进而没有出的情况
				{
					if (data[i*widthBytes + 4*r]==0xFF && data[i*widthBytes + 4*r +1]==0x00 && data[i*widthBytes + 4*r +2]==0x00)
					{
						break;
					}
				}
				if (r<Width)
				{
					data[i*widthBytes + 4*j]=0x00;
					data[i*widthBytes + 4*j +1]=0x00;
					data[i*widthBytes + 4*j +2]=0x40;//缺陷 涂成 暗红色
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------
	int number;//当前缺陷大小
	num_q = (int*)malloc(num_r*sizeof(int));//每两个相邻顶点对应一个缺陷     顶点数=缺陷数
	int q=0;
	//把蓝色边界和暗红色区域  当做连通域     统计缺陷大小
	float dx,dy;
	int s_x,s_y;
	
	point* stack = (point*)malloc(k*g/4*sizeof(point));//计算连通域的栈    估计不会超过红框的1/4------------------------------------
	int s;//栈顶
	
	point* view_flag = (point*)malloc(k*g/4*sizeof(point));//用于判断 当前点是否已访问
	int v;//数组长度
	for (i=0;i<num_r;i++)//B[num_r]中放的是B[0]
	{
		s=0;v=0;
		number = 0;
		memset(stack,0,k*g/4*sizeof(point));
		memset(view_flag,0,k*g/4*sizeof(point));//初始化
		s_x = B[i].x;
		s_y = B[i].y;
		//stack_push(stack,s,s_x,s_y);	s++;//起点不入栈
		//已访问数组    
		view_flag[v].x =s_x; view_flag[v].y=s_y; v++; 

		//决定 第一个走向    分情况讨论
		dx=(float)B[i+1].x-B[i].x;
		dy=(float)B[i+1].y-B[i].y;
		if (dy<0 && dx>0)
		{
			if (!had_view(view_flag,v,B[i].x,B[i].y-1) && 
				(data[(B[i].y-1)*widthBytes + 4*B[i].x + 2] == 0x40 || data[(B[i].y-1)*widthBytes + 4*B[i].x] == 0xFF && data[(B[i].y-1)*widthBytes + 4*B[i].x+1] == 0x00))
			{
				stack_push(stack,s,B[i].x,B[i].y-1);
				s++;
				view_flag[v].x = B[i].x;
				view_flag[v].y = B[i].y-1;
				v++;
			}
			if (!had_view(view_flag,v,B[i].x+1,B[i].y-1) && 
				(data[(B[i].y-1)*widthBytes + 4*(B[i].x+1) + 2] == 0x40 || data[(B[i].y-1)*widthBytes + 4*(B[i].x+1)] == 0xFF && data[(B[i].y-1)*widthBytes + 4*(B[i].x+1)+1] == 0x00))
			{
				stack_push(stack,s,B[i].x+1,B[i].y-1);
				s++;
				view_flag[v].x = B[i].x+1;
				view_flag[v].y = B[i].y-1;
				v++;
			}
			if (!had_view(view_flag,v,B[i].x+1,B[i].y) && 
				(data[(B[i].y)*widthBytes + 4*(B[i].x+1) + 2] == 0x40 || data[(B[i].y)*widthBytes + 4*(B[i].x+1)] == 0xFF && data[(B[i].y)*widthBytes + 4*(B[i].x+1)+1] == 0x00))
			{
				stack_push(stack,s,B[i].x+1,B[i].y);
				s++;
				view_flag[v].x = B[i].x+1;
				view_flag[v].y = B[i].y;
				v++;
			}
			//置已访问  防止跨区域
			view_flag[v].x = B[i].x+1;
			view_flag[v].y = B[i].y+1;
			v++;
			view_flag[v].x = B[i].x;
			view_flag[v].y = B[i].y+1;
			v++;
		}
		if (dx>0 && dy>0)
		{
			if (!had_view(view_flag,v,B[i].x,B[i].y+1) && 
				(data[(B[i].y+1)*widthBytes + 4*B[i].x + 2] == 0x40 || data[(B[i].y+1)*widthBytes + 4*B[i].x] == 0xFF && data[(B[i].y+1)*widthBytes + 4*B[i].x+1] == 0x00))
			{
				stack_push(stack,s,B[i].x,B[i].y+1);
				s++;
				view_flag[v].x = B[i].x;
				view_flag[v].y = B[i].y+1;
				v++;
			}
			if (!had_view(view_flag,v,B[i].x+1,B[i].y+1) && 
				(data[(B[i].y+1)*widthBytes + 4*(B[i].x+1) + 2] == 0x40 || data[(B[i].y+1)*widthBytes + 4*(B[i].x+1)] == 0xFF && data[(B[i].y+1)*widthBytes + 4*(B[i].x+1)+1] == 0x00))
			{
				stack_push(stack,s,B[i].x+1,B[i].y+1);
				s++;
				view_flag[v].x = B[i].x+1;
				view_flag[v].y = B[i].y+1;
				v++;
			}
			view_flag[v].x = B[i].x-1;
			view_flag[v].y = B[i].y;
			v++;
			view_flag[v].x = B[i].x-1;
			view_flag[v].y = B[i].y+1;
			v++;
		}
		if (dx<0 && dy>0)
		{
			if (!had_view(view_flag,v,B[i].x,B[i].y+1) && 
				(data[(B[i].y+1)*widthBytes + 4*B[i].x + 2] == 0x40 || data[(B[i].y+1)*widthBytes + 4*B[i].x] == 0xFF && data[(B[i].y+1)*widthBytes + 4*B[i].x+1] == 0x00))
			{
				stack_push(stack,s,B[i].x,B[i].y+1);
				s++;
				view_flag[v].x = B[i].x;
				view_flag[v].y = B[i].y+1;
				v++;
			}
			if (!had_view(view_flag,v,B[i].x-1,B[i].y+1) && 
				(data[(B[i].y+1)*widthBytes + 4*(B[i].x-1) + 2] == 0x40 || data[(B[i].y+1)*widthBytes + 4*(B[i].x-1)] == 0xFF && data[(B[i].y+1)*widthBytes + 4*(B[i].x-1)+1] == 0x00))
			{
				stack_push(stack,s,B[i].x-1,B[i].y+1);
				s++;
				view_flag[v].x = B[i].x-1;
				view_flag[v].y = B[i].y+1;
				v++;
			}
			if (!had_view(view_flag,v,B[i].x-1,B[i].y) && 
				(data[(B[i].y)*widthBytes + 4*(B[i].x-1) + 2] == 0x40 || data[(B[i].y)*widthBytes + 4*(B[i].x-1)] == 0xFF && data[(B[i].y)*widthBytes + 4*(B[i].x-1)+1] == 0x00))
			{
				stack_push(stack,s,B[i].x-1,B[i].y);
				s++;
				view_flag[v].x = B[i].x-1;
				view_flag[v].y = B[i].y;
				v++;
			}
			view_flag[v].x = B[i].x-1;
			view_flag[v].y = B[i].y-1;
			v++;
			view_flag[v].x = B[i].x;
			view_flag[v].y = B[i].y-1;
			v++;
		}
		if (dy<0 && dx<0)
		{
			if (!had_view(view_flag,v,B[i].x,B[i].y-1) && 
				(data[(B[i].y-1)*widthBytes + 4*B[i].x + 2] == 0x40 || data[(B[i].y-1)*widthBytes + 4*B[i].x] == 0xFF && data[(B[i].y-1)*widthBytes + 4*B[i].x+1] == 0x00))
			{
				stack_push(stack,s,B[i].x,B[i].y-1);
				s++;
				view_flag[v].x = B[i].x;
				view_flag[v].y = B[i].y-1;
				v++;
			}
			if (!had_view(view_flag,v,B[i].x-1,B[i].y-1) && 
				(data[(B[i].y-1)*widthBytes + 4*(B[i].x-1) + 2] == 0x40 || data[(B[i].y-1)*widthBytes + 4*(B[i].x-1)] == 0xFF && data[(B[i].y-1)*widthBytes + 4*(B[i].x-1)+1] == 0x00))
			{
				stack_push(stack,s,B[i].x-1,B[i].y-1);
				s++;
				view_flag[v].x = B[i].x-1;
				view_flag[v].y = B[i].y-1;
				v++;
			}
			view_flag[v].x = B[i].x+1;
			view_flag[v].y = B[i].y-1;
			v++;
			view_flag[v].x = B[i].x+1;
			view_flag[v].y = B[i].y;
			v++;
		}
		while (s>0) //栈不空 
		{
			if (dy==0 || dx == 0)//竖线或者横线 直接退出
			{
				break;
			}
			point p = stack[--s];//栈顶 退出
			if (data[p.y*widthBytes + 4*p.x +2] == 0x40)//如果弹出 一个 暗红点 则number++
			{
				number++;
			}
			next_push(stack,&s,p,B[i+1],view_flag,&v,data,widthBytes);// 寻找下一个合适点
		}
		num_q[q] = number;//存储缺陷大小
		q++;
	}
	free(stack);
	free(view_flag);
	delete[] B;//释放 轮廓顶点
}

void Hand::Save_qx(CString n)//训练手势后  存入数据文件
{
	FILE* fp = fopen("data.txt","ab+");
	//先写入 当前手势
    int strLength1 = n.GetLength() + 1;
    char chArray[30];
    memset(chArray,0, sizeof(char) * 30); 
    strncpy(chArray, n, strLength1);
	fputc(chArray[0],fp);//
	fputc('\t',fp);
	//按大小排序
	for (int i =0; i<num_r;i++)
	{
		for (int j=num_r;j>i;j--)
		{
			if (num_q[j-1] < num_q[j])
			{
				int d = num_q[j-1];
				num_q[j-1] = num_q[j];
				num_q[j] = d;		
			}
		}
	}
	for (i =0;i<num_r;i++)
	{
		fprintf(fp,"%f\t",num_q[i]/area);
	}
	fprintf(fp,"%f",-1.0);//向量结束标志
	fputc('\r',fp);
	fputc('\n',fp);
	fclose(fp);
	free(num_q);
}

char Hand::check()//检查手势    从数据文件中读取数据  确定手势
{
	//给 当前手势的 特征向量排序
	for (int i =0; i<num_r;i++)
	{
		for (int j=num_r;j>i;j--)
		{
			if (num_q[j-1] < num_q[j])
			{
				int d = num_q[j-1];
				num_q[j-1] = num_q[j];
				num_q[j] = d;		
			}
		}
	}
	fstream f;
	f.open("data.txt",ios::in);
	vector* head = new vector;//存入链表   首节点空
	int length=0;//length-1 个节点
	vector* tail = head;
	while (!f.eof())
	{
		vector* p = new vector;
		memset(p,-1,sizeof(vector));
		f>>p->hand;
		double i;
		int j=0;
		f>>i;
		while (i != -1.0)//特征向量 结束标志   自己定义的格式
		{
			p->v[j] = i;
			j++;
			f>>i;
		}
		//把p链上
		tail->next = p;
		tail = p;
		length++;
	}
	// num_q[]  和head[]中的比较
	double* result=(double*)malloc((length-1)*sizeof(double));//result数组中存放当前向量与左右手势向量的距离
	memset(result,0,(length-1)*sizeof(double));
	char r; double min=100000;
	for (i =0;i<length-1;i++)
	{
		vector* l = head->next;
		int j =0;
		while (l->v[j]!=0 && j<num_r)//以短的为准  到0就不再比较
		{
			result[i]+= (num_q[j]/area-l->v[j])*(num_q[j]/area-l->v[j]);
			j++;
		}
		if(sqrt(result[i]) < min)
		{
			min = sqrt(result[i]);
			r = l->hand;
		}
		head = head->next;
	}
	//
	free(result);
	f.close();
	free(num_q);
	//	
	return r;
}



bool Hand::check_FG(BYTE *data,int Height,int Width,int widthBytes)//检查分割结果
{
	/*
	所有的边界点都存储在数组A中    
	从任一个点开始八连通计算与该点联通的的数目与A的数目比较 不想等的话说明有噪音
	*/
	bool result = true;
	int x = A[0].x;
	int y = A[0].y;
	double n = 0; //连通域大小
	point* view = (point*)malloc(Height*Width*sizeof(point));//记录已访问的点
	int vi=0;
	int px,py;
	if (data[y*widthBytes + (x+1)*4] == 0x01)
	{
		px = x+1; py=y;
	}else if(data[y*widthBytes + (x-1)*4] == 0x01)
	{
		px = x-1; py=y;
	}else if(data[(y-1)*widthBytes + x*4] == 0x01)
	{
		px = x; py=y-1;
	}else if(data[(y-1)*widthBytes + (x-1)*4] == 0x01)
	{
		px = x-1; py=y-1;
	}else if(data[(y-1)*widthBytes + (x+1)*4] == 0x01)
	{
		px = x+1; py=y-1;
	}else if(data[(y+1)*widthBytes + x*4] == 0x01)
	{
		px = x; py=y+1;
	}else if(data[(y+1)*widthBytes + (x-1)*4] == 0x01)
	{
		px = x-1; py=y+1;
	}else if(data[(y+1)*widthBytes + (x+1)*4] == 0x01)
	{
		px = x+1; py=y+1;
	}
	view[vi].x = px; view[vi].y = py;
	vi++;
	bool flag=false;
	while(!(px == x && py == y))//转一圈
	{
		if (data[py*widthBytes + (px+1)*4] == 0x01 && !had_view(view,vi,px+1,py))
		{
			px = px+1;
		}else if(data[py*widthBytes + (px-1)*4] == 0x01 && !had_view(view,vi,px-1,py))
		{
			px = px-1; 
		}else if(data[(py-1)*widthBytes + px*4] == 0x01 && !had_view(view,vi,px,py-1))
		{
			py=py-1;
		}else if(data[(py-1)*widthBytes + (px-1)*4] == 0x01 && !had_view(view,vi,px-1,py-1))
		{
			px = px-1; py=py-1;
		}else if(data[(py-1)*widthBytes + (px+1)*4] == 0x01 && !had_view(view,vi,px+1,py-1))
		{
			px = px+1; py=py-1;
		}else if(data[(py+1)*widthBytes + px*4] == 0x01 && !had_view(view,vi,px,py+1))
		{
			py=py+1;
		}else if(data[(py+1)*widthBytes + (px-1)*4] == 0x01 && !had_view(view,vi,px-1,py+1))
		{
			px = px-1; py=py+1;
		}else if(data[(py+1)*widthBytes + (px+1)*4] == 0x01 && !had_view(view,vi,px+1,py+1))
		{
			px = px+1; py=py+1;
		}else
			flag = true;
		if (flag == true)
			break;
		n++;
		view[vi].x = px; view[vi].y = py;
		vi++;
	}
	if (num != n+1)
	{
		result = false;
	}
	free(view);
	return result;
}

void Hand::free_A()
{
	free(A);//释放 手势边界点   如果check_FG()没有通过则 直接释放
}


