#pragma once
#include<opencv2\opencv.hpp>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include <opencv.hpp>
#include <facedetect-dll.h>
#include<iostream>
#include<vector>
#include<math.h>
#include <stdio.h>
#include <io.h>
#include <string>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include<cmath>
#include<fstream>
#include<iostream>
#include<vector>
#include<math.h>
#define MAX_LINE 1024   //定义txt中最大行数。可调整更改
#define DETECT_BUFFER_SIZE 0x20000//在特征点检测当中申请的缓冲区大小
#define PI 3.1415926

using namespace std;
using namespace cv;
//对特征点的一些操作===========================
double Distance(int x0,int y0,int x1,int y1)//计算距离
{
	double dis = sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));
	return dis;
}

double Zoom_ratio(int o_point1[2],int o_point2[2],int t_point1[2],int t_point2[2])//求放缩比例
{
	double o_dis,t_dis,zoom;
	o_dis = Distance(o_point1[0],o_point1[1],o_point2[0],o_point2[1]);
	t_dis = Distance(t_point1[0],t_point1[1],t_point2[0],t_point2[1]);
	zoom = o_dis/t_dis;
	return zoom-1;

}
void Facial_alignment(int (&Original_feature)[68][2],int (&Target_feature)[68][2])//面部特征点对齐
{
	double ratio;
	//int d_x = Target_feature[31][0] - Original_feature[31][0];
	//int d_y = Target_feature[31][1] - Original_feature[31][1];
	////移动坐标点
	//for(int ii=0;ii<68;ii++)
	//{
	//	Target_feature[ii][0] = Target_feature[ii][0]-d_x;
	//	Target_feature[ii][1] = Target_feature[ii][1]-d_y;
	//}
	
	ratio = Zoom_ratio(Original_feature[31],Original_feature[30],Target_feature[31],Target_feature[30]);
	//将脸放大
	
	
	for(int ii=0;ii<68;ii++)
	{
		Target_feature[ii][0] = floor(Target_feature[ii][0] + ratio*(Target_feature[ii][0]-Target_feature[31][0]));
		Target_feature[ii][1] = floor(Target_feature[ii][1] + ratio*(Target_feature[ii][1]-Target_feature[31][1]));
	}

}

void BFacialAlignment(int (&Original_feature)[68][2],int (&Target_feature)[68][2])//面部特征点对齐
{
	double ratio;
	int d_x = Target_feature[31][0] - Original_feature[31][0];
	int d_y = Target_feature[31][1] - Original_feature[31][1];
	//移动坐标点
	for(int ii=0;ii<68;ii++)
	{
		Target_feature[ii][0] = Target_feature[ii][0]-d_x;
		Target_feature[ii][1] = Target_feature[ii][1]-d_y;
	}
	ratio = Zoom_ratio(Original_feature[31],Original_feature[30],Target_feature[31],Target_feature[30]);
	//将脸放大
	
	
	for(int ii=0;ii<68;ii++)
	{
		Target_feature[ii][0] = floor(Target_feature[ii][0] + ratio*(Target_feature[ii][0]-Target_feature[31][0]));
		Target_feature[ii][1] = floor(Target_feature[ii][1] + ratio*(Target_feature[ii][1]-Target_feature[31][1]));
	}
	

}

//移动嘴
void MoveMouth(int (&num1)[68][2],int (&num2)[68][2])
{
	int d_x = num1[62][0] - num2[62][0];
	int d_y = num1[62][1] - num2[62][1];
	for(int ii=48;ii<68;ii++)
	{
		num2[ii][0] = num2[ii][0]-d_x;
		num2[ii][1] = num2[ii][1]-d_y;
	}
}

//移动眼睛
void MoveEye(int (&num1)[68][2],int (&num2)[68][2])
{

	//移动右眼
	int d_x = num1[43][0] - num2[43][0];
	int d_y = num1[43][1] - num2[43][1];
	for(int ii=42;ii<48;ii++)
	{
		num2[ii][0] = num2[ii][0]-d_x;
		num2[ii][1] = num2[ii][1]-d_y;
	}
	//移动左眼
	d_x = num1[40][0] - num2[40][0];
	d_y = num1[40][1] - num2[40][1];
	for(int ii=37;ii<42;ii++)
	{
		num2[ii][0] = num2[ii][0]-d_x;
		num2[ii][1] = num2[ii][1]-d_y;
	}
}


//正向变换时移动坐标点
void FeatureMove(int (&Original_feature)[68][2],int (&Target_feature)[68][2])
{
	int d_x = Target_feature[31][0] - Original_feature[31][0];
	int d_y = Target_feature[31][1] - Original_feature[31][1];
	//移动坐标点
	for(int ii=0;ii<68;ii++)
	{
		Target_feature[ii][0] = Target_feature[ii][0]-d_x;
		Target_feature[ii][1] = Target_feature[ii][1]-d_y;
	}
}
//四舍五入函数
int round(double x)
{
	int y;
	y = floor(x);
	if(x-y>0.5)
		return y+1;
	else
		return y;
}


//显示特征点#######################################################
void Display_feature(Mat (&Image),int feature[][2])
{
	Mat image = Image;
	int row = image.rows;
	int col = image.cols;
	int size =8;//显示红点的大小
	
	for(int ii=0;ii<68;ii++)
	{
		for(int jj=0;jj<size;jj++)
		{
			for(int kk=0;kk<size;kk++)
			{
				if((feature[ii][1]+jj)>=0&&(feature[ii][1]+jj)<row&&(feature[ii][0]+kk)>=0&&(feature[ii][0]+kk)<col)
				{
				image.at<Vec3b>(feature[ii][1]+jj,feature[ii][0]+kk)[0]=0;
				image.at<Vec3b>(feature[ii][1]+jj,feature[ii][0]+kk)[1]=200;
				image.at<Vec3b>(feature[ii][1]+jj,feature[ii][0]+kk)[2]=0;
				}
			}
		
		}
	}
	/*namedWindow( "Or" , CV_WINDOW_AUTOSIZE );
	imshow("OR",image);
	waitKey(0);
	destroyWindow("Or");*/
}



//人脸特征点检测==================================================
bool FeaturePoint(int (&num)[68][2],Mat Image)
{
	int * pR =NULL;
	unsigned char* pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);

	if(!pBuffer)
	{
		return false;
	}

	Mat I = Image;
	Mat Igray;
	cvtColor(I,Igray,CV_BGR2GRAY);
	int doLandmark =1;
	pR = facedetect_multiview_reinforce(pBuffer, (unsigned char*)(Igray.ptr(0)),Igray.cols,Igray.rows, (int)Igray.step,
        1.2f, 2, 48, 0, doLandmark);

	for (int i = 0; i < (pR ? *pR : 0); i++)
    {
        short * p = ((short*)(pR + 1)) + 142 * i;
       // rectangle(I, Rect(p[0], p[1], p[2], p[3]), Scalar(0, 255, 0), 2);
        if (doLandmark)
        {
            for (int j = 0; j < 68; j++)
			{
                //circle(I, Point((int)p[6 + 2 * j], (int)p[6 + 2 * j + 1]), 1, Scalar(0, 0, 255),2);
				num[j][0] = (int)p[6 + 2 * j];
				num[j][1] = (int)p[6 + 2 * j + 1];
				//cout<<j<<":"<<(int)p[6 + 2 * j]<<","<<(int)p[6 + 2 * j + 1]<<endl;
			}
        }
    }
	return true;
}


////插值函数================================================
int pixel(int x,int y,int channel,Mat image)
{
	return image.at<Vec3b>(x,y)[channel];
}

//最近领插值################################################
int Nearest_neighbor_interpolation(double x,double y,int channel,Mat Image)
{
	int x_near = 0;
	int y_near = 0;

	if(x - floor(x) > 0.5)
		x_near = floor(x)+1;
	else
		x_near = floor(x);
	if(y - floor(y) > 0.5)
		y_near = floor(y)+1;
	else
		y_near = floor(y);

	x_near = max(x_near,0);
	x_near = min(x_near,Image.rows-1);

	y_near = max(y_near,0);
	y_near = min(y_near,Image.cols-1);

	return Image.at<Vec3b>(x_near,y_near)[channel];
}

//双线性#########################################################
int Bilinear_interpolation(double x,double y,int channel,Mat Image)
{
	int x_left = floor(x);
	int y_up = floor(y);
	double pixel_value = 0;
	double u = x - x_left;
	double v = y - y_up;
	
	x_left = max(x_left,0);
	x_left = min(x_left,Image.rows-2);
	y_up = max(y_up,0);
	y_up = min(y_up,Image.cols-2);
	
	pixel_value = (1-u)*(1-v)*pixel(x_left,y_up,channel,Image) + (1-u)*v*pixel(x_left,y_up+1,channel,Image) + u*(1-v)*pixel(x_left+1,y_up,channel,Image) + u*v*pixel(x_left,y_up,channel,Image);

	if(pixel_value-int(pixel_value)>0.5)
		return int(pixel_value)+1;
	else
		return int(pixel_value);
}

//双三次##########################################################
//双三次基函数
double S(double x)
{
	double s = 0;
	x = fabs(x);
	if(x<=1)
	{
		s = 1-2*x*x+x*x*x;
	}
	else if((x>1)&&(x<2))
	{
		s = 4-8*x+5*x*x-x*x*x;
	}
	else
	{
		s = 0.0;
	}
	return s;
}

int Bicubic_interpolation(double x,double y,int channel,Mat Image)
{
	double u=x-floor(x);
	double v=y-floor(y);
	double A[4],C[4],B[4];
	int i = floor(x);
	int j = floor(y);

	if(i<1)
		i=1;
	if(i>Image.rows-3)
		i=Image.rows-3;
	if(j<1)
		j=1;
	if(j>Image.cols-3)
		j=Image.cols-3;

	for(int ii=0;ii<4;ii++)
	{
		A[ii] = S(u+1-ii);
		//cout<<A[ii]<<endl;
	}
	for(int ii=0;ii<4;ii++)
	{
		C[ii] = S(v+1-ii);
		//cout<<C[ii]<<endl;
	}
	//waitKey(0);
	for(int ii=0;ii<4;ii++)
	{
		double pixel_value=0;
		for(int jj=0;jj<4;jj++)
		{
			pixel_value = pixel_value + A[jj]*pixel(i-1+ii,j-1+jj,channel,Image);
		}
		B[ii] = pixel_value;
	}
	double pixel_value = 0;
	for(int ii=0;ii<4;ii++)
	{
		pixel_value = pixel_value+C[ii]*B[ii];
	}
	if(pixel_value-int(pixel_value)>=0.5)
	{
		//cout<<"up:"<<pixel_value<<endl;
		return int(pixel_value)+1;
	}
	else
	{
		//cout<<"down:"<<pixel_value<<endl;
		return int(pixel_value);
	}
}


//Lanczos插值算法

double L(double x)
{
	double ls = 0;
	double xabs = fabs(x);
	int a=2;//Lanczos 核的大小：2对应周围4个点

	if(xabs==0)
		ls=1;
	else
	{
		if(xabs>=a)
			ls=1;
		else
		{
			ls = (a*sin(PI*x)*sin(PI*x/a))/(PI*PI*x*x);
		}
	}
	return ls;

}

int Lanczos(double x,double y,int channel,Mat Image)
{
	double u=x-floor(x);
	double v=y-floor(y);
	double A[4],C[4],B[4];
	int i = floor(x);
	int j = floor(y);

	if(i<1)
		i=1;
	if(i>Image.rows-3)
		i=Image.rows-3;
	if(j<1)
		j=1;
	if(j>Image.cols-3)
		j=Image.cols-3;

	for(int ii=0;ii<4;ii++)
	{
		A[ii] = L(u+1-ii);
		//cout<<A[ii]<<endl;
	}
	for(int ii=0;ii<4;ii++)
	{
		C[ii] = S(v+1-ii);
		//cout<<C[ii]<<endl;
	}
	for(int ii=0;ii<4;ii++)
	{
		double pixel_value=0;
		for(int jj=0;jj<4;jj++)
		{
			pixel_value = pixel_value + A[jj]*pixel(i-1+ii,j-1+jj,channel,Image);
		}
		B[ii] = pixel_value;
	}
	double pixel_value = 0;
	for(int ii=0;ii<4;ii++)
	{
		pixel_value = pixel_value+C[ii]*B[ii];
	}
	if(pixel_value-int(pixel_value)>=0.5)
	{
		//cout<<"up:"<<pixel_value<<endl;
		return int(pixel_value)+1;
	}
	else
	{
		//cout<<"down:"<<pixel_value<<endl;
		return int(pixel_value);
	}
}


//反距离加权法（Inverse Distance Weighted）插值

double H(double x,double y,int i,int j)
{
	double hi = sqrt((x-i)*(x-i)+(y-j)*(y-j));
	return hi;
}

int InverseDistanceWeighted(double x,double y,int channel,Mat Image)
{
	int x_left = floor(x);
	int y_up = floor(y);
	double pixel_value = 0;
	double u = x - x_left;
	double v = y - y_up;
	
	x_left = max(x_left,0);
	x_left = min(x_left,Image.rows-2);
	y_up = max(y_up,0);
	y_up = min(y_up,Image.cols-2);
	
	double wi[4];
	double  sumwi=0;
	int kk=0;
	for(int ii=0;ii<2;ii++)
	{
		for(int jj=0;jj<2;jj++)
		{
			wi[kk] = 1/H(x,y,x_left+ii,y_up+jj);
			sumwi = sumwi+wi[kk];
			kk++;
		}
	}
	for(int kk=0;kk<4;kk++)
	{
		wi[kk] = wi[kk]/sumwi;
	}
	pixel_value = wi[0]*pixel(x_left,y_up,channel,Image) + wi[1]*pixel(x_left,y_up+1,channel,Image) + wi[2]*pixel(x_left+1,y_up,channel,Image) + wi[3]*pixel(x_left,y_up,channel,Image);
	if(pixel_value-int(pixel_value)>0.5)
		return int(pixel_value)+1;
	else
		return int(pixel_value);
}

////读取TXT文件####################################
double str2num(string s)//字符串转数字
{
	double num;
	stringstream ss(s);
	ss>>num;
	return num;
}
void get_num(char* line,int num[2])//获得两个数字
{
	double n[4];
	int len = strlen(line);
	string a[4];
	a[0]="";
	for(int i=0;i<7;i++)
	{
		a[0]=a[0]+line[i]; 
	}
	n[0]= str2num(a[0]);

	a[1] = "";
	for(int i=22;i<24;i++)
	{
		a[1]= a[1]+line[i];
	}
	n[1]= str2num(a[1]);

	a[2] = "";
	for(int i=25;i<32;i++)
	{
		a[2] =a[2]+line[i];
	}
	n[2]= str2num(a[2]);

	a[3] = "";
	for(int i=47;i<49;i++)
	{
		a[3] =a[3]+line[i];
	}
	n[3]= str2num(a[3]);

	num[0] = int(pow(10,n[1])*n[0]);
	num[1] = int(pow(10,n[3])*n[2]);
}

void getTXT(int (&num)[68][2],FILE* fp)//获得坐标
{
	int len;
	int j=0;
	char buf[MAX_LINE];  /*缓冲区*/
	while (fgets(buf,MAX_LINE, fp) != NULL)
	{

		len = strlen(buf);
		buf[len - 1] = '\0';  /*去掉换行符*/
		get_num(buf,num[j]);
		//cout<<num[j][0]<<"   "<<num[j][1]<<endl;
		j++;
	}

}

//B样条插值奇函数###########################################
double BG0(double t)
{
	return (-t*t*t+3*t*t-3*t+1)/6;
}

double BG1(double t)
{
	return (3*t*t*t-6*t*t+4)/6;
}

double BG2 (double t)
{
	return (-3*t*t*t+3*t*t+3*t+1)/6;
}

double BG3(double t)
{
	return t*t*t/6;
}
//初始化结点
void InitializeNode(Point** node ,int rp,int cp)
{
	for(int ii=0;ii<rp;ii++)
	{
		for(int jj=0;jj<cp;jj++)
		{
			node[ii][jj].x=0;
			node[ii][jj].y=0;
		}
	}

}

//结点移动距离的计算
void NodeMobile(Point** node,int num1[68][2],int num2[68][2],int DIV)
{
	int ii,jj,kk;
	for(ii=0;ii<68;ii++)
	{
		int i = round(num1[ii][0]/DIV);
		int j = round(num1[ii][1]/DIV);

		node[j+2][i+2].x = num1[ii][0]-num2[ii][0];
		node[j+2][i+2].y = num1[ii][1]-num2[ii][1];
	}
}
//防止越界row/col度一样
int Prevent(int x,int r)
{
	if(x<0)
		return 0;
	if(x>r-1)
		return r-1;
	return x;
}

double Prevent2(double x,int r)
{
	if(x<0)
		return 0;
	if(x>r-1)
		return double(r-1);
	return x;
}


//TPS算法#########################################
//1.奇函数

double G(double x)
{
	double g ;
	if(x!=0)
		g=x*x*log10(x*x);
	else
		g=0;

	return g;
}
//获得距离矩阵，及系数矩阵
void DistanceMatrix(double (&r)[68][68],int num2[68][2])
{
	int ii,jj,kk;
	for(ii=0;ii<68;ii++)
	{
		for(jj=0;jj<68;jj++)
		{
			if(ii==jj)
			{
				r[ii][jj]= 0;
			}
			else
			{
				r[ii][jj] = Distance(num2[ii][0],num2[ii][1],num2[jj][0],num2[jj][1]);

			}
		}
	}
}

//获得系数矩阵
void CoefficientMatrix(double (&g)[68][68],double r[68][68])
{
	int ii,jj,kk;
	for(ii=0;ii<68;ii++)
	{
		for(jj=0;jj<68;jj++)
		{
			if(ii==jj)
			{
				g[ii][jj]= 0;
			}
			else
			{
				g[ii][jj] = G(r[ii][jj]);

			}
		}
	}
}

//获得目标点和坐标点之间的差
void XYDiff(int (&Xdiff)[68],int (&Ydiff)[68],int num1[68][2],int num2[68][2])
{
	int ii,jj;
	for (ii=0;ii<68;ii++)
	{
		Xdiff[ii] = num2[ii][0] - num1[ii][0];
		Ydiff[ii] = num2[ii][1] - num1[ii][1];
	}
}

//获得普通位置的坐标对应的G(rij):共68个
void GR(double (&gr)[68],int x,int y,int num1[68][2])
{
	int ii,jj,kk;
	double r[68];
	for(ii=0;ii<68;ii++)
	{
		r[ii] = Distance(x,y,num1[ii][0],num1[ii][1]);
	}
	for(jj=0;jj<68;jj++)
	{
		gr[jj] = G(r[jj]);
	}

}


//获得普通点的对应坐标
void XY2UV(int (&uv)[2],int x,int y,int num1[68][2],double a[71],double b[71],int rc[2],int (&flag))
{
	int ii,jj,kk;
	double gr[68];
	GR(gr,x,y,num1);
	double u = a[68]+a[69]*x+a[70]*y;
	double v = b[68]+b[69]*x+b[70]*y;
	for(ii=0;ii<68;ii++)
	{
		u = u+a[ii]*gr[ii];
		v = v+b[ii]*gr[ii];
	}
	
	uv[0] = int(max(0,round(u)));
	uv[0] = int(min(uv[0],rc[0]-1));
	if (uv[0]!=round(u))
		flag=0;


	uv[1] = int(max(0,round(v)));
	uv[1] = int(min(uv[1],rc[1]-1));
	if (uv[1]!=round(v))
		flag=0;
}

void getab(int num1[68][2],int num2[68][2],double wI[71][71],double (&aaa)[71],double (&bbb)[71])//获得系数
{

	int num1x[71];
	int num2x[71];
	int num1y[71];
	int num2y[71];
	for(int ii=0;ii<68;ii++)
	{
		num1x[ii] = num1[ii][0];
		num1y[ii] = num1[ii][1];

		num2x[ii] = num2[ii][0];
		num2y[ii] = num2[ii][1];

	}
	for(int ii=68;ii<71;ii++)
	{
		num1x[ii] = 0;
		num1y[ii] = 0;

		num2x[ii] = 0;
		num2y[ii] = 0;
	}

	for(int ii=0;ii<71;ii++)
	{
		aaa[ii] = 0;
		bbb[ii] = 0;
		for(int jj=0;jj<71;jj++)
		{
			aaa[ii] = aaa[ii]+num2x[jj]*wI[ii][jj];
			bbb[ii] = bbb[ii]+num2y[jj]*wI[ii][jj];
		}
	}

}
void getMatrix(double (&LUa)[71][71],int num1[68][2],int num2[68][2])//获得原方程的系数矩阵
{
	int xdiff[68],ydiff[68];
	XYDiff(xdiff,ydiff,num1,num2);
	double r[68][68];
	double g[68][68];
	DistanceMatrix(r,num1);
	CoefficientMatrix(g,r);//系数矩阵
	int num1x[68];
	int num2x[68];
	int num1y[68];
	int num2y[68];
	for(int ii=0;ii<68;ii++)
	{
		num1x[ii] = num1[ii][0];
		num1y[ii] = num1[ii][1];

		num2x[ii] = num2[ii][0];
		num2y[ii] = num2[ii][1];

	}
	int ii,jj,kk;
	for(ii=0;ii<68;ii++)
	{
		for(jj=0;jj<68;jj++)
		{
			LUa[ii][jj] = g[ii][jj];
		}
		LUa[ii][68] = 1;
		LUa[ii][69] = num1[ii][0];
		LUa[ii][70] = num1[ii][1];	
	}

	for(ii=0;ii<71;ii++)
	{
		if(ii<68)
			LUa[68][ii] = 1;
		else
			LUa[68][ii] = 0;
	}
	for(ii=0;ii<71;ii++)
	{
		if(ii<68)
			LUa[69][ii] = num1[ii][0];
		else
			LUa[69][ii] = 0;
	}
	for(ii=0;ii<71;ii++)
	{
		if(ii<68)
			LUa[70][ii] = num1[ii][1];
		else
			LUa[70][ii] = 0;
	}
}


bool Gauss(double (&A)[71][71], double (&B)[71][71], int n,int (&GaussFlag))
{
    int i, j, k;
    float max, temp;
    float t[71][71];                //临时矩阵
                                  //将A矩阵存放在临时矩阵t[n][n]中
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            t[i][j] = A[i][j];
        }
    }
    //初始化B矩阵为单位阵
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            B[i][j] = (i == j) ? (float)1 : 0;
        }
    }
    for (i = 0; i < n; i++)
    {
        //寻找主元
        max = t[i][i];
        k = i;
        for (j = i + 1; j < n; j++)
        {
            if (fabs(t[j][i]) > fabs(max))
            {
                max = t[j][i];
                k = j;
            }
        }
        //如果主元所在行不是第i行，进行行交换
        if (k != i)
        {
            for (j = 0; j < n; j++)
            {
                temp = t[i][j];
                t[i][j] = t[k][j];
                t[k][j] = temp;
                //B伴随交换
                temp = B[i][j];
                B[i][j] = B[k][j];
                B[k][j] = temp;
            }
        }
        //判断主元是否为0, 若是, 则矩阵A不是满秩矩阵,不存在逆矩阵
        if (t[i][i] == 0)
        {
			GaussFlag=1;
            printf("There is no inverse matrix!");
            return false;
        }
        //消去A的第i列除去i行以外的各行元素
        temp = t[i][i];
        for (j = 0; j < n; j++)
        {
            t[i][j] = t[i][j] / temp;        //主对角线上的元素变为1
            B[i][j] = B[i][j] / temp;        //伴随计算
        }
        for (j = 0; j < n; j++)        //第0行->第n行
        {
            if (j != i)                //不是第i行
            {
                temp = t[j][i];
                for (k = 0; k < n; k++)        //第j行元素 - i行元素*j列i行元素
                {
                    t[j][k] = t[j][k] - t[i][k] * temp;
                    B[j][k] = B[j][k] - B[i][k] * temp;
                }
            }
        }
    }
    return true;
}


void Completion( Mat *Image)//正向TPS时会有很多空点，进行补全
{
	int flag=0,col = (*Image).cols , row = (*Image).rows, r=0, c=0;
	for(int ii=1;ii<row-1;ii++)
	{
		for(int jj=1;jj<col-1;jj++)
		{
			if((*Image).at<Vec3b>(ii,jj)[1] == 255)
			{	
				if(flag==0)
				{
					r=ii;
					c=jj-1;
					flag=1;	
					
				}
				else
				{
					r=ii-1;
					c=jj;
					flag=0;	
					
				}
			}
			else
				continue;
		
			for(int kk=0;kk<3;kk++)
				(*Image).at<Vec3b>(ii,jj)[kk]=(*Image).at<Vec3b>(r,c)[kk];
		}
	}

}

//下面是逆向TPS

bool UV2XY(double (&XY)[2],int u,int v,int num2[68][2],double a[71],double b[71],int rc[2])
{
	double x,y;
	int ii,jj,kk;
	double gr[68];
	GR(gr,u,v,num2);
	double A = a[68]+a[69]*u+a[70]*v;
	double B = b[68]+b[69]*u+b[70]*v;
	for(ii=0;ii<68;ii++)
	{
		A = A+a[ii]*gr[ii];
		B = B+b[ii]*gr[ii];
	}
	x=A;
	y=B;
	/*
	x = (A*b[70]-B*a[70])/(a[69]*b[70]-a[70]*b[69]);
	y = (A*b[69]-B*a[69])/(a[70]*b[69]-a[69]*b[70]);
	*/
	if(x<0)
		return false;
	if(x>rc[0]-2)
		return false;
	if(y<0)
		return false;
	if(y>rc[1]-2)
		return false;

	XY[0] = x;//round(x);
	XY[1] = y;//round(y);
	return true;
		
}

void InitializeMat(Mat (&Image))
{
	for(int ii=0;ii<Image.rows;ii++)
	{
		for(int jj=0; jj<Image.cols;jj++)
		{
			
			for(int kk=0;kk<3;kk++)
			{
				Image.at<Vec3b>(ii,jj)[kk]=255;
			}
		}
	}
}


//B样条 

double D(int x,int y,int i,int j)
{
	double dis = sqrt((x-i)*(x-i)+(y-j)*(y-j));
	return dis;
}

//整体变换
void NodeMobile1(Point** node,int num1[68][2],int num2[68][2],int DIV)
{
	int ii,jj,kk;
	for(ii=0;ii<68;ii++)
	{
		int i = floor(double(num1[ii][0])/DIV);
		int j = floor(double(num1[ii][1])/DIV);
		double Dis[2][2];
		Dis[0][0] = D(num1[ii][0],num1[ii][1],i*DIV,j*DIV)+0.00000001;
		Dis[1][0] = D(num1[ii][0],num1[ii][1],(i+1)*DIV,j*DIV)+0.00000001;
		Dis[0][1] = D(num1[ii][0],num1[ii][0],i*DIV,(j+1)*DIV)+0.00000001;
		Dis[1][1] = D(num1[ii][0],num2[ii][0],(i+1)*DIV,(j+1)*DIV)+0.00000001;

		double IvDis[2][2];
		for(int ii=0;ii<2;ii++)
		{
			for(int jj=0;jj<2;jj++)
			{
				IvDis[ii][jj] = (1/Dis[ii][jj])/(1/Dis[0][0]+1/Dis[1][0]+1/Dis[0][1]+1/Dis[1][1]);
			}
		}
		double x = num1[ii][0]-num2[ii][0];
		double y = num1[ii][1]-num2[ii][1];
		
		for(int ii=0;ii<2;ii++)
		{
			for(int jj=0;jj<2;jj++)
			{
				node[j+2+ii][i+2+jj].x =x*IvDis[ii][jj];//node[j+2+ii][i+2+jj].x+x*IvDis[ii][jj];// 
				node[j+2+ii][i+2+jj].y =y*IvDis[ii][jj];// node[j+2+ii][i+2+jj].y+y*IvDis[ii][jj];//

			}
		}
		
	}
}

//细节变换
void NodeMobile2(Point** node2,int num1[68][2],int num2[68][2],int DIV2)
{
	int ii,jj,kk;
	for(ii=48;ii<68;ii++)
	{
		int i = round(double(num1[ii][0])/DIV2);
		int j = round(double(num1[ii][1])/DIV2);

		double Dis[2][2];
		Dis[0][0] = D(num1[ii][0],num1[ii][1],i*DIV2,j*DIV2)+0.00000001;
		Dis[1][0] = D(num1[ii][0],num1[ii][1],(i+1)*DIV2,j*DIV2)+0.00000001;
		Dis[0][1] = D(num1[ii][0],num1[ii][0],i*DIV2,(j+1)*DIV2)+0.00000001;
		Dis[1][1] = D(num1[ii][0],num2[ii][0],(i+1)*DIV2,(j+1)*DIV2)+0.00000001;

		double IvDis[2][2];
		for(int ii=0;ii<2;ii++)
		{
			for(int jj=0;jj<2;jj++)
			{
				IvDis[ii][jj] = (1/Dis[ii][jj])/(1/Dis[0][0]+1/Dis[1][0]+1/Dis[0][1]+1/Dis[1][1]);
			}
		}
		double x = num1[ii][0]-num2[ii][0];
		double y = num1[ii][1]-num2[ii][1];

		
		for(int ii=0;ii<2;ii++)
		{
			for(int jj=0;jj<2;jj++)
			{
				node2[j+2+ii][i+2+jj].x = node2[j+2+ii][i+2+jj].x+x*IvDis[ii][jj];
				node2[j+2+ii][i+2+jj].y = node2[j+2+ii][i+2+jj].y+y*IvDis[ii][jj];

			}
		}
		/*
		node2[j+2][i+2].x = node2[j+2][i+2].x+num1[ii][0]-num2[ii][0];
		node2[j+2][i+2].y = node2[j+2][i+2].y+num1[ii][1]-num2[ii][1];
		*/
	}
}