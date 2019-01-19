#include "Interpolation.h"
#include<math.h>

Interpolation::Interpolation(Mat Image)
{
	Interpolation::image = Image;
	Interpolation::row = Image.rows;
	Interpolation::col = Image.cols;
}


Interpolation::~Interpolation(void)
{
}

int Interpolation::pixel(int x,int y,int channel)
{
	return Interpolation::image.at<Vec3b>(x,y)[channel];
}
int Interpolation::Nearest_neighbor_interpolation(double x,double y,int channel)
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
	x_near = min(x_near,Interpolation::row-1);

	y_near = max(y_near,0);
	y_near = min(y_near,Interpolation::col-1);

	return Interpolation::image.at<Vec3b>(x_near,y_near)[channel];
}

int Interpolation::Bilinear_interpolation(double x,double y,int channel)
{
	int x_left = floor(x);
	int y_up = floor(y);
	double pixel_value = 0;
	double u = x - x_left;
	double v = y - y_up;
	
	x_left = max(x_left,0);
	x_left = min(x_left,Interpolation::row-2);
	y_up = max(y_up,0);
	y_up = min(y_up,Interpolation::col-2);
	
	pixel_value = (1-u)*(1-v)*pixel(x_left,y_up,channel) + (1-u)*v*pixel(x_left,y_up+1,channel) + u*(1-v)*pixel(x_left+1,y_up,channel) + u*v*pixel(x_left,y_up,channel);

	if(pixel_value-int(pixel_value)>0.5)
		return int(pixel_value)+1;
	else
		return int(pixel_value);
	
}



double Interpolation::S(double x)
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

int Interpolation::Bicubic_interpolation(double x,double y,int channel)
{
	double u=x-floor(x);
	double v=y-floor(y);
	double A[4],C[4],B[4];
	int i = floor(x);
	int j = floor(y);

	i = max(1,i);
	i = min(i,Interpolation::row-3);
	j = max(1,j);
	j = max(j,Interpolation::col-3);

	for(int ii=0;ii<4;ii++)
	{
		A[ii] = S(u-1+ii);
	}
	for(int ii=0;i<4;i++)
	{
		C[ii] = S(v-1+ii);
	}

	for(int ii=0;ii<4;ii++)
	{
		double pixel_value=0;
		for(int jj=0;jj<4;jj++)
		{
			pixel_value = pixel_value + C[jj]*pixel(i-1+ii,j-1+jj,channel);
		}
		B[ii] = pixel_value;
	}
	double pixel_value = 0;
	for(int ii=0;ii<4;ii++)
	{
		pixel_value = pixel_value+A[ii]*B[ii];
	}
	if(pixel_value-int(pixel_value)>0.5)
		return int(pixel_value)+1;
	else
		return int(pixel_value);
}