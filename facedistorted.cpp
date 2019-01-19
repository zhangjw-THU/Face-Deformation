#pragma execution_character_set("utf-8") 
#include "facedistorted.h"
#include<iostream>
#include<qmessagebox.h>
#include<qfiledialog.h>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<QString>
#include<string>
#include<qlayout.h>
#include<qlabel.h>
#include<qfile.h>
#include<QDebug>
#include"Attribute.h"
#include <qtextcodec.h>
#include"TPS.h"
#include"qprogressdialog.h"
#include"BSpline.h"
#include"BSplinePlusTPS.h"

Facedistorted::Facedistorted(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	InitializeMat(Image1);
	InitializeMat(Image2);
	Facedistorted::InterpolationFlag=0;//三个都默认选择第一个
	Facedistorted::DeformationFlag=0;
	Facedistorted::DirectionFlag=0;
	Facedistorted::TransNum=0;

	Facedistorted::IFReadImageFlag[0] =0;
	Facedistorted::IFReaNumFlag[0] = 0;

	Facedistorted::IFReadImageFlag[1] =0;
	Facedistorted::IFReaNumFlag[1]=0;
	Facedistorted::IFCreatFlag=0;
	Facedistorted::GaussFlag=0;
	Facedistorted::MoveFlag=0;
	Facedistorted::GridSize=30;

	validator = new QIntValidator(1,100,this);
	ui.lineEdit->setValidator(validator);
	
}

Facedistorted::~Facedistorted()
{
	delete validator;
}



void BPlusTPS(Mat Image1,Mat Image2,int(&num1)[68][2],int (&num1cp)[68][2],int (&num2)[68][2],int num2cp[68][2],Mat (&I1Positive),Mat (&Image1R),Mat (&Image2P),Mat (&Image2R),int DirectionFlag,int InterFlag,int ZoomFlag,int (&GaussFlag),int MoveFlag,int GSize)
{
	int num11[68][2];
	int num11cp[68][2];
	Mat Image1P(Image1.rows,Image1.cols,Image1.type());
	BSpline2(Image1,Image2,num1,num1cp,num2,num2cp,I1Positive,DirectionFlag,InterFlag,GSize);

	FeaturePoint(num11,I1Positive);
	FeaturePoint(num11cp,I1Positive);

	TPS(I1Positive,Image2,num1,num1cp,num2,num2cp,Image1R,Image1R,Image2P,Image2R,DirectionFlag,InterFlag,ZoomFlag,GaussFlag,MoveFlag);

}
void Facedistorted::OpenImage1()
{
	QString path0 = QFileDialog::getOpenFileName(this,tr("Open File"),NULL,tr("txtfile(*.jpg)"));
	if(path0.isEmpty())
	{
	QMessageBox numshow1;
	QString info1=info1.fromLocal8Bit("提示");
	numshow1.setWindowTitle(info1);
	QString info=info.fromLocal8Bit("未选择任何文件，请重新读取！");
	numshow1.setText(info);
	numshow1.exec();
	}
	else
	{
	QImage qimage;
	qimage.load(path0);
	string path_image1 = path0.toStdString();
	Image1 = imread(path_image1);
	row = Image1.rows;
	col = Image1.cols;
	Mat QImage1;
	cvtColor(Image1, Image1, CV_BGR2RGB);
	cv::resize(Image1,QImage1,Size(300,200));;
	QImage im1 = QImage((const unsigned char*)(QImage1.data), QImage1.cols, QImage1.rows, QImage::Format_RGB888);
	label1 = new QLabel();
	label1->setScaledContents(true);
    label1->setPixmap(QPixmap::fromImage(im1));
    label1->resize(QSize(im1.width(), im1.height()));
	ui.scrollArea->setWidget(label1);

	Facedistorted::IFReadImageFlag[0]=1;

	QMessageBox numshow;
	QString info1=info1.fromLocal8Bit("原图");
	numshow.setWindowTitle(info1);
	QString info=info.fromLocal8Bit("请继续读取或者自动识别特征点。");
	numshow.setText(info);
	numshow.exec();
	}
	
}

void Facedistorted::ReadNum1()
{
	QImage qimage;
	QString path1 = QFileDialog::getOpenFileName(this,tr("Open File"),NULL,tr("txtfile(*.txt)"));
	qimage.load(path1);
    string path_num1 = path1.toStdString();
	FILE *fnum1;
	fnum1 = fopen(path_num1.c_str(),"r");
	//cout<<path1.toStdString()<<endl;
	if((fnum1 = fopen(path_num1.c_str(),"r"))==NULL)
	{
	QMessageBox numshow1;
	QString info1=info1.fromLocal8Bit("提示");
	numshow1.setWindowTitle(info1);
	QString info=info.fromLocal8Bit("未选择任何文件，请重新读取！");
	numshow1.setText(info);
	numshow1.exec();
	}
	else
	{
	getTXT(num1,fnum1);
	//getTXT(num1cp,fnum1);
	for(int ii=0;ii<68;ii++)
	{
		num1cp[ii][0] =num1[ii][0];
		num1cp[ii][1] = num1[ii][1];
	}

	Facedistorted::IFReaNumFlag[0]=1;

	QMessageBox numshow1;
	QString info1=info1.fromLocal8Bit("原图特征点");
	numshow1.setWindowTitle(info1);

	QString info=info.fromLocal8Bit("特征点读取成功。");
	numshow1.setText(info);
	numshow1.exec();
	}
	
}

void Facedistorted::ExtractNum1()
{
	if(IFReadImageFlag[0]==1)
	{
	FeaturePoint(num1,Image1);
	FeaturePoint(num1cp,Image1);

	Facedistorted::IFReaNumFlag[0]=1;

	QMessageBox numshow1;
	QString info1=info1.fromLocal8Bit("人脸识别");
	numshow1.setWindowTitle(info1);
	QString info=info.fromLocal8Bit("特征点识别成功");
	numshow1.setText(info);
	numshow1.exec();
	}
	else
	{
	QMessageBox numshow1;
	QString info1=info1.fromLocal8Bit("提示");
	numshow1.setWindowTitle(info1);
	QString info=info.fromLocal8Bit("没有原图，请先导入图片！");
	numshow1.setText(info);
	numshow1.exec();
	}
}

void Facedistorted::ShowNum1()
{
	if((IFReadImageFlag[0]==1)&&(IFReaNumFlag[0]==1))
	{
	Mat Image1Show;
	Image1Show = Image1.clone();
	Display_feature(Image1Show,num1cp);
	Mat QImage1;
	cv::resize(Image1Show,QImage1,Size(300,200));

	QImage im1 = QImage((const unsigned char*)(QImage1.data), QImage1.cols, QImage1.rows, QImage::Format_RGB888);
	label4 = new QLabel();
	label4->setScaledContents(true);
    label4->setPixmap(QPixmap::fromImage(im1));
    label4->resize(QSize(im1.width(), im1.height()));
	ui.scrollArea_4->setWidget(label4);
	}
	else
	{
	QMessageBox numshow1;
	QString info1=info1.fromLocal8Bit("提示");
	numshow1.setWindowTitle(info1);
	QString info=info.fromLocal8Bit("数据不全。请先读取原图以及特征点！");
	numshow1.setText(info);
	numshow1.exec();
	}
	

}


void Facedistorted::OpenImage2()
{
	QString path2 = QFileDialog::getOpenFileName(this,tr("Open File"),NULL,tr("txtfile(*.jpg)"));
	if(path2.isEmpty())
	{
	QMessageBox numshow1;
	QString info1=info1.fromLocal8Bit("提示");
	numshow1.setWindowTitle(info1);
	QString info=info.fromLocal8Bit("未选择任何文件，请重新读取！");
	numshow1.setText(info);
	numshow1.exec();
	}
	else
	{
	QImage qimage;
	qimage.load(path2);
	string path_image2 = path2.toStdString();
	
	Image2 = imread(path_image2);
	Mat QImage2;
	cvtColor(Image2, QImage2, CV_BGR2RGB);
	cv::resize(QImage2,QImage2,Size(300,200));;
	QImage im2 = QImage((const unsigned char*)(QImage2.data), QImage2.cols, QImage2.rows, QImage::Format_RGB888);
	
	label2 = new QLabel();
	label2->setScaledContents(true);
    label2->setPixmap(QPixmap::fromImage(im2));
    label2->resize(QSize(im2.width(), im2.height()));
	ui.scrollArea_2->setWidget(label2);

	Facedistorted::IFReadImageFlag[1]=1;

	QMessageBox numshow2;
	QString info1=info1.fromLocal8Bit("目标图");
	numshow2.setWindowTitle(info1);
	QString info=info.fromLocal8Bit("请继续读取或者自动识别特征点。");
	numshow2.setText(info);
	numshow2.exec();
	}
}

void Facedistorted::ReadNum2()
{
	QImage qimage;
	QString path3 = QFileDialog::getOpenFileName(this,tr("Open File"),NULL,tr("txtfile(*.txt)"));
	qimage.load(path3);
    string path_num2 = path3.toStdString();
	FILE *fnum2;
	//fnum2 = fopen(path_num2.c_str(),"r");
	if((fnum2 = fopen(path_num2.c_str(),"r"))==NULL)
	{
	QMessageBox numshow1;
	QString info1=info1.fromLocal8Bit("提示");
	numshow1.setWindowTitle(info1);
	QString info=info.fromLocal8Bit("未选择任何文件，请重新读取！");
	numshow1.setText(info);
	numshow1.exec();
	}
	else
	{
		
	getTXT(num2,fnum2);
	//getTXT(num2cp,fnum2);
	for(int ii=0;ii<68;ii++)
	{
		num2cp[ii][0] = num2[ii][0];
		num2cp[ii][1] = num2[ii][1];
	}


	Facedistorted::IFReaNumFlag[1]=1;

	QMessageBox numshow3;
	QString info1=info1.fromLocal8Bit("目标图特征点");
	numshow3.setWindowTitle(info1);
	QString info=info.fromLocal8Bit("特征点读取成功。");
	numshow3.setText(info);
	numshow3.exec();
	}
}

void Facedistorted::ExtractNum2()
{
	if(IFReadImageFlag[1]==1)
	{
	FeaturePoint(num2,Image2);
	FeaturePoint(num2cp,Image2);

	Facedistorted::IFReaNumFlag[1]=1;

	QMessageBox numshow1;
	QString info1=info1.fromLocal8Bit("人脸识别");
	numshow1.setWindowTitle(info1);
	QString info=info.fromLocal8Bit("特征点识别成功");
	numshow1.setText(info);
	numshow1.exec();
	}
	else
	{
	QMessageBox numshow1;
	QString info1=info1.fromLocal8Bit("提示");
	numshow1.setWindowTitle(info1);
	QString info=info.fromLocal8Bit("无目标图，请先导入图片！");
	numshow1.setText(info);
	numshow1.exec();
	}

}

void Facedistorted::ShowNum2()
{
	if((IFReadImageFlag[1]==1)&&(IFReaNumFlag[1]==1))
	{
	Mat Image2Show;
	Image2Show = Image2.clone();
	cvtColor(Image2Show, Image2Show, CV_RGB2BGR);
	Display_feature(Image2Show,num2cp);
	Mat QImage1;
	cv::resize(Image2Show,QImage1,Size(300,200));

	QImage im1 = QImage((const unsigned char*)(QImage1.data), QImage1.cols, QImage1.rows, QImage::Format_RGB888);
	label5 = new QLabel();
	label5->setScaledContents(true);
    label5->setPixmap(QPixmap::fromImage(im1));
    label5->resize(QSize(im1.width(), im1.height()));
	ui.scrollArea_4->setWidget(label5);
	}
	else
	{
	QMessageBox numshow1;
	QString info1=info1.fromLocal8Bit("提示");
	numshow1.setWindowTitle(info1);
	QString info=info.fromLocal8Bit("数据不全。请先读取目标图以及特征点！");
	numshow1.setText(info);
	numshow1.exec();
	}

}


void Facedistorted::ZoomNum()
{
	Facial_alignment(num1,num2);

	ImageCreat();
}

void Facedistorted::ImageCreat()
{
	if((IFReadImageFlag[0]==1)&&(IFReaNumFlag[0]==1)&&(IFReadImageFlag[1]==1)&&(IFReaNumFlag[1]==1))
	{
	//放缩标志
	if(ui.radioButton_8->isChecked())
		ZoomFlag=1;
	else
		ZoomFlag=0;
	
	//移动标志
	if(ui.radioButton_9->isChecked())
		MoveFlag=1;
	else
		MoveFlag=0;

	//读取参数
	if(ui.radioButton_2->isChecked())
		DeformationFlag=1;//0:TPS;1:B样条脸部轮变换；2:全脸变换;3:B+TPS
	else if(ui.radioButton->isChecked())
		DeformationFlag=0;
	else if(ui.radioButton_12->isChecked())
		DeformationFlag=2;
	
	else if(ui.radioButton_13->isChecked())
		DeformationFlag=3;
		

	//B样条网格大小
	QString str1 = ui.lineEdit->text();
	int GSize = str1.toInt();
	if(GSize<1||GSize>100)
	{
		GSize=30;
	}

	if(ui.radioButton_4->isChecked())
		DirectionFlag=1;//0：逆向；1：正向
	else
		DirectionFlag=0;

	if(ui.radioButton_7->isChecked())
		InterpolationFlag=1;//0最近领；1双三次；2双线性；3：Lanczos；4：反距离
	else if(ui.radioButton_6->isChecked())
		InterpolationFlag=2;
	else if(ui.radioButton_5->isChecked())
		InterpolationFlag=0;
	else if(ui.radioButton_11->isChecked())
		InterpolationFlag=3;
	else
		InterpolationFlag=4;

	Mat Image1P(Image1.rows,Image1.cols,Image1.type());
	Mat Image1R(Image1.rows,Image1.cols,Image1.type());
	Mat Image2P(Image2.rows,Image2.cols,Image2.type());
	Mat Image2R(Image2.rows,Image2.cols,Image2.type());
	if(DeformationFlag==0)
	{
		TPS(Image1,Image2,num1,num1cp,num2,num2cp,Image1P,Image1R,Image2P,Image2R,DirectionFlag,InterpolationFlag,ZoomFlag,GaussFlag,MoveFlag);
	}
	else if(DeformationFlag==1)
	{
		BSpline1(Image1,Image2,num1,num1cp,num2,num2cp,Image1P,DirectionFlag,InterpolationFlag,GSize);
		//Completion(&Image1P);
	}
	else if(DeformationFlag==2)
	{
		BSpline2(Image1,Image2,num1,num1cp,num2,num2cp,Image1P,DirectionFlag,InterpolationFlag,GSize);
		//Completion(&Image1P);
	}
	
	else if(DeformationFlag==3)
	{
		BPlusTPS(Image1,Image2,num1,num1cp,num2,num2cp,Image1P,Image1R,Image2P,Image2R,DirectionFlag,InterpolationFlag,ZoomFlag,GaussFlag,MoveFlag,GSize);
	}
	

	if(GaussFlag==1)
	{
	GaussFlag=0;
	QMessageBox numshow1;
	QString info1=info1.fromLocal8Bit("提示");
	numshow1.setWindowTitle(info1);
	QString info=info.fromLocal8Bit("特征点检测有误。请选择读入特征点或者选取其他人脸。");
	numshow1.setText(info);
	numshow1.exec();
	return;
	}

	Image1Positive=Image1P;
	Image2Positive=Image2P;
	Image1Reserve=Image1R;
	Image2Reserve=Image2R;
	Mat QImage2;
	if(DeformationFlag==3)
	{
		if(DirectionFlag==1)
		{
			QImage2=Image1R;
		}
		else
		{
			QImage2=Image2P;
		}
	}
	else
	{
	if(DeformationFlag==1||DeformationFlag==2)
		QImage2=Image1P;
	else if(DirectionFlag==1)
		QImage2=Image1P;
	else
		QImage2=Image2P;
	}
	TransNum++;

	cv::resize(QImage2,QImage2,Size(500,500));
	QImage im3 = QImage((const unsigned char*)(QImage2.data), QImage2.cols, QImage2.rows, QImage::Format_RGB888);
	label3 = new QLabel();
	label3->setScaledContents(true);
    label3->setPixmap(QPixmap::fromImage(im3));
    label3->resize(QSize(im3.width(), im3.height()));
	//label3->resize(ui.scrollArea_3->size());
	ui.scrollArea_3->setWidget(label3);

	IFCreatFlag=1;

	}
	else
	{
	QMessageBox numshow1;
	QString info1=info1.fromLocal8Bit("提示");
	numshow1.setWindowTitle(info1);
	QString info=info.fromLocal8Bit("数据不全。请先读取图片以及特征点！");
	numshow1.setText(info);
	numshow1.exec();
	}
	
}

void Facedistorted::ImageSave()
{
	if(IFCreatFlag==1)
	{
	QString path1 = QFileDialog::getExistingDirectory(this, "choose src Directory","/");
    string path = path1.toStdString();

	srand(time(0));
	int name =rand()%1000+10;
	Mat Image1temp = Image1Positive;
	Mat Image2temp = Image2Positive;
	Mat Image1Rtemp = Image1Reserve;
	Mat Image2Rtemp;
	cvtColor(Image1temp, Image1temp, CV_RGB2BGR);
	cvtColor(Image2temp,Image2temp,CV_RGB2BGR);
	cvtColor(Image1Rtemp,Image1Rtemp,CV_RGB2BGR);
	if(DeformationFlag==0)
	{
		if(DirectionFlag==1)
			imwrite(path+to_string(name)+".jpg",Image1temp);
		else
			imwrite(path+to_string(name)+".jpg",Image2temp);
	}
	else if(DeformationFlag==1||DeformationFlag==2)
	{
		imwrite(path+to_string(name)+".jpg",Image1temp);
	}
	if(DeformationFlag==3)
	{
		if(DirectionFlag==1)
		{
			imwrite(path+to_string(name)+".jpg",Image1Rtemp);
		}
		else
		{
			imwrite(path+to_string(name)+".jpg",Image2temp);
		}
	}

	QMessageBox msg0;
	QTextCodec *codec = QTextCodec::codecForName("GBK");
	msg0.setText(codec->toUnicode("保存成功"));
	msg0.exec();
	}
	else
	{
	QMessageBox numshow1;
	QString info1=info1.fromLocal8Bit("提示");
	numshow1.setWindowTitle(info1);
	QString info=info.fromLocal8Bit("尚未生成。请先生成图片！");
	numshow1.setText(info);
	numshow1.exec();
	}
}

