





int maxRGB(int g, int b, int r)
{
	int max;
	if (g > b && g > r)
		max = g;
	else 
	{
		if( b > r)
			max = b;
		else max = r;
	}
	return max;
}

int minRGB(int g, int b, int r)
{
	int min;
	if ( g<b && g<r)
		min = g;
	else
	{
		if (b<r)
			min = b;
		else min = r;
	}
	return min;
}

struct spoint 
{
	int x;//坐标x   对应列
	int y;//坐标y   对应行
	double ang;//夹角
};

struct point//用于计算 缺陷大小
{
	int x;
	int y;
};

void stack_push(point* s,int pos,int x,int y)//入栈
{
	s[pos].x=x;
	s[pos].y=y;
}


bool had_view(point* view,int length,int x,int y)//查看是否已访问   计算缺陷向量时用
{
	point* head = view;
	for (int i =0;i<length;i++)
	{
		if (head[i].x==x && head[i].y==y)
		{
			return true;
		}
	}
	return false;
}

/*
参数：栈指针  栈顶位置 当前点 结束点 已访问数组指针 数组大小 图像指针 一行所占字节数
*/
void next_push(point* stack,int* s,point pos,spoint end,point* view_flag,int* v,BYTE* data,int widthBytes)//寻找下一个合适的入栈的 点
{
	if (pos.x == end.x && pos.y == end.y || pos.x-1 == end.x && pos.y == end.y || pos.x+1 == end.x && pos.y == end.y 
		|| pos.x == end.x && pos.y-1 == end.y || pos.x-1 == end.x && pos.y-1 == end.y || pos.x+1 == end.x && pos.y-1 == end.y 
		|| pos.x == end.x && pos.y+1 == end.y || pos.x-1 == end.x && pos.y+1 == end.y || pos.x+1 == end.x && pos.y+1 == end.y )
		//到达边界  不入栈
	{
		return;
	}
	//八个方向
	//data[i*widthBytes + 4*j +2]=0x40;//缺陷 暗红色   蓝色边界
	if (!had_view(view_flag,*v,pos.x-1,pos.y) && 
		(data[pos.y*widthBytes + 4*(pos.x-1) + 2] == 0x40 || data[pos.y*widthBytes + 4*(pos.x-1)] == 0xFF && data[pos.y*widthBytes + 4*(pos.x-1)+1] == 0x00))
	{
		stack_push(stack,*s,pos.x-1,pos.y);
		(*s)++;
		view_flag[*v].x = pos.x-1;
		view_flag[*v].y = pos.y;//记录已访问
		(*v)++;
	}
	if (!had_view(view_flag,*v,pos.x-1,pos.y-1) && 
		(data[(pos.y-1)*widthBytes + 4*(pos.x-1) + 2] == 0x40 || data[(pos.y-1)*widthBytes + 4*(pos.x-1)] == 0xFF && data[(pos.y-1)*widthBytes + 4*(pos.x-1)+1] == 0x00))
	{
		stack_push(stack,*s,pos.x-1,pos.y-1);
		(*s)++;
		view_flag[*v].x = pos.x-1;
		view_flag[*v].y = pos.y-1;//记录已访问
		(*v)++;
	}
	if (!had_view(view_flag,*v,pos.x,pos.y-1) && 
		(data[(pos.y-1)*widthBytes + 4*(pos.x) + 2] == 0x40 || data[(pos.y-1)*widthBytes + 4*(pos.x)] == 0xFF && data[(pos.y-1)*widthBytes + 4*(pos.x)+1] == 0x00))
	{
		stack_push(stack,*s,pos.x,pos.y-1);
		(*s)++;
		view_flag[*v].x = pos.x;
		view_flag[*v].y = pos.y-1;//记录已访问
		(*v)++;
	}
	if (!had_view(view_flag,*v,pos.x+1,pos.y-1) && 
		(data[(pos.y-1)*widthBytes + 4*(pos.x+1) + 2] == 0x40 || data[(pos.y-1)*widthBytes + 4*(pos.x+1)] == 0xFF && data[(pos.y-1)*widthBytes + 4*(pos.x+1)+1] == 0x00))
	{
		stack_push(stack,*s,pos.x+1,pos.y-1);
		(*s)++;
		view_flag[*v].x = pos.x+1;
		view_flag[*v].y = pos.y-1;//记录已访问
		(*v)++;
	}
	if (!had_view(view_flag,*v,pos.x+1,pos.y) && 
		(data[pos.y*widthBytes + 4*(pos.x+1) + 2] == 0x40 || data[pos.y*widthBytes + 4*(pos.x+1)] == 0xFF && data[pos.y*widthBytes + 4*(pos.x+1)+1] == 0x00))
	{
		stack_push(stack,*s,pos.x+1,pos.y);
		(*s)++;
		view_flag[*v].x = pos.x+1;
		view_flag[*v].y = pos.y;//记录已访问
		(*v)++;
	}
	if (!had_view(view_flag,*v,pos.x+1,pos.y+1) && 
		(data[(pos.y+1)*widthBytes + 4*(pos.x+1) + 2] == 0x40 || data[(pos.y+1)*widthBytes + 4*(pos.x+1)] == 0xFF && data[(pos.y+1)*widthBytes + 4*(pos.x+1)+1] == 0x00))
	{
		stack_push(stack,*s,pos.x+1,pos.y+1);
		(*s)++;
		view_flag[*v].x = pos.x+1;
		view_flag[*v].y = pos.y+1;//记录已访问
		(*v)++;
	}
	if (!had_view(view_flag,*v,pos.x,pos.y+1) && 
		(data[(pos.y+1)*widthBytes + 4*pos.x + 2] == 0x40 || data[(pos.y+1)*widthBytes + 4*pos.x] == 0xFF && data[(pos.y+1)*widthBytes + 4*pos.x+1] == 0x00))
	{
		stack_push(stack,*s,pos.x,pos.y+1);
		(*s)++;
		view_flag[*v].x = pos.x;
		view_flag[*v].y = pos.y+1;//记录已访问
		(*v)++;
	}
	if (!had_view(view_flag,*v,pos.x-1,pos.y+1) && 
		(data[(pos.y+1)*widthBytes + 4*(pos.x-1) + 2] == 0x40 || data[(pos.y+1)*widthBytes + 4*(pos.x-1)] == 0xFF && data[(pos.y+1)*widthBytes + 4*(pos.x-1)+1] == 0x00))
	{
		stack_push(stack,*s,pos.x-1,pos.y+1);
		(*s)++;
		view_flag[*v].x = pos.x-1;
		view_flag[*v].y = pos.y+1;//记录已访问
		(*v)++;
	}

	



}

void sort(spoint* A,int num)//排序    按夹角从小到大
{
	for (int i =0;i<num;i++)
	{
		for (int j=num;j>i;j--)
		{
			spoint a;
			if (A[j-1].ang > A[j].ang)
			{
				a = A[j-1];
				A[j-1] = A[j];
				A[j] = a;
			}
		}
	}
	i--;
}

struct vector //标准手势的向量
{
	char hand;
	double v[60];//估计所有手势的边界不会超过 60个点
	vector* next;
};

