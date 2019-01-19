//该文件只是TPS顶层的一些实现
//具体的过程都在AttrV.h文件里面

#include<iostream>  
#include<iomanip>   
#include <stdio.h>
#include <io.h>
#include <string>
#include <fstream>
#include <sstream>
#include<iostream>
#include <iostream>
#include <string>
#include <vector>
#include<fstream>
#include<math.h>
#include"AttrV.h"
#include <sstream>
#include<string>
#include <cmath>
#include <algorithm>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>

using namespace cv;
using namespace std;   

void TPS(Mat Image1,Mat Image2,int(&num1)[68][2],int num1py[68][2],int (&num2)[68][2],int num2py[68][2],Mat (&I1Positive),Mat (&I1Reverse),Mat (&I2Positive),Mat (&I2Reverse),int DirFlag,int InterFlag,int ZoomFlag,int (&GaussFlag),int MoveFlag)
{
	for(int ii=0;ii<68;ii++)
		{
			num1[ii][0] =num1py[ii][0];
			num1[ii][1] = num1py[ii][1];
			
			num2[ii][0] = num2py[ii][0];
			num2[ii][1] = num2py[ii][1];
		}
	if(ZoomFlag==1)
		Facial_alignment(num1,num2);//脸部对齐

	if(MoveFlag==1)
	{
		FeatureMove(num1,num2);
	}
	else
	{
		for(int ii=0;ii<68;ii++)
		{
			num1[ii][0] =num1py[ii][0];
			num1[ii][1] = num1py[ii][1];
			
			num2[ii][0] = num2py[ii][0];
			num2[ii][1] = num2py[ii][1];
		}
	}
	double w[71][71],wI[71][71];//w为系数矩阵，wI为系数逆矩阵
	double rw[71][71],rwI[71][71];//用于逆向变换

	getMatrix(w,num1,num2);//获得系数矩阵
	Gauss(w,wI,71,GaussFlag);//获得系数矩阵的逆矩阵

	getMatrix(rw,num2,num1);//用于逆向变换
	Gauss(rw,rwI,71,GaussFlag);

	double aaa[71],bbb[71];//aaa为x的系数，bbb为y的系数
	double raaa[71],rbbb[71];//逆向变换的系数

	getab(num1,num2,wI,aaa,bbb);//获得aaa,bbb的值
	getab(num2,num1,rwI,raaa,rbbb);


	//Mat I1Positive(Image1.rows,Image1.cols,Image1.type());//存储正向变化的结果
	//Mat I1Reverse(Image1.rows,Image1.cols,Image1.type());//存储逆向变化的结果图
	InitializeMat(I1Positive);
	InitializeMat(I1Reverse);
	InitializeMat(I2Positive);
	InitializeMat(I2Reverse);

	int I1cr[2],I2cr[2];//分别存储图像的行列值，先r后c
	I1cr[0] = Image1.cols;
	I1cr[1] = Image1.rows;
	I2cr[0] = Image2.cols;
	I2cr[1] = Image2.rows;

	int PorR = DirFlag;//正向逆向标志：1表示正向；0表示逆向

	if(PorR==1)
	{
		int ii,jj,kk,uv[2],flag=1;
		for(ii=0;ii<Image1.cols;ii++)
		{
			for(jj=0; jj<Image1.rows;jj++)
			{
				flag=1;
				XY2UV(uv,ii,jj,num1,aaa,bbb,I1cr,flag);
				
				if(flag==0)
				{
					for(kk=0;kk<3;kk++)
					{
						I1Positive.at<Vec3b>(uv[1],uv[0])[kk]=0;
					}
				}
				else
				{
					for(kk=0;kk<3;kk++)
					{
						I1Positive.at<Vec3b>(uv[1],uv[0])[kk]=Image1.at<Vec3b>(jj,ii)[kk];
					}
				}
			}
		}
		Completion(&I1Positive);//正向生成时，图像有空缺，进行邻近补全
	}

	else
	{
		double xy[2];
		int ii,jj,kk,flag=1;
		for(ii=0;ii<Image2.cols;ii++)
		{
			for(jj=0; jj<Image2.rows;jj++)
			{
				flag=1;
				I1cr[1]=Image1.rows;
				I1cr[0]=Image1.cols;
				if(UV2XY(xy,ii,jj,num2,raaa,rbbb,I1cr))
				{
					for(kk=0;kk<3;kk++)
					{
						if(InterFlag==1)//0最近领；1双三次；2双线性；3：Lanczos；4：反距离
							I2Positive.at<Vec3b>(jj,ii)[kk]=Bicubic_interpolation(xy[1],xy[0],kk,Image1);//Image1.at<Vec3b>(xy[0],xy[1])[kk];
						else if(InterFlag==2)
							I2Positive.at<Vec3b>(jj,ii)[kk]=Bilinear_interpolation(xy[1],xy[0],kk,Image1);
						else if(InterFlag==0)
							I2Positive.at<Vec3b>(jj,ii)[kk]=Nearest_neighbor_interpolation(xy[1],xy[0],kk,Image1);
						else if(InterFlag==3)
							I2Positive.at<Vec3b>(jj,ii)[kk]=Lanczos(xy[1],xy[0],kk,Image1);
						else 
							I2Positive.at<Vec3b>(jj,ii)[kk]=InverseDistanceWeighted(xy[1],xy[0],kk,Image1);
					}
				}
			}
		}
	}
}