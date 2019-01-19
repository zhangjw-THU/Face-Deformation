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
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>


using namespace cv; 
using namespace std;
//const int DIV = 30;
const int DIV2 =15;

void BSpline1(Mat Image1,Mat Image2,int (&num1)[68][2],int num1cp[68][2],int (&num2)[68][2],int num2cp[68][2],Mat (&I1Positive),int DirFlag,int InterFlag,int GSize)
{
	int DIV=GSize;

	for(int ii=0;ii<68;ii++)
		{
			num1[ii][0] =num1cp[ii][0];
			num1[ii][1] = num1cp[ii][1];
			
			num2[ii][0] = num2cp[ii][0];
			num2[ii][1] = num2cp[ii][1];
		}
	BFacialAlignment(num1,num2);
	int r = floor(Image1.rows/DIV);
	int c = floor(Image1.cols/DIV);
	int rp = r+4;
	int cp = c+4;
	round(r);
	Point** node;
	node = new Point *[r + 4];  
	for(int ii=0;ii<r+4;ii++)
	{
		*(node + ii) = new Point[c + 4];
	}
	InitializeNode(node,rp,cp);//每一个网格点初始化为0

	int r2 = floor(Image1.rows/DIV2);
	int c2 = floor(Image1.cols/DIV2);
	int rp2 = r2+4;
	int cp2 = c2+4;
	round(r2);
	Point** node2;
	node2 = new Point *[r2 + 4];  
	for(int ii=0;ii<r2+4;ii++)
	{
		*(node2 + ii) = new Point[c2 + 4];
	}
	InitializeNode(node2,rp2,cp2);


	//MoveEye(num1,num2);
	//MoveMouth(num1,num2);
	NodeMobile1(node,num1,num2,DIV);//根据特征点的位移给网格点的位移幅值

	int PorR = 2;//InterFlag;//正向逆向标志

	//变脸廓
	if(PorR == 1)
	{
		for(int ii=0;ii<Image1.rows;ii++)
		{
			for(int jj=0;jj<Image1.cols;jj++)
			{
				int i0 = floor(double(ii)/double(DIV));
				int j0 = floor(double(jj)/double(DIV));

				double u = double(double((ii-i0*DIV))/(DIV));
				double v = double(double(jj-j0*DIV)/(DIV));
				
				double Gu[4],Gv[4],movex=0,movey=0;
				Gu[0] = BG0(u);
				Gu[1] = BG1(u);
				Gu[2] = BG2(u);
				Gu[3] = BG3(u);

				Gv[0] = BG0(v);
				Gv[1] = BG1(v);
				Gv[2] = BG2(v);
				Gv[3] = BG3(v);

				for(int kk=0;kk<4;kk++)
				{
					for(int hh=0;hh<4;hh++)
					{
						movex = movex+Gu[kk]*Gv[hh]*node[i0+kk][j0+hh].x;
						movey = movey+Gu[kk]*Gv[hh]*node[i0+kk][j0+hh].y;
					}

				}
				int x=round(ii+movey);
				int y=round(jj+movex);
				x = Prevent(x,I1Positive.rows);
				y = Prevent(y,I1Positive.cols);
				
				for(int kk=0;kk<3;kk++)
				{
					//if(I1Positive.at<Vec3b>(x,y)[0]==0)
						I1Positive.at<Vec3b>(x,y)[kk] = Image1.at<Vec3b>(ii,jj)[kk];
				}

				
			}
		}
		Completion(&I1Positive);

	}
	else
	{
		for(int ii=0;ii<Image1.rows;ii++)
		{
			for(int jj=0;jj<Image1.cols;jj++)
			{
				int i0 = floor(double(ii)/double(DIV));
				int j0 = floor(double(jj)/double(DIV));

				double u = double(double((ii-i0*DIV))/(DIV));
				double v = double(double(jj-j0*DIV)/(DIV));

				double Gu[4],Gv[4],movex=0,movey=0;
				Gu[0] = BG0(u);
				Gu[1] = BG1(u);
				Gu[2] = BG2(u);
				Gu[3] = BG3(u);

				Gv[0] = BG0(v);
				Gv[1] = BG1(v);
				Gv[2] = BG2(v);
				Gv[3] = BG3(v);

				for(int kk=0;kk<4;kk++)
				{
					for(int hh=0;hh<4;hh++)
					{
						movex = movex+Gu[kk]*Gv[hh]*node[i0+kk][j0+hh].y;
						movey = movey+Gu[kk]*Gv[hh]*node[i0+kk][j0+hh].x;
					}

				}
				double x=ii-movex;
				double y=jj-movey;
				x = Prevent2(x,I1Positive.rows);
				y = Prevent2(y,I1Positive.cols);
				for(int kk=0;kk<3;kk++)
				{
					//0最近领；1双三次；2双线性；3：Lanczos；4：反距离
					if(InterFlag==1)
					{
						I1Positive.at<Vec3b>(ii,jj)[kk]=Bicubic_interpolation(x,y,kk,Image1);
					}
					else if(InterFlag==2)
					{
						I1Positive.at<Vec3b>(ii,jj)[kk]=Bilinear_interpolation(x,y,kk,Image1);
					}
					else
					{
						I1Positive.at<Vec3b>(ii,jj)[kk]=Nearest_neighbor_interpolation(x,y,kk,Image1);
					}
					/*
					else if(InterFlag==3)
					{
						I1Positive.at<Vec3b>(ii,jj)[kk]=Lanczos(x,y,kk,Image1);
					}
					else
					{
						I1Positive.at<Vec3b>(ii,jj)[kk]=InverseDistanceWeighted(x,y,kk,Image1);
					}
					*/
					//I1Positive.at<Vec3b>(ii,jj)[kk] = Bilinear_interpolation(x,y,kk,Image1);//Image1.at<Vec3b>(x,y)[kk];
				}
			}
		}
	
	}

	Mat I1PTemp(I1Positive.rows,I1Positive.cols,I1Positive.type());
	I1PTemp = I1Positive;
	NodeMobile2(node2,num1,num2,DIV2);//根据特征点的位移给网格点的位移幅值
	//变细节
	/*
	//PorR=2;
	if(PorR == 1)
	{
		for(int ii=0;ii<Image1.rows;ii++)
		{
			for(int jj=0;jj<Image1.cols;jj++)
			{
				int i0 = floor(double(ii)/double(DIV2));
				int j0 = floor(double(jj)/double(DIV2));

				double u = double(double((ii-i0*DIV2))/(DIV2));
				double v = double(double(jj-j0*DIV2)/(DIV2));
				
				double Gu[4],Gv[4],movex=0,movey=0;
				Gu[0] = BG0(u);
				Gu[1] = BG1(u);
				Gu[2] = BG2(u);
				Gu[3] = BG3(u);

				Gv[0] = BG0(v);
				Gv[1] = BG1(v);
				Gv[2] = BG2(v);
				Gv[3] = BG3(v);

				for(int kk=0;kk<4;kk++)
				{
					for(int hh=0;hh<4;hh++)
					{
						movex = movex+Gu[kk]*Gv[hh]*node2[i0+kk][j0+hh].x;
						movey = movey+Gu[kk]*Gv[hh]*node2[i0+kk][j0+hh].y;
					}

				}
				int x=round(ii+movey);
				int y=round(jj+movex);
				x = Prevent(x,I1Positive.rows);
				y = Prevent(y,I1Positive.cols);
				
				for(int kk=0;kk<3;kk++)
				{
					//if(I1Positive.at<Vec3b>(x,y)[0]==0)
					I1Positive.at<Vec3b>(x,y)[kk] = I1PTemp.at<Vec3b>(ii,jj)[kk];
				}
				
				
			}
		}
		Completion(&I1Positive);

	}
	else
	{
		for(int ii=0;ii<Image1.rows;ii++)
		{
			for(int jj=0;jj<Image1.cols;jj++)
			{
				int i0 = floor(double(ii)/double(DIV2));
				int j0 = floor(double(jj)/double(DIV2));

				double u = double(double((ii-i0*DIV2))/(DIV2));
				double v = double(double(jj-j0*DIV2)/(DIV2));
				
				

				double Gu[4],Gv[4],movex=0,movey=0;
				Gu[0] = BG0(u);
				Gu[1] = BG1(u);
				Gu[2] = BG2(u);
				Gu[3] = BG3(u);

				Gv[0] = BG0(v);
				Gv[1] = BG1(v);
				Gv[2] = BG2(v);
				Gv[3] = BG3(v);

				for(int kk=0;kk<4;kk++)
				{
					for(int hh=0;hh<4;hh++)
					{
						movex = movex+Gu[kk]*Gv[hh]*node2[i0+kk][j0+hh].y;
						movey = movey+Gu[kk]*Gv[hh]*node2[i0+kk][j0+hh].x;
					}

				}
				double x=ii-movex;
				double y=jj-movey;
				x = Prevent2(x,I1Positive.rows);
				y = Prevent2(y,I1Positive.cols);
				for(int kk=0;kk<3;kk++)
				{
					if(InterFlag==1)
					{
						I1Positive.at<Vec3b>(ii,jj)[kk]=Bicubic_interpolation(x,y,kk,I1PTemp);
					}
					else if(InterFlag==2)
					{
						I1Positive.at<Vec3b>(ii,jj)[kk]=Bilinear_interpolation(x,y,kk,I1PTemp);

					}
					else
					{
						I1Positive.at<Vec3b>(ii,jj)[kk]=Nearest_neighbor_interpolation(x,y,kk,I1PTemp);
					}

					
					//I1Positive.at<Vec3b>(ii,jj)[kk] = Bilinear_interpolation(x,y,kk,I1PTemp);//Image1.at<Vec3b>(x,y)[kk];
				}
			}
		}
	
	}
	*/
	


	for(int ii=0;ii<r+4;ii++)
	{
		delete *(node + ii);
	}
	delete node;

	for(int ii=0;ii<r2+4;ii++)
	{
		delete *(node2 + ii);
	}
	delete node2;
}

void BSpline2(Mat Image1,Mat Image2,int (&num1)[68][2],int num1cp[68][2],int (&num2)[68][2],int num2cp[68][2],Mat (&I1Positive),int DirFlag,int InterFlag,int GSize)
{
	int DIV = GSize;

	for(int ii=0;ii<68;ii++)
		{
			num1[ii][0] =num1cp[ii][0];
			num1[ii][1] = num1cp[ii][1];
			
			num2[ii][0] = num2cp[ii][0];
			num2[ii][1] = num2cp[ii][1];
		}
	BFacialAlignment(num1,num2);
	int r = floor(Image1.rows/DIV);
	int c = floor(Image1.cols/DIV);
	int rp = r+4;
	int cp = c+4;
	round(r);
	Point** node;
	node = new Point *[r + 4];  
	for(int ii=0;ii<r+4;ii++)
	{
		*(node + ii) = new Point[c + 4];
	}

	InitializeNode(node,rp,cp);//每一个网格点初始化为0

	int r2 = floor(Image1.rows/DIV2);
	int c2 = floor(Image1.cols/DIV2);
	int rp2 = r2+4;
	int cp2 = c2+4;
	round(r2);
	Point** node2;
	node2 = new Point *[r2 + 4];  
	for(int ii=0;ii<r2+4;ii++)
	{
		*(node2 + ii) = new Point[c2 + 4];
	}
	InitializeNode(node2,rp2,cp2);


	//MoveEye(num1,num2);
	//MoveMouth(num1,num2);
	NodeMobile(node,num1,num2,DIV);//根据特征点的位移给网格点的位移幅值

	int PorR = 2;//InterFlag;//正向逆向标志

	//变脸廓
	if(PorR == 1)
	{
		for(int ii=0;ii<Image1.rows;ii++)
		{
			for(int jj=0;jj<Image1.cols;jj++)
			{
				int i0 = floor(double(ii)/double(DIV));
				int j0 = floor(double(jj)/double(DIV));

				double u = double(double((ii-i0*DIV))/(DIV));
				double v = double(double(jj-j0*DIV)/(DIV));
				
				double Gu[4],Gv[4],movex=0,movey=0;
				Gu[0] = BG0(u);
				Gu[1] = BG1(u);
				Gu[2] = BG2(u);
				Gu[3] = BG3(u);

				Gv[0] = BG0(v);
				Gv[1] = BG1(v);
				Gv[2] = BG2(v);
				Gv[3] = BG3(v);

				for(int kk=0;kk<4;kk++)
				{
					for(int hh=0;hh<4;hh++)
					{
						movex = movex+Gu[kk]*Gv[hh]*node[i0+kk][j0+hh].x;
						movey = movey+Gu[kk]*Gv[hh]*node[i0+kk][j0+hh].y;
					}

				}
				int x=round(ii+movey);
				int y=round(jj+movex);
				x = Prevent(x,I1Positive.rows);
				y = Prevent(y,I1Positive.cols);
				
				for(int kk=0;kk<3;kk++)
				{
					//if(I1Positive.at<Vec3b>(x,y)[0]==0)
						I1Positive.at<Vec3b>(x,y)[kk] = Image1.at<Vec3b>(ii,jj)[kk];
				}

				
			}
		}
		Completion(&I1Positive);

	}
	else
	{
		for(int ii=0;ii<Image1.rows;ii++)
		{
			for(int jj=0;jj<Image1.cols;jj++)
			{
				int i0 = floor(double(ii)/double(DIV));
				int j0 = floor(double(jj)/double(DIV));

				double u = double(double((ii-i0*DIV))/(DIV));
				double v = double(double(jj-j0*DIV)/(DIV));

				double Gu[4],Gv[4],movex=0,movey=0;
				Gu[0] = BG0(u);
				Gu[1] = BG1(u);
				Gu[2] = BG2(u);
				Gu[3] = BG3(u);

				Gv[0] = BG0(v);
				Gv[1] = BG1(v);
				Gv[2] = BG2(v);
				Gv[3] = BG3(v);

				for(int kk=0;kk<4;kk++)
				{
					for(int hh=0;hh<4;hh++)
					{
						movex = movex+Gu[kk]*Gv[hh]*node[i0+kk][j0+hh].y;
						movey = movey+Gu[kk]*Gv[hh]*node[i0+kk][j0+hh].x;
					}

				}
				double x=ii-movex;
				double y=jj-movey;
				x = Prevent2(x,I1Positive.rows);
				y = Prevent2(y,I1Positive.cols);
				for(int kk=0;kk<3;kk++)
				{
					//0最近领；1双三次；2双线性；3：Lanczos；4：反距离
					if(InterFlag==1)
					{
						I1Positive.at<Vec3b>(ii,jj)[kk]=Bicubic_interpolation(x,y,kk,Image1);
					}
					else if(InterFlag==2)
					{
						I1Positive.at<Vec3b>(ii,jj)[kk]=Bilinear_interpolation(x,y,kk,Image1);
					}
					else
					{
						I1Positive.at<Vec3b>(ii,jj)[kk]=Nearest_neighbor_interpolation(x,y,kk,Image1);
					}
					//I1Positive.at<Vec3b>(ii,jj)[kk] = Bilinear_interpolation(x,y,kk,Image1);//Image1.at<Vec3b>(x,y)[kk];
				}
			}
		}
	
	}

	Mat I1PTemp(I1Positive.rows,I1Positive.cols,I1Positive.type());
	I1PTemp = I1Positive;
	NodeMobile2(node2,num1,num2,DIV2);//根据特征点的位移给网格点的位移幅值
	//变细节
	
	//PorR=2;
	/*
	if(PorR == 1)
	{
		for(int ii=0;ii<Image1.rows;ii++)
		{
			for(int jj=0;jj<Image1.cols;jj++)
			{
				int i0 = floor(double(ii)/double(DIV2));
				int j0 = floor(double(jj)/double(DIV2));

				double u = double(double((ii-i0*DIV2))/(DIV2));
				double v = double(double(jj-j0*DIV2)/(DIV2));
				
				double Gu[4],Gv[4],movex=0,movey=0;
				Gu[0] = BG0(u);
				Gu[1] = BG1(u);
				Gu[2] = BG2(u);
				Gu[3] = BG3(u);

				Gv[0] = BG0(v);
				Gv[1] = BG1(v);
				Gv[2] = BG2(v);
				Gv[3] = BG3(v);

				for(int kk=0;kk<4;kk++)
				{
					for(int hh=0;hh<4;hh++)
					{
						movex = movex+Gu[kk]*Gv[hh]*node2[i0+kk][j0+hh].x;
						movey = movey+Gu[kk]*Gv[hh]*node2[i0+kk][j0+hh].y;
					}

				}
				int x=round(ii+movey);
				int y=round(jj+movex);
				x = Prevent(x,I1Positive.rows);
				y = Prevent(y,I1Positive.cols);
				
				for(int kk=0;kk<3;kk++)
				{
					//if(I1Positive.at<Vec3b>(x,y)[0]==0)
					I1Positive.at<Vec3b>(x,y)[kk] = I1PTemp.at<Vec3b>(ii,jj)[kk];
				}
				
				
			}
		}
		Completion(&I1Positive);

	}
	else
	{
		for(int ii=0;ii<Image1.rows;ii++)
		{
			for(int jj=0;jj<Image1.cols;jj++)
			{
				int i0 = floor(double(ii)/double(DIV2));
				int j0 = floor(double(jj)/double(DIV2));

				double u = double(double((ii-i0*DIV2))/(DIV2));
				double v = double(double(jj-j0*DIV2)/(DIV2));
				
				

				double Gu[4],Gv[4],movex=0,movey=0;
				Gu[0] = BG0(u);
				Gu[1] = BG1(u);
				Gu[2] = BG2(u);
				Gu[3] = BG3(u);

				Gv[0] = BG0(v);
				Gv[1] = BG1(v);
				Gv[2] = BG2(v);
				Gv[3] = BG3(v);

				for(int kk=0;kk<4;kk++)
				{
					for(int hh=0;hh<4;hh++)
					{
						movex = movex+Gu[kk]*Gv[hh]*node2[i0+kk][j0+hh].y;
						movey = movey+Gu[kk]*Gv[hh]*node2[i0+kk][j0+hh].x;
					}

				}
				double x=ii-movex;
				double y=jj-movey;
				x = Prevent2(x,I1Positive.rows);
				y = Prevent2(y,I1Positive.cols);
				for(int kk=0;kk<3;kk++)
				{
					if(InterFlag==1)
					{
						I1Positive.at<Vec3b>(ii,jj)[kk]=Bicubic_interpolation(x,y,kk,I1PTemp);
					}
					else if(InterFlag==2)
					{
						I1Positive.at<Vec3b>(ii,jj)[kk]=Bilinear_interpolation(x,y,kk,I1PTemp);

					}
					else
					{
						I1Positive.at<Vec3b>(ii,jj)[kk]=Nearest_neighbor_interpolation(x,y,kk,I1PTemp);
					}

					
					//I1Positive.at<Vec3b>(ii,jj)[kk] = Bilinear_interpolation(x,y,kk,I1PTemp);//Image1.at<Vec3b>(x,y)[kk];
				}
			}
		}
	
	}
	*/
	for(int ii=0;ii<r+4;ii++)
	{
		delete *(node + ii);
	}
	delete node;

	for(int ii=0;ii<r2+4;ii++)
	{
		delete *(node2 + ii);
	}
	delete node2;
}