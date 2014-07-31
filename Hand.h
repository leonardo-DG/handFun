
#include "vfw.h"

#ifndef _HAND_H_
#define _HAND_H_

class Hand
{
protected:
	char _hand;
	static HWND hWnd;//窗口句柄
public:
	void findHand_4ToBinaryimage(BYTE * data,int Height,int Width,int widthBytes);
	void free_A();
	
	
	
	void setHWND(HWND hWndCap)
	{
		hWnd = hWndCap;
	}
	char check();//检查是什么手势

	void First_border2B(BYTE *data,int Height,int Width,int widthBytes,int num);//图像边界  干扰    把边框num个像素变0x00
	 //捕获一帧后的回调函数         待优化
	static BOOL callBack(HWND hWndCap,LPVIDEOHDR videohead);//窗口句柄    捕获的视频流信息   定义成静态的   便于CCameraDlg调用 

	void RgbToGrade(BYTE *data,int Height,int Width,int widthBytes);//rgb转灰度

	double RGBToHSV(int r, int g, int b);//RGB空间 转为 HSV空间  主要是得到H

	void findHand_1ToBinaryimage(BYTE *data,int Height,int Width,int widthBytes);//找到手势的位置    基于肤色 HSV空间

	void findHand_2ToBinaryimage(BYTE *data,int Height,int Width,int widthBytes); //原创算法  测试

	void findHand_3ToBinaryimage(BYTE *data,int Height,int Width,int widthBytes);//基于肤色 YUV空间

	void boxOutHand(BYTE *data,int Height,int Width,int widthBytes);//框出手部

	void middleSmooth(BYTE *data,int Height,int Width,int widthBytes);//中值滤波

	void Picture_PengZhang(BYTE *data,int Height,int Width,int widthBytes);//膨胀

	void Picture_FuShi(BYTE *data,int Height,int Width,int widthBytes);//腐蚀

	void Picture_Clearhole(BYTE *data,int Height,int Width,int widthBytes);//去孔

	void get_QueXian_vector(BYTE *data,int Height,int Width,int widthBytes);//计算 缺陷 大小

	void get_OutLine(BYTE *data,int Height,int Width,int widthBytes);//最大轮廓

	int get_broad_line(BYTE *data,int Height,int Width,int widthBytes);//画出边界

	void get_handku(BYTE *data,int Height,int Width,int widthBytes);//建库用   去毛边

	void Save_qx(CString n);//保存训练的 手势结果

	bool check_FG(BYTE *data,int Height,int Width,int widthBytes);//检查分割结果
	
};

#endif