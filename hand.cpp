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

int k;//�����
int g;//���߶�

spoint* A=NULL;//�߽��
int num;

spoint* B=NULL;//��������ϵĵ�
int num_r;

int* num_q = NULL;//ȱ������

double area;//�������

void Hand::boxOutHand(BYTE *data,int Height,int Width,int widthBytes)//����ֲ�
{
	bool flag_1=FALSE,flag_2=FALSE,flag_3=FALSE,flag_4=FALSE;
	int a[4][2];//�洢�߽��
	for (int i =0; i<Height;i++)
	{
		for (int j = 0; j< Width;j++)
		{
			if (data[i*widthBytes + j*4] == 0xFF)
			{
				if (flag_1 == FALSE)//��͵�
				{
					a[0][0] = i;
					a[0][1] = j;
					flag_1 = TRUE;
				}
			}
			if (data[(Height - i -1)*widthBytes + j*4] == 0xFF)
			{
				if (flag_2 == FALSE)//��ߵ�
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
				if (flag_3 == FALSE)//�����
				{
					a[2][0] = j;
					a[2][1] = i;
					flag_3 = TRUE;
				}
			}
			if (data[j*widthBytes + (Width -i - 1)*4] == 0xFF)
			{
				if (flag_4 == FALSE)//���ҵ�
				{
					a[3][0] = j;
					a[3][1] = Width - i -1;
					flag_4 = TRUE;
				}
			}
		}
	}
	g = a[3][1] - a[2][1];//���ο� �߶�
	for(i = a[2][1];i<a[3][1];i++)//����
	{
		data[a[1][0]*widthBytes + i*4] = 0x00;
		data[a[1][0]*widthBytes + i*4+1] = 0x00;
		data[a[1][0]*widthBytes + i*4+2] = 0xFF;

		data[a[0][0]*widthBytes + i*4] = 0x00;
		data[a[0][0]*widthBytes + i*4+1] = 0x00;
		data[a[0][0]*widthBytes + i*4+2] = 0xFF;
	}
	k = a[1][0] - a[0][0];//���ο� ���
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

//����һ֡��Ļص�����
BOOL  Hand::callBack(HWND hWndCap,LPVIDEOHDR videohead)//���ھ��    �������Ƶ����Ϣ��û��ֱ��ʹ�ã�
{
	Hand hand;//�ھ�̬�����з��ʷǾ�̬��Ա
	//����ʱ��  ÿ��5��ʶ��һ��
	SYSTEMTIME cti;
	GetSystemTime(&cti);
	if ((CCameraDlg::ti.wSecond - cti.wSecond)%5 != 0)
	{
		return false;
	}
	//hand.setHWND(hWndCap);
	//DWORD time = videohead->dwTimeCaptured;
	//DWORD size = videohead->dwBytesUsed;//���ṹ�� LPVIDEOHDR
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
	    char ch[9] = "";//�����ʼ��
	itoa(R,ch,16);  //ʮ����
*/
	CCameraDlg c = new CCameraDlg();
	CRect rect; 
	CWnd* pWnd = new CWnd(); 
	pWnd->Attach(hWndCap);
	pWnd->GetWindowRect(&rect);//�õ�  ����λ��
    HBITMAP hBitmap = c.CopyScreenToBitmap(rect); //�õ����

	BITMAP Bitmap;      
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);//�õ�bmpͼ��
	LONG  widthBytes =  Bitmap.bmWidthBytes;//ÿһ�� ��ռ���ֽ���
/*
	//LPBYTE data = (LPBYTE)Bitmap.bmBits;//������ָ��
	LONG lHeight = Bitmap.bmHeight;//�߶�
	LONG lWidth = Bitmap.bmWidth;//���
	LONG  widthBytes =  Bitmap.bmWidthBytes;//ÿһ�� ��ռ���ֽ���
    WORD  planes =  Bitmap.bmPlanes;//��ɫƽ����
    WORD  bitsPixel  =  Bitmap.bmBitsPixel;//���ص�λ��    4  X8 R8 G8 B8     X������ͨ��
*/
	//�õ� ���ؾ���
	//CBitmap bmp;
	//bmp.GetBitmap(&Bitmap);
/*
A:	BYTE* data = (BYTE*)GlobalAlloc(GPTR,widthBytes*lHeight);//��̬����
	if (data==0)
		goto A;
	bmp.GetBitmapBits(widthBytes*lHeight,data);
*/
	
	LPBITMAPINFOHEADER lpbi;   
	HANDLE       hDib, hPal;
	BITMAPINFOHEADER   bi;
	HDC     hDC = ::GetDC(hWndCap);   //����õ� DC
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
	// �����ɫ��   
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal=SelectPalette(hDC,(HPALETTE)hPal,FALSE);
		RealizePalette(hDC);
	}
	// ��ȡ�õ�ɫ�����µ�����ֵ
	int p = GetDIBits(hDC,hBitmap,0,(UINT)Bitmap.bmHeight,(LPSTR)lpbi+sizeof(BITMAPINFOHEADER), (BITMAPINFO *)lpbi,DIB_RGB_COLORS);
	
	//����ԭʼͼ��
	CTime tm;
	tm=CTime::GetCurrentTime();
	CString name;
	name=tm.Format(".\\pic\\%Y��%m��%d��%Hʱ%M��%S��.bmp");//ʱ���ʽ
	char * str;
	str=name.GetBuffer(name.GetLength()); //ת���ַ�������
	c.SaveBitmapToFile(hBitmap,str);//�浽��ǰĿ¼��
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	BYTE * data = (BYTE*)lpbi + sizeof(BITMAPINFOHEADER);//�õ�������ָ�� 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	//1. ���Ʒָ�
	//hand.RgbToGrade(data,Bitmap.bmHeight,Bitmap.bmWidth,widthBytes);//ת�ɻҶ�ͼ
	hand.findHand_1ToBinaryimage(data,Bitmap.bmHeight,Bitmap.bmWidth,widthBytes);
	//hand.findHand_2ToBinaryimage(data,Bitmap.bmHeight,Bitmap.bmWidth,widthBytes);
	//hand.findHand_3ToBinaryimage(data,Bitmap.bmHeight,Bitmap.bmWidth,widthBytes);
//---------------------------------------------------------------------------------------------------------------------------------------------	
	//2. �˲�����  ������ֵ�˲� 
	hand.middleSmooth(data,Bitmap.bmHeight,Bitmap.bmWidth,widthBytes);
//---------------------------------------------------------------------------------------------------------------------------------------------	
	//3.��̬ѧ�˲�   �ȸ�ʴ�����ͣ������㣩ȥ�������㣻   �������ٸ�ʴ�������㣩��׶�     	
	hand.First_border2B(data,Bitmap.bmHeight,Bitmap.bmWidth,widthBytes,2);//�ѱ߽����������ͼ��

	for (int i = 0; i<2;i++)//��ʴ����
	{
		hand.Picture_FuShi(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	}
	for (i = 0;i<3;i++)//���ʹ���
	{
		hand.Picture_PengZhang(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
	}
	//Ч����Ȼ���ã������Լ��ķ��� ��ȫ������׶�?
//---------------------------------------------------------------------------------------------------------------------------------------------
	//4.���������ȡ               
	hand.boxOutHand(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//����ֲ�
//----------------------------------------------------------------------------------------------------------------------------------------------
	//���ͼ����û������ ��get_broad_line��������������ȡ���κα�Ե�� ��������ҲҪ�ж�һ��
	int b_broad = hand.get_broad_line(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//�߽��
	if (b_broad == -1)
	{
		return false;
	}
//---------------------------------------------------------------------------------------------------------------------------------------------
	char re[2];
	memset(re,0,sizeof(char)*2);
	if (b_broad == 1)
	{	
		//Ҫ���к��漸��  ���뱣֤ͼ���Ѵ���ķǳ�����    �������Ƿ���ȫ�ָ�����ƣ�һ����ͨ��
		bool bl = hand.check_FG(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);
		if (bl == true)//��������
		{
			hand.get_OutLine(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//͹��
			//5.������ȡ 
			hand.get_QueXian_vector(data,Bitmap.bmHeight,Bitmap.bmWidth,Bitmap.bmWidthBytes);//����ȱ�ݴ�С
			//6.����ʶ��
			re[0] = hand.check();
		}else//�кܴ�ĸ���
		{
			re[0] = '!';
		}
	}else
	{
		re[0] = '#';
	}
	hand.free_A();
//---------------------------------------------------------------------------------------------------------------------------------------------
	//7. ��ʾ ʶ����                                                                                                                                                                                                                                                 
	c.m_listboxresult.InsertString(-1,re);
	c.showHand(re);
//---------------------------------------------------------------------------------------------------------------------------------------------
	//8.����
	c.readRESULT(re);
	delete c;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���� ��������
	int nam = SetDIBits(hDC,hBitmap,0,(UINT)Bitmap.bmHeight,data, (BITMAPINFO *)lpbi,DIB_RGB_COLORS);
	//bmp.SetBitmapBits(Bitmap.bmWidthBytes*Bitmap.bmHeight,data);//����bmp������
	//DWORD d = GetLastError();
	tm=CTime::GetCurrentTime();
	name=tm.Format(".\\pic\\%Y��%m��%d��%Hʱ%M��%S��.bmp");//ʱ���ʽ
	str=name.GetBuffer(name.GetLength()); //ת���ַ�������
	c.SaveBitmapToFile(hBitmap,str);//�浽��ǰĿ¼��
	//�ָ���ɫ��   
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}
	//���   
	GlobalUnlock(hDib);
	GlobalFree(hDib);		
	return true;
}

double Hand::RGBToHSV(int r, int g, int b)//�õ� ɫ����Ϣ
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

void Hand::RgbToGrade(BYTE *data,int Height,int Width,int widthBytes)//�Ҷ�ͼ
{
	BYTE r,g,b;//       BYTE ʵ������ unsigned char
	//��ɫ˳�� BGR
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

//�ҵ������� ͼ���е�λ��    ͼ��ָ�     ����HSV�ռ�
/*
	ȥ������������    ͷ���Ǻڵģ�ͷ�����µ�ȫ��ȥ��������
*/
void Hand::findHand_1ToBinaryimage(BYTE * data,int Height,int Width,int widthBytes)    //���ȶ�
{
	BYTE r,g,b;//       BYTE ʵ������ unsigned char
	//��ɫ˳�� BGR
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
			d = d/3.1416*180;//����ת�ɶ�
			if (g>=b)
				h = d;
			else h= 2*180 - d;
			if( h>1.9 && h<20.5 && r>g && g>b)  //����  �õ� ��ɫ ��Χ
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

//ͳ�Ʒ��֣� R:69-100  G:60-80  B:54-73    r-g>10      ����ͳ�Ʒ���
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
				data[i*widthBytes + 4*j]=0xFF;//��
				data[i*widthBytes + 4*j +1 ]=0xFF;
				data[i*widthBytes + 4*j + 2 ]=0xFF;
			}
			else
			{
				data[i*widthBytes + 4*j]=0x00;//��
				data[i*widthBytes + 4*j +1 ]=0x00;
				data[i*widthBytes + 4*j + 2 ]=0x00;
			}
		}
	}
}

//����YUV�ռ�
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
			//���������Ϣ ��Ҷ˹����
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

void Hand::findHand_4ToBinaryimage(BYTE * data,int Height,int Width,int widthBytes)//�򵥱�����
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
				data[i*widthBytes + 4*j]=0xFF;//��
				data[i*widthBytes + 4*j +1 ]=0xFF;
				data[i*widthBytes + 4*j + 2 ]=0xFF;
			}
			else
			{
				data[i*widthBytes + 4*j]=0x00;//��
				data[i*widthBytes + 4*j +1 ]=0x00;
				data[i*widthBytes + 4*j + 2 ]=0x00;
			}
		}
	}
}







void Hand::middleSmooth(BYTE *data,int Height,int Width,int widthBytes)//��ֵ�˲�
{
	int j,i,y,x;
	//BYTE p[9];//3 * 3 �˲�����
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
			//�Ƚ� 0x00 0xFF ��Ŀ		
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


void Hand::Picture_FuShi(BYTE *data,int Height,int Width,int widthBytes)//ͼ��ʴ   ȥ��
{
	int i, j, flag;
	//������ʱ�ռ�
	int sum = Height * Width * 4;
	BYTE *tmpdata = (BYTE*)malloc(sum);
	if (tmpdata == NULL)
	{
		::MessageBox(NULL,"�ڴ�����ʧ��!����",NULL,MB_OK);
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
							if(flag == 0)//ֻҪ��һ�����˳�
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

void Hand::Picture_PengZhang(BYTE *data,int Height,int Width,int widthBytes)//ͼ������ ���   
{
	int i, j, flag;
	//������ʱ�ռ�
	int sum = Height * Width * 4;
	BYTE *tmpdata = (BYTE*)malloc(sum);
	if (tmpdata == NULL)
	{
		::MessageBox(NULL,"�ڴ�����ʧ��!���ԣ�",NULL,MB_OK);
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
						//��������������һ��Ϊ255
						//�򽫸õ���Ϊ255
							if ( tmpdata[i * widthBytes + j*4] == 0xFF || tmpdata[m * widthBytes + n*4] == 0xFF)
							{
								flag = 0;
								break;
							}
						}
						if(flag == 0)//ֻҪ��һ�����˳�
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
��Ϊ��һ����ͨ�����Լ���һ���ڵ��������Ҷ��а׵㣬������жϸõ������Ƶ�  ����������������
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
			if (data[i*widthBytes+j*4] == 0xFF)//�±߽�
			{
				data[i*widthBytes+j*4] =0x00;
				data[i*widthBytes+j*4+1] =0x00;
				data[i*widthBytes+j*4+2] =0x00;
			}
			if (data[(Height - i-1)*widthBytes + j*4] == 0xFF)//�ϱ߽�
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
			if (data[i*widthBytes + j*4] == 0xFF)//�ұ߽�
			{
				data[i*widthBytes + j*4] = 0x00;
				data[i*widthBytes + j*4+1] = 0x00;
				data[i*widthBytes + j*4+2] = 0x00;
			}
			if (data[i*widthBytes+(Width - 1 - j)*4] == 0xFF)//��߽�
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
	A = (spoint*)malloc((g+k)*4*sizeof(spoint));//����ռ� ��������
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
				data[i*widthBytes + j*4] = 0x01;//�ж�����
				data[i*widthBytes + j*4+1] = 0xFF;//��ɫ------------------------------------------------------------------------------------
				data[i*widthBytes + j*4+2] = 0x00;
				//�洢������
				A[t].x = j;//��   x
				A[t].y = i;//��   y
				t++;
			}
		}
	}
	num = t;//�߽�����
	if (t==0)
	{
		return 0;
	}
	return 1;
}

void Hand::get_OutLine(BYTE *data,int Height,int Width,int widthBytes)//�������
{
	int start_i=A[0].x;
	int start_j=A[0].y;//��ʼ��
	//����任   ����ʼ��Ϊԭ��
	for (int i=1;i<=num;i++)
	{
		A[i-1].x = A[i].x-start_i;
		A[i-1].y = A[i].y-start_j;
 		A[i-1].ang = atan2(A[i-1].y,A[i-1].x);//����
	}
	num--;
	sort(A,num);//�ѱ߽�� ���ռн�����
	B = new spoint[g];// ͹������  ���Ʋ��ᳬ���߶�----------------------------------------------------------------------------------------
	B[0].x = A[num].x; B[0].y = A[num].y;//���һ������� ͷ��
	B[1].x = 0; B[1].y = 0;//ջ��
	num_r = 1;//�������
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
		else // ���� ���㹲��  ֻ��һ���߽�
			num_r--;		
	}
	//num_r  ����ζ�����
	//���껹ԭ
	for (i=1;i<=num_r;i++)
	{
		B[i-1].x = B[i-1].x + start_i;
		B[i-1].y = B[i-1].y + start_j;
		data[B[i-1].y*widthBytes + (B[i-1].x-2)*4] = 0x00; //��ʶ�� ����ζ���
		data[B[i-1].y*widthBytes + (B[i-1].x-2)*4+1] = 0xFF;
		data[B[i-1].y*widthBytes + (B[i-1].x-2)*4+2] = 0xFF;
		
		data[B[i-1].y*widthBytes + (B[i-1].x+2)*4] = 0x00;
		data[B[i-1].y*widthBytes + (B[i-1].x+2)*4+1] = 0xFF;
		data[B[i-1].y*widthBytes + (B[i-1].x+2)*4+2] = 0xFF;
		
	}
	//����������  	
	double x,y;
	float dx,dy,k;
	for (i=0;i<num_r;i++) 
	{	
		dx=(float)(B[i+1].x-B[i].x);
		dy=(float)(B[i+1].y-B[i].y);
		x=B[i].x; y=B[i].y;
		if (i == num_r-1)//��β              Ҳ�����˺����� ȱ�ݴ�С
		{
			dx=(float)(B[0].x-B[i].x);
			dy=(float)(B[0].y-B[i].y);
			B[i+1].x = B[0].x;
			B[i+1].y = B[0].y;
		}
		if (dx == 0)//����  б�ʲ�����ʱ
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
		k=dy / dx;//����б��
		//��б������
		if (k==0)// ˮƽ��
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

void Hand::get_QueXian_vector(BYTE *data,int Height,int Width,int widthBytes)//����ȱ�ݴ�С
{
	int flag;
	area = 0;
	for (int i =0;i<Height;i++)//Ԥ����
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
				for (int r=j;r<Width;r++)//�����жϺ��滹������   ��ֹ���� ֻ�н���û�г������
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
					data[i*widthBytes + 4*j +2]=0x40;//ȱ�� Ϳ�� ����ɫ
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------
	int number;//��ǰȱ�ݴ�С
	num_q = (int*)malloc(num_r*sizeof(int));//ÿ�������ڶ����Ӧһ��ȱ��     ������=ȱ����
	int q=0;
	//����ɫ�߽�Ͱ���ɫ����  ������ͨ��     ͳ��ȱ�ݴ�С
	float dx,dy;
	int s_x,s_y;
	
	point* stack = (point*)malloc(k*g/4*sizeof(point));//������ͨ���ջ    ���Ʋ��ᳬ������1/4------------------------------------
	int s;//ջ��
	
	point* view_flag = (point*)malloc(k*g/4*sizeof(point));//�����ж� ��ǰ���Ƿ��ѷ���
	int v;//���鳤��
	for (i=0;i<num_r;i++)//B[num_r]�зŵ���B[0]
	{
		s=0;v=0;
		number = 0;
		memset(stack,0,k*g/4*sizeof(point));
		memset(view_flag,0,k*g/4*sizeof(point));//��ʼ��
		s_x = B[i].x;
		s_y = B[i].y;
		//stack_push(stack,s,s_x,s_y);	s++;//��㲻��ջ
		//�ѷ�������    
		view_flag[v].x =s_x; view_flag[v].y=s_y; v++; 

		//���� ��һ������    ���������
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
			//���ѷ���  ��ֹ������
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
		while (s>0) //ջ���� 
		{
			if (dy==0 || dx == 0)//���߻��ߺ��� ֱ���˳�
			{
				break;
			}
			point p = stack[--s];//ջ�� �˳�
			if (data[p.y*widthBytes + 4*p.x +2] == 0x40)//������� һ�� ����� ��number++
			{
				number++;
			}
			next_push(stack,&s,p,B[i+1],view_flag,&v,data,widthBytes);// Ѱ����һ�����ʵ�
		}
		num_q[q] = number;//�洢ȱ�ݴ�С
		q++;
	}
	free(stack);
	free(view_flag);
	delete[] B;//�ͷ� ��������
}

void Hand::Save_qx(CString n)//ѵ�����ƺ�  ���������ļ�
{
	FILE* fp = fopen("data.txt","ab+");
	//��д�� ��ǰ����
    int strLength1 = n.GetLength() + 1;
    char chArray[30];
    memset(chArray,0, sizeof(char) * 30); 
    strncpy(chArray, n, strLength1);
	fputc(chArray[0],fp);//
	fputc('\t',fp);
	//����С����
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
	fprintf(fp,"%f",-1.0);//����������־
	fputc('\r',fp);
	fputc('\n',fp);
	fclose(fp);
	free(num_q);
}

char Hand::check()//�������    �������ļ��ж�ȡ����  ȷ������
{
	//�� ��ǰ���Ƶ� ������������
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
	vector* head = new vector;//��������   �׽ڵ��
	int length=0;//length-1 ���ڵ�
	vector* tail = head;
	while (!f.eof())
	{
		vector* p = new vector;
		memset(p,-1,sizeof(vector));
		f>>p->hand;
		double i;
		int j=0;
		f>>i;
		while (i != -1.0)//�������� ������־   �Լ�����ĸ�ʽ
		{
			p->v[j] = i;
			j++;
			f>>i;
		}
		//��p����
		tail->next = p;
		tail = p;
		length++;
	}
	// num_q[]  ��head[]�еıȽ�
	double* result=(double*)malloc((length-1)*sizeof(double));//result�����д�ŵ�ǰ�������������������ľ���
	memset(result,0,(length-1)*sizeof(double));
	char r; double min=100000;
	for (i =0;i<length-1;i++)
	{
		vector* l = head->next;
		int j =0;
		while (l->v[j]!=0 && j<num_r)//�Զ̵�Ϊ׼  ��0�Ͳ��ٱȽ�
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



bool Hand::check_FG(BYTE *data,int Height,int Width,int widthBytes)//���ָ���
{
	/*
	���еı߽�㶼�洢������A��    
	����һ���㿪ʼ����ͨ������õ���ͨ�ĵ���Ŀ��A����Ŀ�Ƚ� ����ȵĻ�˵��������
	*/
	bool result = true;
	int x = A[0].x;
	int y = A[0].y;
	double n = 0; //��ͨ���С
	point* view = (point*)malloc(Height*Width*sizeof(point));//��¼�ѷ��ʵĵ�
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
	while(!(px == x && py == y))//תһȦ
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
	free(A);//�ͷ� ���Ʊ߽��   ���check_FG()û��ͨ���� ֱ���ͷ�
}


