#ifndef FACEDISTORTED_H
#define FACEDISTORTED_H
//pragma execution_character_set("utf-8") 
#include <QtWidgets/QMainWindow>
#include "ui_facedistorted.h"
#include<iostream>
#include<qmessagebox.h>
#include<qfiledialog.h>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<QString>
#include<string>
#include<qlayout.h>
#include<qdialog.h>
using namespace std;
using namespace cv;
class Facedistorted : public QMainWindow
{
	Q_OBJECT

public:
	Facedistorted(QWidget *parent = 0);
	~Facedistorted();
	int num1[68][2];
	int num2[68][2];
	int num1cp[68][2];
	int num2cp[68][2];
	int row;
	int col;
	int TransNum;
	int DeformationFlag;//图像变形方式的选择；默认为TPS
	int InterpolationFlag;//插值方式的选择；默认为最近领
	int DirectionFlag;//变形方向；默认为逆向
	int ZoomFlag;//放缩标志
	int MoveFlag;
	int IFCreatFlag;//是否生成图片的标志
	int GaussFlag;//解方程的标志；若为0，正常；为1；解不了；

	int IFReadImageFlag[2];//是否读取图像：0：没读；1读了;两张图
	int IFReaNumFlag[2];//是否读取num,0：没读；1：读了
	int GridSize;


	Mat Image1;//原图；
	Mat Image2;//目标图
	Mat Image1Positive;//正向变换
	Mat Image1Reserve;//逆向变换
	Mat Image2Positive;//和图2一般大小
	Mat Image2Reserve;
	QValidator *validator;
	

private:
	Ui::FacedistortedClass ui;

	//用于显示图片
	QLabel *label1;
	QLabel *label2;
	QLabel *label3;
	QLabel *label4;
	QLabel *label5;

public slots:
	void OpenImage1();
	void OpenImage2();
	void ImageCreat();
	void ImageSave();
	void ReadNum1();
	void ReadNum2();
	void ExtractNum1();
	void ExtractNum2();
	void ZoomNum();
	void ShowNum1();
	void ShowNum2();
};

#endif // FACEDISTORTED_H
