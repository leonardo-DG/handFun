
#include "vfw.h"

#ifndef _HAND_H_
#define _HAND_H_

class Hand
{
protected:
	char _hand;
	static HWND hWnd;//���ھ��
public:
	void findHand_4ToBinaryimage(BYTE * data,int Height,int Width,int widthBytes);
	void free_A();
	
	
	
	void setHWND(HWND hWndCap)
	{
		hWnd = hWndCap;
	}
	char check();//�����ʲô����

	void First_border2B(BYTE *data,int Height,int Width,int widthBytes,int num);//ͼ��߽�  ����    �ѱ߿�num�����ر�0x00
	 //����һ֡��Ļص�����         ���Ż�
	static BOOL callBack(HWND hWndCap,LPVIDEOHDR videohead);//���ھ��    �������Ƶ����Ϣ   ����ɾ�̬��   ����CCameraDlg���� 

	void RgbToGrade(BYTE *data,int Height,int Width,int widthBytes);//rgbת�Ҷ�

	double RGBToHSV(int r, int g, int b);//RGB�ռ� תΪ HSV�ռ�  ��Ҫ�ǵõ�H

	void findHand_1ToBinaryimage(BYTE *data,int Height,int Width,int widthBytes);//�ҵ����Ƶ�λ��    ���ڷ�ɫ HSV�ռ�

	void findHand_2ToBinaryimage(BYTE *data,int Height,int Width,int widthBytes); //ԭ���㷨  ����

	void findHand_3ToBinaryimage(BYTE *data,int Height,int Width,int widthBytes);//���ڷ�ɫ YUV�ռ�

	void boxOutHand(BYTE *data,int Height,int Width,int widthBytes);//����ֲ�

	void middleSmooth(BYTE *data,int Height,int Width,int widthBytes);//��ֵ�˲�

	void Picture_PengZhang(BYTE *data,int Height,int Width,int widthBytes);//����

	void Picture_FuShi(BYTE *data,int Height,int Width,int widthBytes);//��ʴ

	void Picture_Clearhole(BYTE *data,int Height,int Width,int widthBytes);//ȥ��

	void get_QueXian_vector(BYTE *data,int Height,int Width,int widthBytes);//���� ȱ�� ��С

	void get_OutLine(BYTE *data,int Height,int Width,int widthBytes);//�������

	int get_broad_line(BYTE *data,int Height,int Width,int widthBytes);//�����߽�

	void get_handku(BYTE *data,int Height,int Width,int widthBytes);//������   ȥë��

	void Save_qx(CString n);//����ѵ���� ���ƽ��

	bool check_FG(BYTE *data,int Height,int Width,int widthBytes);//���ָ���
	
};

#endif