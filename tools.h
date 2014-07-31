





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
	int x;//����x   ��Ӧ��
	int y;//����y   ��Ӧ��
	double ang;//�н�
};

struct point//���ڼ��� ȱ�ݴ�С
{
	int x;
	int y;
};

void stack_push(point* s,int pos,int x,int y)//��ջ
{
	s[pos].x=x;
	s[pos].y=y;
}


bool had_view(point* view,int length,int x,int y)//�鿴�Ƿ��ѷ���   ����ȱ������ʱ��
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
������ջָ��  ջ��λ�� ��ǰ�� ������ �ѷ�������ָ�� �����С ͼ��ָ�� һ����ռ�ֽ���
*/
void next_push(point* stack,int* s,point pos,spoint end,point* view_flag,int* v,BYTE* data,int widthBytes)//Ѱ����һ�����ʵ���ջ�� ��
{
	if (pos.x == end.x && pos.y == end.y || pos.x-1 == end.x && pos.y == end.y || pos.x+1 == end.x && pos.y == end.y 
		|| pos.x == end.x && pos.y-1 == end.y || pos.x-1 == end.x && pos.y-1 == end.y || pos.x+1 == end.x && pos.y-1 == end.y 
		|| pos.x == end.x && pos.y+1 == end.y || pos.x-1 == end.x && pos.y+1 == end.y || pos.x+1 == end.x && pos.y+1 == end.y )
		//����߽�  ����ջ
	{
		return;
	}
	//�˸�����
	//data[i*widthBytes + 4*j +2]=0x40;//ȱ�� ����ɫ   ��ɫ�߽�
	if (!had_view(view_flag,*v,pos.x-1,pos.y) && 
		(data[pos.y*widthBytes + 4*(pos.x-1) + 2] == 0x40 || data[pos.y*widthBytes + 4*(pos.x-1)] == 0xFF && data[pos.y*widthBytes + 4*(pos.x-1)+1] == 0x00))
	{
		stack_push(stack,*s,pos.x-1,pos.y);
		(*s)++;
		view_flag[*v].x = pos.x-1;
		view_flag[*v].y = pos.y;//��¼�ѷ���
		(*v)++;
	}
	if (!had_view(view_flag,*v,pos.x-1,pos.y-1) && 
		(data[(pos.y-1)*widthBytes + 4*(pos.x-1) + 2] == 0x40 || data[(pos.y-1)*widthBytes + 4*(pos.x-1)] == 0xFF && data[(pos.y-1)*widthBytes + 4*(pos.x-1)+1] == 0x00))
	{
		stack_push(stack,*s,pos.x-1,pos.y-1);
		(*s)++;
		view_flag[*v].x = pos.x-1;
		view_flag[*v].y = pos.y-1;//��¼�ѷ���
		(*v)++;
	}
	if (!had_view(view_flag,*v,pos.x,pos.y-1) && 
		(data[(pos.y-1)*widthBytes + 4*(pos.x) + 2] == 0x40 || data[(pos.y-1)*widthBytes + 4*(pos.x)] == 0xFF && data[(pos.y-1)*widthBytes + 4*(pos.x)+1] == 0x00))
	{
		stack_push(stack,*s,pos.x,pos.y-1);
		(*s)++;
		view_flag[*v].x = pos.x;
		view_flag[*v].y = pos.y-1;//��¼�ѷ���
		(*v)++;
	}
	if (!had_view(view_flag,*v,pos.x+1,pos.y-1) && 
		(data[(pos.y-1)*widthBytes + 4*(pos.x+1) + 2] == 0x40 || data[(pos.y-1)*widthBytes + 4*(pos.x+1)] == 0xFF && data[(pos.y-1)*widthBytes + 4*(pos.x+1)+1] == 0x00))
	{
		stack_push(stack,*s,pos.x+1,pos.y-1);
		(*s)++;
		view_flag[*v].x = pos.x+1;
		view_flag[*v].y = pos.y-1;//��¼�ѷ���
		(*v)++;
	}
	if (!had_view(view_flag,*v,pos.x+1,pos.y) && 
		(data[pos.y*widthBytes + 4*(pos.x+1) + 2] == 0x40 || data[pos.y*widthBytes + 4*(pos.x+1)] == 0xFF && data[pos.y*widthBytes + 4*(pos.x+1)+1] == 0x00))
	{
		stack_push(stack,*s,pos.x+1,pos.y);
		(*s)++;
		view_flag[*v].x = pos.x+1;
		view_flag[*v].y = pos.y;//��¼�ѷ���
		(*v)++;
	}
	if (!had_view(view_flag,*v,pos.x+1,pos.y+1) && 
		(data[(pos.y+1)*widthBytes + 4*(pos.x+1) + 2] == 0x40 || data[(pos.y+1)*widthBytes + 4*(pos.x+1)] == 0xFF && data[(pos.y+1)*widthBytes + 4*(pos.x+1)+1] == 0x00))
	{
		stack_push(stack,*s,pos.x+1,pos.y+1);
		(*s)++;
		view_flag[*v].x = pos.x+1;
		view_flag[*v].y = pos.y+1;//��¼�ѷ���
		(*v)++;
	}
	if (!had_view(view_flag,*v,pos.x,pos.y+1) && 
		(data[(pos.y+1)*widthBytes + 4*pos.x + 2] == 0x40 || data[(pos.y+1)*widthBytes + 4*pos.x] == 0xFF && data[(pos.y+1)*widthBytes + 4*pos.x+1] == 0x00))
	{
		stack_push(stack,*s,pos.x,pos.y+1);
		(*s)++;
		view_flag[*v].x = pos.x;
		view_flag[*v].y = pos.y+1;//��¼�ѷ���
		(*v)++;
	}
	if (!had_view(view_flag,*v,pos.x-1,pos.y+1) && 
		(data[(pos.y+1)*widthBytes + 4*(pos.x-1) + 2] == 0x40 || data[(pos.y+1)*widthBytes + 4*(pos.x-1)] == 0xFF && data[(pos.y+1)*widthBytes + 4*(pos.x-1)+1] == 0x00))
	{
		stack_push(stack,*s,pos.x-1,pos.y+1);
		(*s)++;
		view_flag[*v].x = pos.x-1;
		view_flag[*v].y = pos.y+1;//��¼�ѷ���
		(*v)++;
	}

	



}

void sort(spoint* A,int num)//����    ���нǴ�С����
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

struct vector //��׼���Ƶ�����
{
	char hand;
	double v[60];//�����������Ƶı߽粻�ᳬ�� 60����
	vector* next;
};

