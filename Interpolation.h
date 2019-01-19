#pragma once
#include<opencv2\opencv.hpp>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>
#include<vector>
#include<math.h>

using namespace std;
using namespace cv;

class Interpolation
{
public:
	Interpolation(Mat Image);
	~Interpolation(void);
	int pixel(int x,int y,int channel);

	int Nearest_neighbor_interpolation(double x,double y,int channel);//最近邻插值

	int Bilinear_interpolation(double x, double y, int channel);//双线性

	double S(double x);
	int Bicubic_interpolation(double x,double ,int channel);

private:
	Mat image;
	int row;
	int col;
};

